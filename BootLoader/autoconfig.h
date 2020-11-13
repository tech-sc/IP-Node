/****************************************************************************
**-------------------------------------------------------------------------**
**                            ****                                         **
**                            ****                                         **
**                            ******o***                                   **
**                      ********_///_****                                  **
**                      ***** /_//_/ ****                                  **
**                       ** ** (__/ ****                                   **
**                           *********                                     **
**                            ****                                         **
**                            ***                                          **
**                                                                         **
**         Copyright (c) 1998-2005 Texas Instruments Incorporated          **
**                        ALL RIGHTS RESERVED                              **
**                                                                         **
**-------------------------------------------------------------------------**
*****************************************************************************/
/****************************************************************************
 * FILE PURPOSE:    Board hardware configuration parameters
 ****************************************************************************
 * FILE NAME:       autoconfig.h
 *
 * DESCRIPTION:     Board hardware configuration parameters.
 *                  Autogenerated file. Do not modify.
 ***************************************************************************/
#ifndef __AUTO_CONFIG_H__
#define __AUTO_CONFIG_H__
#ifndef TNETV1050_BOARD
#define TNETV1050_BOARD
#endif
#ifndef BOARD
#define BOARD TNETV1050SDB
#endif
#ifndef TNETV1050SDB
#define TNETV1050SDB TNETV1050SDB
#endif
/****************************************************************************
 * Clock frequencies configuration
 ****************************************************************************/
#define CONF_REFXTAL_FREQ	
#define CONF_AFEXTAL_FREQ	
#define CONF_OSCIN_FREQ	
#define CONF_XTAL1IN_FREQ	
#define CONF_XTAL2IN_FREQ	
#define CONF_XTAL3IN_FREQ	
#define CONF_AUDIO_FREQ	
#define CONF_ALT_FREQ	
#define CONF_CPU_FREQ	
#define CONF_SYS_FREQ	
#define CONF_MIPS_FREQ	
#define CONF_PBUS_FREQ	
/****************************************************************************
 * SDRAM and FLASH Memory sizes configuration
 ****************************************************************************/
#define CONF_SDRAM_SZ	
#define CONF_FLASH_SZ	
#define CONF_HEAP_SZ	
/****************************************************************************
 * EMIF configuration
 ****************************************************************************/
#define EMIF_SDRAMCFG_NM	
#define EMIF_SDRAMCFG_PA	
#define EMIF_SDRAMCFG_IBANK	
#define EMIF_SDRAMCFG_EBANK	
#define EMIF_SDRAMCFG_PAGESIZE	
#define EMIF_SDRAM_REFRESH_INTERVAL	
#define EMIF_ASYNC_WAITCYCLE_MAX_EXT_WAIT	
#define EMIF_ASYNC_WAITCYCLE_WAIT_POL	
#define EMIF_ASYNC_BANK_1_SS	
#define EMIF_ASYNC_BANK_1_EW	
#define EMIF_ASYNC_BANK_1_W_SETUP	
#define EMIF_ASYNC_BANK_1_W_STROBE	
#define EMIF_ASYNC_BANK_1_W_HOLD	
#define EMIF_ASYNC_BANK_1_R_SETUP	
#define EMIF_ASYNC_BANK_1_R_STROBE	
#define EMIF_ASYNC_BANK_1_R_HOLD	
#define EMIF_ASYNC_BANK_1_TA	
#define EMIF_ASYNC_BANK_1_ASIZE	
#define EMIF_ASYNC_BANK_2_SS	
#define EMIF_ASYNC_BANK_2_EW	
#define EMIF_ASYNC_BANK_2_W_SETUP	
#define EMIF_ASYNC_BANK_2_W_STROBE	
#define EMIF_ASYNC_BANK_2_W_HOLD	
#define EMIF_ASYNC_BANK_2_R_SETUP	
#define EMIF_ASYNC_BANK_2_R_STROBE	
#define EMIF_ASYNC_BANK_2_R_HOLD	
#define EMIF_ASYNC_BANK_2_TA	
#define EMIF_ASYNC_BANK_2_ASIZE	
#define EMIF_ASYNC_BANK_3_SS	
#define EMIF_ASYNC_BANK_3_EW	
#define EMIF_ASYNC_BANK_3_W_SETUP	
#define EMIF_ASYNC_BANK_3_W_STROBE	
#define EMIF_ASYNC_BANK_3_W_HOLD	
#define EMIF_ASYNC_BANK_3_R_SETUP	
#define EMIF_ASYNC_BANK_3_R_STROBE	
#define EMIF_ASYNC_BANK_3_R_HOLD	
#define EMIF_ASYNC_BANK_3_TA	
#define EMIF_ASYNC_BANK_3_ASIZE	
#define EMIF_ASYNC_BANK_4_SS	
#define EMIF_ASYNC_BANK_4_EW	
#define EMIF_ASYNC_BANK_4_W_SETUP	
#define EMIF_ASYNC_BANK_4_W_STROBE	
#define EMIF_ASYNC_BANK_4_W_HOLD	
#define EMIF_ASYNC_BANK_4_R_SETUP	
#define EMIF_ASYNC_BANK_4_R_STROBE	
#define EMIF_ASYNC_BANK_4_R_HOLD	
#define EMIF_ASYNC_BANK_4_TA	
#define EMIF_ASYNC_BANK_4_ASIZE	
#define EMIF_SDRAMCFG_NM_16	
#define EMIF_ASYNC_BANK_1_ASIZE_16	
#define EMIF_ASYNC_BANK_2_ASIZE_16	
#define EMIF_ASYNC_BANK_3_ASIZE_16	
#define EMIF_ASYNC_BANK_4_ASIZE_16	
/****************************************************************************
 * MAC ports configuration
 ****************************************************************************/
#define CONF_NUM_MAC_PORT	
#define EXTERNAL_PHY_PORT0	
#define MAC0_EXT_PHY_MASK	
#define MAC0_INT_PHY_MASK	
#define EXTERNAL_PHY_PORT1	
#define MAC1_EXT_PHY_MASK	
#define MAC1_INT_PHY_MASK	
#define EXTERNAL_PHY_RESET_GPIO	
/****************************************************************************
 * Cache configuration
 ****************************************************************************/
#define CONF_CACHE	
/****************************************************************************
 * Miscellaneous configuration (if applicable)
 ****************************************************************************/
#ifndef TFTP
#define TFTP 1
#endif
#ifndef TFTP_FAILOVER_SUPPROT
#define TFTP_FAILOVER_SUPPROT 1
#endif
#ifndef DHCP_SUPPORT
#define DHCP_SUPPORT 1
#endif
#ifndef TIBINARY_SUPPORT
#define TIBINARY_SUPPORT 1
#endif
#endif /* __AUTO_CONFIG_H__ */
