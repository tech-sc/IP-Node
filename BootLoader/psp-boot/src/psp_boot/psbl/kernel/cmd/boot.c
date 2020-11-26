/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    The boot command
 *******************************************************************************
 * FILE NAME:       boot.c
 *
 * DESCRIPTION:     The boot command
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#include <psbl/ffs.h>
#include <psbl/debug.h>
#include <psbl/system.h>
#include <psbl/stdio.h>
#include <psbl/errno.h>
#include <psbl/hw.h>
#include <psbl/env.h>
#include <psbl/string.h>
#include <psbl/revision.h>
#include <psbl/sysdefs.h>
#include <psbl/support.h>
#include "../../net/cpmac.h"
#include "files.h"
#include "shell.h"
#include "sbl_omod.h"
#include "avreset.h"

#ifdef TFTP
int tftp(int argc, char *argv[]);
#endif

#if defined(SEAD2) || defined(VOLCANO_ARIES_BOARD)
void DispStr(char *val);
#define DISP_STR(s) DispStr(s)
#endif

void waitForLinkAfterReset(); 
int boot(void);

#ifdef FTP_SERVER_SUPPORT
#include "main.h"
extern int crashFlag;
#endif

extern char bootfile_name[128]; /* filled by DHCP module */
extern sh_CB *sh_cb;
extern t_env_var env_vars[];
extern Status load_image(int argc, char *argv[], u4 *app_entry);
extern bit32u gopc;
#ifndef FFS
extern int fmt(int argc, char *argv[]);
#endif
Status SingleImage_TestImage( unsigned int *GoPC, char *FileName );
void ResetMMS(void);
#if defined( TFTP_FAILOVER_SUPPROT ) && defined( TFTP )
void boot_tftp_fo( char *file_name, char *argv[] );
int boot_tftp_alg( char *file_name, char *svr_file_name, char *port, char *argv[] );
#endif

#if defined (VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)
/* To Disable CPDMA of Aries ESWITCH */
static void aries_eswitch_dma_shutdown(void)
{
/*Code added to support Aries ESWITCH reset requirement*/
#define ESS_DMA_IntMask_Set   (CPDMA_BASE + 0xb8)
#define ESS_DMA_IntMask_Clear (CPDMA_BASE + 0xbc)
#define ESS_Tx_IntMask_Clear  (CPDMA_BASE + 0x8c)
#define ESS_Rx_IntMask_Clear  (CPDMA_BASE + 0xac)
#define ESS_DMA_Tx_Control    (CPDMA_BASE + 0x04)
#define ESS_DMA_Tx_Teardown   (CPDMA_BASE + 0x08)
#define ESS_DMA_Rx_Control    (CPDMA_BASE + 0x14)
#define ESS_DMA_Rx_Teardown   (CPDMA_BASE + 0x18)
#define ESS_DMA_Soft_Reset    (CPDMA_BASE + 0x1c)
#define ESS_ALE_port_control  (0xa8640548)

     volatile int no_channel,delay;

     *(volatile unsigned int *)ESS_ALE_port_control &= 0x10;
     *(volatile unsigned int *)ESS_DMA_Tx_Control = 0x0;
     *(volatile unsigned int *)ESS_DMA_Rx_Control = 0x0;

     for(no_channel = 0; no_channel < 8; no_channel++)
     {
       *(volatile unsigned int *)ESS_DMA_Tx_Teardown = no_channel;
       *(volatile unsigned int *)ESS_Tx_IntMask_Clear = 1 << no_channel;
         for(delay = 0;delay < 1000;delay++);
     }
     for(no_channel = 0; no_channel < 8; no_channel++)
     {
       *(volatile unsigned int *)ESS_DMA_Rx_Teardown = no_channel;
       *(volatile unsigned int *)ESS_Rx_IntMask_Clear = 1 << no_channel;
        for(delay = 0;delay < 1000;delay++);
     }
     *(volatile unsigned int *)ESS_DMA_IntMask_Set = 0x0;
     *(volatile unsigned int *)ESS_DMA_IntMask_Clear = 0xffffffff;
     *(volatile unsigned int *)ESS_DMA_Soft_Reset = 0x1;
      /* wait for reset complete */
      while(*(volatile unsigned int *)ESS_DMA_Soft_Reset != 0)
      for(delay = 0;delay < 1000;delay++);
}
#endif

sh_cmd bootcmd __attribute__((section(".cmdlist"))) = {
        "boot",
        (FUNC_PTR)boot,
        PRIV_USER,
		"Boot the OS image according to settings in 'BOOTCFG'"
};

int boot(void)
{
  Status status = SBL_EFAILURE;
  char file_name[ARG_LEN] =  {0};
  char *argv[SH_ARGC_MAX];
#ifdef FFS
  int i;
#endif
  char boot_cfg[80];
  char *boot_order, *boot_file;
  int boot_finish = FALSE;
  char	*tftpd_argv[3];		/* IPCS-V4 PG ADD */

  if( sys_getenv( "BOOTCFG" ) != NULL )
  {            
     strcpy( boot_cfg, sys_getenv( "BOOTCFG" ) );
  }
  else
  {
     goto bootfail;
  }
  
  /* 
  * at the end of this code, boot_order will point to the second part of 
  * boot_config and boot_file will point to third part of boot_config
  * any further parts are ignored. (currently)
  */
  boot_order = strchr( boot_cfg, ':' );
  if( boot_order )
  {
     *boot_order = 0;
     boot_order++;
     boot_file = strchr( boot_order, ':' );
     if( boot_file )
     {
        *boot_file  = 0;
        boot_file++;
     }else
     {
        goto bootfail;
     }
  }
  else
  {
     goto bootfail;
  }

#ifdef TFTP

  /* Wait for link in case of network boot */
  if( (toupper( boot_order[0] ) == 'N') ||
     (toupper( boot_order[1] ) == 'N') )
  {
     /* Wait for link to come up */
     waitForLinkAfterReset( );        
  }
#endif /* if TFTP */

#if SHELL_DEBUG
  sys_printf( "boot order: %s\n", boot_order );
  sys_printf( "boot file: %s\n", boot_file );
#endif    

#ifdef DHCP_SUPPORT    
  if( (toupper( boot_cfg[0] ) == 'D') && (toupper( boot_file[0] ) == 'A') )
  {
     if( bootfile_name[0] != 0 )
     {
        boot_file = &bootfile_name[0];
     }
     else
     {
        goto bootfail;
     }
  }else 
#endif            
     if( (toupper( boot_file[0] ) == '"') )
  {
     boot_file = boot_file + 1;
     boot_file[strlen( boot_file ) -1 ] = 0;
  }
  else
  {
     goto bootfail;            
  }

#if SHELL_DEBUG
  sys_printf( "boot file: %s\n", boot_file );
#endif    

  ResetMMS( );
#ifdef TFTP	
  if( toupper( boot_order[0] ) == 'F' )
#else
  /* Network boot is not supported without TFTP. Check both boot orders */
  if( toupper( boot_order[0] ) == 'F' || toupper( boot_order[1] ) == 'F' )
#endif
  {            
#ifdef TFTP	
     boot_from_flash:            
#endif
#ifdef FFS    
#ifdef SECONDARY_FFS
     if( boot_file[ 0 ] == '/' &&
        boot_file[ 1 ] == 's' &&
        boot_file[ 2 ] == 'l' &&
        boot_file[ 3 ] == 'o' &&
        boot_file[ 4 ] == 't' &&
        boot_file[ 5 ] == '1' &&
        boot_file[ 6 ] == '/' )
     {
        strcpy( file_name, boot_file );
     }
     else
     {
        strcpy( file_name, "/bin/" );
        strcat( file_name, boot_file );	
     }  
#else
     strcpy( file_name, "/bin/" );
     strcat( file_name, boot_file );	
#endif            
     if( !IsFileExist( file_name, "x" ) )
     {
        sys_printf( "\nBoot image \"%s\" not found.\n", file_name );
        goto alt_boot; /* try the alternate boot mechanism */
     }
#else
     strcpy( file_name, boot_file );
#endif
     argv[0] = file_name;	

#ifdef NSP_SINGLE_IMAGE_SUPPORT
     status = SingleImage_TestImage( &gopc, argv[0] );
#else
     status = load_image( 1, argv, &gopc );
#endif        
     if( SBL_SUCCESS == status )
     {		  
        sys_printf( "\nBooting...\n" );
        status = ExeAt( gopc, sh_cb->argc, sh_cb->argv, (char **)env_vars );
        if( status == SBL_SUCCESS )
        {
           return SBL_SUCCESS;
        } 
     }
  	/* IPCS-V4 PG ADD-STA */
  	else if (SBL_ECKSUM == status )
  	{	/* チェックサム以上の場合、tftpサーバ起動する */
  		tftpd_argv[0] = "IPCS_V4_PROG.BIN";
  		tftpd_argv[1] = "mtd1";
  		tftpd_argv[2] = NULL;
  		tftpd(3, tftpd_argv);
  	}
  	/* IPCS-V4 PG ADD-END */
#ifdef FFS                
     alt_boot:
     /* If FFS and FTP are enabled, it gives a compilation 
        error if this statement is not included */
     for(i=0;i<1;i++);
        	
#endif
#ifdef TFTP	
     if( (toupper( boot_order[1] ) == 'N') && (boot_finish == FALSE) )
     { 
        /* now is the time to try a tftp to flash */
        int ii;
#ifdef FFS                
        if( IsFileExist( file_name, "x" ) )
        {
           argv[1] = file_name;
           rm( 2, argv );
        }
#else
        argv[1] = file_name;
        fmt( 2, argv );
#endif
        argv[1] = boot_file;
        argv[2] = file_name;                
        for( ii = 1; ii < sh_cb->argc; ii++ )
        {
           argv[2+ii] = sh_cb->argv[ii];
        }
        /* tftp to flash */
        if( tftp( 3 + ii - 1, argv ) == SBL_SUCCESS )
        { 
           boot_finish = TRUE;
           boot( );
        }                
     }
#endif
  } 
#ifdef TFTP	
  else if( toupper( boot_order[0] ) == 'N' )
  {
     unsigned int ii;
#if SHELL_DEBUG            
     sys_printf( "boot file : %s\n", boot_file );
#endif            
     argv[1] = "-r";
     argv[2] = boot_file;
     for( ii = 1; ii < sh_cb->argc; ii++ )
     {
        argv[2+ii] = sh_cb->argv[ii];
     }
     if( (status = tftp( 3 + ii - 1 , argv )) != SBL_SUCCESS )
     {
        if( (toupper( boot_order[1] ) == 'F') && (boot_finish == FALSE) )
        {
           boot_finish = TRUE;
           goto boot_from_flash;
        }
     }                    
  }
  #if defined( TFTP_FAILOVER_SUPPROT )
  if ( file_name[0] != 0 )
    boot_tftp_fo( file_name, argv );
  #endif       
#endif	
  bootfail:    
  boot_finish = FALSE;
  if( status != SBL_SUCCESS )
  {
     sys_printf( "\nOS boot failed." );
  }
#ifdef FTP_SERVER_SUPPORT
  crashFlag = CRASH_KERNEL;
#endif
#if defined (VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)
  aries_eswitch_dma_shutdown();
#endif  
  return status;
}


#if defined( TFTP_FAILOVER_SUPPROT ) && defined( TFTP )
void boot_tftp_fo( char *file_name, char *argv[] )
{
  char *port;
  char *fname;

  /* Get server filename */
  if( ( fname = sys_getenv( "TFTP_FO_FNAME" ) ) == NULL )
  {
     sys_printf( "TFTP Failover error: TFTP_FO_FNAME not configured.\n" );
     return;
  }

  argv[1] = file_name;

  sys_printf( "\nTFTP Failover: Erasing %s...\n", file_name );

#ifdef FFS
  /* Erase file */
  ffs_remove( file_name );
#else
  /* Format the partition */
  fmt( 2, argv );
  sys_printf( "\n" );
#endif            

  /* Check if user wanted to download from a special port */
  if( ( port = sys_getenv( "TFTP_FO_PORTS" ) ) == NULL )
  {
     /* Run TFTP algorithm */
     boot_tftp_alg( file_name, fname, NULL, argv );
  }
  else
  {
     char new_port[ 6 ];
     int j = 0;
     int i = 0;

     for( ;; )
     {
        Status ret;
        
        /* Copy port number */
        while( ( port[ i ] != 0 ) && ( (port[ i ]>='0')&&(port[ i ]<='9') ) && ( j < 5 ) )
        { 
           new_port[ j++ ] = port[ i++ ];
        }

        if( j == 0 )
           break;

        new_port[ j ] = 0;   

        /* Run TFTP algorithm */
        ret = boot_tftp_alg( file_name, fname, new_port, argv );
        
        /* Actually, there is no "success" because it boots.
           However, if there is no flash space, return with error */
        if( ret == SBL_SUCCESS || ret == SBL_ERESCRUNCH )
           break;

        /* No more ports */
        if( port[ i ] == 0 )
           break;
        else
        {
           i++;   
           j=0;
        }  
     }   
  }
}  

int boot_tftp_alg( char *file_name, char *svr_file_name, char *port, char *argv[] )
{
  Status ret = SBL_EFAILURE;
  int argc = 0;     

#ifdef NSP_SINGLE_IMAGE_SUPPORT
  argv[++argc] = "-n";
#endif

  if( port )
  {
     argv[++argc] = "-p";
     argv[++argc] = port;
  }
  else
    port = "69";

  argv[++argc] = svr_file_name;
  argv[++argc] = file_name;                

  sys_printf( "TFTP Failover: Downloading using port %s: %s to %s...\n", port, svr_file_name, file_name );

  /* tftp to flash and boot */
  if( ( ret = tftp( argc+1, argv ) ) == SBL_SUCCESS )
     boot();

  return ret;
}
#endif
