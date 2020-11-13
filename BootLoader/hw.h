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
 * FILE PURPOSE:    SoC related definitions
 *******************************************************************************
 * FILE NAME:       hw.h
 *
 * DESCRIPTION:     SoC related definitions
 *
 ******************************************************************************/

#ifndef _HW_H_
#define _HW_H_

#ifdef _STANDALONE
#include <psbl/sysconf.h>
#endif

#define EMIF_SDRAM_BASE           0x94000000
#define EMIF_SDRAM_MAX_SIZE       0x08000000 /*0x08000000 or 0x01000000*/

#ifdef BE
#define ASCII_DISP_BASE           0xbc4001c3
#else
#define ASCII_DISP_BASE           0xbc4001c0
#endif

#define ASCII_DISP_OFFSET         8


#if defined(SEAD2)  /* The board being used is SEAD2 */

#define SIO1__BASE     0xbc700000
#define SIO1__OFFSET   8
#define SIO1__EN       1
#define SIO1__FDEN     1
#define SIO1__RSTMASK  0x00
#define SIO0__BASE     0xa8610e00
#define SIO0__OFFSET   4
#define SIO0__EN       1
#define SIO0__FDEN     1
#define SIO0__RSTMASK  0x01

#else

#define SIO_OFFSET     4
#if defined(VOLCANO_ARIES_BOARD)
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x02
#define SIO0_BASE     0xa8610e00
#define SIO0_RSTMASK  0x01
#elif defined(TNETV1051_BOARD)
#define SIO0_BASE     0xa8613f00
#define SIO0_RSTMASK  0x80
#define SIO2_BASE     0xa8610e00
#define SIO2_RSTMASK  0x01
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x02
#elif defined(TNETV1050_BOARD) || defined(TNETV1061_BOARD)
#define SIO1_BASE     0xa8610f00
#define SIO1_RSTMASK  0x42
#define SIO0_BASE     0xa8610e00
#define SIO0_RSTMASK  0x01
#endif/*#if define (VOLCANO_ARIES_BOARD)*/

#endif /* ifdef SEAD2 */

#define CS0_BASE 0xb0000000

#define _CS0_SIZE CONF_FLASH_SZ
#ifndef _ASSEMBLER_
extern unsigned int CS0_SIZE;
#endif


#define CS1_BASE 0xb4000000
#define CS1_SIZE CONF_SDRAM_SZ

#define CS3_BASE 0xbc000000
#ifdef NAND_SBL
#define _CS3_SIZE (16*1024*1024)
#else
#define _CS3_SIZE (128*1024*1024)
#endif

#define CS4_BASE 0xbd000000

#ifdef DUAL_FLASH
#define CS4_SIZE _CS0_SIZE
#else
#define CS4_SIZE (16*1024*1024)
#endif

#define CS5_BASE 0xbe000000
#define CS5_SIZE (16*1024*1024)

#if defined(TNETV1051_BOARD)
#ifndef NAND_SBL
#ifndef EMIF_BASE
#define EMIF_BASE        0xA8610800
#endif
#define EMIF_REV        (*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(EMIF_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(EMIF_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(EMIF_BASE+0x1c))

#define ASYNC_BASE       0xA8610800
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#else
/* ARIES SDB */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(DDR_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(DDR_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(DDR_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(DDR_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))
#endif /* NAND_SBL */

#elif defined(VOLCANO_ARIES_BOARD)
/* VOLCANO_ARIES_BOARD */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(DDR_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(DDR_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(DDR_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(DDR_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#elif defined(TNETV1061_BOARD)
/* PETRA TNETV1061_BOARD */
#define BOOTCR_PLL_BYPASS_BITNUM  5
#define BOOTCR_PLL_MODE_BITNUM    25
#define BOOTCR_SYSPLSEL_BITNUM	  15	
#define BOOTCR_ASYNCPLSEL_BITNUM  17
#define ASYNC_BASE       0xA8610800
#define DDR_BASE         0xA8613000
#define EMIF_REV        (*(volatile unsigned int*)(ASYNC_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(ASYNC_BASE+0x04))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(ASYNC_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(ASYNC_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(ASYNC_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(ASYNC_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))
#define DDR_DRAMCTL     (*(volatile unsigned int*)(DDR_BASE+0x08))
#define DDR_REFRESH     (*(volatile unsigned int*)(DDR_BASE+0x0c))

#else

#ifndef EMIF_BASE
#define EMIF_BASE        0xA8610800
#endif
#define ASYNC_BASE       0xA8610800
#define EMIF_REV        (*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC     (*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL    (*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH    (*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0  (*(volatile unsigned int*)(EMIF_BASE+0x10))
#define EMIF_ASYNC_CS3  (*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS4  (*(volatile unsigned int*)(EMIF_BASE+0x18))
#define EMIF_ASYNC_CS5  (*(volatile unsigned int*)(EMIF_BASE+0x1c))
#define NAND_FLASH_CTRL_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x60))
#define NAND_FLASH_STAT_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x64))
#define NAND_FLASH_ECC1_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x70))
#define NAND_FLASH_ECC2_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x74))
#define NAND_FLASH_ECC3_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x78))
#define NAND_FLASH_ECC4_REG (*(volatile unsigned int*)(ASYNC_BASE + 0x7C))

#endif /* TNETV1051_BOARD */

#define GPIO_BASE     0xa8610900
#if defined(TNETV1050_BOARD) || defined(TNETV1061_BOARD)
/* These defines are for GPIO EN,OUT and DAT 0 */
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0x18))
#define GPIO_OUT        (*(volatile unsigned int *)(GPIO_BASE+0x8))
#define GPIO_DIR        (*(volatile unsigned int *)(GPIO_BASE+0x10))
#else
#define GPIO_EN         (*(volatile unsigned int *)(GPIO_BASE+0xc))
#define GPIO_OUT        (*(volatile unsigned int *)(GPIO_BASE+0x4))
#define GPIO_DIR        (*(volatile unsigned int *)(GPIO_BASE+0x8))
#ifndef __ASSEMBLER__
#define APP_DIDR_HI     (*(volatile unsigned int *)(GPIO_BASE+0x3c))
#else
#define APP_DIDR_HI     0xa861093c
#define PLL_300         0xb7fe
#define PLL_250         0x97fe
#endif
#define ARIES_1051     0
#define ARIES_1052     1
#define ARIES_1053     2 
#endif



#define RESET_BASE  0xa8611600
#define RESET_REG       (*(volatile unsigned *)(RESET_BASE+0x00))
#define RESET_STATUS    (*(volatile unsigned *)(RESET_BASE+0x08))
#if defined(TNETV1051_BOARD)
#define ARIES_PIN_MUX_BASE           (RESET_BASE+0x0C)
#define ARIES_PIN_MUX_REG_ADDR(n)    ((ARIES_PIN_MUX_BASE+((n)*4)))
#define ARIES_PIN_MUX_REG(n)         (*(volatile unsigned *)(ARIES_PIN_MUX_BASE+((n)*4)))
#endif

#define BOOTCR      0xa8611a00

#ifdef DUAL_FLASH
#define SEC_FLASH_BASE          CS4_BASE
#define SEC_FLASH_SIZE          CS4_SIZE
#define _SEC_FLASH_SIZE         CONF_FLASH_SZ
#endif

#if defined (TNETD73XX_BOARD)
#define IS_OHIO_CHIP() (( (REG32_R( 0xA8610914,15,0 ) == 0x2b) || (REG32_R( 0xA8610914,15,0 ) == 0x18)) ? 1:0)
#define AVALANCHE_MII_SEL_REG         (0xa8611A08)
#endif
#endif /* _HW_H_ */


