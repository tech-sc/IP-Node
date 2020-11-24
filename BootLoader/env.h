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
 * FILE PURPOSE:    Environment variables header file.
 *******************************************************************************
 * FILE NAME:       env.h
 *
 * DESCRIPTION:     Environment variables header file.
 *
 ******************************************************************************/

#ifndef _ENV_H_
#define	_ENV_H_

#ifdef _STANDALONE
#include "sysconf.h"
#endif

#ifdef _STANDALONE
#define enter_critical_section()
#define exit_critical_section()
#else
void enter_critical_section(void);
void exit_critical_section(void);
#endif

#ifndef _STANDALONE
unsigned int strlen(const char *str);
int strcmp(const char *A, const char *B);
char *strcpy(char *DST, const char *SRC);
#endif

int  EnvInit(void);
int  sys_setenv(const char *, const char *);
int sys_setpermenv(const char *env_nm, const char *env_val);
int  sys_unsetenv(const char *);
#ifndef _STANDALONE
int FWBGet_flash_type(void);
int sys_initenv(void);
#endif
void sh_printenv(void);
char *sys_getenv(const char *);
int sys_defragenv(void);
char* sys_getienv(int var_num);
char* sys_getivar (int var_num);
int sys_getenv_index(int index, char** name, char** value);
int EnvAddAlias(const char* orig, char* alias);
int IsReadOnlyVar( const char* env_nm );


typedef struct t_env_var {
	char      var;
	char  *	  val;
}t_env_var;

#ifdef FTP_SERVER_SUPPORT
#ifndef ENV_SPACE_SIZE /* Should be defined by bootcfg.mak file */
#define ENV_SPACE_SIZE      (10 * 1024) 
#endif
#endif
#define AUTO_DEFRAG_ENVIRONMENT     TRUE

/* control field decode */
#define ENV_GARBAGE_BIT         0x01    /* Env is garbage if this bit is off */
#define ENV_DYNAMIC_BIT         0x02    /* Env is dynamic if this bit is off */
#define ENV_PERM_BIT            0x04    /* Env is a backup of a permanent value */


typedef enum ENV_VARS {
    env_vars_start = 0,
    CPUFREQ,
    MEMSZ,
    FLASHSZ,
    MODETTY0,
    MODETTY1,
    PROMPT,
    BOOTCFG,
    HWA_0,
#if !defined (AVALANCHE) || defined(TNETC401B)
    HWA_1,
#endif
#if !defined(TNETV1020_BOARD)
    HWA_RNDIS,
#endif
#if defined (TNETD73XX_BOARD)
    HWA_3,
#endif
    IPA,
    IPA_SVR,
    BLINE_MAC0,
#if !defined (AVALANCHE) || defined(TNETC401B)
    BLINE_MAC1,
#endif
#if !defined(TNETV1020_BOARD)
    BLINE_RNDIS,
#endif
#if defined (TNETD73XX_BOARD)
    BLINE_ATM,
#endif
#if !defined(TNETV1020_BOARD)
    USB_PID,
    USB_VID,
    USB_EPPOLLI,
#endif
  IPA_GATEWAY,
  SUBNET_MASK,
#if defined (TNETV1050_BOARD)
    BLINE_ESWITCH,
#endif
#if !defined(TNETV1020_BOARD)
    USB_SERIAL,
    HWA_HRNDIS,      /* Host (PC) side RNDIS address */
#endif
    REMOTE_USER,
    REMOTE_PASS,
    REMOTE_DIR,
    SYSFREQ,
    LINK_TIMEOUT,
#ifndef AVALANCHE     /* Avalanche boards use only one mac port */
    MAC_PORT,
#endif
    PATH,
    __HOSTNAME,
#ifdef WLAN
     HW_REV_MAJOR,
     HW_REV_MINOR,
     HW_PATCH,
     SW_PATCH,
     SERIAL_NUMBER,
#endif
    TFTPCFG,
#if defined (TNETV1050_BOARD)
    HWA_ESWITCH,
#endif
    BUILD_OPS,
    TFTP_FO_FNAME,
    TFTP_FO_PORTS,
    CONSOLE_STATE,
    MIPSFREQ,
    /*
     * Add new env variables here.
     * NOTE: New environment variables should always be placed at the end, ie
     *       just before env_vars_end.
     */
	IPL_VERSION,									/* IPCS_V4 PG ADD */

    env_vars_end
} ENV_VARS;

#define ENVSTR(x)               #x
#define _ENV_ENTRY_NAME(x, y)   {x, ENVSTR(y)}
#define _ENV_ENTRY(x)           _ENV_ENTRY_NAME(x,x)

typedef struct ENVDESC {
    ENV_VARS   idx;
    const char      *nm;
    char      *alias;
} ENVDESC;
extern ENVDESC env_ns[];

/* Internal macros */
#define IsEnvGarbage(var)       (((var)->ctrl & ENV_GARBAGE_BIT) == 0)
#define IsEnvDynamic(var)       (((var)->ctrl & ENV_DYNAMIC_BIT) == 0)
#define IsEnvPerm(var)          (((var)->ctrl & ENV_PERM_BIT) == 0)

#define MAX_ENV_ENTRY           (block_size/FLASH_ENV_ENTRY_SIZE)
#define ENV_CELL_SIZE           16
#endif /* _ENV_H_ */
