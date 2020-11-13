/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  UF7200step2 LU部 BootLoader header file						  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _UF7200s2_H_
#define _UF7200s2_H_
#include "hw.h"			/* SDK提供ヘッダファイル */

/* GPIO controller */		/* TBD:BASEアドレスのMSB3bitが変わるかも（メーカ問合せ中） */
#if 0 /* 以下内容で inc/psbl/hw.h に定義済み */
#define GPIO_BASE			0xa8610900
#endif
#define GPIO_IN0			(*(volatile unsigned int *)(GPIO_BASE+0x00))
#define GPIO_IN1			(*(volatile unsigned int *)(GPIO_BASE+0x04))
#define GPIO_OUT0			GPIO_OUT
#define GPIO_OUT1			(*(volatile unsigned int *)(GPIO_BASE+0x0c))
#define GPIO_DIR0			GPIO_DIR
#define GPIO_DIR1			(*(volatile unsigned int *)(GPIO_BASE+0x14))
#define GPIO_EN0			GPIO_EN
#define GPIO_EN1			(*(volatile unsigned int *)(GPIO_BASE+0x1c))

#define GPIO_OUT0_INIT		0x00008008
#define GPIO_OUT1_INIT		0x00000020
#define GPIO_DIR0_INIT		0xFFDF7FF7
#define GPIO_DIR1_INIT		0x000FFFDF
#define GPIO_EN0_INIT		0xFFDB7BF7
#define GPIO_EN1_INIT		0x000FFFDF

/* pin selector */
#define PINSEL_BASE			0xA8611600
#define PINSEL8				(*(volatile unsigned int *)(PINSEL_BASE+0x28))
#define PINSEL9				(*(volatile unsigned int *)(PINSEL_BASE+0x2c))
#define PINSEL10			(*(volatile unsigned int *)(PINSEL_BASE+0x30))
//#define PINSEL11			(*(volatile unsigned int *)(PINSEL_BASE+0x34))
//#define PINSEL12			(*(volatile unsigned int *)(PINSEL_BASE+0x38))
//#define PINSEL13			(*(volatile unsigned int *)(PINSEL_BASE+0x3c))
#define PINSEL14			(*(volatile unsigned int *)(PINSEL_BASE+0x40))
#define PINSEL15			(*(volatile unsigned int *)(PINSEL_BASE+0x44))
#define PINSEL16			(*(volatile unsigned int *)(PINSEL_BASE+0x48))
#define PINSEL17			(*(volatile unsigned int *)(PINSEL_BASE+0x4c))
#define PINSEL18			(*(volatile unsigned int *)(PINSEL_BASE+0x50))

#define PINSEL8_INIT		0x00080000
#define PINSEL9_INIT		0x20085520
#define PINSEL10_INIT		0x01540808
#define PINSEL14_INIT		0x00100000
#define PINSEL15_INIT		0x00000000
#define PINSEL16_INIT		0x00000000
#define PINSEL17_INIT		0xA0000000
#define PINSEL18_INIT		0x0000000A

/* power controller */
#define POWER_BASE			0xA8610A00
#define POWERDN_CR			(*(volatile unsigned int *)(POWER_BASE+0x00))
#define PULLUPDN_CR1		(*(volatile unsigned int *)(POWER_BASE+0x08))
#define WAKEUP_CR			(*(volatile unsigned int *)(POWER_BASE+0x0C))
#define PULLUPDN_CR2		(*(volatile unsigned int *)(POWER_BASE+0x10))
#define PULLUPDN_CR3		(*(volatile unsigned int *)(POWER_BASE+0x14))

#define POWERDN_CR_INIT		0x2B900089
#define PULLUPDN_CR1_INIT	0x00000000
#define WAKEUP_CR_INIT		0x00000000
#define PULLUPDN_CR2_INIT	0x00000000
#define PULLUPDN_CR3_INIT	0x00000000

/* reset controller */
#define RESET_BASE			0xa8611600
#define PRCR				(*(volatile unsigned int *)(RESET_BASE+0x00))
#define SWRCR				(*(volatile unsigned int *)(RESET_BASE+0x04))
#define RSR					(*(volatile unsigned int *)(RESET_BASE+0x08))
#define EPRCR				(*(volatile unsigned int *)(RESET_BASE+0x0C))

#define PRCR_INIT			0x2C551C7D

/* clock controller */
#define CLOCK_BASE			0xa8610A20
#define SCLKCR				(*(volatile unsigned int *)(CLOCK_BASE+0x00))
#define ECLKCR				(*(volatile unsigned int *)(CLOCK_BASE+0x20))

#define SCLKCR_INIT			0x00000000
#define ECLKCR_INIT			0x00000000

/* EMIF controller */
#if 0 /* 以下内容で inc/psbl/hw.h に定義済み */
#define EMIF_BASE			 0xA8610800
#define EMIF_REV			(*(volatile unsigned int*)(EMIF_BASE+0x00))
#define EMIF_GASYNC			(*(volatile unsigned int*)(EMIF_BASE+0x04))
#define EMIF_DRAMCTL		(*(volatile unsigned int*)(EMIF_BASE+0x08))
#define EMIF_REFRESH		(*(volatile unsigned int*)(EMIF_BASE+0x0c))
#define EMIF_ASYNC_CS0		(*(volatile unsigned int*)(EMIF_BASE+0x10))
#endif
#define EMIF_ASYNC_CS1		(*(volatile unsigned int*)(EMIF_BASE+0x14))
#define EMIF_ASYNC_CS0_INIT	0x086E4BDD
#define EMIF_ASYNC_CS1_INIT	0x3D9FECFC

#if 0 /* 以下内容で inc/psbl/hw.h に定義済み */
#define DDR_BASE			0xA8613000
#define EMIF_DRAMCTL		(*(volatile unsigned int*)(DDR_BASE+0x08))
#define EMIF_REFRESH		(*(volatile unsigned int*)(DDR_BASE+0x0c))
#endif
#define EMIF_DRAMTMG		(*(volatile unsigned int *)(DDR_BASE+0x20))
#define DDR_STT				(*(volatile unsigned int *)(DDR_BASE+0x24))
#define DDR_CTL				(*(volatile unsigned int *)(DDR_BASE+0x28))
#define DRAM_SREF			(*(volatile unsigned int *)(DDR_BASE+0x3C))

#define EMIF_DRAMCTL_INIT	0x00004C22
#define EMIF_REFRESH_INIT	0x00000933
#define EMIF_DRAMTMG_INIT	0x5A226820
#define DDR_CTL_INIT		(0x294A8A46)		/* TBD 設計要綱FWIF_v004版 */
#define DRAM_SREF_INIT		0x0000000C

/* Interrupt controller */
/*	bootでは未使用なので定義しない */
/* SPI controller */
/*	bootでは未使用なので定義しない */
/* UART controller */
/*	psbl/kernel/sio.cのSioInit()でch0使用しているので、既存のまま流用する */
/*	但し、BASEアドレスがハード仕様とことなるので、ハード仕様に合わせて再定義する */
#undef	SIO0_BASE
#undef	SIO0_RSTMASK
#define SIO0_BASE		 	0xa8610e00
#define SIO0_RSTMASK 		0x01				/* Peripheral Reset Control Register (PRCR)のbit0:UART_0_RST */


/* GPIO firmware register */
#define GPIO_DDRCHK_MODE	GPIO_IN0
#define DDRCHK_NORMAL		(1<<18)				/* =1:通常モード／=0:検査モード */
#define GPIO_FPGA_CONFIG	GPIO_IN0
#define FPGACONFIG_DONE		(1<<10)				/* =1:完了／=0:コンフィグ中 */
#define GPIO_LED			GPIO_OUT0
#define GPIO_LED_OFF		(1<<21)				/* =1:LED消灯／=0:LED点灯 */
#define FPGA_PROG			GPIO_OUT0
#define FPGA_PROG_INACT		(1<<15)				/* =1:インアクティブ／=0:アクティブ */
#define SPI_GATE			GPIO_OUT0
#define SPI_GATE_CLOSE		(1<<3)				/* =1:ゲート閉塞／=0:開放 */


/* FPGA firmware register */	/* TBD:BASEアドレスのMSB3bitが変わるかも（メーカ問合せ中） */
/*	bootで使用するレジスタのみ定義する */
#define FPGA_BASE			0x1C000000
#define FPGA_LED			(*(volatile unsigned char *)(FPGA_BASE+0x35))
#define FPGA_BUS_CHK		(*(volatile unsigned char *)(FPGA_BASE+0x39))
/*	reg = reg & MASK | VALUE; でセットする */
#define LED_BLINK_MASK		0xC0
#define LED_ON				0x00				/* =00:点灯 */
#define LED_BLINK			(1<<6)				/* =01:点滅 */
#define LED_120INT			(2<<6)				/* =10:500ms周期点滅 */
#define LED_60INT			(3<<6)				/* =11:1000ms周期交互点滅 */
#define LED_CTRL_MASK		0x20
#define LED_CTRL_LU			0x00				/* =0:LU側制御 */
#define LED_CTRL_CS			(1<<5)				/* =1:CS側制御 */
#define LED_MASK			0x07
#define LED_BLUE			(1<<2)				/* =1:青点灯／=0:滅灯 */
#define LED_GREEN			(1<<1)				/* =1:緑点灯／=0:滅灯 */
#define LED_RED				(1<<0)				/* =1:赤点灯／=0:滅灯 */


/* DDR mem address */		/* TBD:BASEアドレスのMSB3bitが変わるかも（メーカ問合せ中） */
#define DDR_BASE			0x14000000
#define DDR_TOP_ADDR		DDR_BASE
#define DDR_TAIL_ADDR		(DDR_BASE+0x04000000-2)


extern void nsleep(int tim);
extern void msleep(int tim);
extern void SoC_CPUSetting(void);
extern void SoC_EMIFSetting(void);
extern void SoC_GPIOSetting(void);
extern void FPGAConfigChk(void);
extern void FPGA_BusChk(void);
extern void DDR_SimpleMemChk(void);
extern void DDR_DetailMemChk(void);
extern void DDR_MemChk(void);


#endif /* _UF7200s2_H_ */
/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  UF7200step2 LU部 BootLoader header file						  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
