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
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    Environment variables routines.
 *******************************************************************************
 * FILE NAME:       env.c
 *
 * DESCRIPTION:     Environment variables routines.
 *
 ******************************************************************************/

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#include <psbl/stdio.h>
#include <psbl/env.h>
#include <psbl/flashop.h>
#include <psbl/debug.h>
#include <psbl/errno.h>
#include <psbl/sysdefs.h>
#include <psbl/heapmgr.h>
#include <psbl/build_ops.h>
#include <psbl/string.h>
#include "shell.h"
#else
#include "platform.h"
#include "pal.h"
#include "env.h"
#include "flashop.h"
#include "debug.h"
#include "errno.h"
#include "shell.h"
#endif

#include "UF7200s2.h"								/* IPCS_V4 PG ADD */

#define MAX_ENV_ENTRY           (block_size/FLASH_ENV_ENTRY_SIZE)
#ifndef AVALANCHE_DCL_BOOTCR
#define AVALANCHE_DCL_BOOTCR    0xA8611A00
#endif
unsigned int env_size;
unsigned int env_base;

/* TODO: remove this */
t_env_var env_vars[10];

#ifdef PERMANENT_VARIABLES

int max_perm_var_size;        /* Size of permanent variables space in bytes */
unsigned char *perm_env_ptr;
#endif /* PERMANENT_VARIABLES */

ENVDESC env_ns[] = {
    _ENV_ENTRY(env_vars_start), /* start. */
    _ENV_ENTRY(CPUFREQ),
    _ENV_ENTRY(MEMSZ),
    _ENV_ENTRY(FLASHSZ),
    _ENV_ENTRY(MODETTY0),
    _ENV_ENTRY(MODETTY1),
    _ENV_ENTRY(PROMPT),
    _ENV_ENTRY(BOOTCFG),
    _ENV_ENTRY(HWA_0),
#if !defined (AVALANCHE) || defined(TNETC401B)
    _ENV_ENTRY(HWA_1),
#endif
#if !defined(TNETV1020_BOARD)
    _ENV_ENTRY(HWA_RNDIS),
#endif
#if defined (TNETD73XX_BOARD)
    _ENV_ENTRY(HWA_3),
#endif
    _ENV_ENTRY(IPA),
    _ENV_ENTRY(IPA_SVR),
    _ENV_ENTRY(IPA_GATEWAY),
    _ENV_ENTRY(SUBNET_MASK),
    _ENV_ENTRY(BLINE_MAC0),
#if !defined (AVALANCHE) || defined(TNETC401B)
    _ENV_ENTRY(BLINE_MAC1),
#endif
#if !defined(TNETV1020_BOARD)
    _ENV_ENTRY(BLINE_RNDIS),
#endif
#if defined (TNETD73XX_BOARD)
    _ENV_ENTRY(BLINE_ATM),
#endif
#if !defined(TNETV1020_BOARD)
    _ENV_ENTRY(USB_PID),
    _ENV_ENTRY(USB_VID),
    _ENV_ENTRY(USB_EPPOLLI),
#endif
#if defined (TNETV1050_BOARD)
    _ENV_ENTRY(BLINE_ESWITCH),
#endif
#if !defined(TNETV1020_BOARD)
    _ENV_ENTRY(USB_SERIAL),
    _ENV_ENTRY(HWA_HRNDIS),
#endif
    _ENV_ENTRY(REMOTE_USER),
    _ENV_ENTRY(REMOTE_PASS),
    _ENV_ENTRY(REMOTE_DIR),
    _ENV_ENTRY(SYSFREQ),
    _ENV_ENTRY(LINK_TIMEOUT),
#ifndef AVALANCHE       /* Avalanche boards use only one mac port */
    _ENV_ENTRY(MAC_PORT),
#endif
    _ENV_ENTRY(PATH),
    _ENV_ENTRY_NAME(__HOSTNAME, HOSTNAME),
#ifdef WLAN
    _ENV_ENTRY(HW_REV_MAJOR),
    _ENV_ENTRY(HW_REV_MINOR),
    _ENV_ENTRY(HW_PATCH),
    _ENV_ENTRY(SW_PATCH),
    _ENV_ENTRY(SERIAL_NUMBER),
#endif
    _ENV_ENTRY(TFTPCFG),
#if defined (TNETV1050_BOARD)
    _ENV_ENTRY(HWA_ESWITCH),
#endif
    _ENV_ENTRY(BUILD_OPS),
    _ENV_ENTRY(TFTP_FO_FNAME),
    _ENV_ENTRY(TFTP_FO_PORTS),
    _ENV_ENTRY(CONSOLE_STATE),
    _ENV_ENTRY(MIPSFREQ),
    /*
     * Add new entries below this.
     */
    _ENV_ENTRY(IPL_VERSION),						/* IPCS_V4 PG ADD */

    /* Adam2 environment name alias. Temporary. */
    _ENV_ENTRY_NAME(IPA,     my_ipaddress),
    _ENV_ENTRY_NAME(MIPSFREQ, cpufrequency),
    _ENV_ENTRY_NAME(SYSFREQ, sysfrequency),
    _ENV_ENTRY_NAME(HWA_0,   maca),
#ifndef AVALANCHE
    _ENV_ENTRY_NAME(HWA_1,   macb),
#endif
    _ENV_ENTRY_NAME(MEMSZ,   memsize),

    _ENV_ENTRY(env_vars_end) /* delimiter. */
};


static PSBL_REC* psbl_rec;

char envErrReadOnly[] = { "Env: %s is read-only.\n" };

#ifdef NAND_SBL
/*Function declaration for NOR and NAND Flash functions*/
extern int EnvInit_nand(void);
extern int sys_setenv_nand(const char *env_nm, const char *env_val);
#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES
extern int sys_setpermenv_nand(const char *env_nm, const char *env_val);
#endif
#endif
extern int sys_unsetenv_nand(const char *env_nm);
extern void echo_nand(int argc, char **argv);
#ifndef _STANDALONE
int sys_initenv_nand(void);
#endif
extern void sh_printenv_nand(void);
extern char* sys_getenv_nand(const char *var);
extern int sys_defragenv_nand(void);
extern char* sys_getienv_nand(int var_num);
extern char* sys_getivar_nand (int var_num);
extern int sys_getenv_index_nand(int index, char** name, char** value);
extern int EnvAddAlias_nand(const char* orig, char* alias);
extern char* GetResolvedEnvName_nand(const char *envName);
#endif
extern int EnvInit_nor(void);
extern int sys_setenv_nor(const char *env_nm, const char *env_val);
#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES
extern int sys_setpermenv_nor(const char *env_nm, const char *env_val);
#endif
#endif
extern int sys_unsetenv_nor(const char *env_nm);
extern void echo_nor(int argc, char **argv);
#ifndef _STANDALONE
extern int sys_initenv_nor(void);
#endif
extern void sh_printenv_nor(void);
extern char* sys_getenv_nor(const char *var);
extern int sys_defragenv_nor(void);
extern char* sys_getienv_nor(int var_num);
extern char* sys_getivar_nor (int var_num);
extern int sys_getenv_index_nor(int index, char** name, char** value);
extern int EnvAddAlias_nor(const char* orig, char* alias);
extern char* GetResolvedEnvName_nor(const char *envName);
/* #define RAM_FLASH */
#ifdef RAM_FLASH
extern unsigned char ram_flash[];
#endif

int EnvInit(void)
{
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
    volatile int  *bootcr_reg;
#endif
    int status;
    static char after_EnvInit = 0;
#ifdef RAM_FLASH
    unsigned int i;
#endif
    

    if( after_EnvInit )
      return TRUE;
    else
      after_EnvInit = 1;

#if defined(TNETV1051_BOARD)  
    bootcr_reg = (volatile int *)AVALANCHE_DCL_BOOTCR;

    if((*bootcr_reg) & (1 << 8))
    psbl_rec = (PSBL_REC*) 0x94000800;
    else
    psbl_rec = (PSBL_REC*) 0x94000300;
#elif defined(TNETV1061_BOARD) && defined(NAND_SBL)
    psbl_rec = (PSBL_REC*) 0x94000800;
#else
    psbl_rec = (PSBL_REC*) 0x94000300;
#endif    

    env_size = psbl_rec->env_size;
    env_base = psbl_rec->env_base;

#ifdef RAM_FLASH
    env_size = 0x20000;	
    env_base = (unsigned int)(&ram_flash[0]);
    for(i=0; i<0x20000; i++)
		ram_flash[i]=0xff;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = EnvInit_nand();
    else
#endif
        status = EnvInit_nor();
#else  /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = EnvInit_nand();
#else
    status = EnvInit_nor();
#endif
#endif
    


    return status;
}

int sys_setenv(const char *env_nm, const char *env_val)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif
    if( ( !env_nm ) || ( !env_val ) )
     return SBL_EFAILURE;

    /*
     * CPUFREQ and SYSFREQ should not be modified
     */
#ifndef _STANDALONE
    if( IsReadOnlyVar( env_nm ) )
    {
        sys_printf(envErrReadOnly, env_nm);
        return SBL_SUCCESS;
    }
#endif
#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_setenv_nand(env_nm, env_val);
    else
#endif
        status = sys_setenv_nor(env_nm, env_val);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_setenv_nand(env_nm, env_val);
#else
    status = sys_setenv_nor(env_nm, env_val);
#endif
#endif

    return status;

}


#ifdef _STANDALONE
#ifdef PERMANENT_VARIABLES

int sys_setpermenv(const char *env_nm, const char *env_val)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

    if( ( !env_nm ) || ( !env_val ) )
     return SBL_EFAILURE;

#ifndef _STANDALONE
    if( IsReadOnlyVar( env_nm ) )
        {
          sys_printf(envErrReadOnly, env_nm);
          return SBL_SUCCESS;
        }
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_setpermenv_nand(env_nm, env_val);
    else
#endif
        status = sys_setpermenv_nor(env_nm, env_val);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_setpermenv_nand(env_nm, env_val);
#else
    status = sys_setpermenv_nor(env_nm, env_val);
#endif
#endif

    return status;

}

#endif
#endif

int sys_unsetenv(const char *env_nm)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif


    if( IsReadOnlyVar( env_nm ) )
      goto fail;

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_unsetenv_nand(env_nm);
    else
#endif
        status = sys_unsetenv_nor(env_nm);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_unsetenv_nand(env_nm);
#else
    status = sys_unsetenv_nor(env_nm);
#endif
#endif

    return status;

fail:
    sys_printf(envErrReadOnly, env_nm);
    return SBL_EFAILURE;
}



void echo(int argc, char **argv)
{    
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        echo_nand(argc, argv);
        else
#endif
        echo_nor(argc, argv);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
        echo_nand(argc, argv);
#else
        echo_nor(argc, argv);
#endif
#endif

}

#ifndef _STANDALONE
#ifdef RAM_FLASH
extern unsigned char ram_flash[];
#endif
int sys_initenv(void)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
    volatile int  *bootcr_reg;
#endif
    static char after_init = 0;
#ifdef RAM_FLASH
    unsigned int i;
#endif
    

    if( after_init )
      return TRUE;
    else
      after_init = 1;

#ifdef TNETV1051_BOARD 
    bootcr_reg = (volatile int *)AVALANCHE_DCL_BOOTCR;

    if((*bootcr_reg) & (1 << 8))
    psbl_rec = (PSBL_REC*) 0x94000800;
    else
    psbl_rec = (PSBL_REC*) 0x94000300;
#elif defined(NAND_SBL) 
    psbl_rec = (PSBL_REC*) 0x94000800;
#else
    psbl_rec = (PSBL_REC*) 0x94000300;
#endif    

    env_size = psbl_rec->env_size;
    env_base = psbl_rec->env_base;

#ifdef RAM_FLASH
    env_size = 0x20000;	
    env_base = (unsigned int)(&ram_flash[0]);
    for(i=0; i<0x20000; i++)
		ram_flash[i]=0xff;
#endif

#ifdef PERMANENT_VARIABLES
    max_perm_var_size = psbl_rec->permenv_size;
    perm_env_ptr = (unsigned char *)psbl_rec->permenv_base;
#endif
#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_initenv_nand();
    else
#endif
        status = sys_initenv_nor();
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_initenv_nand();
#else
    status = sys_initenv_nor();
#endif
#endif


    return status;

}
#endif

void sh_printenv(void)
{    
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        sh_printenv_nand();
    else
#endif
        sh_printenv_nor();
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    sh_printenv_nand();
#else
    sh_printenv_nor();
#endif
#endif

}

char* sys_getenv(const char *var)
{
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        return sys_getenv_nand(var);
    else
#endif
        return sys_getenv_nor(var);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    return sys_getenv_nand(var);
#else
    return sys_getenv_nor(var);
#endif
#endif

}

int sys_defragenv(void)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_defragenv_nand();
    else
#endif
        status = sys_defragenv_nor();
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_defragenv_nand();
#else
    status = sys_defragenv_nor();
#endif
#endif


    return status;

}

char* sys_getienv(int var_num)
{
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        return sys_getienv_nand(var_num);
    else
#endif
        return sys_getienv_nor(var_num);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    return sys_getienv_nand(var_num);
#else
    return sys_getienv_nor(var_num);
#endif
#endif

}

char* sys_getivar (int var_num)
{    
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        return sys_getivar_nand(var_num);
    else
#endif
        return sys_getivar_nor(var_num);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    return sys_getivar_nand(var_num);
#else
    return sys_getivar_nor(var_num);
#endif
#endif

}

int sys_getenv_index(int index, char** name, char** value)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = sys_getenv_index_nand(index, name, value);
    else
#endif
        status = sys_getenv_index_nor(index, name, value);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = sys_getenv_index_nand(index, name, value);
#else
    status = sys_getenv_index_nor(index, name, value);
#endif
#endif

    return status;
}
int EnvAddAlias(const char* orig, char* alias)
{
    int status;
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        status = EnvAddAlias_nand(orig, alias);
    else
#endif
        status = EnvAddAlias_nor(orig, alias);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    status = EnvAddAlias_nand(orig, alias);
#else
    status = EnvAddAlias_nor(orig, alias);
#endif
#endif


    return status;

}

char* GetResolvedEnvName(const char *envName)
{
#if defined(TNETV1051_BOARD)  
    int isSlotNand=0;
#endif

#ifdef TNETV1051_BOARD 
    if ((((unsigned int)env_base >> 28) & 0xe) != 0x8 && (((unsigned int)env_base >> 28) & 0xe) != 0xA)
        isSlotNand = 1;
#if defined(NAND_SBL) || !defined(_STANDALONE)         
    if(isSlotNand)
        return GetResolvedEnvName_nand(envName);
    else
#endif
        return GetResolvedEnvName_nor(envName);
#else /* !TNETV1051_BOARD */
#if defined(NAND_SBL)
    return GetResolvedEnvName_nand(envName);
#else
    return GetResolvedEnvName_nor(envName);
#endif
#endif

}

int IsReadOnlyVar( const char* env_nm )
{
  if( (strcmp("CPUFREQ", env_nm) == 0) ||
      ( strcmp("SYSFREQ", env_nm) == 0 ) || (strcmp("BUILD_OPS", env_nm) == 0)
      || (strcmp("MIPSFREQ", env_nm) == 0) || (strcmp(IPL_VER, env_nm) == 0))	/* IPCS_V4 PG ADD */
    return 1;

  return 0;
}
