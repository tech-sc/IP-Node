/*-----------------------------------------------------------------------------*/
/*                                                                             */
/*   Copyright (C) 1996-2002 by Texas Instruments, Inc.  All rights reserved.  */
/*                                                                             */
/*     IMPORTANT - READ CAREFULLY BEFORE PROCEEDING TO USE SOFTWARE.           */
/*                                                                             */
/*  This document is displayed for you to read prior to using the software     */
/*  and documentation.  By using the software and documentation, or opening    */
/*  the sealed packet containing the software, or proceeding to download the   */
/*  software from a Bulletin Board System(BBS) or a WEB Server, you agree to   */
/*  abide by the following Texas Instruments License Agreement. If you choose  */
/*  not to agree with these provisions, promptly discontinue use of the        */
/*  software and documentation and return the material to the place you        */
/*  obtained it.                                                               */
/*                                                                             */
/*                               *** NOTE ***                                  */
/*                                                                             */
/*  The licensed materials contain MIPS Technologies, Inc. confidential        */
/*  information which is protected by the appropriate MIPS Technologies, Inc.  */
/*  license agreement.  It is your responsibility to comply with these         */
/*  licenses.                                                                  */
/*                                                                             */
/*                   Texas Instruments License Agreement                       */
/*                                                                             */
/*  1. License - Texas Instruments (hereinafter "TI"), grants you a license    */
/*  to use the software program and documentation in this package ("Licensed   */
/*  Materials") for Texas Instruments broadband products.                      */
/*                                                                             */
/*  2. Restrictions - You may not reverse-assemble or reverse-compile the      */
/*  Licensed Materials provided in object code or executable format.  You may  */
/*  not sublicense, transfer, assign, rent, or lease the Licensed Materials    */
/*  or this Agreement without written permission from TI.                      */
/*                                                                             */
/*  3. Copyright - The Licensed Materials are copyrighted. Accordingly, you    */
/*  may either make one copy of the Licensed Materials for backup and/or       */
/*  archival purposes or copy the Licensed Materials to another medium and     */
/*  keep the original Licensed Materials for backup and/or archival purposes.  */
/*                                                                             */
/*  4. Runtime and Applications Software - You may create modified or          */
/*  derivative programs of software identified as Runtime Libraries or         */
/*  Applications Software, which, in source code form, remain subject to this  */
/*  Agreement, but object code versions of such derivative programs are not    */
/*  subject to this Agreement.                                                 */
/*                                                                             */
/*  5. Warranty - TI warrants the media to be free from defects in material    */
/*  and workmanship and that the software will substantially conform to the    */
/*  related documentation for a period of ninety (90) days after the date of   */
/*  your purchase. TI does not warrant that the Licensed Materials will be     */
/*  free from error or will meet your specific requirements.                   */
/*                                                                             */
/*  6. Remedies - If you find defects in the media or that the software does   */
/*  not conform to the enclosed documentation, you may return the Licensed     */
/*  Materials along with the purchase receipt, postage prepaid, to the         */
/*  following address within the warranty period and receive a refund.         */
/*                                                                             */
/*  TEXAS INSTRUMENTS                                                          */
/*  Application Specific Products, MS 8650                                     */
/*  c/o ADAM2 Application Manager                                              */
/*  12500 TI Boulevard                                                         */
/*  Dallas, TX 75243  - U.S.A.                                                 */
/*                                                                             */
/*  7. Limitations - TI makes no warranty or condition, either expressed or    */
/*  implied, including, but not limited to, any implied warranties of          */
/*  merchantability and fitness for a particular purpose, regarding the        */
/*  licensed materials.                                                        */
/*                                                                             */
/*  Neither TI nor any applicable licensor will be liable for any indirect,    */
/*  incidental or consequential damages, including but not limited to loss of  */
/*  profits.                                                                   */
/*                                                                             */
/*  8. Term - The license is effective until terminated.   You may terminate   */
/*  it at any other time by destroying the program together with all copies,   */
/*  modifications and merged portions in any form. It also will terminate if   */
/*  you fail to comply with any term or condition of this Agreement.           */
/*                                                                             */
/*  9. Export Control - The re-export of United States origin software and     */
/*  documentation is subject to the U.S. Export Administration Regulations or  */
/*  your equivalent local regulations. Compliance with such regulations is     */
/*  your responsibility.                                                       */
/*                                                                             */
/*                         *** IMPORTANT NOTICE ***                            */
/*                                                                             */
/*  Texas Instruments (TI) reserves the right to make changes to or to         */
/*  discontinue any semiconductor product or service identified in this        */
/*  publication without notice. TI advises its customers to obtain the latest  */
/*  version of the relevant information to verify, before placing orders,      */
/*  that the information being relied upon is current.                         */
/*                                                                             */
/*  TI warrants performance of its semiconductor products and related          */
/*  software to current specifications in accordance with TI's standard        */
/*  warranty. Testing and other quality control techniques are utilized to     */
/*  the extent TI deems necessary to support this warranty. Unless mandated    */
/*  by government requirements, specific testing of all parameters of each     */
/*  device is not necessarily performed.                                       */
/*                                                                             */
/*  Please be aware that Texas Instruments products are not intended for use   */
/*  in life-support appliances, devices, or systems. Use of a TI product in    */
/*  such applications without the written approval of the appropriate TI       */
/*  officer is prohibited. Certain applications using semiconductor devices    */
/*  may involve potential risks of injury, property damage, or loss of life.   */
/*  In order to minimize these risks, adequate design and operating            */
/*  safeguards should be provided by the customer to minimize inherent or      */
/*  procedural hazards. Inclusion of TI products in such applications is       */
/*  understood to be fully at the risk of the customer using TI devices or     */
/*  systems.                                                                   */
/*                                                                             */
/*  TI assumes no liability for TI applications assistance, customer product   */
/*  design, software performance, or infringement of patents or services       */
/*  described herein. Nor does TI warrant or represent that license, either    */
/*  expressed or implied, is granted under any patent right, copyright, mask   */
/*  work right, or other intellectual property right of TI covering or         */
/*  relating to any combination, machine, or process in which such             */
/*  semiconductor products or services might be or are used.                   */
/*                                                                             */
/*  All company and/or product names are trademarks and/or registered          */
/*  trademarks of their respective manaufacturers.                             */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
#include <psbl/ffs.h>
#include <psbl/stdio.h>
#include <psbl/stdlib.h>
#include <psbl/string.h>
#include <psbl/revision.h>
#include <psbl/hw.h>
#include <psbl/env.h>
#include <psbl/sysconf.h>
#include <psbl/heapmgr.h>
#include <psbl/flashop.h>
#include <psbl/errno.h>
#include <psbl/fmm.h>
#include <psbl/debug.h>
#include "mms.h"
#include "sio.h"
#include "main.h"
#include "files.h"
#include <psbl/support.h>
#include "shell.h"
#include "sbl_omod.h"
#include <psbl/loadfile.h>
#include <psbl/omod_gzip.h>
#include <psbl/omod_7zip.h>
#include <psbl/omod_lzma.h>
#include <psbl/sysdefs.h>
#include "../net/net.h"
#include "autoconfig.h"
#include "../net/cpmac.h"

#if 0
#define  CPGMAC_MODE
#endif

#if defined (NAND_SBL) || defined (DUAL_FFS)
#include <psbl/nand.h>
#endif

#if defined(FTP_SERVER_SUPPORT) && defined (EB)
#error PC Application support not available in BIG ENDIAN Mode.
#endif

#if defined(FTP_SERVER_SUPPORT) && defined (TFTP)
#error PC Application support not available with TFTP protocol.
#endif

#if defined(FTP_SERVER_SUPPORT) && defined (FTP)
#error PC Application support not available along with FTP client support.
#endif

#if defined(DHCP_SUPPORT) || defined(FTP_SERVER_SUPPORT)
#include "../net/tinyip.h"
#include "../net/tinyftp.h"
#endif


#ifdef FTP_SERVER_SUPPORT
extern int ftpServerStatus;
#endif

#if defined (NAND_SBL) || defined (DUAL_FFS)
extern FLASHDEF global_nand_info;
extern unsigned int nand_init(unsigned int chipsel, unsigned int mask);
#endif
extern HMEM_CB hmem_cb;
extern volatile unsigned int _MipsFrequency;

/*Heap Size is now constant = 1Mbytes*/
#define HEAP_SIZE CONF_HEAP_SZ

#define        phys(a)                 (((int)(a))&~0xe0000000)
#define        virt_nc(a)              ((phys(a))|0xa0000000)
#define        virt_c(a)               ((phys(a))|0x80000000)

#define CPSW3G_GIG_LAN_PHY0                 0x1
#define CPSW3G_GIG_PC_PHY1                  0x4
#define CPSW3G_EPHY0_PWRRST_BYP             0x10
#define CPSW3G_EPHY1_PWRRST_BYP             0x20


unsigned int packet_filter_enabled = 1;

extern volatile int AvSio0Inited;
void SioInit(void);
bit32u getflashsize(bit32u base, bit32u size);
bit32u get2flashsize(bit32u base, bit32u size);
int ffs_autoload(void);
void FlushDCache();
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
int t_get();
void t_clear();
int FWBGet_flash_type();
size_t strlen(const char *s);
void delay_usecs(unsigned int usecs);
void printLastResetCause ();
void printChipdbgInfo(void);
int is_soc_tnetv1057(void);
BOOL lk_st(u4 itf);
extern volatile int AvSio0Inited;
void psbl_chook_init3(void);
void psbl_chook_init4(void);
void startDHCP(void);
void clkc_delay(unsigned int counter);
void setClockPLL(unsigned int mult, unsigned int div, unsigned int clock_id);
int   lava_no_gig();
void SetALEUnicastEntry(int index, int VLANId, unsigned int UniAddrHigh, unsigned int UniAddrLow, int portNumber);
void SetALEMultcastEntry(int index, int VLANId, unsigned int UniAddrHigh, unsigned int UniAddrLow, int portMask, int Super);
void SetALEVlanEntry(int index, int VLANId, unsigned int vlanMemberList, unsigned int unregMcastMask,
                                      int regMcastMask, int forceUntaggedEgress);
void dump_ale();
void decode_macaddr(const char *mac_addr, unsigned int *hi_addr,
                                    unsigned int *lo_addr);
char* getMacEnvVarName();
void MdioWr(UINT16 phyAddr, UINT16 regNum, UINT16 data);
#if defined(TNETV1051_BOARD)
void config_ext_phy_leds(void);
#endif
int Phy0,Phy1;
int g_reset_swr2;
  
#ifndef SEAD2
/* Check environment variable and lock serial port if required*/
void SioCheckSerialLock( void );

/* Check if serial port is locked */
int SioIsSerialLocked( void );
#endif

#ifdef TNETV1061_BOARD

#define AVALANCHE_PIN_SEL_BASE     0xA861160C
typedef struct
{
    volatile unsigned int reg[24];
}
PIN_SEL_REG_ARRAY_T;
static volatile PIN_SEL_REG_ARRAY_T *pin_sel_array = (PIN_SEL_REG_ARRAY_T*)AVALANCHE_PIN_SEL_BASE;

static void petra_mux_lan_led_enable(void)
{
    pin_sel_array->reg[4]  &= ~0x3f000000;  /* LAN LED bits   : b xx11 1111 xxxx xxxx xxxx xxxx xxxx xxxx */
    pin_sel_array->reg[4]  |=  0x15000000;  /* Bit settings   : b xx01 0101 xxxx xxxx xxxx xxxx xxxx xxxx */
}

static void petra_mux_ext_switch_enable(void)
{
    *(volatile unsigned int *)0xa861166c &= ~(0x1);     /* Reset the Marvell switch */

    *(volatile unsigned int *)0xa8610918 &= ~(1<<7); /* This is required for proper MDIO operation when using an
                           external MII interface (see Petra spec for details). */

    pin_sel_array->reg[3]  &= ~0x0ff00000;  /* PC MII bits    : b xxxx 1111 1111 xxxx xxxx xxxx xxxx xxxx */
    pin_sel_array->reg[3]  |=  0x0aa00000;  /* Bit settings   : b xxxx 1010 1010 xxxx xxxx xxxx xxxx xxxx */

    pin_sel_array->reg[4]  &= ~0x000f3ff3;  /* PC MII bits    : b xxxx xxxx xxxx 1111 xx11 1111 1111 xx11 */
    pin_sel_array->reg[4]  |=  0x000a2aa2;  /* Bit settings   : b xxxx xxxx xxxx 1010 xx10 1010 1010 xx10 */

    pin_sel_array->reg[15] &= ~0x0fc00cc0;  /* PC MII bits    : b xxxx 1111 11xx xxxx xxxx 11xx 11xx xxxx */
    pin_sel_array->reg[15] |=  0x0a800880;  /* Bit settings   : b xxxx 1010 10xx xxxx xxxx 10xx 10xx xxxx */

    *(volatile unsigned int *)0xA8610A08 |= 0x001f0000; /* Disable Petra internal pull-up/downs on Petra pins
                          connected to Marvell boot-mode pins (see below).
                          This is required in order for the boot-mode settings to
                          be read correctly by the Marvell when it comes out of reset.
                          SSP0_0/PC_MII_RDV   PULLPD[16] = 1'b1
                          SSP0_1/PC_MII_RDV   PULLPD[17] = 1'b1
                          SSP0_2/PC_MII_RDV   PULLPD[18] = 1'b1
                          SSP0_3/PC_MII_RDV   PULLPD[19] = 1'b1
                          GPIO4/PC_MII_RDV    PULLPD[20] = 1'b1 */

    *(volatile unsigned int *)0xa861166c |= 1;      /* Un-reset the Marvell switch */

}

static void petra_mux_1061sdb_gpio_enable(void)
{
    pin_sel_array->reg[4]  &= ~0x0000000c;  /* GPIO5 bits     : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 11xx */
    pin_sel_array->reg[4]  |=  0x00000004;  /* Bit settings   : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 01xx */

    pin_sel_array->reg[15] &= ~0x003fc000;  /* GPIO0-3 bits   : b xxxx xxxx xx11 1111 11xx xxxx xxxx xxxx */
    pin_sel_array->reg[15] |=  0x00154000;  /* Bit settings   : b xxxx xxxx xx01 0101 01xx xxxx xxxx xxxx */

    pin_sel_array->reg[3]  &= ~0x0000000c;  /* GPIO22 bits    : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 11xx */
    pin_sel_array->reg[3]  |=  0x00000008;  /* Bit settings   : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 10xx */

    pin_sel_array->reg[5]  &= ~0x0000000f;  /* GPIO19-20 bits : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 1111 */
    pin_sel_array->reg[5]  &=  0x0000000a;  /* Bit settings   : b xxxx xxxx xxxx xxxx xxxx xxxx xxxx 1010 */
}

#define LAN_CPGMAC_TO_PC_MII    0x1
#define  PC_CPGMAC_TO_PC_MII    0x2

static void petra_config_cpgmac_to_pc_mii(unsigned int lan_or_pc_config)
{
    if(PC_CPGMAC_TO_PC_MII == lan_or_pc_config)
        *(volatile unsigned int*) 0xa8611a10 = 0x00000024; /* Enabled the PC port to drive the PC MII Lines.
                                  Put internal PC EPHY into a power-up reset
                                  bypass mode.  */
    else if(LAN_CPGMAC_TO_PC_MII == lan_or_pc_config)
        *(volatile unsigned int*) 0xa8611a10 = 0x00000002; /* Enabled the LAN port to drive the PC MII Lines. */
    else
        ;
}
#endif

void aries_sdb_led_on(int led)
{
    *(volatile UINT8*)(KSEG1(0x1e000000 + 1))=0x00;
    *(volatile UINT8*)(KSEG1(0x1e000000 + 1))=led;
}


/*Here user must specify LED nubers to switch off*/
void aries_sdb_led_off(int led)
{
    int led_state=0;
    led_state^=(0x1<<led);
    *(volatile UINT8*)(KSEG1(0x1e000000 + 1))=led_state;
}

extern volatile unsigned int _CpuFrequency;
extern volatile unsigned int _SysFrequency;
extern unsigned int _VbusFrequency;
unsigned int _MemSize;

#define TRUE    (1==1)
#define FALSE   (1==2)

const char *sbl_strings[] = {
    "%s: Module not supported\n",
    "File checksum error\n",
    "File format not supported\n"
};

sbl_opt_module sbl_mods[5];

/* networking related globals */
u1 et_bcast_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#ifdef IPV6
u1 sys_ip_addr[IP6_ADDR_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
u1 svr_ip_addr[IP6_ADDR_LEN];
u1 subnet_mask[IP6_ADDR_LEN];
u1 tgt_ip_addr[IP6_ADDR_LEN];
int addr_family;
#else
u1 sys_ip_addr[IP_ADDR_LEN] = { 0, 0, 0, 0 };
u1 svr_ip_addr[IP_ADDR_LEN];
u1 subnet_mask[IP_ADDR_LEN];
u1 tgt_ip_addr[IP_ADDR_LEN];
#endif /* IPV6 */
u1 tgt_et_addr[ETHER_ADDR_LEN];
u1 sys_et_addr[ETHER_ADDR_LEN];
int ipa_len;
int iph_sz;

#if defined(SEAD2) || defined(VOLCANO_ARIES_BOARD)
void DispStr(char *val);
#define DISP_STR(s) DispStr(s)
#else
void DispStr(char *val)
{
}                               /* to satisfy mips4k_init.S */

#define DISP_STR(s) DispStr(s)
#endif
void DispHex(int val);
extern int end;
extern sh_CB *sh_cb;

#if defined (SEAD2)

#define BCR_BYPASS (1<<5)
void GetFrequencyAndInitSio( void )
{
    char sbuf[30];

    /*
     * Set CPU Frequency if different.
     */
    _MipsFrequency = _SysFrequency = _CpuFrequency =
        (BOOTCR & BCR_BYPASS) ? CONF_REFXTAL_FREQ : CONF_CPU_FREQ;
    sys_sprintf(sbuf, "%d", _CpuFrequency);
    sys_setenv("CPUFREQ", sbuf);
    sys_setenv("SYSFREQ", sbuf);

    SioInit();
}

#endif

extern sbl_ext_fileformat *sbl_mod_tibinary;
extern sbl_ext_fileformat *sbl_mod_elf;
extern sbl_ext_gzip *sbl_mod_gzip;
extern sbl_ext_7zip *sbl_mod_7zip;
extern sbl_ext_lzma *sbl_mod_lzma;

extern int cfg_cpufreq(bit32 sysf, bit32 cpuf, bit32u dbg_opt);
void configureFrequencies(void);
void configureMemSizes(int memsize);
void initMacPorts(void);
void startMDIO();
void startGPIO();
void resetExtSwitch(void);
#if defined( TNETD73XX_BOARD ) || defined( TNETV1050_BOARD )||defined(TNETV1051_BOARD)
static void ConfigEphyTrim(void);
#endif
void waitForLinkAfterReset();

#ifdef TNETD73XX_BOARD
int is_ohio_chip;
#endif

extern char _etext_rom, _stext_rom, _edata, _sdata;

unsigned int bootFlag = TRUE;

#ifdef FTP_SERVER_SUPPORT
int crashFlag = CRASH_NONE;
#endif

/* These initializations are important. Do not change */
int conf_cpmac_if = -1;
int linkTimeOut = -1;
unsigned int CS0_SIZE = 0;
#ifdef DUAL_FFS
unsigned int CS3_SIZE = 0;
#endif


extern int max_perm_var_size;        /* Size of permanent variables space in bytes */
#ifdef NAND_SBL
extern unsigned int perm_env_ptr;
extern void copy_vector(bit32u adr);
#else
extern unsigned char *perm_env_ptr;
#endif
void c_entry(int memsize)
{
#ifdef SEAD2
    char disp_prompt[8];
#endif
    int ibaud, ii;
    char ch;
    char stbuf[32];
    PSBL_REC *psbl_rec;
#if defined(TNETV1051_BOARD) || defined(VOLCANO_ARIES_BOARD)
    char disp_prompt[8];
    volatile bit32u mips32;
#else
    char disp_prompt[50];
#endif
#ifdef PERMANENT_VARIABLES
    int bootsize;
#endif
/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(4);
#endif
    if((RESET_STATUS & 0x7) == 0x4)
    {
       g_reset_swr2 = 1;
    }
    else
    {
       g_reset_swr2 = 0;
    }
    _MemSize = memsize;
#if defined(TNETV1051_BOARD)
    /* Following are hack for support aries sdb flashop */
    mips32 = REG32_R(BOOTCR, 25, 25);
    if(get_soc_variant() == ARIES_1052)
    {
        _CpuFrequency = MHZ(125);
    }
    else
    {
        _CpuFrequency = MHZ(300);
    }
    _MipsFrequency = _CpuFrequency;
    _SysFrequency = _CpuFrequency;
    if(mips32)
    {
        _SysFrequency = _SysFrequency >> 1;
    }
    _VbusFrequency = _SysFrequency >> 1;
#endif
#ifdef NAND_SBL
    psbl_rec = (PSBL_REC *) 0x94000800;
#else
    psbl_rec = (PSBL_REC *) 0x94000300;
#endif
    memset((char *) psbl_rec, 0, sizeof(PSBL_REC));

    sh_cb = (sh_CB *) & (psbl_rec->sh_cb);
    psbl_rec->psbl_size = ((int) (&_etext_rom)) + ((int) &_edata -
                    (int) &_sdata) - ((int) &_stext_rom);

    DISP_STR("ResetAll");

#ifdef TNETD73XX_BOARD
    is_ohio_chip = IS_OHIO_CHIP();
#endif

#if defined (NAND_SBL) || defined (DUAL_FFS)
    nand_init(NAND_CHIP_SEL,NAND_CHIP_MASK);
    get_nand_info(&global_nand_info);
#endif
    CS0_SIZE = getflashsize(CS0_BASE, _CS0_SIZE);

#ifdef DUAL_FFS
    CS3_SIZE = get2flashsize(CS3_BASE, _CS3_SIZE);
#endif
    /*
     * Reset all modules:
     * only UART, GPIO and CPMAC are brought out of reset later
     */
    if(!g_reset_swr2)
    {
        RESET_REG = 0;
    }

#ifdef TIBINARY_SUPPORT
    strcpy(sbl_mods[TIBINARY_ID].m_nm, "tibinary");
    sbl_mods[TIBINARY_ID].m_p = sbl_mod_tibinary;
#endif
#ifdef ELF_SUPPORT
    strcpy(sbl_mods[ELF_ID].m_nm, "elf");
    sbl_mods[ELF_ID].m_p = sbl_mod_elf;
#endif
#ifdef INCLUDE_GZIP
    strcpy(sbl_mods[GZIP_ID].m_nm, "gzip");
    sbl_mods[GZIP_ID].m_p = sbl_mod_gzip;
#endif
#ifdef INCLUDE_7ZIP
    strcpy(sbl_mods[ZIP7_ID].m_nm, "7zip");
    sbl_mods[ZIP7_ID].m_p = sbl_mod_7zip;
#endif
#ifdef INCLUDE_LZMA
    strcpy(sbl_mods[LZMA_ID].m_nm, "lzma");
    sbl_mods[LZMA_ID].m_p = sbl_mod_lzma;
#endif

#if (CONF_CACHE == CACHE_WB)
    FlushDCache();
#endif
    
    /* Initialize flash type */
#ifdef NAND_SBL
    FWBGet_flash_type_nand();
#else
    FWBGet_flash_type();
#endif

    DISP_STR("BasicEnv");

    FMMInit();
/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(5);
#endif

/* Remember where the permenant variables start */
#ifdef PERMANENT_VARIABLES
    bootsize = psbl_rec->psbl_size;
#ifdef NAND_SBL
    perm_env_ptr = psbl_rec->psbl_size;
    perm_env_ptr = perm_env_ptr + (global_nand_info.page_size - (perm_env_ptr % global_nand_info.page_size));
    /* Take into account pbl size ( max 4k)*/
    perm_env_ptr = perm_env_ptr + (global_nand_info.page_size *2);
    psbl_rec->psbl_size = perm_env_ptr + global_nand_info.page_size;
#else
    perm_env_ptr = (unsigned char*)(CS0_BASE + psbl_rec->psbl_size);
#endif
    psbl_rec->psbl_size = psbl_rec->psbl_size + MIN_PERM_VAR_SIZE;
#ifdef NAND_SBL
    /* Round off the bootsize to nearest block */
    psbl_rec->psbl_size = FWBGetNextBlock_nand(psbl_rec->psbl_size);
    max_perm_var_size = psbl_rec->psbl_size - perm_env_ptr;
    psbl_rec->permenv_size = max_perm_var_size;
    psbl_rec->permenv_base = perm_env_ptr;
#else
    /* Round off the bootsize to nearest block */
    psbl_rec->psbl_size = FWBGetNextBlock(CS0_BASE + psbl_rec->psbl_size) - CS0_BASE;
    max_perm_var_size = psbl_rec->psbl_size - bootsize;
#endif
#endif

    FMMConfigSections();

    /* Allocate flash space for env and ffs */
    FMMGetSectionInfo(FMM_ENV_TAG,&(psbl_rec->env_base),&(psbl_rec->env_size));
#ifdef FFS
    FMMGetSectionInfo(FMM_FFS_TAG,&(psbl_rec->ffs_base),&(psbl_rec->ffs_size));
 #ifdef SECONDARY_FFS
    FMMGetSectionInfo(FMM_SECONDARY_FFS_TAG,&(psbl_rec->ffs2_base),&(psbl_rec->ffs2_size));
 #else
    /* Clear secondary partition information */
    psbl_rec->ffs2_base = 0;
    psbl_rec->ffs2_size = 0;
 #endif
 #ifdef DUAL_FFS
     FMMGetSectionInfo(FMM_DUAL_FFS_TAG,&(psbl_rec->ffs_base_dual),&(psbl_rec->ffs_size_dual));
#endif
#else
    /* Clear primary and secondary partition information */
    psbl_rec->ffs_base = 0;
    psbl_rec->ffs_size = 0;

    psbl_rec->ffs2_base = 0;
    psbl_rec->ffs2_size = 0;
 #ifdef DUAL_FFS
    psbl_rec->ffs_base_dual = 0;
    psbl_rec->ffs_size_dual = 0;
#endif
#endif

/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(6);
#endif

    DISP_STR("HeapMgr");

    _malloc_init((bit8u *) (KSEG0(CS1_BASE) + CONF_SDRAM_SZ - HEAP_SIZE),
                 HEAP_SIZE);
    
    /*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(7);
#endif

    EnvInit();
/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(8);
#endif

    if(g_reset_swr2)
    {
        g_reset_swr2 = 1;
        sys_sprintf(stbuf, "%d",g_reset_swr2);
        sys_setenv("SOFT_RESET_SWR2", stbuf );
    }
    else
    {
        g_reset_swr2 = 0;
        sys_sprintf(stbuf, "%d", g_reset_swr2);
        sys_setenv("SOFT_RESET_SWR2", stbuf );
    }
    PrintfRestore();

    /* BU specific hook just into C code */
    psbl_chook_init3();
    configureMemSizes(memsize);

    /* These routines just make sure baud is proper */
    SioGetBaud("MODETTY0", &ibaud);
    SioGetBaud("MODETTY1", &ibaud);

#ifdef FFS
    if (sys_getenv("PATH") == NULL) {
        sys_setenv("PATH", "/bin:/ttyS");
    }
#endif

/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(9);
#endif

    DISP_STR("Loop");
#if defined (TNETV1051_BOARD)
     /* TODO: Add code for selecting the PHYS - external or internal */
     /* By default the PHYs are internal and no setting required */

     /* Set the proper PIN MUX setting for the SDB/ADB boards */
     /* UART 1 settings - 1-24 are pin mux and 0-23(for macro)*/
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),13,12,0x1);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),15,14,0x1);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),17,16,0x1);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),19,18,0x1);
     /* UART 0 settings */
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),23,22,0x2);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((8-1)),29,28,0x2);
     /* UART2 settings */
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),15,14,0x1);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),17,16,0x3);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),19,18,0x2);
     REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),21,20,0x1);
#endif

    configureFrequencies();

/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(10);
#endif

#ifdef NAND_SBL
    copy_vector(0x94000200);
    copy_vector(0x94000280);
    copy_vector(0x94000300);
    copy_vector(0x94000400);
    copy_vector(0x94000380);
#endif

     /*
     * Print the banner now.
     */
    sys_printf("\nBasic POST completed...     Success.\n");
    printLastResetCause();
    sys_printf("\nPSPBoot%d.%d rev: %d.%d.%d.%d",
            MonitorMajorRev, MonitorMinorRev, MonitorMajorRev, MonitorMinorRev, IncRev, BuildNum);
    sys_printf("\n(c) Copyright 2002-2008 Texas Instruments, Inc. "
               "All Rights Reserved.\n\n");
 

#ifdef FFS
    DISP_STR("FFSInit");
    _FileSystemInit();
#endif
/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(11);
#endif

    initMacPorts();
    startMDIO();

#if !defined(TNETV1051_BOARD)
    resetExtSwitch();
#endif

#ifdef TNETV1061_BOARD
    startGPIO();
    petra_mux_lan_led_enable();
    petra_config_cpgmac_to_pc_mii(PC_CPGMAC_TO_PC_MII);
    petra_mux_ext_switch_enable();
    petra_mux_1061sdb_gpio_enable();
#endif

#if defined( TNETD73XX_BOARD ) || defined( TNETV1050_BOARD )||defined(TNETV1051_BOARD)
    /* Configure EPHY trim value */
    ConfigEphyTrim();
#endif

    /* BU specific hook after bootloader basic setup */
    psbl_chook_init4();

#ifndef SEAD2
    /* Check if serial port is locked */
    SioCheckSerialLock();
#endif
    /*
     * Read the boot config and find out how you want to boot
     * BOOT_CFG: The structure of BOOT_CFG variable is as follows:
     * <where to get information>:<order of boot image search>:<boot file name>
     * Example
     * d:fn:a => Get information from dhcp, search for image in flash,
     *           if not found in flash use the network information provided
     *           by dhcp to fetch the image, burn it to flash and boot. The
     *           boot file name is auto, meaning take what ever dhcp gives.
     * d:nf:a => Get information from dhcp, try to fetch the image from
     *           network, if failed, try to boot from flash. Image in flash
     *           is not updated if network boot suceeds.
     */
    if (sys_getenv("BOOTCFG") != NULL) {
#ifdef DHCP_SUPPORT
        if (toupper(sys_getenv("BOOTCFG")[0]) == 'D') {
            startDHCP();
        }
#endif
    } else {
        bootFlag = FALSE; /* cant boot if BOOTCFG is not set */
    }

#ifdef FTP_SERVER_SUPPORT
    if (sys_getenv("crash") != NULL) {
        sys_unsetenv("crash");
        crashFlag = CRASH_FS;
        bootFlag = FALSE;       /* do not boot if crashed */
    }
#ifdef TNETD73XX_BOARD
    waitForLinkAfterReset();
    leds_init();

    lk_st(conf_cpmac_if);
#endif
#ifdef TNETV1061_BOARD
    waitForLinkAfterReset();
    lk_st(conf_cpmac_if);
#endif
    ftpServerStatus = STATUS_NONE;
    FTPServer();
#endif /* FTP_SERVER_SUPPORT */

    if (NULL == sys_getenv("PROMPT")) {
        sys_setenv("PROMPT", "(psbl)");
    }

    DISP_STR("Prompt");
/*LED indicator for debug purpose*/
#if defined(TNETV1051_BOARD)
aries_sdb_led_on(12);
#endif

    AppCopyVectors();

#if defined (SEAD2) ||defined (VOLCANO_ARIES_BOARD) ||defined(TNETV1051_BOARD)
    sys_sprintf(disp_prompt, "%s " "%d.%d-%d", "psbl", MonitorMajorRev,
                    MonitorMinorRev, IncRev);
#endif

    DISP_STR("Wait");

    ch = 0;

    if (!bootFlag) { /* do not boot if crashed. */
        goto skipboot;
    }

    SioFlush();
    strcpy(sh_cb->cmd, "boot");

#ifndef SEAD2
    if ( !SioIsSerialLocked() )
#endif
    {
      sys_printf("Press ESC for monitor... 3");

      for (ii = 300; ii > 0; ii--) {
        if (!(ii % 100))
            sys_printf("\b%d", ii / 100);

        /* In case PC App is used, do not delay */
#ifdef FTP_SERVER_SUPPORT
        delay_usecs(KHZ(6));
#else
        delay_usecs(KHZ(8));
#endif
        if(bootFlag == FALSE) {
            goto skipboot;
        }
        if (SioInCharCheck(&ch) == TRUE) {
                if (((ch & 0xff) == 0x1b) || ((ch & 0xff) == 0x2b)) { /* 0x2b == '+' for easy use in scripts */
                goto skipboot;
            }
        }
      }

      if (ch != 0x1b) {           /* process for boot. */
        DISP_STR("Boot");
        sys_printf("\n");

      } else {                    /* fall to monitor. */
        skipboot:
        sh_cb->cmd[0] = '\0';
      }

    }

    sh_cb->priv_state = PRIV_ROOT;
    strcpy(sh_cb->prompt, sys_getenv("PROMPT"));

    DISP_STR(disp_prompt);
#if defined(TNETV1051_BOARD)
    /* Set LED config for PHYs */
   config_ext_phy_leds ();
#endif    
    sh_init();

#if defined(VOLCANO_ARIES_BOARD)
    lava_no_gig();
#endif

    shell();
}

#if defined(TNETV1051_BOARD)
extern void cfg_eswitchfreq(void);
#endif

void configureFrequencies(void)
{
    unsigned int cpu_freq, sys_freq;
    volatile bit32u mips32;

    mips32 = REG32_R(BOOTCR, 25, 25);

    if (sys_getenv("CPUFREQ") != NULL)
    {
        cpu_freq = strtol(sys_getenv("CPUFREQ"), NULL, 10);
    }
    else
    {
#if defined(TNETV1051_BOARD)    
        if(get_soc_variant() == ARIES_1052)
        {
            cpu_freq = MHZ(125);
        }
        else
#endif            
        {
        
            if(mips32)
            {
                cpu_freq = CONF_CPU_FREQ;
            }
            else
            {
                cpu_freq = CONF_SYS_FREQ;
            }
        }
    }
    if (sys_getenv("SYSFREQ") != NULL) {
        sys_freq = strtol(sys_getenv("SYSFREQ"), NULL, 10);
    } else 
    {
#if defined(TNETV1051_BOARD)    
        if(get_soc_variant() == ARIES_1052)
        {
            sys_freq = MHZ(125);
        }
        else
#elif defined(TNETV1050_BOARD)
        if(is_soc_tnetv1057())
        {
            sys_freq = KHZ( 87500 );
        }
        else
#endif            
        {
        sys_freq = CONF_SYS_FREQ;
    }
    }
    if((mips32 == 0)&& (cpu_freq != sys_freq))
       cpu_freq = sys_freq;
#if defined(NAND_SBL)
    {
       extern unsigned int _VbusFrequency;
       _MipsFrequency = cpu_freq;
       _VbusFrequency = sys_freq >> 1;
       SioInit( );
    }
#endif
    /* Serial is initialized here .. */
#if defined(VOLCANO_ARIES_BOARD) || defined(TNETV1061_BOARD) || defined(TNETV1050_BOARD)
    cfg_cpufreq(sys_freq, cpu_freq, FALSE);
#else
    {
        char sbuf[30];

#ifndef NAND_SBL
        cfg_cpufreq(sys_freq,cpu_freq, FALSE);
#endif
        sys_sprintf( sbuf, "%u",cpu_freq);
        _CpuFrequency = cpu_freq;
        sys_setenv( "CPUFREQ", sbuf );
        sys_sprintf( sbuf, "%u",cpu_freq);
        sys_setenv( "MIPSFREQ", sbuf );
        sys_sprintf( sbuf, "%u",sys_freq);
        _SysFrequency = sys_freq;
        sys_setenv( "SYSFREQ", sbuf );
#if defined(TNETV1051_BOARD)
        cfg_eswitchfreq();
#endif
    }
#endif

}

void configureMemSizes(int memsize)
{
    char buf[16];
    sys_sprintf(buf, "0x%08x", memsize);
    sys_setenv("MEMSZ", buf);

#ifdef   DUAL_FLASH
    sys_sprintf(buf, "0x%08x",
                CS0_SIZE + SEC_FLASH_SIZE);
#else
    sys_sprintf(buf, "0x%08x", CS0_SIZE);
#endif

    sys_setenv("FLASHSZ", buf);
#if defined(TNETV1051_BOARD)
    sys_sprintf(buf, "0x%08x", HEAP_SIZE);
    sys_setenv("HEAPSZ", buf);
#endif
}

void initMacPorts(void)
{
#ifndef AVALANCHE               /* avalanche boards do not need MAC_PORT */
    char *tmp;
#if (defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD))
#define CPSW3G_TOKEN_PARSE(str)       { if ((tok = (char *)strsep ((str), ":")) == NULL) return; }
    int  i, j,temp;
    unsigned int hi_addr, lo_addr;
    char  *localString = NULL,*tok;
#endif    
    /* read the MAC_PORT variable to find out the used port */
    if ((tmp = sys_getenv("MAC_PORT")) != NULL) {
        conf_cpmac_if = strtol(tmp, NULL, 10);
    }
#endif

#if (defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD))
    /*Get exiernal/internal Phy information*/
    localString = sys_getenv("CPGMAC_I");
    if(!localString)
    {
     /* Use internal phy as its not defined in bootloader env. variable*/
     Phy0 = 1;
     Phy1 = 1;
    }
    else
    {
		CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	Phy0 = strtol(tok, NULL, 10);
       	CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	CPSW3G_TOKEN_PARSE(&localString);
       	Phy1 = strtol(tok, NULL, 10);
    }
    if(get_soc_variant() == ARIES_1053)
    {
        Phy0 = Phy1 = 0;    /* 1053 uses external PHYs only */
    }
    if(Phy0||Phy1)/*Use internal phy*/
    {
    	if(!g_reset_swr2)
       	{ 
        	/* MII */
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),25,24,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),27,26,0x2);

         	/* RGMII LEDs */
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),25,24,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),27,26,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),5,4,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),7,6,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),29,28,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),31,30,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((18-1)),1,0,0x2);
         	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((18-1)),3,2,0x2);
       }
    }
    else /*Use external Phy*/
    {
		/*Reset GPIO Module*/
       	REG32_RMW(RESET_BASE, 6, 6, 0x01);
         
	   	/* MII */
       	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),25,24,0x1);
       	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((15-1)),27,26,0x1);

	   	/* RGMII LEDs */
       	REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),21,20,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),23,22,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),25,24,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((16-1)),27,26,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),5,4,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),7,6,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),25,24,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),27,26,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),29,28,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((17-1)),31,30,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((18-1)),1,0,0x1);
        REG32_RMW(ARIES_PIN_MUX_REG_ADDR((18-1)),3,2,0x1);
    }
#else
     /* Use internal phy as default */
     Phy0 = 1;
     Phy1 = 1;    
#endif /* (defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)) */

    if ((conf_cpmac_if != 0) && (conf_cpmac_if != 1))
    {
#if defined(AR7RD)  || defined (AR7WRD) || defined (AR7VWi) || defined( TNETV1020VDB ) || defined(TNETC4602_BOARD)
        conf_cpmac_if = 1;
#else
        conf_cpmac_if = 0;
#endif
    }

#ifdef TNETD73XX_BOARD
    if(is_ohio_chip)
    {
     /* Ohio has only one CPMAC interface */
        conf_cpmac_if = 0;
    }
#endif

  /* This may not be applicable to all the SoCs */
  if (conf_cpmac_if == 0)
  {
#ifdef TNETD73XX_BOARD
     if (is_ohio_chip)
     {
        /* Only one CPMAC0 is available in Ohio */
        #if ( EXTERNAL_PHY_PORT0 || EXTERNAL_PHY_PORT1 )
            /* MII pins are connected to the MII interface on the EMAC0 module.*/
        REG32_RMW(AVALANCHE_MII_SEL_REG,0,0,1);

        /* Phy 0 in reset */
        REG32_RMW(RESET_BASE, 26, 26, 0);
        #else
        /* Phy 0 out of reset */
        REG32_RMW(RESET_BASE, 26, 26, 1);
        #endif
     }

#endif
    if(!Phy0)
     {
#ifdef TNETD73XX_BOARD
        /* MII pins are connected to the MII interface on the EMAC0 module.*/
        REG32_RMW(AVALANCHE_MII_SEL_REG,0,0,1);
#endif
         if(!g_reset_swr2) 
         {
           /* Phy 0 in reset */
           REG32_RMW(RESET_BASE, 26, 26, 0);
         } 
         /*Clear GPIO_EN bit 11*/
         (*(volatile UINT32 *)(0xa861091c)) &= 0xfffff7ff;
         /* 1000 Mbps external phy */
         (*(volatile UINT32 *)(0xa8611a10)) |= (CPSW3G_GIG_LAN_PHY0 |  CPSW3G_EPHY0_PWRRST_BYP);
     }
     else
     {
         if(!g_reset_swr2)
         {
           /* Phy 0 out of reset */
           REG32_RMW(RESET_BASE, 26, 26, 1);
         }
         /* 1000 Mbps external phy */
         (*(volatile UINT32 *)(0xa8611a10)) &= (~CPSW3G_GIG_LAN_PHY0 &  ~CPSW3G_EPHY0_PWRRST_BYP);
        

     }
  }
  else  /* conf_cpmac_if = 1 */
  {
#ifndef TNETV1061SDB
     if(!Phy1)
     {
        if(!g_reset_swr2) 
        {
          /* Phy 1 in reset */
          REG32_RMW(RESET_BASE, 28, 28, 0);
        }
        /*Clear GPIO_EN bit 11*/
        (*(volatile UINT32 *)(0xa861091c)) &= 0xfffff7ff;
        /* 1000 Mbps external phy */
        (*(volatile UINT32 *)(0xa8611a10)) |= (CPSW3G_GIG_PC_PHY1 |  CPSW3G_EPHY1_PWRRST_BYP);
       
     }
    else
    {
       if(!g_reset_swr2) 
       {
         /* Phy 1 out of reset */
         REG32_RMW(RESET_BASE, 28, 28, 1);
       } 
       /* 1000 Mbps external phy */
       (*(volatile UINT32 *)(0xa8611a10)) &= (~CPSW3G_GIG_PC_PHY1 &  ~CPSW3G_EPHY1_PWRRST_BYP);
      
    }
#else
        //REG32_RMW(0xa8611600, 28, 28, 0);       /* Phy 1 - Reset EPHY1 as external swtich is used on SBD board */
        delay_usecs(200);
#endif
  }
  delay_usecs(200);

#if defined(TNETV1050_BOARD)
    /* Bring the ethernet switch subsystem out of reset */
    REG32_RMW(RESET_BASE, 27, 27, 1);
    delay_usecs(200);

#if ( EXTERNAL_PHY_PORT0 ) || ( EXTERNAL_PHY_PORT1 )
  /* Put the EPHy0 on reset and MDIO out of reset*/
  *((volatile UINT32 *)(RESET_BASE)) |= 0x18420000;
  delay_usecs(200);

  /* Get the Eswitch alt rst pin out of reset*/
  REG32_RMW(0xA8642884, 0, 0, 1);

#ifndef EL
  /* Put External phy in BE mode */
  REG32_RMW(0xA8642884, 24, 24, 1);
#endif
  delay_usecs(200);

  /*Select lan MII mode and Mio dio mode*/
  *((volatile UINT32 *)(0xA8642880))|= 0x0000001F;
  *((volatile UINT32 *)(0xA8642880))&= 0x00000001;
  delay_usecs(200);

  /* Set the Reset GPIO 6  PIN_SEL_5 Register*/
  *((volatile UINT32 *)(RESET_BASE +0x1c)) &= ~0xC00;
  *((volatile UINT32 *)(RESET_BASE +0x1c)) |= 0x400;

  /* Set the GPIO's to 2nd function for MII port 0 PIN_SEL_9 Register*/
  *((volatile UINT32 *)(RESET_BASE +0x2c))|= 0xAA28AA28;
  delay_usecs(200);

  /* PIN_SEL_10 Register*/
  *((volatile UINT32 *)(RESET_BASE +0x30))&= 0x00000000;
  *((volatile UINT32 *)(RESET_BASE +0x30))|= 0x0000AA0A;
  delay_usecs(200);
#else
  /* CPMAC_Big_Endian Flag has to be turned off in little endian mode */
  #ifdef EL
  REG32_W(0xA8642884, 0x000000ce);
  #else
  REG32_W(0xA8642884, 0x010000ce);
  #endif

  /* MR:1319 set the pin sel register to set LED indication for MACs */
  *((unsigned int*)(RESET_BASE + 0x30)) |=  0x50150000;
  *((unsigned int*)(RESET_BASE + 0x34)) |=  0x00000015;
#endif
#endif

#if !(defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD) || defined(TNETV1061SBD))
   
  /* demultiplex MII and PHY pins */
  if(conf_cpmac_if == 1)
     GPIO_EN &= (0xffffffff ^ 0x0c000000);
#if !defined(TNETV1050_BOARD)
  else
     GPIO_EN &= (0xffffffff ^ 0x0c03c000);
#endif
#else
     if(!g_reset_swr2)     
     {
        /* Bring the PHY0 out of Reset */
        REG32_RMW(RESET_BASE, 26, 26, 1);
        /* Bring the PHY1 out of Reset */
        REG32_RMW(RESET_BASE, 28, 28, 1);

        /* Bring the switch out of Reset */
        REG32_RMW(RESET_BASE, 27, 27, 1);
     }

#if !defined (VOLCANO_ARIES_BOARD)
  //   REG32_RMW(RESET_BASE, 6, 6, 1);
#endif
    if(!g_reset_swr2)
    {
      REG32_RMW(CPSW_SRST, 0, 0, 1);
      for(i=0; i<=10; i++)
      {
        delay_usecs(200);
        temp = REG32_R(CPSW_SRST, 0, 0);
        if (temp == 0) break;
      }
      if(i>=10)
      {
        return ;
      }
      
#if defined(CPGMAC_MODE)
     REG32_W(CPSW_STAT_PORT_EN, 0x03);
#else
     REG32_W(CPSW_STAT_PORT_EN, 0x07);
#endif
#if defined(TNETV1051_BOARD)
     REG32_W(CPSW_PRI_TYPE, 0x00);
     decode_macaddr(sys_getenv("HWA_0"), &hi_addr, &lo_addr);
     hi_addr = (((hi_addr >> 8) & 0xFF) | ((hi_addr & 0xFF) << 8));
     hi_addr |= ((lo_addr >> 8) & 0xFF0000);
     hi_addr |= ((lo_addr << 8) & 0xFF000000);
     lo_addr = (((lo_addr >> 8) & 0xFF) | ((lo_addr & 0xFF) << 8));
     /* NOTE: hi_addr becomes HI ADDR and same reversal with lo_addr */
     REG32_W(CPSW_P0_SRC_ADDR_L0W, hi_addr);
     REG32_W(CPSW_P0_SRC_ADDR_HI,  lo_addr);
     
     decode_macaddr(sys_getenv("HWA_1"), &hi_addr, &lo_addr);
     hi_addr = (((hi_addr >> 8) & 0xFF) | ((hi_addr & 0xFF) << 8));
     hi_addr |= ((lo_addr >> 8) & 0xFF0000);
     hi_addr |= ((lo_addr << 8) & 0xFF000000);
     lo_addr = (((lo_addr >> 8) & 0xFF) | ((lo_addr & 0xFF) << 8));
     /* NOTE: hi_addr becomes HI ADDR and same reversal with lo_addr */
     REG32_W(CPSW_P1_SRC_ADDR_L0W, hi_addr);
     REG32_W(CPSW_P1_SRC_ADDR_HI,  lo_addr);

     /* Set the Switch Control registers Flow control enabled, VLAN aware */
     REG32_W(CPSW_CTRL, 0x00000200);

#if defined(CPGMAC_MODE)
     /* Port 0 belongs to default VLAN = 0x100 */
     REG32_W(CPSW_P0_VLAN, 0x00000100);
     /* Port 1 belongs to default VLAN = 0x101 */
     REG32_W(CPSW_P1_VLAN, 0x00000101);
     /* Port 2 belongs to default VLAN = 0x102 */
     REG32_W(CPSW_P2_VLAN, 0x00000102);
#endif

     /* Init the ALE registers and clear ALE */
     REG32_W(CPALE_CTRL, 0xC0000000);
     for(i=0; i<1000;i++);
     REG32_W(CPALE_CTRL, 0x80000000);

#if defined(CPGMAC_MODE)
     REG32_W(CPALE_UNK_VLAN, 0x07000007);
#else
     REG32_W(CPALE_UNK_VLAN, 0x07000007);
#endif
     REG32_W(CPALE_PRESCALE, 0x000FFFFF);
     if(conf_cpmac_if == 0)
     {
       /* Setting for port 0 into ALE */
       if (sys_getenv("HWA_0") != NULL)
       {
          decode_macaddr(sys_getenv("HWA_0"), &hi_addr, &lo_addr);
#if defined(CPGMAC_MODE)
          SetALEUnicastEntry(0, 0x100, hi_addr, lo_addr, 0x2);
#else
          SetALEUnicastEntry(0, 0x000, hi_addr, lo_addr, 0x2);
#endif
       }
     }
     else
     {
        /* Setting for port 1 into ALE */
        /* Set the ALE table */
        if (sys_getenv("HWA_1") != NULL)
        {
           decode_macaddr(sys_getenv("HWA_1"), &hi_addr, &lo_addr);
#if defined(CPGMAC_MODE)
           SetALEUnicastEntry(0, 0x101, hi_addr, lo_addr, 0x2);
#else
           SetALEUnicastEntry(0, 0x000, hi_addr, lo_addr, 0x2);
#endif
       }
     }
#if defined(CPGMAC_MODE)
     /* Set the ALE for VLAN IDs 0x100 - Port 0 and Port 2 are member */
     SetALEVlanEntry(0x3, 0x0100, 0x5, 0x5, 0x5, 0x5);

     /* Set the ALE for VLAN IDs 0x101 - Port 1 and Port 2 are member */
     SetALEVlanEntry(0x4, 0x0101, 0x6, 0x6, 0x6, 0x6);
     /* Set the ALE for VLAN IDs 0x102 - Port 2 is the member */
     SetALEVlanEntry(0x5, 0x0102, 0x7, 0x0, 0x0, 0x7);

    /*Read the MAC_PORT env variable here and set the parameter*/
    if(conf_cpmac_if == 0)
    {
        /* Set a broadcast entry */
        SetALEMultcastEntry( 0x6, 0x102, 0x0000FFFF, 0xFFFFFFFF, 0x5, 0x1);
    }
    else if(conf_cpmac_if == 1)
    {
         /* Set a broadcast entry */
         SetALEMultcastEntry( 0x6, 0x102, 0x0000FFFF, 0xFFFFFFFF, 0x6, 0x1);
    }

    else
    {
         /* Set a broadcast entry */
         SetALEMultcastEntry( 0x6, 0x102, 0x0000FFFF, 0xFFFFFFFF, 0x7, 0x1);
    }


#endif

#endif
     /* Clear out the STAT registers */
      for(i=0; i<0x90; i+=4)
        REG32_W((CPSW_STATS_BASE+i) , 0x00000000);
     /* Initialize of the Switch registers ends */

     /* Initialize the CPDMA registers */
     REG32_RMW(CPDMA_SRST, 0, 0, 1);
     for(i=0; i<=10; i++)
     {
        delay_usecs(200);
        temp = REG32_R(CPSW_SRST, 0, 0);
        if(temp == 0x0) break;
     }
     if(i>=10)
     {
        return ;
     }
#if defined(TNETV1051_BOARD)
     REG32_RMW(CPDMA_DMA_CTRL, 4, 0, 0x1);
     for(i=0; i<8; i++)
     {
         REG32_W(CPMAC_A_TX_DMAHDP(i), 0x00);
         REG32_W(CPMAC_A_RX_DMAHDP(i), 0x00);
         REG32_W(CPMAC_A_TX_DMACP(i),  0x00);
         REG32_W(CPMAC_A_RX_DMACP(i),  0x00);
     }
     /* Initialize the CPDMA registers ends */
#endif
     /* Initialize the CPMAC registers */
     for(j=0; j<2; j++)
     {
        REG32_RMW(CPMAC_MAC_RST(j), 0, 0, 1);
        for(i=0; i<=10; i++)
        {
           delay_usecs(200);
           temp = REG32_R(CPSW_SRST, 0, 0);
           if(temp == 0x0) break;
        }
        if(i>=10)
        {
           return ;
        }
#if 0
#if defined(TNETV1051_BOARD)
        /* MAC Control - Flow control enabled */
        REG32_W(CPMAC_MAC_CTRL(j),  0x48039);
        /*
            Enable TX/RX in Control registers.
        */
        REG32_W(CPMAC_A_TX_CTRL, 1);
        REG32_W(CPMAC_A_RX_CTRL, 1);
#endif
#endif
     }

#if defined (CPGMAC_MODE)
     REG32_W(CPALE_PORT_CTRL_0, (CPMAC_NOLEARN | CPMAC_PORT_FORWARD));
     REG32_W(CPALE_PORT_CTRL_1, (CPMAC_NOLEARN | CPMAC_PORT_FORWARD));
     REG32_W(CPALE_PORT_CTRL_2, (CPMAC_NOLEARN | CPMAC_PORT_FORWARD));
#else
     REG32_W(CPALE_PORT_CTRL_0, CPMAC_PORT_FORWARD);
     REG32_W(CPALE_PORT_CTRL_1, CPMAC_PORT_FORWARD);
     REG32_W(CPALE_PORT_CTRL_2, CPMAC_PORT_FORWARD);
#endif

/* Initialize the CPMAC registers -- ends */
/* VOLCANO_ARIES_BOARD ---- end    */


}
#endif

}

void decode_macaddr(const char *mac_addr, unsigned int *hi_addr,
                                          unsigned int *lo_addr) {
    char          *idx_start = (char*)mac_addr;
    char          *idx_end;
    unsigned int   ii;

    if (NULL == mac_addr) {
        *hi_addr = 0;
        *lo_addr = 0;
        return;
    }

    *hi_addr = *lo_addr = 0;
    for (ii = 0; ii < 2; ii++) {
        *hi_addr |= strtol(idx_start, &idx_end, 16) << (((2 - 1) - ii) * 8);
        idx_start = ++idx_end;
    }
    for (; ii < 6; ii++) {
        *lo_addr |= strtol(idx_start, &idx_end, 16) << (((6 - 1) - ii) * 8);
        idx_start = ++idx_end;
    }
}

#if defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)
void SetALEUnicastEntry(int index, int VLANId, unsigned int UniAddrHigh, unsigned int UniAddrLow, int portNumber)
{
      unsigned int entry;

       /* If VLAN Id == 0 then it is taken as unicast address or it is taken as VLAN unicast address */
      REG32_W(CPALE_TBLW0, UniAddrLow);
      if(VLANId)
      {
          entry = UniAddrHigh | 0x30000000  |(VLANId << 16);
          REG32_W(CPALE_TBLW1, entry);
      }
      else
      {
          entry = UniAddrHigh | 0x10000000;
          REG32_W(CPALE_TBLW1, entry);
      }
      entry = portNumber;
      REG32_W(CPALE_TBLW2, entry);

      /* Write the entry to the ALE */
      REG32_W(CPALE_TBLCTL , (0x80000000 | index));

}

void SetALEMultcastEntry(int index, int VLANId, unsigned int UniAddrHigh, unsigned int UniAddrLow, int portMask, int Super)
{
      unsigned int entry;

       /* If VLAN Id == 0 then it is taken as unicast address or it is taken as VLAN unicast address */
      REG32_W(CPALE_TBLW0, UniAddrLow);
      if(VLANId)
      {
          entry = UniAddrHigh | 0xF0000000  |(VLANId << 16);
          REG32_W(CPALE_TBLW1, entry);
      }
      else
      {
          entry = UniAddrHigh | 0xD0000000;
          REG32_W(CPALE_TBLW1, entry);
      }
      entry = portMask | (Super <<3);
      REG32_W(CPALE_TBLW2, entry);

      /* Write the entry to the ALE */
      REG32_W(CPALE_TBLCTL , (0x80000000 | index));

}

void SetALEVlanEntry(int index, int VLANId, unsigned int vlanMemberList, unsigned int unregMcastMask,
                                      int regMcastMask, int forceUntaggedEgress)
{
      unsigned int entry;

      if(VLANId)
      {
          entry =  vlanMemberList  | (unregMcastMask << 8)  | ( regMcastMask << 16)  | (forceUntaggedEgress << 24);
          REG32_W(CPALE_TBLW0, entry);
      }

      REG32_W(CPALE_TBLW1, 0x20000000 | (VLANId << 16));
      REG32_W(CPALE_TBLW2, 0x00000000);

      /* Write the entry to the ALE */
      REG32_W(CPALE_TBLCTL , (0x80000000 | index));
}


#if CPMAC_DEBUG
void dump_ale()
{
  unsigned int i, free=0,info0, info1, info2;;

  sys_printf("\n");
  for (i=0; i<1024; i++)
   {
      REG32_W(CPALE_TBLCTL , i);
      info0 = REG32_R(CPALE_TBLW0 , 31,0);
      info1 = REG32_R(CPALE_TBLW1 , 31,0);
      info2 = REG32_R(CPALE_TBLW2 , 31,0);

      if ((info0==0xfffffffe)&&((info1&0x0ffff)==0x0ffff)) continue;


     switch((info1>>28)&3)
    {
    case 0: free++;
      break;
    case 1: //MacAddress
    if (info1&(1<<8))
           sys_printf("%4d : Address: %02x%02x%02x%02x%02x%02x, Member:%03x Su=%x STP=%d\n",
                   i,(info1>>8)&0x0ff,(info1>>0)&0x0ff,(info0>>24)&0x0ff,
                   (info0>>16)&0x0ff,(info0>>8)&0x0ff,(info0>>0)&0x0ff,
                   (info2>>0)&0x7,((info2>>3)&0x1),(info1>>30)&0x3);
      else
           sys_printf("%4d : Address: %02x%02x%02x%02x%02x%02x, Port: %03x Se=%x Bl=%d\n",
                   i,(info1>>8)&0x0ff,(info1>>0)&0x0ff, (info0>>24)&0x0ff,
                   (info0>>16)&0x0ff,(info0>>8)&0x0ff,(info0>>0)&0x0ff,
                   (info2>>0)&0x3, ((info1>>30)&0x1), (info1>>31)&0x1);
      break;
    case 2: //VLAN
            sys_printf("%4d : Vlanid: %04d, UTagged: %x, Mult: %x, UMult: %x, Member: %x\n",
                 i,((info1>>16)&0x0fff),
                 ((info0>>24)&0x07),((info0>>16)&0x07),((info0>>8)&0x07),((info0>>0)&0x07));
      break;
    case 3: //VLAN-MacAddress

            if (info1&(1<<8))
            sys_printf("%4d : Address: %02x%02x%02x%02x%02x%02x, VlanId: %04d, Member: %03x\n",
                   i,(info1>>8)&0x0ff,(info1>>0)&0x0ff,
                   (info0>>24)&0x0ff,(info0>>16)&0x0ff,(info0>>8)&0x0ff,(info0>>0)&0x0ff,
                   (info1>>16)&0x0fff,
                   (info2>>0)&0x7);
      else

            sys_printf("%4d : Address: %02x%02x%02x%02x%02x%02x, VlanId: %04d, Port: %03x\n",
                   i,(info1>>8)&0x0ff,(info1>>0)&0x0ff,
                   (info0>>24)&0x0ff,(info0>>16)&0x0ff,(info0>>8)&0x0ff,(info0>>0)&0x0ff,
                   (info1>>16)&0x0fff,
                   (info2>>0)&0x3);
      break;
    default:
      break;
    }
    }
    sys_printf("\n\r"); 

}
#endif
#endif


static char macname[10];
char* getMacEnvVarName()
{
#if (CONF_NUM_MAC_PORT  == 1)
    strcpy(macname, "HWA_0");
#else
    sys_sprintf(macname, "HWA_%d", conf_cpmac_if);
#endif
    return macname;
}

void startGPIO()
{
    /* put and bring the GPIO out of reset. */
    REG32_RMW(0xa8611600, 6, 6, 0);
    delay_usecs(200);
    REG32_RMW(0xa8611600, 6, 6, 1);
    delay_usecs(200);
}

void startMDIO()
{
  if(!g_reset_swr2)    
  {
     /* put and bring the MDIO out of reset. */
     REG32_RMW(RESET_BASE, 22, 22, 0);
     delay_usecs(200);
     REG32_RMW(RESET_BASE, 22, 22, 1);
     delay_usecs(200);
   
#if defined(TNETV1051_BOARD)
    /* ToDo: Do required MDIO init */
#endif
    /* Start MDIO */
#if defined(TNETD73XX_BOARD) || defined (TNETV1020_BOARD) || defined (TNETD73XX_EMULATION) \
            || defined(TNETC4602_BOARD) || defined(TNETV1061_BOARD) || defined (TNETV1061_EMULATION) \
            || defined(TNETC4700_BOARD)
     REG32_W(0xa8611e04,
            (1 << 30) | ((_CpuFrequency / 1000000) & MASK(15, 0)));
#elif defined(TNETV1050_BOARD) || defined (TNETV1050_EMULATION)
     REG32_W(0xa8642004,
            (1 << 30) | ((_CpuFrequency / 1000000) & MASK(15, 0)));
#elif defined (VOLCANO_ARIES_BOARD)
     /* set the clock divider to 0x1FF and enable the MDIO */
     REG32_W(0xa8611e04,((1 << 30) | ( 1<<24 ) |0x1FF));
#elif defined (TNETV1051_BOARD)
     /* set the clock divider to 0x1FF and enable the MDIO */
     REG32_W(0xa8611e04,((1 << 30) | ( 1<<24 ) |0xFF));
#endif
  }
   
}

void ResetMMS(void)
{
#ifdef FFS
    bit32u memsize;
    char *cp;

    memsize = (cp =
               sys_getenv("MEMSZ")) ? strtol(cp, NULL, 10) : (_MemSize);
    memsize -= (1 * 1024 * 1024);

    _FCBCloseWriteFiles();
    minit(virt_c(CS1_BASE + (1 * 1024 * 1024)), memsize);
#endif
}

int getmonitorinfo(int *rp, char **rd, char **rt)
{

  /**rp=(MonitorMajorRev<<8)|(MonitorMinorRev);*/
    *rp = (MonitorMajorRev << 16) | (MonitorMinorRev << 8) | (IncRev);
    *rd = ( char *)__DATE__;
    *rt = ( char *)__TIME__;
    return (0);
}


void DispHex(int val)
{
#if defined(AVALANCHE_EVM3)
    sysHwI2CInit();
    evm3_i2c_led_print((char) val);
#endif /* ifdef AVALANCHE_EVM3 */
}

#if defined(SEAD2) || defined(VOLCANO_ARIES_BOARD)
void DispStr(char *val)
{
    int i;
    char *led_ptr;

    led_ptr = (char *) ASCII_DISP_BASE;
    for (i = 0; i < 8; i++) {
        if (*val) {
            *led_ptr = *val++;
        } else {
            *led_ptr = ' ';
        }
        led_ptr += ASCII_DISP_OFFSET;
    }
}
#endif /* ifdef SEAD2 */

int _wrap_bcheck(unsigned int adr1, unsigned int adr2, unsigned int len)
{
    unsigned int *ip1, *ip2;

    ip1 = (unsigned int *) adr1;
    ip2 = (unsigned int *) adr2;
    len >>= 4;
    while (len) {
        if (*ip1++ != *ip2++)
            return (FALSE);
        len--;
    }
    return (TRUE);
}

#ifdef DUAL_FFS
bit32u get2flashsize(bit32u base, bit32u size)
{
#ifdef NAND_SBL

    bit32u blocksize, i, adr1, adr2, flag;

    i = size / (1024 * 1024);
    blocksize = 64 * 1024;
    do {
        adr1 = base;
        adr2 = base + ((i * 1024 * 1024) >> 1);
        i >>= 1;
    }
    while ( ( ( flag = _wrap_bcheck(adr1, adr2, blocksize)) == TRUE ) && (i));

    if( ( i == 0 ) && ( flag == TRUE ) )
    {
      int j = 2;

      do
      {
        adr1= base;
        adr2 = base + ( ( 1024 * 1024 ) >> j );

        j += 1;
      } while ((_wrap_bcheck(adr1, adr2, blocksize)) && (j < 5));

      return ((( 1024 * 1024 ) >> ((j - 2) - (128*1024*2))));
    }

    i <<= 1;
    return ((i * 1024 * 1024) - (128*1024*2));

#else
    if(!CS0_SIZE)
        return 0;
    if(global_nand_info.maker_code == 236 )
    {
        if (global_nand_info.device_code == 241 )
        {
            return (0x8000000);
        }
    }
#if defined(TNETV1051_BOARD)
    return (0x8000000);
#endif

#endif
}
#endif
bit32u getflashsize(bit32u base, bit32u size)
{
#ifdef NAND_SBL
    if(global_nand_info.maker_code == 236 )
    {
        if (global_nand_info.device_code == 241 )
        {
            return 0x8000000;
        }
    }
#if defined(TNETV1051_BOARD)
    return 0x8000000;/*return default size*/
#endif

#else
    bit32u blocksize, i, adr1, adr2, flag;

    i = size / (1024 * 1024);
    blocksize = 64 * 1024;
    do {
        adr1 = base;
        adr2 = base + ((i * 1024 * 1024) >> 1);
        i >>= 1;
    }
    while ( ( ( flag = _wrap_bcheck(adr1, adr2, blocksize)) == TRUE ) && (i));

    if( ( i == 0 ) && ( flag == TRUE ) )
    {
      int j = 2;

      do
      {
        adr1= base;
        adr2 = base + ( ( 1024 * 1024 ) >> j );

        j += 1;
      } while ((_wrap_bcheck(adr1, adr2, blocksize)) && (j < 5));

#ifdef FLASH_DEBUG
      printf( "Flash size: %d\n", (( 1024 * 1024 ) >> (j - 2)) );
#endif
      return (( 1024 * 1024 ) >> (j - 2));
    }

    i <<= 1;
#ifdef FLASH_DEBUG
      printf( "Flash size: %d\n", (i * 1024 * 1024) );
#endif

    return (i * 1024 * 1024);
#endif
}

void SetRefClkDiv(int val, int verbose)
{
    SioFlush();

    /* Set the SysFrequency divider */
    REG32_W(0xa8610a20, (val & 0x1F) << 16);
    clkc_delay(10100);

#if defined (SEAD2)
    GetFrequencyAndInitSio();
#endif
}

void SetRefClkPll(int v, int verbose)
{
    SioFlush();
#if defined (SEAD2)
    GetFrequencyAndInitSio();
#endif
}

int GetCpuFreq(void)
{
    return (_CpuFrequency);
}

int GetSystemFreq(void)
{
    return (_SysFrequency);
}

int GetPBusFreq(void)
{
    return (_SysFrequency/2);
}

#if !defined(NAND_SBL) || defined(DUAL_FFS)
int GetFlashOps(Adam2Flash * ops)
{
    if (ops) {
        ops->FWBGetBlockSize = FWBGetBlockSize;
        ops->FWBErase = FWBErase;
        ops->FWBOpen = FWBOpen;
        ops->FWBWriteByte = FWBWriteByte;
        ops->FWBClose = FWBClose;
        ops->FWBUnLock = FWBUnLock;
        ops->FWBLock = FWBLock;
        ops->FWBGetNextBlock = FWBGetNextBlock;
    }
    return (sizeof(Adam2Flash));
}
#endif

#if defined (NAND_SBL) || defined (DUAL_FFS)
int GetFlashOps_nand(Adam2Flash * ops)
{
    if (ops) {
        ops->FWBGetBlockSize = FWBGetBlockSize_nand;
        ops->FWBErase = FWBErase_nand;
        ops->FWBOpen = FWBOpen_nand;
        ops->FWBWriteByte = FWBWriteByte_nand;
        ops->FWBClose = FWBClose_nand;
        ops->FWBUnLock = FWBUnLock_nand;
        ops->FWBLock = FWBLock_nand;
        ops->FWBGetNextBlock = FWBGetNextBlock_nand;
    }
    return (sizeof(Adam2Flash));
}
#endif
#define rdtscl(dest) \
__asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest))

void delay_usecs(unsigned int usecs)
{

    unsigned int i;
    for (i = 0; i < usecs; i++) {
        t_clear();
        while ( (unsigned int)t_get() < (_CpuFrequency / (2 * 1000000)));
    }
}

void printLastResetCause ()
{
    sys_printf("Last reset cause: ");
    switch((RESET_STATUS & 0x7)) {
        case 0x0: sys_printf("Hardware reset (Power-on reset)\n");
        break;
        case 0x1: sys_printf("Software reset (memory controller also reset)\n");
        break;
        case 0x2: sys_printf("Watchdog reset\n");
        break;
        case 0x3: sys_printf("Software reset (memory controller not reset)\n");
        break;
        case 0x4: sys_printf("Software reset (switch subsystem not reset)\n");
        break;
        default:  sys_printf("Unknown\n");
    }

}


/*
 * This function depends on the global variable - linkTimeOut
 * The idea behind this function is to wait for link come up after
 * reset before networking transactions start. This waiting should be
 * done only once after reset and not everytime a networking
 * transaction starts.
 */
void waitForLinkAfterReset()
{

    if (linkTimeOut == -1) {    /* did not wait for link still */

        if (sys_getenv("LINK_TIMEOUT") == NULL) {
            linkTimeOut = 3;    /* 3 Seconds default wait for link */
        } else {
            linkTimeOut = strtol(sys_getenv("LINK_TIMEOUT"), NULL, 10);
        }
        delay_usecs(linkTimeOut * MHZ(1));
    }

}

/* code to sense the link */
#if defined (TNETD73XX_BOARD) || defined (TNETV1050_BOARD) || defined (SEAD2) \
            || defined (TNETV1020_BOARD) || defined(TNETC4602_BOARD) || defined (TNETV1061_BOARD) \
|| defined(TNETC4700_BOARD) || defined (VOLCANO_ARIES_BOARD) || defined (TNETV1051_BOARD)

#define MDIO_GO             0x80000000
#define MDIO_WRITE          0x40000000
#define MDIO_READ           0x00000000
#define MII_STATUS_REG      1

/* MDIO definitions */

#if defined(TNETD73XX_BOARD) || defined (TNETV1020_BOARD) || defined (TNETD73XX_EMULATION) \
                 || defined (TNETC4602_BOARD) || defined (TNETV1061_BOARD) || defined (TNETV1061_EMULATION) \
|| defined(TNETC4700_BOARD) || defined (VOLCANO_ARIES_BOARD) || defined (TNETV1051_BOARD)

#define MDIO_BASE           0xa8611e00
#endif

#if defined(TNETV1050_BOARD) || defined (TNETV1050_EMULATION)
#define MDIO_BASE           0xa8642000
#endif

#define MDIO_CTRL               (( volatile unsigned int) (MDIO_BASE + 0x04))
#define MDIO_ACK                (*(volatile UINT32*)(MDIO_BASE + 0x08))
#define MDIO_USRACCESS(channel) (*(volatile UINT32*)(MDIO_BASE + (0x80 + (8*(channel)))))

void MdioWaitForAccessComplete();
void switchMDIOMode();

void MdioWaitForAccessComplete()
{
    while ((MDIO_USRACCESS(conf_cpmac_if) & MDIO_GO) != 0);
}

#if !defined(VOLCANO_ARIES_BOARD)
UINT16 MdioRd(int phyAddr, int regNum)
{
    MDIO_USRACCESS(conf_cpmac_if) =
            MDIO_GO | MDIO_READ | ((regNum & 0x1F) << 21) |
            ((phyAddr & 0x1F) << 16);
    MdioWaitForAccessComplete();
    return (MDIO_USRACCESS(conf_cpmac_if) & 0xFFFF);
}
#else
UINT16 MdioRd(int PhyAddr, int RegNum)
{
  MdioWaitForAccessComplete();
  MDIO_USRACCESS(conf_cpmac_if) =
            MDIO_GO | MDIO_READ | ((RegNum & 0x1F) << 21) |
            ((PhyAddr & 0x1F) << 16);
  MdioWaitForAccessComplete();
  if(MDIO_USRACCESS(conf_cpmac_if)&(1<<29))
    return (MDIO_USRACCESS(conf_cpmac_if)&0xFFFF);    /* Return reg value on successful ACK */
  return ((unsigned short)0xFFFF);
}
#endif

void MdioWr(UINT16 phyAddr, UINT16 regNum, UINT16 data)
{
    MdioWaitForAccessComplete();
    MDIO_USRACCESS(conf_cpmac_if) =
            MDIO_GO | MDIO_WRITE | ((regNum & 0x1F) << 21) |
            ((phyAddr & 0x1F) << 16) | (data & 0xFFFF);
}

#if defined (VOLCANO_ARIES_BOARD) /* LAVA no gig code starts */
void MdioSetBits(int PhyAddr, int RegNum, UINT16 BitMask)
{
  MdioWr( PhyAddr, RegNum, MdioRd(PhyAddr,RegNum) | BitMask );
}

int IsMarvellGigPhy(int PhyAddr)
{
  if( MdioRd(PhyAddr,2) == 0x0141 )       /* Read OUI MSbs */
  {
    unsigned short PhyID;

    PhyID = MdioRd(PhyAddr,3) & ~0xF;     /* Read OUI LSb and model number (ignore rev number) */
    if( PhyID==0x0CC0 || PhyID==0x0CD0 )
      return TRUE;
  }
  return FALSE;
}

unsigned int MdioBase=0, MdioResetBit=0, MdioResetMask=0;

int lava_no_gig()
{
  int i;

  int FirstTime=TRUE;
  UINT32 ActivePhys;

  /* Delay for getting the PHYs */
  for(i=0; i<=0x100000; i++);
  MdioBase = MDIO_BASE;
  ActivePhys = REG32_R(CPMDIO_ALIVE, 31, 0);
  sys_printf("   ActivePhys = 0x%08x\n",ActivePhys);
  for(i=0;i<32;i++)
  {
    if( ActivePhys & (1<<i) )
    {
      if( IsMarvellGigPhy(i) )
      {
        if(FirstTime)
        {
          sys_printf("Disabling gigabit capabilities for LAVA Marvell phy(s): %d",i);
          FirstTime=FALSE;
        }
        else
          sys_printf(",%d",i);

          MdioWr(i,9,0);              /* Set PHY register 9 to 0 (disables gigbit capability) */
          MdioSetBits(i,16,(1<<11));  /* Set PHY register 16, bit 11 for CRS */
      }
    }
  }
  sys_printf("\n");
  return 0;
}
#endif /*lava_no_gig code end here*/

#if CPMAC_DEBUG
int MdioProbe()
{
    int phy_addr, status;

    sys_printf("Mdio Ack register: %08x\n",MDIO_ACK);
    sys_printf("Cpmac Interface: %x\n",conf_cpmac_if);

    for (phy_addr = 0; phy_addr < 32; phy_addr++) {
      if (MDIO_ACK & (1 << phy_addr)) {
            status = MdioRd(phy_addr, MII_STATUS_REG);
            sys_printf("Phy addr = %d  ", phy_addr);
            sys_printf("Link state = %d\n", (status & 0x4) >> 2);
       }
    }
}
#endif
int MdioGetLinkState(unsigned int phy_mask)
{
    unsigned int act_phy, phy_addr = 0, link_state = FALSE;

#if CPMAC_DEBUG
    sys_printf("Getting Link state of interface :%d\n", conf_cpmac_if);
    sys_printf("Ack reg                         :%08x\n", MDIO_ACK);
    sys_printf("Configured PHY mask             :%08x\n", phy_mask);
#endif

    act_phy =  (MDIO_ACK & phy_mask);

    if (act_phy) {
        /* find the phy number */
#if CPMAC_DEBUG
        sys_printf("Active Phy mask             :%08x\n", act_phy);
#endif
        while(act_phy) {
            while(!(act_phy & 0x1)) {
                phy_addr++;
                act_phy >>= 1;
            }
            link_state = ((MdioRd(phy_addr, MII_STATUS_REG) & 0x4) >> 2);
#if CPMAC_DEBUG
            sys_printf("Phy Address: %u Link State      :%u\n", phy_addr, link_state);
#endif
            if(link_state == TRUE) {
                break;
            } else {
                /* Go to next phy. */
                act_phy >>= 1;
                phy_addr++;
            }
        }
    }
    return link_state;
}

unsigned int MdixPortState;

BOOL lk_st(u4 itf)
{
    if (conf_cpmac_if == 0) {

#ifdef TNETD73XX_BOARD
       UINT32 cvr = (*(volatile UINT32*)0xa8610914 );

       /* On AR7 PG 2.0 boards, see if the link is present on EPHY, if not,
        * switch the MDI mode */
       if( ( ( ( cvr >> 20) & 0xF ) >= 2 ) || is_ohio_chip )
       {
            if(MdioGetLinkState(MAC0_INT_PHY_MASK) != TRUE) {
                switchMDIOMode();
                linkTimeOut = -1; /* this will enable the timeout */
                waitForLinkAfterReset();
            }
       }
#endif

#if defined(TNETV1061_BOARD) || defined(TNETV1061_EMULATION)
        if(MdioGetLinkState(MAC0_INT_PHY_MASK) != TRUE) {
        switchMDIOMode();
        linkTimeOut = -1;  /*this will enable the timeout*/
        waitForLinkAfterReset();
    }
#endif

#if defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)
if(!Phy0 && !conf_cpmac_if)
        return MdioGetLinkState(MAC0_EXT_PHY_MASK);
else
        return MdioGetLinkState(MAC0_INT_PHY_MASK);
#else
        return MdioGetLinkState(MAC0_INT_PHY_MASK);
#endif        

    } else if (conf_cpmac_if == 1) {
#if defined(TNETV1061_BOARD) || defined(TNETV1061_EMULATION)
        if(MdioGetLinkState(MAC1_INT_PHY_MASK) != TRUE) {
            switchMDIOMode();
            linkTimeOut = -1;  /*this will enable the timeout*/
            waitForLinkAfterReset();
        }
#endif

#if defined(VOLCANO_ARIES_BOARD) || defined(TNETV1051_BOARD)
if(!Phy1 && conf_cpmac_if)
        return MdioGetLinkState(MAC1_EXT_PHY_MASK);
else
        return MdioGetLinkState(MAC1_INT_PHY_MASK);
#else
        return MdioGetLinkState(MAC1_INT_PHY_MASK);
#endif        
    }

    return 0;
}

#define PORT_STATE_MDIX 1
#define PORT_STATE_MDIO 0

#if defined(TNETD73XX_BOARD)

void switchMDIOMode() {

    if(MdixPortState == PORT_STATE_MDIO) {
        /* switch to mdix mode */
        GPIO_OUT |= 0x10000000;
        MdixPortState = PORT_STATE_MDIX;
    } else {
        /* switch to mdio mode */
        GPIO_OUT &= ~0x10000000;
        MdixPortState = PORT_STATE_MDIO;
    }
}
#endif

#if defined(TNETV1061_BOARD) || defined(TNETV1061_EMULATION)

void switchMDIOMode() {

    if(conf_cpmac_if == 0)  /*  MAC0_INT_PHY_MASK - LAN_PHY */
    {
        if(MdixPortState == PORT_STATE_MDIO) {
            /* switch to mdix mode */
            GPIO_OUT = (GPIO_OUT & 0xf0ffffff) | 0x01000000 ;
            MdixPortState = PORT_STATE_MDIX;
        } else {
            /* switch to mdio mode */
            GPIO_OUT &= ~0x0f000000;
            MdixPortState = PORT_STATE_MDIO;
        }

    } else if (conf_cpmac_if == 1){
            if(MdixPortState == PORT_STATE_MDIO) {    /* MAC1_INT_PHY_MASK -  PC_PHY    */
            /* switch to mdix mode */
                    GPIO_OUT = (GPIO_OUT & 0x0fffffff) | 0x10000000 ;
                    MdixPortState = PORT_STATE_MDIX;
            } else {
                /* switch to mdio mode */
                GPIO_OUT &= ~0xf0000000;
                MdixPortState = PORT_STATE_MDIO;
        }

    }

}
#endif
#endif

/* PHY Trim implementation for TNETD73XX and TNETV1050 */
#if defined( TNETD73XX_BOARD ) || defined( TNETV1050_BOARD )||defined(TNETV1051_BOARD)

#if defined(TNETD73XX_BOARD)

#define CHIP_ID_REG_ADDR             0xA8610914
#define DIE_ID_LOW_ADDR              0xA8610918
#define DIE_ID_HIGH_ADDR             0xA861091C

#elif defined (TNETV1050_BOARD)

#define CHIP_ID_REG_ADDR             0xA8610924
#define DIE_ID_LOW_ADDR              0xA8610928
#define DIE_ID_HIGH_ADDR             0xA861092C

#endif

#define TNETD73XX_EPHY_TRIM_VALUE    0xAC  /* SANGAM constant trim value */
#define TNETV1050_EPHY_TX_SLOPES     0x04  /* TITAN constant TX slopes value */

/******************************************************************************
*
* Function Name : WriteEphyTestRegs
*
* Purpose       : Program EPHY registers
*
* Parameters    : PHY number, register ID, register value
*
******************************************************************************/
static void WriteEphyTestRegs(UINT16 MiiPhyNum, UINT16 TestCfgRegNum, UINT16 CfgValue)
{
  /* This sequence will change the PHY to test mode */
  MdioWr(MiiPhyNum, 20, 0x0000);
  MdioWr(MiiPhyNum, 20, 0x0400);
  MdioWr(MiiPhyNum, 20, 0x0000);
  MdioWr(MiiPhyNum, 20, 0x0400);

  /* Write register value */
  MdioWr(MiiPhyNum, 23, 0x8100 | (CfgValue & 0xFF));
  MdioWr(MiiPhyNum, 20, 0x4400 | (TestCfgRegNum & 0xFF));

  /* Back to normal mode */
  MdioWr(MiiPhyNum, 20, 0x0000);
}

#ifdef TNETV1050_BOARD
/******************************************************************************
*
* Function Name : GetTrimValue
*
* Purpose       : Get PHY trim values from die id register
*
* Parameters    : (output) Phy1 value, Phy2 value (NULL if not exists)
*
******************************************************************************/
static void GetTrimValue( UINT16 *Phy0TrimValue, UINT16 *Phy1TrimValue )
{
  UINT32 *DieId = (UINT32 *)DIE_ID_LOW_ADDR;
  UINT16 TrimValue;
  UINT16 DieIdCode;
/*
PC PHY1         Die ID Code       Change to output
Trim Setting                          level
Code
000             000 (changed 7/13)  +75-85 mV
001             001                 +55-65 mV
010             010                 +35-45 mV
011             100 (changed 7/13)  +15-25 mV
100             011 (changed 7/13)  Default 0
101             101                 -15-25 mV
110             110                 -35-45 mV
111             111                 -55-65 mV


PC PHY1         Die ID Code       Change to output
Trim Setting                          level
Code
000             000 (changed 7/13)  +75-85 mV
001             011 (changed 7/13)  +55-65 mV
010             010                 +35-45 mV
011             001 (changed 7/13)  +15-25 mV
100             100                 Default 0
101             101                 -15-25 mV
110             110                 -35-45 mV
111             111                 -55-65 mV
*/

#ifdef EPHY_DEBUG
   sys_printf("DIDR1: 0x%X\r\n", *DieId);
#endif

  /* Extract bits 14,15 */
  DieIdCode = (UINT16)((*DieId >> 14) & 0x3);

  /* Extract bit 49 */
  DieId = (UINT32 *)DIE_ID_HIGH_ADDR;

#ifdef EPHY_DEBUG
   sys_printf("DIDR2: 0x%X\r\n", *DieId);
#endif

  DieIdCode |= (UINT16)(((*DieId >> 17) & 0x1) << 2);

  /* Change in the table, July 13th 2004 */
  if(DieIdCode == 4)
    TrimValue = 3;
  else
    if(DieIdCode == 3 )
      TrimValue = 4;
    else
      TrimValue = DieIdCode;

  /* Calculate trim value - Add 0x08, shift to the right place.
     Add also TX slopes value */
  *Phy0TrimValue = (((TrimValue + 0x08) << 4) | TNETV1050_EPHY_TX_SLOPES);

#ifdef EPHY_DEBUG
   sys_printf("PHY0 Trim value: 0x%X\r\n", *Phy1TrimValue);
#endif

  /* Extract bits 50 - 52 */
  DieIdCode = (UINT16)((*DieId >> 18) & 0x7);

  /* Change in the table, July 13th 2004 */
  if(DieIdCode == 1)
    TrimValue = 3;
  else
    if( DieIdCode == 3 )
      TrimValue = 1;
    else
      TrimValue = DieIdCode;


  *Phy1TrimValue = (((TrimValue + 0x08) << 4) | TNETV1050_EPHY_TX_SLOPES);

#ifdef EPHY_DEBUG
   sys_printf("PHY1 Trim value: 0x%X\r\n", *Phy2TrimValue);
#endif

}
#endif

/******************************************************************************
*
* Function Name     : ConfigEphyTrim
*
* Purpose           : Configure EPHY Trim settings
*
* Parameters        : None
*
******************************************************************************/
static void ConfigEphyTrim(void)
{
#ifdef TNETV1050_BOARD             /* TITAN */

  UINT16 Phy0TrimValue;
  UINT16 Phy1TrimValue;

  /* Get trim value */
  GetTrimValue(&Phy0TrimValue, &Phy1TrimValue);

  /* Configure EPHY 0 */
  WriteEphyTestRegs(30, 0x16, Phy0TrimValue);

  /* Configure EPHY 1 */
  WriteEphyTestRegs(31, 0x16, Phy1TrimValue);

#elif defined(TNETD73XX_BOARD)   /* SANGAM */

  /* Configure EPHY 0 */
  WriteEphyTestRegs(31, 0x16, TNETD73XX_EPHY_TRIM_VALUE);

#endif
#if defined(TNETV1051_BOARD)

#define APP_DIDR_LO 0xA8610938
   UINT32 TxAmp[8] = {0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf}; /* Lookup value for Tx amp*/
   UINT32 TxSlop[4] = {0x0,0x4,0x8,0xc}; /*Lookup value for Tx slope*/
   UINT32 Tx_cfg,tx_amp,tx_slope;
   UINT32 PHY_DIDR_LO;
 
   /* Read Die ID register to detemine phy trim value to set*/
   PHY_DIDR_LO = *(bit32u *)APP_DIDR_LO;
   
   /*check whether Phy Trim is programmed, if not programmed then return */
   if((PHY_DIDR_LO & 0x400) == 0)
   return;
   
   /*In case of soft reset don't program Phy Trim*/
   if(g_reset_swr2)
    return;

   /*only for internal phy*/
   if(Phy0||Phy1)
   {
     /*find Tx amplitude & slope for LAN Phy*/
     if(conf_cpmac_if == 0)
     { 
       tx_amp   = PHY_DIDR_LO & 0x7;
       tx_slope = (PHY_DIDR_LO & 0xC0) >> 6;
     }
     /*find Tx amplitude & slope for PC Phy*/
     else 
     {
       tx_amp   = (PHY_DIDR_LO & 0x38) >> 3;
       tx_slope = (PHY_DIDR_LO & 0x300) >> 8;
     }

     /*Program phy trim Value*/ 
     Tx_cfg = (TxAmp[tx_amp]<< 4)|TxSlop[tx_slope];
     /* Configure EPHY 0 */
     if(conf_cpmac_if == 0)
        WriteEphyTestRegs(15, 0x16, Tx_cfg);
     else
     /*Configure EPHY 1 */
        WriteEphyTestRegs(31, 0x16, Tx_cfg);
   }
  else
  /*Do not set Phy Trim for Gig Phy(external Phy)*/
   return;
#endif
}
#endif

void resetExtSwitch(void)
{
#if ( EXTERNAL_PHY_PORT0 ) || ( EXTERNAL_PHY_PORT1 )
  /* GPIO out of reset */
  REG32_RMW(RESET_BASE, 6, 6, 0x01);
  /* Put the EPhy Duplex pin in GPIO mode */
  GPIO_EN |= (1 << EXTERNAL_PHY_RESET_GPIO);

  /* set the Pin direction to out */
  GPIO_DIR &= ~(1 << EXTERNAL_PHY_RESET_GPIO);

  /* make reset low */
  GPIO_OUT &= ~(1 << EXTERNAL_PHY_RESET_GPIO);

  /* wait 200 Msecs */
  delay_usecs(KHZ(200));

  /* make reset line high again */
  GPIO_OUT |= (1 << EXTERNAL_PHY_RESET_GPIO);
#endif
}

#if defined(TNETV1051_BOARD)
void config_ext_phy_leds(void)
{
  int i, phynum = 0, phyaddr[2]={MAC0_EXT_PHY_MASK,MAC1_EXT_PHY_MASK};
  
    if(!Phy0 || !Phy1)
    {
       /* Set LED config register for Agere PHYs */
       for(i=0;i<2;i++)
       { 
          while(!(phyaddr[i] & 0x1))
          {
             phynum++;
             phyaddr[i] >>= 1;
          }
          MdioWr(phynum, 28,0xF4FA);
          phynum = 0;
       }
    }
}

UINT8 get_soc_variant(void)
{
    UINT32 reg_val;
    static BOOL first_time = 1;
    static UINT8 soc_var;
    
    if(first_time)
    {
        reg_val = APP_DIDR_HI;
        soc_var = (UINT8) reg_val & 3;
        first_time = 0;
    }
    
    return(soc_var);
}
#endif 

#if defined(TNETV1050_BOARD)
int is_soc_tnetv1057(void)
{
    #define DIE_ID_HIGH_ADDR             0xA861092C
    UINT32 tnv1057_id;
    char *tmp;

    /* If TNV1057_DEBUG is an environment variable 
     * that determines whether TNV1057 emulation and DSP 
     * clock test are enabled or disabled. 
     *      1 = TNV1057 emulation mode
     *      2 = DSP clock test (For this value, the chip must be a real 1057 )
     *      3 = TNV1057 emulation mode AND DSP clock test
     */
    if((tmp=sys_getenv("TNV1057_DEBUG")) != NULL)
    {
        if((tmp[0] == '1') || (tmp[0] == '3') )
            return 1;
    }

/* Check eFuse DieID bit 53 (Titan register 0x0861:092C, bit 21); */
    tnv1057_id = REG32_R(DIE_ID_HIGH_ADDR,21,21);

    if(tnv1057_id)
 	return 1;
    else
	return 0;

}


void printChipdbgInfo(void)
{
    volatile bit32u mips32;
  bit32u sysclk_in = 1, cpuclk_in = 1;
    volatile bit32u *pSCLKCR, *pS_CLKMD, *pMIPSCR, *pMIPSMD, *pDieID_HI, *pBOOTCR;

    pSCLKCR = (bit32u *)0xa8610A20;
    pS_CLKMD = (bit32u *)0xa8610A30;
    pMIPSCR = (bit32u *)0xa8610A40;
    pMIPSMD = (bit32u *)0xa8610A50;
    pDieID_HI = (bit32u *)0xA861092C;
    pBOOTCR = (bit32u *)BOOTCR;

    sys_printf(" BOOTCR (0xa8611a00) = 0x%08X\n", *pBOOTCR);

    mips32 = is_soc_tnetv1057();
    if(mips32)
        sys_printf("\n This is TNETV1057 SOC");

    sys_printf("\n DieID_high is 0x%08X", *pDieID_HI);

    sysclk_in = get_base_frequency(0);      /* Sys clk */
    sys_printf("\n Base Sys Clock frequency %d\n", sysclk_in);

    cpuclk_in = get_base_frequency(1);      /* CPU clk */
    sys_printf("\n Base CPU Clock frequency %d\n", cpuclk_in);

    cfg_cpufreq(0,0, TRUE);

    sys_printf(" SCLKCR (08610A20 bit 0-4 DIV) = 0x%08X\n", *pSCLKCR);
       
    sys_printf(" S_CLKMD (08610A30 bit 12-15 MUL) = 0x%08X\n", *pS_CLKMD);
       
    sys_printf(" MIPSCR (08610A40 bit 0-4 DIV) = 0x%08X\n", *pMIPSCR);
       
    sys_printf(" MIPSMD (08610A50 bit 12-15 MUL) = 0x%08X\n", *pMIPSMD);
    sys_printf(" MEMSZ=0x%08X \n", _MemSize);
    
}
#endif

    

