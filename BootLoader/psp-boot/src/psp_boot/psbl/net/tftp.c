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
 * FILE PURPOSE:    TFTP support routines.
 *******************************************************************************
 * FILE NAME:       tftp.c
 *
 * DESCRIPTION:     TFTP support routines.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#include <psbl/errno.h>
#include <psbl/heapmgr.h>
#include <psbl/stdio.h>
#include <psbl/stdlib.h>
#include <psbl/string.h>
#include <psbl/debug.h>
#include <psbl/loadfile.h>
#include <psbl/hw.h>
#ifdef NAND_SBL
#include <psbl/nand.h>
#endif
#include "../kernel/sbl_omod.h"
#include "ether.h"
#include "ip.h"
#ifdef IPV6
#include "ip6.h"
#include "icmp6.h"
#endif
#include "udp.h"
#include "tftp.h"
#include "arp.h"
#include "../kernel/shell.h"
#include "../kernel/avreset.h"
#include "../kernel/files.h"

#ifdef NAND_SBL
extern FLASHDEF global_nand_info;
#if !defined(NSP_SINGLE_IMAGE_SUPPORT)
static u4 write_to_flash_size;
unsigned int page_diff;
#endif
#endif


char* GetResolvedEnvName(char *envName);
void FlushDCache();
void FlushICache();
void delay_usecs(unsigned int usecs); 
Status get_ftype( int argc, char *argv[], unsigned int *id);
#ifdef AVALANCHE    
void sed_shutdown();
#endif
extern int count;
extern int data_count;
extern u1  sys_ip_addr[];
extern u1  svr_ip_addr[];
extern int ipa_len;
extern int iph_sz;
#ifdef IPV6
extern int addr_family;
#endif

u4 TFTP_LOCAL_PORT = TFTP_CLIENT_PORT;

static int tftp_to_ram_argc;
static char **tftp_to_ram_argv;
#ifdef NAND_SBL
BOOL 	   oob_aware_write;	
#endif
#define TFTP_RRQ         1
#define TFTP_WRQ         2
#define TFTP_DATA        3
#define TFTP_ACK         4
#define TFTP_ERROR       5

typedef struct tftp_port_pair
{
  u2    src;
  u2    dst;
} tftp_conn;

typedef struct tftp_state
{
  BOOL       prtcl_st;
  BOOL       rx_st;
  tftp_conn  conn;
  u2         opcode;
  u2         block_no;
  u2         block_sz;
  u2         txfer_block_sz;
  char      *pkt;
  char       flname[256];							/* IPCS-V4 PG ADD */
  char       mode[6];								/* IPCS-V4 PG ADD */
} tftp_state;

/* Protocol state vars. */
#define REQUEST_STATE 1
#define DATA_STATE    2
#define RXDONE_STATE  3
#define DUPPKT_STATE  4 /* indicates duplicate packet received */

tftp_state tftp_cb;
extern u1 sys_et_addr[6];

char* sys_getenv(char*);
char* getMacEnvVarName();

#ifdef NAND_SBL
extern unsigned char dummy_buf[NAND_PAGE_SIZE];
#endif
#ifdef NSP_SINGLE_IMAGE_SUPPORT
extern Status SingleImage_CheckImage( char *FileName, int FileSize, char* Trailer, int TrailerLen );
#endif

Status tftp_req(char *getfile, u2 src_port, u2 dst_port )
{
  char     *buf;
  tftp_pkt *frame;
  u4        len;

  if( (buf = get_netbuf( NETBUFF_SZ )) == NULL )
     return SBL_EFAILURE;

#if TFTP_DEBUG
  sys_printf( "\ntftp_req state..." );
#endif
  /* Form the TFTP request first. */
  frame = (tftp_pkt *)(buf + ET_HDR_SZ + iph_sz + UDP_HDR_SZ + 2);
  frame->opcode = hs2net( TFTP_RRQ );
  len = 2;

  strcpy( frame->rrq, getfile );
  len += strlen( getfile ) + 1;

  strcpy( (char *)(frame->rrq + len - 2), "octet" );
  len += strlen( "octet" ) + 1;

  /* At this point, we have: (1) tftp req pkt formed, (2) size determined.*/
  udp_send( svr_ip_addr, src_port, dst_port, buf, len );

#if CPMAC_DEBUG
  cpmac_stats( );
#endif

  return SBL_SUCCESS;
}

/* TODO: TEMP. multiplex this with tftp_req. */
Status tftp_send_ack(u2 block_no, u2 src_port, u2 dst_port)
{
  char     *buf;
  tftp_pkt *frame;
  u4        len;

  if( (buf = get_netbuf( NETBUFF_SZ )) == NULL )
     return SBL_EFAILURE;

#if TFTP_DEBUG
  sys_printf( "\nACK block: %d", block_no );
#endif

  /* Form the TFTP request first. */
  frame = (tftp_pkt *)(buf + ET_HDR_SZ + iph_sz + UDP_HDR_SZ + 2);
  frame->opcode = hs2net( TFTP_ACK );
  len = 2;

  *(u2 *)((char *)frame + len) = hs2net( block_no );

#if TFTP_DEBUG
  sys_printf( "\nACK block: %d", hs2net (*(u2 *)((char *)frame + len)) );
#endif

  len += 2;

  udp_send( svr_ip_addr, src_port, dst_port, buf, len );

  return SBL_SUCCESS;
}

FFS_FILE *ffsfile;
extern sbl_opt_module sbl_mods[3];
extern sh_CB *sh_cb;
#ifdef NSP_SINGLE_IMAGE_SUPPORT
static u4 write_to_flash_size;
#ifdef NAND_SBL
unsigned int page_diff;
#endif
static char nsp_check_crc = FALSE;
#define NSP_CRC_TRAILER_LEN 8
char Trailer[NSP_CRC_TRAILER_LEN];
int  TrailerLen;
#endif

Status tftp_get_file(char *getfile, char *dstfile, u2 dst_port)
{
  Status        ret;
  u4            ii, pp;
  u4            is_ram_dl = FALSE, is_start = TRUE;
  u4            mod_id, temp, app_entry = 0;
  char * buf, *retx_cnt, *tftpcfg, * next_cnt;
  int status;
  u4 write_to_ram = FALSE;
  volatile char *write_to_ram_ptr = (volatile char *) CS1_BASE + 5 * 1024 * 1024;
  volatile u4 write_to_ram_size = 0;
  u4 TFTP_TMO = 0, TFTP_RETXCNT = 0;
  char mode[10];

  /*VI 87364: Set the default tftp local port*/
  int real_time = t_get();
  TFTP_LOCAL_PORT = (*((u2 *)&sys_et_addr[4]) +real_time) | 0x8000;
#if TFTP_DEBUG
  int i;
  sys_printf("system ip address: \n");
  for(i=0;i<6;i++)
  {
	sys_printf("%02x ",(unsigned char )sys_et_addr[i]);
  }
  sys_printf("\nRealtime :%d\n",real_time);
  sys_printf("TFTP source port :%d\n",TFTP_LOCAL_PORT);
#endif    

  tftp_cb.prtcl_st = REQUEST_STATE;
  tftp_cb.rx_st    = FALSE;
  tftp_cb.block_no = 0;
  tftp_cb.txfer_block_sz = 512;
  tftp_cb.conn.dst = dst_port;
#ifdef NSP_SINGLE_IMAGE_SUPPORT
  write_to_flash_size = 0;
  TrailerLen = 0;
#endif
	

  if( 0 == dstfile[0] )
     is_ram_dl = TRUE;                
  else if( strcmp( dstfile, "/dev/ram" ) == 0 )
     write_to_ram = TRUE;                    

  if( ( is_ram_dl == FALSE ) && ( write_to_ram == FALSE ) )
  {
#ifdef NAND_SBL
	if(oob_aware_write == TRUE)
	{
	     strcpy(mode, "o");
	}
	else
#endif	
	strcpy(mode,"wb");
		 
	if( !(ffsfile = fopen( dstfile, &mode[0])) )
	{
		sys_printf( "FFS: Failed open file %s", dstfile );
		return SBL_EFAILURE;
	}
  }
  if( (tftpcfg = sys_getenv( "TFTPCFG" )) != NULL )
  {
     TFTP_TMO = strtol( tftpcfg, &retx_cnt, 10 );    
     retx_cnt++;
     TFTP_RETXCNT = strtol( retx_cnt, &next_cnt, 10 );
     next_cnt++;
     TFTP_LOCAL_PORT = strtol( next_cnt, NULL, 10 );
  }
  /* Perform sanity check */
  if( !TFTP_TMO || TFTP_TMO > 10 )
  {
#ifdef NAND_SBL  
     TFTP_TMO = 10;
#else	 
     TFTP_TMO = 5;
#endif	 
  }
  if( !TFTP_RETXCNT || TFTP_RETXCNT > 10 )
  {
 #ifdef NAND_SBL 
     TFTP_RETXCNT = 9;
#else	 
     TFTP_RETXCNT = 5;
#endif	 
  }
  if ( !TFTP_LOCAL_PORT || ( TFTP_LOCAL_PORT >= ( 64 * 1024 ) ) )
  {
     TFTP_LOCAL_PORT = TFTP_CLIENT_PORT;
  }
  
  tftp_cb.conn.src = TFTP_LOCAL_PORT;
     
#if TFTP_DEBUG
  sys_printf( "\nTFTP_TMO: %d, TFTP_RETXCNT: %d, TFTP_LOCAL_PORT: %d \n", TFTP_TMO, TFTP_RETXCNT, TFTP_LOCAL_PORT );
#endif    
  tftp_rx:
  for( ii = 0; ii < TFTP_RETXCNT; ii++ )
  {
     if( tftp_cb.prtcl_st == REQUEST_STATE )
        tftp_req( getfile, tftp_cb.conn.src, tftp_cb.conn.dst );
     else
     {
        tftp_send_ack( tftp_cb.block_no, tftp_cb.conn.src, tftp_cb.conn.dst );
        if( tftp_cb.prtcl_st == RXDONE_STATE )
        {
#if TFTP_DEBUG                    
           sys_printf( "Completed TFTP transfer\n" );
#endif          
#ifdef AVALANCHE    
           sed_shutdown( );
#endif                    
           if( write_to_ram == TRUE )
           {
#if TFTP_DEBUG                        
              sys_printf( "write to ram success\n" );
#endif                    
              return write_to_ram_size;                        
           }else if( is_ram_dl == FALSE )
           {
              if( fclose( ffsfile ) != 0 )
                 return SBL_ERESCRUNCH;
           }
           else
           {
#if TFTP_DEBUG                       
              sys_printf( "Got file. Executing at 0x%08x\n", app_entry );                        
#endif                    
#if (CONF_CACHE == CACHE_WB) 
                 FlushDCache( );
              FlushICache( );
#endif                    
              /* mask the first two arguments : tftp and -r */
              ExeAt( app_entry, tftp_to_ram_argc, tftp_to_ram_argv, NULL ); 
           }
           return SBL_SUCCESS; /* Data txfer done. */
        }
     }
#if TFTP_DEBUG        
     sys_printf( "\nWaiting for packet..\n" );
#endif        
     for( pp = 0; pp < TFTP_TMO * (4000); pp++ )
     {

        delay_usecs( 100 );
        if( SBL_SUCCESS != (ret = netif_poll( )) )
        {
           return ret;
        }

#if TFTP_DEBUG
        sys_printf( "\nGot packet\n" );
#endif			
        if( tftp_cb.rx_st == TRUE )
        {
           if( tftp_cb.opcode == TFTP_DATA )
           {

              /* - Copy the data, free the buffer.
               * - Send an ACK.
               * - See if this is the last pkt.
               */
#if TFTP_DEBUG
              sys_printf( "Got packet number: %d\n", tftp_cb.block_no );
#endif
              if (tftp_cb.prtcl_st == DUPPKT_STATE) {
                 /* received dup packet, send dupACK right way */
                 tftp_cb.prtcl_st = DATA_STATE;
                 tftp_cb.rx_st    = FALSE;
                 break;
              }

              buf =  (char *)(tftp_cb.pkt + sizeof(ether_header) + 
                 iph_sz + UDP_HDR_SZ + 4);

              if( tftp_cb.block_no == 1 )
              {
                 /*ResetMMS(); TODO*/
                 /*if (!(ffsfile = fopen(dstfile, "wb"))) {
                 sys_printf("\nFFS: Failed open file %s", 
                    dstfile);
                 return SBL_EFAILURE;
                 }*/
                 tftp_cb.prtcl_st = DATA_STATE;
                 is_start = TRUE;
                 if( is_ram_dl == TRUE )
                 {
                    char *argv[2];

                    argv[ 0 ] = "R"; /* Stands for RAM download */
                    argv[ 1 ] = buf;                           

                    if( get_ftype( 2, argv, &mod_id ) != SBL_SUCCESS )
                    {
                       sys_printf( "\nUnsupported file format download to RAM. Aborting.\n" );
                       free_netbuf( tftp_cb.pkt );
                       return SBL_EFAILURE;
                    }
                    if( mod_id == GZIP_ID || mod_id == ZIP7_ID || mod_id == LZMA_ID )
                    {
                       sys_printf( "\nCompressed image download to RAM not supported. Aborting.\n" );                                
                       free_netbuf( tftp_cb.pkt );
                       return SBL_EFAILURE;
                    }                       
                 }
              }
              else
              {
                 is_start = FALSE;
              }

              if( write_to_ram == TRUE )
              {
#if TFTP_DEBUG                            
                 sys_printf( "writing at address %x\n", write_to_ram_ptr );
#endif                        
                 memcpy( (char*)write_to_ram_ptr, buf, tftp_cb.block_sz );
                 write_to_ram_ptr += tftp_cb.block_sz;
                 write_to_ram_size += tftp_cb.block_sz;

              }
              else if( is_ram_dl == FALSE )
              {
                  int towrite = tftp_cb.block_sz;
                  char* wbuff = buf;

#ifdef NSP_SINGLE_IMAGE_SUPPORT
                  if (nsp_check_crc)
                  {
                      if (TrailerLen)
                      {
                          if( fwrite(&Trailer[0] ,1, TrailerLen, ffsfile ) != TrailerLen )
                          {
                              sys_printf( "\nFlash Write error." );
                              free_netbuf( tftp_cb.pkt );                            
                              return SBL_EFAILURE;
                          }
                          write_to_flash_size += TrailerLen;
                          TrailerLen = 0;
                      }
                      TrailerLen = (towrite>=NSP_CRC_TRAILER_LEN) ? NSP_CRC_TRAILER_LEN : towrite;
                      memcpy(&Trailer[0], wbuff+towrite-TrailerLen, TrailerLen);
                      towrite -= TrailerLen;
                  }
#endif
                  
                 if (towrite)
					/* Data pointer need to be changed to block_buf */
					 if( fwrite( wbuff ,1, towrite, ffsfile ) !=
                         towrite )
                 {
                    sys_printf( "\nFlash Write error." );
                    free_netbuf( tftp_cb.pkt );                            
                    return SBL_EFAILURE;
                 }
#ifdef NSP_SINGLE_IMAGE_SUPPORT
                 write_to_flash_size += towrite;
#endif                        
              }
              else
              {
                 status = ((sbl_ext_fileformat *)(sbl_mods[mod_id].m_p))->load_ram( buf, 
                    tftp_cb.block_sz, is_start, &temp ); 
                 if( status == SBL_SUCCESS )
                 {
#if TFTP_DEBUG
                    sys_printf( "Got the entry point: at 0x%08x\n", temp );
#endif
                    app_entry = temp;
                 }
                 if( (status != SBL_SUCCESS) && (status != SBL_FILECONTINUE) )
                 {
#if TFTP_DEBUG
                    sys_printf( "TFTP abort requested: status: %d\n", status );
#endif
                    free_netbuf( tftp_cb.pkt );   
                    return SBL_EFAILURE;
                 }                       
              }

              free_netbuf( tftp_cb.pkt );

              if( tftp_cb.block_sz < tftp_cb.txfer_block_sz )
                 tftp_cb.prtcl_st = RXDONE_STATE;

              /* start pending on next data from start of timer. */
              tftp_cb.rx_st = FALSE; /* clear before next iteration. */
              goto tftp_rx;

           }
           else if( tftp_cb.opcode == TFTP_ERROR )
           {
              return SBL_EFAILURE;
           }
        }
     } /* poll network itf for data. */
#if TFTP_DEBUG        
     sys_printf( "Timed out. Retransmitting ACK packet.\n" );
#endif        
  } /* re-transmit counter. */
  sys_printf( "\nTFTP Timed out\n" ); 
  return SBL_EFAILURE;
}

/* IPCS_V4 PG ADD-STA */
Status tftpd_put_file(char *getfile, char *dstfile, u2 dst_port)
{
  Status        ret;
  u4            ii, pp;
  u4            is_ram_dl = FALSE, is_start = TRUE;
  u4            mod_id, temp, app_entry = 0;
  char * buf, *retx_cnt, *tftpcfg, * next_cnt;
  int status;
  u4 write_to_ram = FALSE;
  volatile char *write_to_ram_ptr = (volatile char *) CS1_BASE + 5 * 1024 * 1024;
  volatile u4 write_to_ram_size = 0;
  u4 TFTP_TMO = 0, TFTP_RETXCNT = 0;
  char mode[10];

  /*VI 87364: Set the default tftp local port*/
  int real_time = t_get();
  TFTP_LOCAL_PORT = (*((u2 *)&sys_et_addr[4]) +real_time) | 0x8000;
#if TFTP_DEBUG
  int i;
  sys_printf("system ip address: \n");
  for(i=0;i<6;i++)
  {
	sys_printf("%02x ",(unsigned char )sys_et_addr[i]);
  }
  sys_printf("\nRealtime :%d\n",real_time);
  sys_printf("TFTP source port :%d\n",TFTP_LOCAL_PORT);
#endif    

  tftp_cb.prtcl_st = REQUEST_STATE;
  tftp_cb.rx_st    = FALSE;
  tftp_cb.block_no = 0;
  tftp_cb.txfer_block_sz = 512;
  tftp_cb.conn.dst = dst_port;
#ifdef NSP_SINGLE_IMAGE_SUPPORT
  write_to_flash_size = 0;
  TrailerLen = 0;
#endif
	

  if( 0 == dstfile[0] )
     is_ram_dl = TRUE;                
  else if( strcmp( dstfile, "/dev/ram" ) == 0 )
     write_to_ram = TRUE;                    

  if( ( is_ram_dl == FALSE ) && ( write_to_ram == FALSE ) )
  {
#ifdef NAND_SBL
	if(oob_aware_write == TRUE)
	{
	     strcpy(mode, "o");
	}
	else
#endif	
	strcpy(mode,"wb");
		 
	if( !(ffsfile = fopen( dstfile, &mode[0])) )
	{
		sys_printf( "FFS: Failed open file %s", dstfile );
		return SBL_EFAILURE;
	}
  }
  if( (tftpcfg = sys_getenv( "TFTPCFG" )) != NULL )
  {
     TFTP_TMO = strtol( tftpcfg, &retx_cnt, 10 );    
     retx_cnt++;
     TFTP_RETXCNT = strtol( retx_cnt, &next_cnt, 10 );
     next_cnt++;
     TFTP_LOCAL_PORT = strtol( next_cnt, NULL, 10 );
  }
  /* Perform sanity check */
  if( !TFTP_TMO || TFTP_TMO > 10 )
  {
#ifdef NAND_SBL  
     TFTP_TMO = 10;
#else	 
     TFTP_TMO = 5;
#endif	 
  }
  if( !TFTP_RETXCNT || TFTP_RETXCNT > 10 )
  {
 #ifdef NAND_SBL 
     TFTP_RETXCNT = 9;
#else	 
     TFTP_RETXCNT = 5;
#endif	 
  }
  if ( !TFTP_LOCAL_PORT || ( TFTP_LOCAL_PORT >= ( 64 * 1024 ) ) )
  {
     TFTP_LOCAL_PORT = TFTP_CLIENT_PORT;
  }
  
  tftp_cb.conn.src = TFTP_LOCAL_PORT;
     
#if TFTP_DEBUG
  sys_printf( "\nTFTP_TMO: %d, TFTP_RETXCNT: %d, TFTP_LOCAL_PORT: %d \n", TFTP_TMO, TFTP_RETXCNT, TFTP_LOCAL_PORT );
#endif    
  tftp_rx:
  for( ii = 0; ii < TFTP_RETXCNT; ii++ )
  {
     if( tftp_cb.prtcl_st == REQUEST_STATE )
        ;/* IPCS-V4 DEL */ //tftp_req( getfile, tftp_cb.conn.src, tftp_cb.conn.dst );
     else
     {
        tftp_send_ack( tftp_cb.block_no, tftp_cb.conn.src, tftp_cb.conn.dst );
        if( tftp_cb.prtcl_st == RXDONE_STATE )
        {
#if TFTP_DEBUG                    
           sys_printf( "Completed TFTP transfer\n" );
#endif          
#ifdef AVALANCHE    
           sed_shutdown( );
#endif                    
           if( write_to_ram == TRUE )
           {
#if TFTP_DEBUG                        
              sys_printf( "write to ram success\n" );
#endif                    
              return write_to_ram_size;                        
           }else if( is_ram_dl == FALSE )
           {
              if( fclose( ffsfile ) != 0 )
                 return SBL_ERESCRUNCH;
           }
           else
           {
#if TFTP_DEBUG                       
              sys_printf( "Got file. Executing at 0x%08x\n", app_entry );                        
#endif                    
#if (CONF_CACHE == CACHE_WB) 
                 FlushDCache( );
              FlushICache( );
#endif                    
              /* mask the first two arguments : tftp and -r */
              ExeAt( app_entry, tftp_to_ram_argc, tftp_to_ram_argv, NULL ); 
           }
           return SBL_SUCCESS; /* Data txfer done. */
        }
     }
#if TFTP_DEBUG        
     sys_printf( "\nWaiting for packet..\n" );
#endif        
     for( pp = 0; pp < TFTP_TMO * (4000); pp++ )
     {

        delay_usecs( 100 );
        if( SBL_SUCCESS != (ret = netif_poll( )) )	/* １パケット受信 */
        {
           return ret;
        }

#if TFTP_DEBUG
        sys_printf( "\nGot packet\n" );
#endif			
        if( tftp_cb.rx_st == TRUE )
        {
           if( tftp_cb.opcode == TFTP_DATA )
           {

              /* - Copy the data, free the buffer.
               * - Send an ACK.
               * - See if this is the last pkt.
               */
#if TFTP_DEBUG
              sys_printf( "Got packet number: %d\n", tftp_cb.block_no );
#endif
              if (tftp_cb.prtcl_st == DUPPKT_STATE) {
                 /* received dup packet, send dupACK right way */
                 tftp_cb.prtcl_st = DATA_STATE;
                 tftp_cb.rx_st    = FALSE;
                 break;
              }

              buf =  (char *)(tftp_cb.pkt + sizeof(ether_header) + 
                 iph_sz + UDP_HDR_SZ + 4);

              if( tftp_cb.block_no == 1 )
              {
                 /*ResetMMS(); TODO*/
                 /*if (!(ffsfile = fopen(dstfile, "wb"))) {
                 sys_printf("\nFFS: Failed open file %s", 
                    dstfile);
                 return SBL_EFAILURE;
                 }*/
                 tftp_cb.prtcl_st = DATA_STATE;
                 is_start = TRUE;
                 if( is_ram_dl == TRUE )
                 {
                    char *argv[2];

                    argv[ 0 ] = "R"; /* Stands for RAM download */
                    argv[ 1 ] = buf;                           

                    if( get_ftype( 2, argv, &mod_id ) != SBL_SUCCESS )
                    {
                       sys_printf( "\nUnsupported file format download to RAM. Aborting.\n" );
                       free_netbuf( tftp_cb.pkt );
                       return SBL_EFAILURE;
                    }
                    if( mod_id == GZIP_ID || mod_id == ZIP7_ID || mod_id == LZMA_ID )
                    {
                       sys_printf( "\nCompressed image download to RAM not supported. Aborting.\n" );                                
                       free_netbuf( tftp_cb.pkt );
                       return SBL_EFAILURE;
                    }                       
                 }
              }
              else
              {
                 is_start = FALSE;
              }

              if( write_to_ram == TRUE )
              {
#if TFTP_DEBUG                            
                 sys_printf( "writing at address %x\n", write_to_ram_ptr );
#endif                        
                 memcpy( (char*)write_to_ram_ptr, buf, tftp_cb.block_sz );
                 write_to_ram_ptr += tftp_cb.block_sz;
                 write_to_ram_size += tftp_cb.block_sz;

              }
              else if( is_ram_dl == FALSE )
              {
                  int towrite = tftp_cb.block_sz;
                  char* wbuff = buf;

#ifdef NSP_SINGLE_IMAGE_SUPPORT
                  if (nsp_check_crc)
                  {
                      if (TrailerLen)
                      {
                          if( fwrite(&Trailer[0] ,1, TrailerLen, ffsfile ) != TrailerLen )
                          {
                              sys_printf( "\nFlash Write error." );
                              free_netbuf( tftp_cb.pkt );                            
                              return SBL_EFAILURE;
                          }
                          write_to_flash_size += TrailerLen;
                          TrailerLen = 0;
                      }
                      TrailerLen = (towrite>=NSP_CRC_TRAILER_LEN) ? NSP_CRC_TRAILER_LEN : towrite;
                      memcpy(&Trailer[0], wbuff+towrite-TrailerLen, TrailerLen);
                      towrite -= TrailerLen;
                  }
#endif
                  
                 if (towrite)
					/* Data pointer need to be changed to block_buf */
					 if( fwrite( wbuff ,1, towrite, ffsfile ) !=
                         towrite )
                 {
                    sys_printf( "\nFlash Write error." );
                    free_netbuf( tftp_cb.pkt );                            
                    return SBL_EFAILURE;
                 }
#ifdef NSP_SINGLE_IMAGE_SUPPORT
                 write_to_flash_size += towrite;
#endif                        
              }
              else
              {
                 status = ((sbl_ext_fileformat *)(sbl_mods[mod_id].m_p))->load_ram( buf, 
                    tftp_cb.block_sz, is_start, &temp ); 
                 if( status == SBL_SUCCESS )
                 {
#if TFTP_DEBUG
                    sys_printf( "Got the entry point: at 0x%08x\n", temp );
#endif
                    app_entry = temp;
                 }
                 if( (status != SBL_SUCCESS) && (status != SBL_FILECONTINUE) )
                 {
#if TFTP_DEBUG
                    sys_printf( "TFTP abort requested: status: %d\n", status );
#endif
                    free_netbuf( tftp_cb.pkt );   
                    return SBL_EFAILURE;
                 }                       
              }

              free_netbuf( tftp_cb.pkt );

              if( tftp_cb.block_sz < tftp_cb.txfer_block_sz )
                 tftp_cb.prtcl_st = RXDONE_STATE;

              /* start pending on next data from start of timer. */
              tftp_cb.rx_st = FALSE; /* clear before next iteration. */
              goto tftp_rx;

           }
           /* IPCS-V4 PG ADD-STA */
           else if( tftp_cb.opcode == TFTP_RRQ )
           {
              free_netbuf( tftp_cb.pkt );			/* 受信したパケットバッファの解放 */
              tftp_send_ack(0, tftp_cb.conn.src, tftp_cb.conn.dst);
           }
           /* IPCS-V4 PG ADD-END */
           else if( tftp_cb.opcode == TFTP_ERROR )
           {
              return SBL_EFAILURE;
           }
        }
     } /* poll network itf for data. */
#if TFTP_DEBUG        
     sys_printf( "Timed out. Retransmitting ACK packet.\n" );
#endif        
  } /* re-transmit counter. */
  sys_printf( "\nTFTP Timed out\n" ); 
  return SBL_EFAILURE;
}
/* IPCS_V4 PG ADD-END */

Status tftp_in(char *dgram, u2 dst_port, u2 src_port, u4 len)
{
  char  *data;
  char  *error_str;
  u2     blk_no;
  Status ret = SBL_SUCCESS;

  if( dst_port != tftp_cb.conn.src )
  {
     sys_printf( "Invalid destination port (%d vs %d)\n", dst_port, tftp_cb.conn.src );
     goto tftp_in_done;
  }

  /* Grab the first connection and any discard duplicate one. */
  if( REQUEST_STATE == tftp_cb.prtcl_st )
  {
     tftp_cb.conn.dst = src_port;
  }
  else if( tftp_cb.conn.dst != src_port )
  {
     goto tftp_in_done;
  }

  data = (char *)(dgram + sizeof(ether_header) + iph_sz + UDP_HDR_SZ);
  blk_no = net2hs (*(u2 *)(data + 2));

  switch( net2hs( *(u2 *)data ) )
	{
	/* IPCS-V4 PG ADD-STA */
	 case TFTP_WRQ:
		tftp_cb.rx_st	= TRUE;
		tftp_cb.opcode	= TFTP_WRQ;
		tftp_cb.block_no= 0;

		strncpy(tftp_cb.flname, (data + 2), 256);
		tftp_cb.flname[255] = '\0';
		strncpy(tftp_cb.mode, (data + 2 + strlen(tftp_cb.flname)), 6);
		tftp_cb.mode[5] = '\0';
		if (0 == strcmp(tftp_cb.mode, "octed")) {
			return SBL_SUCCESS;
		}
		/* mode=netasciiは対応しない */
		ret = SBL_EFAILURE;
		break;
	/* IPCS-V4 PG ADD-END */

     case TFTP_DATA:
           tftp_cb.rx_st    = TRUE;
           tftp_cb.opcode   = TFTP_DATA;

        if( blk_no == tftp_cb.block_no + 1 )
        {
           tftp_cb.block_no = blk_no;
           tftp_cb.block_sz = len - 4;
           tftp_cb.pkt      = dgram;

#if TFTP_DEBUG
           sys_printf( "\npkt rx: block: %d, block_sz: %d\n", 
              tftp_cb.block_no, tftp_cb.block_sz );
#endif
           return SBL_SUCCESS;    /* free will be done by the client. */
        }
        else if (blk_no == tftp_cb.block_no)
        {
#if TFTP_DEBUG                    
              sys_printf( "\nDuplicate pkt received from server. block#: %d.",
                 net2hs( *(u2 *)(data + 2) ) );
#endif                
            tftp_cb.prtcl_st = DUPPKT_STATE;
        }
        else 
            ret = SBL_EFAILURE; /* unexpected block number */
        break;

     case TFTP_ERROR:
        tftp_cb.rx_st  = TRUE;
        tftp_cb.opcode = TFTP_ERROR;

        /* check if it is an unknown error */
        if( (blk_no == 0) && (net2hs( *(data +4) == 0 )) )
        { 
              error_str = "[Unknown Error]";
        }
        else
        {
              error_str = (data + 4);
        }
        sys_printf( "\nTFTP: Error[%d]: %s", net2hs( *(u2 *)(data + 2) ), error_str );
        ret = SBL_EFAILURE;
        break;

     default:
       {
#if TFTP_DEBUG            
        sys_printf( "\nUnexpected TFTP pkt (opcode: %d) received",
           net2hs( *(u2 *)data ) );
#endif           
       }
  }

  tftp_in_done:
  free_netbuf( dgram );
  return ret;
}

#include "cpmac.h"
extern u1 svr_ip_addr[IP_ADDR_LEN];
extern u1 tgt_ip_addr[IP_ADDR_LEN];
extern u1 subnet_mask[IP_ADDR_LEN];
extern u1 sys_ip_addr[IP_ADDR_LEN];


extern int conf_cpmac_if;

void tftp_usage(void)
{
#ifdef NSP_SINGLE_IMAGE_SUPPORT
#ifdef NAND_SBL
  sys_printf( "Usage: tftp [-n] [-o] [-i tftp-server ip] <src-file> <dst-file>\n"
     "       tftp [-r] <src_file> [optional args to application]" );
#else	 
  sys_printf( "Usage: tftp [-n] [-i tftp-server ip] <src-file> <dst-file>\n"
     "       tftp [-r] <src_file> [optional args to application]" );
#endif	 

#else
#ifdef NAND_SBL
  sys_printf( "Usage: tftp [-o] [-i tftp-server ip] <src-file> <dst-file>\n"
     "       tftp [-r] <src_file> [optional args to application]" );
#else
  sys_printf( "Usage: tftp [-i tftp-server ip] <src-file> <dst-file>\n"
     "       tftp [-r] <src_file> [optional args to application]" );

#endif

#endif               
}

#define ARG_LEN 150 

int tftp(int argc, char *argv[])
{
  u4		 ii, pp;
  char    *idx_start;
  char    *svr_ip = NULL;
  BOOL     svrip_status;
  BOOL     reqd_gateway;
  char     getfile[ARG_LEN];
  char     dstfile[ARG_LEN];
  Status   ret = SBL_SUCCESS;
  volatile unsigned int ix,ALE_UNKNOWN;
  int tftp_to_ram = FALSE;
  u2 dst_port = TFTP_SVR_PORT;


  getfile[0] = dstfile[0] = 0;
  svrip_status = FALSE;
  reqd_gateway = FALSE;
#ifdef NAND_SBL
  oob_aware_write = FALSE;
#endif  

#ifdef NSP_SINGLE_IMAGE_SUPPORT
  nsp_check_crc = FALSE;
#ifdef NAND_SBL
  if( argc < 2 || argc > 8)
#else
  if( argc < 2 || argc > 7)
#endif  
#else
#ifdef NAND_SBL
     if(argc <2 || argc > 7)	
#else
     if(argc <2 || argc > 6)
#endif
#endif
        goto tftp_options;

     for( ii = 1; ii < argc; ii++ )
     {
        if( *argv[ii] == '-' )
        {
           idx_start = argv[ii];
           if( ++idx_start == NULL )
           { 
              goto tftp_options;
           }
           switch( *idx_start )
           {
              case 'i':
                 if( svrip_status == FALSE )
                 {
                    svrip_status = TRUE;
                    svr_ip = argv[++ii];
                 }
                 else
                 {
                       goto tftp_options;
                 }
                 break;
#ifdef NAND_SBL
	          case 'o':
		         oob_aware_write = TRUE;
		         break;		
#endif				 
              case 'r':
                 tftp_to_ram = TRUE;
                 break;
              case 'p':
                 if( argv[ ++ii ] )
                    dst_port = atoui( argv[ ii ] );
                 break;                                            
#ifdef NSP_SINGLE_IMAGE_SUPPORT
              case 'n':
                 nsp_check_crc = TRUE;
                 break;
#endif
           }
        } /* if (*argv[ii] == '-') */
        else
        {
           break;
        }
     } /* for (argc) */

     if( ii < argc )
     {
        memcpy( getfile, argv[ii], strlen( argv[ii] ) + 1 );
        tftp_to_ram_argc = argc - ii;
        tftp_to_ram_argv = &argv[ii];
        ii++;
     }
     else
     {
        goto tftp_options;
     }
     if( ii < argc )
     {
        if( tftp_to_ram == FALSE )
        {
           memcpy( dstfile, argv[ii], strlen( argv[ii] ) + 1 );
        }
     }

     if( tftp_to_ram )
     {    /* no dest file: download to RAM */
#if TFTP_DEBUG            
        sys_printf( "Downloading to RAM\n" );
#endif        
        dstfile[0] = 0; /* Mark to indicate t that there is no dest file */
     }

     if( TRUE == IsFileExist( dstfile, "r" ) )
     {
        sys_printf( "Destination File exists !" );
        return -1;
     }


  	
     if( (idx_start = sys_getenv( getMacEnvVarName( ) )) )
        eth_pton (idx_start, sys_et_addr);
     else
     {
        sys_printf( "%s not configured.\n", 
           GetResolvedEnvName( getMacEnvVarName( ) ) );
        return -1;
     }
     if( (idx_start = sys_getenv("IPA")) )
     {
#ifdef IPV6
        addr_family = get_ipa_family (idx_start);
        if (addr_family == IPA_IPV6) {
            ipa_len = IP6_ADDR_LEN;
            iph_sz  = IP6_HDR_SZ;
            ip6_pton (idx_start, sys_ip_addr);
        }
        else
#endif
        {
            ipa_len = IP_ADDR_LEN;
            iph_sz  = IP_HDR_SZ;
            ip4_pton (idx_start, sys_ip_addr);
        }
     }
     else
     {
        sys_printf( "%s not configured.\n", GetResolvedEnvName( "IPA" ) );
        return -1;
     }
     if( svrip_status != TRUE && !(svr_ip = sys_getenv("IPA_SVR")) )
     {
         sys_printf( "%s not configured.\n", GetResolvedEnvName( "IPA_SVR" ) );
        return -1;
     }
     else
     {
#ifdef IPV6
        if (get_ipa_family (svr_ip) == addr_family) {
            if (addr_family == IPA_IPV6)
                ip6_pton (svr_ip, tgt_ip_addr);
            else
#endif
                ip4_pton (svr_ip, tgt_ip_addr);
            memcpy (svr_ip_addr, tgt_ip_addr, ipa_len);
#ifdef IPV6
        }
        else {
            sys_printf ("%s and %s not in the same address family.\n",
                        GetResolvedEnvName ("IPA"), 
                        GetResolvedEnvName ("IPA_SVR"));
            return -1;
        }
#endif
     }
     if ( (idx_start = sys_getenv("IPA_PREFIX")))
            pfx_pton (idx_start, subnet_mask, ipa_len);
     else if( (idx_start = sys_getenv("SUBNET_MASK")) )
     {
#ifdef IPV6
        if (get_ipa_family (idx_start) == addr_family) {
            if (addr_family == IPA_IPV6)
                ip6_pton (idx_start, subnet_mask);
            else
#endif
                ip4_pton (idx_start, subnet_mask);
#ifdef IPV6
        }
        else {
            sys_printf ("%s and %s not in the same address family.\n",
                        GetResolvedEnvName ("IPA"), 
                        GetResolvedEnvName ("SUBNET_MASK"));
            return -1;
        }
#endif
     }
     else
     {
        sys_printf ("Neither %s nor %s configured.\n", 
                    GetResolvedEnvName ("IPA_PREFIX"),
                    GetResolvedEnvName ("SUBNET_MASK"));
        return -1;
     }


     for( pp = 0; pp < ipa_len; pp++ )
     {
        if( subnet_mask[pp] )
        {
#if CPMAC_DEBUG                
           sys_printf( "\nsubnet_mask[%d]: %x", pp, subnet_mask[pp] );
#endif            
           if( (sys_ip_addr[pp] & subnet_mask[pp]) != 
              (svr_ip_addr[pp] & subnet_mask[pp]) )
           {
#if CPMAC_DEBUG                    
              sys_printf( "\nsys_ip_addr[%d]: %x", pp, sys_ip_addr[pp] );
              sys_printf( "\nsvr_ip_addr[%d]: %x", pp, svr_ip_addr[pp] );
#endif            
              reqd_gateway = TRUE;
              break;
           }
        }
     }

     if( TRUE == reqd_gateway )
     {
#if CPMAC_DEBUG            
        sys_printf( "\nDebug: Host/Server not on the same subnet\n" );
#endif  
        if( (idx_start = sys_getenv("IPA_GATEWAY")) )
        {
#ifdef IPV6
            if (get_ipa_family (idx_start) == addr_family) {
                if (addr_family == IPA_IPV6)
                    ip6_pton (idx_start, tgt_ip_addr);
                else
#endif
                    ip4_pton (idx_start, tgt_ip_addr);
#ifdef IPV6
            }
            else {
                sys_printf ("%s and %s not in the same address family.\n",
                            GetResolvedEnvName ("IPA"), 
                            GetResolvedEnvName ("IPA_GATEWAY"));
                return -1;
           }
#endif
        }
        else
        {
           sys_printf( "%s not configured.\n", 
              GetResolvedEnvName( "IPA_GATEWAY" ) );
           return -1;
        }
     }


#if CPMAC_DEBUG
     hmem_dbg( hmem_cb.desc_start );
#endif
     ALE_UNKNOWN = *(volatile unsigned int *)0xa8640518;
     *(volatile unsigned int *)0xa8640518 = 0x07030307;
     if( 0 != cpmac_init( 0 ) )
     {
        sys_printf( "mac init failed !\n" );
#ifdef AVALANCHE    
        sed_shutdown( );
#endif    
        return -1;
     }

     for( ix = 0; ix < 10000; ix++ );

     if (
#ifdef IPV6
         (addr_family == IPA_IPV6 && (ret=ndp_ar(tgt_ip_addr)) == SBL_ELKDN) ||
         (addr_family != IPA_IPV6 &&
#endif
     /* Send gratuituous arp. */
          SBL_EFAILURE == arp_query( sys_ip_addr ) )
#ifdef IPV6
        )
#endif
     {
#ifdef AVALANCHE
        sed_shutdown( );
#endif
        return -1;
     }

     if (
#ifdef IPV6
         (addr_family == IPA_IPV6 && ret != SBL_SUCCESS ) || 
         (addr_family != IPA_IPV6 && 
#endif
          SBL_SUCCESS != arp_resolve( tgt_ip_addr ) )
#ifdef IPV6
     )
#endif
     {
        sys_printf( "\nHost not found." );
#ifdef AVALANCHE
        sed_shutdown( );
#endif        
        return -1;
     }
     if( strcmp( dstfile, "/dev/ram" ) == 0 )
     {
        int size;
        size = tftp_get_file( getfile, dstfile, dst_port );
#ifdef AVALANCHE        
        if( size <= 0 )
        {
           sed_shutdown( );                
        }
#endif
        return size;   
     }
     ret = tftp_get_file( getfile, dstfile, dst_port );    
#ifdef NAND_SBL
	  page_diff = (write_to_flash_size  / (global_nand_info.page_size) - write_to_flash_size /(global_nand_info.page_size+global_nand_info.spare_area));
	  write_to_flash_size = write_to_flash_size - page_diff * global_nand_info.page_size;
#endif
				  

#ifdef AVALANCHE
     if( ret != SBL_SUCCESS )
     {
        sed_shutdown( );
     }
#endif                       
#if CPMAC_DEBUG
     hmem_dbg( hmem_cb.desc_start );
#endif

#ifdef NSP_SINGLE_IMAGE_SUPPORT
     if( ( nsp_check_crc ) && ( ret == SBL_SUCCESS ) )
     {
         if (TrailerLen != 8)
         {
             sys_printf("\nAccumulated Trailer length too short. Skipping CRC check.\n");

                 return SBL_SUCCESS;
         }
        /* Check image for checksum and mark it as valid */
        return SingleImage_CheckImage( dstfile, ( int )write_to_flash_size, Trailer, TrailerLen);
     }
#endif
     *(volatile unsigned int *)0xa8640518 = ALE_UNKNOWN;
     return ret;

     tftp_options:
     tftp_usage( );
     return -1;
}

/* IPCS_V4 PG ADD-STA */
int tftpd(int argc, char *argv[])
{
  u4		 ii, pp;
  char    *idx_start;
  char    *svr_ip = NULL;
  BOOL     svrip_status;
  BOOL     reqd_gateway;
  char     getfile[ARG_LEN];
  char     dstfile[ARG_LEN];
  Status   ret = SBL_SUCCESS;
  volatile unsigned int ix,ALE_UNKNOWN;
  int tftp_to_ram = FALSE;
  u2 dst_port = TFTP_SVR_PORT;


  getfile[0] = dstfile[0] = 0;
  svrip_status = FALSE;
  reqd_gateway = FALSE;
#ifdef NAND_SBL
  oob_aware_write = FALSE;
#endif  

#ifdef NSP_SINGLE_IMAGE_SUPPORT
  nsp_check_crc = FALSE;
#ifdef NAND_SBL
  if( argc < 2 || argc > 8)
#else
  if( argc < 2 || argc > 7)
#endif  
#else
#ifdef NAND_SBL
     if(argc <2 || argc > 7)	
#else
     if(argc <2 || argc > 6)
#endif
#endif
        goto tftp_options;

     for( ii = 1; ii < argc; ii++ )
     {
        if( *argv[ii] == '-' )
        {
           idx_start = argv[ii];
           if( ++idx_start == NULL )
           { 
              goto tftp_options;
           }
           switch( *idx_start )
           {
              case 'i':
                 if( svrip_status == FALSE )
                 {
                    svrip_status = TRUE;
                    svr_ip = argv[++ii];
                 }
                 else
                 {
                       goto tftp_options;
                 }
                 break;
#ifdef NAND_SBL
	          case 'o':
		         oob_aware_write = TRUE;
		         break;		
#endif				 
              case 'r':
                 tftp_to_ram = TRUE;
                 break;
              case 'p':
                 if( argv[ ++ii ] )
                    dst_port = atoui( argv[ ii ] );
                 break;                                            
#ifdef NSP_SINGLE_IMAGE_SUPPORT
              case 'n':
                 nsp_check_crc = TRUE;
                 break;
#endif
           }
        } /* if (*argv[ii] == '-') */
        else
        {
           break;
        }
     } /* for (argc) */

     if( ii < argc )
     {
        memcpy( getfile, argv[ii], strlen( argv[ii] ) + 1 );
        tftp_to_ram_argc = argc - ii;
        tftp_to_ram_argv = &argv[ii];
        ii++;
     }
     else
     {
        goto tftp_options;
     }
     if( ii < argc )
     {
        if( tftp_to_ram == FALSE )
        {
           memcpy( dstfile, argv[ii], strlen( argv[ii] ) + 1 );
        }
     }

     if( tftp_to_ram )
     {    /* no dest file: download to RAM */
#if TFTP_DEBUG            
        sys_printf( "Downloading to RAM\n" );
#endif        
        dstfile[0] = 0; /* Mark to indicate t that there is no dest file */
     }

     if( TRUE == IsFileExist( dstfile, "r" ) )
     {
        sys_printf( "Destination File exists !" );
        return -1;
     }


  	
     if( (idx_start = sys_getenv( getMacEnvVarName( ) )) )
        eth_pton (idx_start, sys_et_addr);
     else
     {
        sys_printf( "%s not configured.\n", 
           GetResolvedEnvName( getMacEnvVarName( ) ) );
        return -1;
     }
     if( (idx_start = sys_getenv("IPA")) )
     {
#ifdef IPV6
        addr_family = get_ipa_family (idx_start);
        if (addr_family == IPA_IPV6) {
            ipa_len = IP6_ADDR_LEN;
            iph_sz  = IP6_HDR_SZ;
            ip6_pton (idx_start, sys_ip_addr);
        }
        else
#endif
        {
            ipa_len = IP_ADDR_LEN;
            iph_sz  = IP_HDR_SZ;
            ip4_pton (idx_start, sys_ip_addr);
        }
     }
     else
     {
        sys_printf( "%s not configured.\n", GetResolvedEnvName( "IPA" ) );
        return -1;
     }
     if( svrip_status != TRUE && !(svr_ip = sys_getenv("IPA_SVR")) )
     {
         sys_printf( "%s not configured.\n", GetResolvedEnvName( "IPA_SVR" ) );
        return -1;
     }
     else
     {
#ifdef IPV6
        if (get_ipa_family (svr_ip) == addr_family) {
            if (addr_family == IPA_IPV6)
                ip6_pton (svr_ip, tgt_ip_addr);
            else
#endif
                ip4_pton (svr_ip, tgt_ip_addr);
            memcpy (svr_ip_addr, tgt_ip_addr, ipa_len);
#ifdef IPV6
        }
        else {
            sys_printf ("%s and %s not in the same address family.\n",
                        GetResolvedEnvName ("IPA"), 
                        GetResolvedEnvName ("IPA_SVR"));
            return -1;
        }
#endif
     }
     if ( (idx_start = sys_getenv("IPA_PREFIX")))
            pfx_pton (idx_start, subnet_mask, ipa_len);
     else if( (idx_start = sys_getenv("SUBNET_MASK")) )
     {
#ifdef IPV6
        if (get_ipa_family (idx_start) == addr_family) {
            if (addr_family == IPA_IPV6)
                ip6_pton (idx_start, subnet_mask);
            else
#endif
                ip4_pton (idx_start, subnet_mask);
#ifdef IPV6
        }
        else {
            sys_printf ("%s and %s not in the same address family.\n",
                        GetResolvedEnvName ("IPA"), 
                        GetResolvedEnvName ("SUBNET_MASK"));
            return -1;
        }
#endif
     }
     else
     {
        sys_printf ("Neither %s nor %s configured.\n", 
                    GetResolvedEnvName ("IPA_PREFIX"),
                    GetResolvedEnvName ("SUBNET_MASK"));
        return -1;
     }


     for( pp = 0; pp < ipa_len; pp++ )
     {
        if( subnet_mask[pp] )
        {
#if CPMAC_DEBUG                
           sys_printf( "\nsubnet_mask[%d]: %x", pp, subnet_mask[pp] );
#endif            
           if( (sys_ip_addr[pp] & subnet_mask[pp]) != 
              (svr_ip_addr[pp] & subnet_mask[pp]) )
           {
#if CPMAC_DEBUG                    
              sys_printf( "\nsys_ip_addr[%d]: %x", pp, sys_ip_addr[pp] );
              sys_printf( "\nsvr_ip_addr[%d]: %x", pp, svr_ip_addr[pp] );
#endif            
              reqd_gateway = TRUE;
              break;
           }
        }
     }

     if( TRUE == reqd_gateway )
     {
#if CPMAC_DEBUG            
        sys_printf( "\nDebug: Host/Server not on the same subnet\n" );
#endif  
        if( (idx_start = sys_getenv("IPA_GATEWAY")) )
        {
#ifdef IPV6
            if (get_ipa_family (idx_start) == addr_family) {
                if (addr_family == IPA_IPV6)
                    ip6_pton (idx_start, tgt_ip_addr);
                else
#endif
                    ip4_pton (idx_start, tgt_ip_addr);
#ifdef IPV6
            }
            else {
                sys_printf ("%s and %s not in the same address family.\n",
                            GetResolvedEnvName ("IPA"), 
                            GetResolvedEnvName ("IPA_GATEWAY"));
                return -1;
           }
#endif
        }
        else
        {
           sys_printf( "%s not configured.\n", 
              GetResolvedEnvName( "IPA_GATEWAY" ) );
           return -1;
        }
     }


#if CPMAC_DEBUG
     hmem_dbg( hmem_cb.desc_start );
#endif
     ALE_UNKNOWN = *(volatile unsigned int *)0xa8640518;
     *(volatile unsigned int *)0xa8640518 = 0x07030307;
     if( 0 != cpmac_init( 0 ) )
     {
        sys_printf( "mac init failed !\n" );
#ifdef AVALANCHE    
        sed_shutdown( );
#endif    
        return -1;
     }

     for( ix = 0; ix < 10000; ix++ );

     if (
#ifdef IPV6
         (addr_family == IPA_IPV6 && (ret=ndp_ar(tgt_ip_addr)) == SBL_ELKDN) ||
         (addr_family != IPA_IPV6 &&
#endif
     /* Send gratuituous arp. */
          SBL_EFAILURE == arp_query( sys_ip_addr ) )
#ifdef IPV6
        )
#endif
     {
#ifdef AVALANCHE
        sed_shutdown( );
#endif
        return -1;
     }

     if (
#ifdef IPV6
         (addr_family == IPA_IPV6 && ret != SBL_SUCCESS ) || 
         (addr_family != IPA_IPV6 && 
#endif
          SBL_SUCCESS != arp_resolve( tgt_ip_addr ) )
#ifdef IPV6
     )
#endif
     {
        sys_printf( "\nHost not found." );
#ifdef AVALANCHE
        sed_shutdown( );
#endif        
        return -1;
     }
     if( strcmp( dstfile, "/dev/ram" ) == 0 )
     {
        int size;
        size = tftpd_put_file( getfile, dstfile, dst_port );	/* IPCS_V4 PG ADD */
#ifdef AVALANCHE        
        if( size <= 0 )
        {
           sed_shutdown( );                
        }
#endif
        return size;   
     }
     ret = tftpd_put_file( getfile, dstfile, dst_port );    	/* IPCS_V4 PG ADD */
#ifdef NAND_SBL
	  page_diff = (write_to_flash_size  / (global_nand_info.page_size) - write_to_flash_size /(global_nand_info.page_size+global_nand_info.spare_area));
	  write_to_flash_size = write_to_flash_size - page_diff * global_nand_info.page_size;
#endif
				  

#ifdef AVALANCHE
     if( ret != SBL_SUCCESS )
     {
        sed_shutdown( );
     }
#endif                       
#if CPMAC_DEBUG
     hmem_dbg( hmem_cb.desc_start );
#endif

#ifdef NSP_SINGLE_IMAGE_SUPPORT
     if( ( nsp_check_crc ) && ( ret == SBL_SUCCESS ) )
     {
         if (TrailerLen != 8)
         {
             sys_printf("\nAccumulated Trailer length too short. Skipping CRC check.\n");

                 return SBL_SUCCESS;
         }
        /* Check image for checksum and mark it as valid */
        return SingleImage_CheckImage( dstfile, ( int )write_to_flash_size, Trailer, TrailerLen);
     }
#endif
     *(volatile unsigned int *)0xa8640518 = ALE_UNKNOWN;
     return ret;

     tftp_options:
     return -1;
}
/* IPCS_V4 PG ADD-END */
