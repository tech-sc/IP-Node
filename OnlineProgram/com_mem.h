/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       共通関数のヘッダファイル                                        */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _COM_MEM_H_
#define _COM_MEM_H_
#include <stdint.h>

/* メモリアドレスについては、UF7200IP_step2設計要項_ファームインタフェース編参照 */

/* バス幅 */
#define BYTE_WIDTH				sizeof(uint8_t)		/* FPGA */
#define WORD_WIDTH				sizeof(uint16_t)	/* DDR */
#define DWORD_WIDTH				sizeof(uint32_t)	/* SoC(CPU,GPIO等)など */

/* メモリアクセス APIs */
extern BYTE com_memInit( void );
extern BYTE com_memRead( off_t phy_addr, uint16_t width, uint16_t len, void *buff_p );
extern BYTE com_memWrite( off_t phy_addr, uint16_t width, uint16_t len, void *buff_p );
extern BYTE com_memUpdate( off_t phy_addr, uint16_t width, uint32_t mask, uint32_t val, void *prev_p );


/* FPGAレジスタ */
#define FPGA_MEM_BASE			0x1C000000
#define FPGA_HWVER3				(FPGA_BASE+0x30)
#define FPGA_HWVER2				(FPGA_BASE+0x31)
#define FPGA_HWVER1				(FPGA_BASE+0x32)
#define FPGA_HWVER0				(FPGA_BASE+0x33)
#define FPGA_PKGVER				(FPGA_BASE+0x34)
#define FPGA_LED				(FPGA_BASE+0x35)
#define FPGA_HOSYU_SW			(FPGA_BASE+0x36)
#define FPGA_IO_ALM				(FPGA_BASE+0x37)
#define FPGA_BUS_CHK			(FPGA_BASE+0x39)

/* bit論理和で指定する定数 */
#define LED_ON                  0x00                /* 点灯 */
#define LED_BLINK               (0x01<<6)           /* 65ms消灯/20ms点灯 */
#define LED_120INT              (0x02<<6)           /* 500ms周期点滅 */
#define LED_60INT               (0x03<<6)           /* 1s周期点滅 */
#define LED_LU                  0x00                /* LED制御権:AC制御 */
#define LED_CS                  (0x01<<5)           /* LED制御権:CS制御 */
#define LED_BLUE                0x04                /* 青色LED点灯／0=消灯 */
#define LED_GREEN               0x02                /* 緑色LED点灯／0=消灯 */
#define LED_RED                 0x01                /* 赤色LED点灯／0=消灯 */

/* マスク定数 */
#define ON_BLINK_MASK           (LED_BLINK | LED_120INT | LED_60INT)
#define LED_CTRL_MASK           LED_CS
#define COLOR_MASK              (LED_BLUE | LED_GREEN | LED_RED)

/* FPGAレジスタアクセス APIs */
extern BYTE com_FpgaRegRead( off_t phy_addr, uint16_t len, uint8_t *buff_p );
extern BYTE com_FpgaRegWrite( off_t phy_addr, uint8_t val );
extern BYTE com_FpgaLED( uint8_t mask, uint8_t val, uint8_t *prev_p );


/* GPIOレジスタ物理アドレス */
#define GPIO_IN0_REG            (0x08610900)        /* FPGA-DONE信号、検査モードSW入力 */
#define GPIO_IN1_REG            (0x08610904)        /* 未使用 */
#define GPIO_OUT0_REG           (0x08610908)        /* SPI-ROMゲート制御、FPGA-Program制御、検査結果LED制御 */
#define GPIO_OUT1_REG           (0x0861090C)        /* マスタリセット出力(未使用) */

/* GPIOレジスタbit */
#define SPI_GATE_CLOSE          (1<<3)              /* SPI-ROMゲート制御 =1:SPI-ROMゲート閉塞／=0:SPI-ROMゲート開放 */
#define FPGA_DONE               (1<<10)             /* FPGA-DONE信号     =1:FPGAコンフィグ完了／=0:コンフィグ中 */
#define FPGA_PROG_INACT         (1<<15)             /* FPGA-Program制御  =1:通常時／=0:再コンフィグ実施(1→0→1) */
#define MODE_SW_ON              (1<<18)             /* 検査モードSW      =1:通常モード／=0:検査モード */
#define GPIO_LED_OFF            (1<<21)             /* 検査結果LED制御   =1:消灯／=0:点灯 */

/* GPIOレジスタマスク */
#define SPI_GATE_MASK           SPI_GATE_CLOSE      /* SPI-ROMゲート制御 */
#define FPGA_DONE_MASK          FPGA_DONE           /* FPGA-DONE信号 */
#define FPGA_PROG_MASK          FPGA_PROG_INACT     /* FPGA-Program制御 */
#define MODE_SW_MASK            MODE_SW_ON          /* 検査モードSW入力 */
#define GPIO_LED_MASK           GPIO_LED_OFF        /* 検査結果LED制御 */

/* GPIOレジスタアクセス APIs */
extern BYTE com_GpioRegRead( off_t phy_addr, uint32_t mask, uint32_t *buff_p );
extern BYTE com_GpioRegUpdate( off_t phy_addr, uint32_t mask, uint32_t bit );

/* IPLパージョン取得 API */
extern BYTE com_IPLVerGet( char *ver );

extern int com_SpiflashRead(WORD addr, WORD size, WORD *data_p);

#endif /* _COM_MEM_H_ */

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       共通関数のヘッダファイル                                        */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
