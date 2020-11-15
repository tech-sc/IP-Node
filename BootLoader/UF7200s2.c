/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  BootLoaderのソースファイル									  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <stdio.h>
#include <stdint.h>

/*** ユーザ作成ヘッダの取り込み ***/
#include "UF7200s2.h"

/*** 自ファイル内でのみ使用する#define マクロ ***/
#define	NANO_CNT		100		/* 1nsecのnopカウント値 */

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/
/******************************************************************************/
/* マクロ名	  レジスタライト												  */
/* 機能概要	  レジスタへのライトを行う										  */
/* パラメータ adr : (in)	レジスタアドレス								  */
/*			  val : (in)	ライトするデータ値								  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define RegWrite(adr,val)										\
	adr = val

/******************************************************************************/
/* マクロ名	  レジスタリード												  */
/* 機能概要	  レジスタへのライトを行う										  */
/* パラメータ adr : (in)	レジスタアドレス								  */
/* リターン	  リードしたデータ												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define RegRead(adr)											\
	adr

/******************************************************************************/
/* マクロ名	  DDRメモリライト												  */
/* 機能概要	  メモリライトを行う											  */
/* パラメータ adr : (in)	レジスタアドレス								  */
/*			  val : (in)	ライトするデータ値								  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define DDRWrite(adr,val)										\
	*((volatile unsigned short *)adr) = (unsigned short)val

/******************************************************************************/
/* マクロ名	  DDRメモリリード												  */
/* 機能概要	  メモリリードを行う											  */
/* パラメータ adr : (in)	レジスタアドレス								  */
/* リターン	  リードしたデータ												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define DDRRead(adr)											\
	*(volatile unsigned short *)adr

/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/
/*** ファイル内で共有するstatic 変数宣言 ***/
/*** static 関数宣言 ***/

/******************************************************************************/
/* 関数名	  検査結果LED点滅												  */
/* 機能概要	  リターンすることなくGPIO-LEDを点滅する						  */
/* パラメータ なし															  */
/* リターン	  しない														  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
__inline void GPIO_LedBlink()
{
	RegWrite(GPIO_LED, RegRead(GPIO_LED) & ~GPIO_LED_OFF);	
	while(1)
	{
		msleep(200);
		RegWrite(GPIO_LED, RegRead(GPIO_LED) ^ GPIO_LED_OFF);
	}
}

/******************************************************************************/
/* 関数名	  青LED点滅														  */
/* 機能概要	  FPGA-青LEDを早点滅する										  */
/* パラメータ なし															  */
/* リターン	  しない														  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
__inline void FPGA_BlueLedBlink(void)
{
	RegWrite(FPGA_LED, LED_BLINK | LED_CTRL_LU | LED_BLUE);
	while(1);
}

/******************************************************************************/
/* 関数名	  ナノ秒ウェイト												  */
/* 機能概要	  ナノ秒単位のウェイト処理										  */
/* パラメータ tim : (in)	ウェイトする時間								  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void nsleep(int32_t tim)
{
	int32_t		i = 0;

	while(tim > 0)
	{
		i = 0;
		while (i < NANO_CNT) {
			__asm("nop");
			i++;
		}
		tim--;
	}
}

/******************************************************************************/
/* 関数名	  ミリ秒ウェイト												  */
/* 機能概要	  ミリ秒単位のウェイト処理										  */
/* パラメータ tim : (in)	ウェイトする時間								  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void msleep(int32_t tim)
{
	int32_t		i = 0;

	while(tim > 0)
	{
		i = 0;
		while (i < NANO_CNT * 1000) {
			__asm("nop");
			i++;
		}
		tim--;
	}
}

/******************************************************************************/
/* 関数名	  CPUの初期設定													  */
/* 機能概要	  SoC内CPU周辺のコントローラを初期設定する						  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void SoC_CPUSetting(void)
{
	/* PINコンフィグレーションのレジスタ設定 */
	RegWrite(PINSEL8, PINSEL8_INIT);
	RegWrite(PINSEL9, PINSEL9_INIT);
	RegWrite(PINSEL10, PINSEL10_INIT);
	RegWrite(PINSEL14, PINSEL14_INIT);
	RegWrite(PINSEL15, PINSEL15_INIT);
	RegWrite(PINSEL16, PINSEL16_INIT);
	RegWrite(PINSEL17, PINSEL17_INIT);
	RegWrite(PINSEL18, PINSEL18_INIT);

	/* Powerコントローラのレジスタ設定 */
	RegWrite(POWERDN_CR, POWERDN_CR_INIT);
	RegWrite(PULLUPDN_CR1, PULLUPDN_CR1_INIT);
	RegWrite(WAKEUP_CR, WAKEUP_CR_INIT);
	RegWrite(PULLUPDN_CR2, PULLUPDN_CR2_INIT);
	RegWrite(PULLUPDN_CR3, PULLUPDN_CR3_INIT);

	/* リセットコントローラのレジスタ設定 */
	RegWrite(PRCR, PRCR_INIT);

	/* クロックコントローラのレジスタ設定 */
	RegWrite(SCLKCR, SCLKCR_INIT);
	RegWrite(ECLKCR, ECLKCR_INIT);
}


/******************************************************************************/
/* 関数名	  EMIF(DDR含む)の初期設定										  */
/* 機能概要	  SoC内DDR含むEMIFコントローラを初期設定する					  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void SoC_EMIFSetting(void)
{
	/* EMIFコントローラのレジスタ設定 */
	RegWrite(EMIF_DRAMCTL, EMIF_DRAMCTL_INIT);
	RegWrite(EMIF_REFRESH, EMIF_REFRESH_INIT);
	RegWrite(EMIF_ASYNC_CS0, EMIF_ASYNC_CS0_INIT);
	RegWrite(EMIF_ASYNC_CS1, EMIF_ASYNC_CS1_INIT);
	RegWrite(EMIF_DRAMTMG, EMIF_DRAMTMG_INIT);
	RegWrite(DRAM_SREF, DRAM_SREF_INIT);
}

/******************************************************************************/
/* 関数名	  GPIOの初期設定												  */
/* 機能概要	  SoC内LED含むGPIOコントローラを初期設定する					  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void SoC_GPIOSetting(void)
{
	/* GPIOコントローラのレジスタ設定 */
	RegWrite(GPIO_OUT0, GPIO_OUT0_INIT);
	RegWrite(GPIO_OUT1, GPIO_OUT1_INIT);
	RegWrite(GPIO_DIR0, GPIO_DIR0_INIT);
	RegWrite(GPIO_DIR1, GPIO_DIR1_INIT);
	RegWrite(GPIO_EN0, GPIO_EN0_INIT);
	RegWrite(GPIO_EN1, GPIO_EN1_INIT);

	/* FPGAをリセットする */
	RegWrite(SPI_GATE, RegRead(SPI_GATE) & ~SPI_GATE_CLOSE);		/* ゲート開放 */
	RegWrite(FPGA_PROG, (RegRead(FPGA_PROG) & ~FPGA_PROG_INACT) | FPGA_PROG_INACT);	/* HI */
	RegWrite(FPGA_PROG, RegRead(FPGA_PROG) & ~FPGA_PROG_INACT);		/* Lo=ACT */
	nsleep(55);		/* wait 55ns以上 */
	RegWrite(FPGA_PROG, (RegRead(FPGA_PROG) & ~FPGA_PROG_INACT) | FPGA_PROG_INACT);	/* Hi */
	RegWrite(SPI_GATE, (RegRead(SPI_GATE) & ~SPI_GATE_CLOSE) | SPI_GATE_CLOSE);		/* ゲート閉塞 */
}

/******************************************************************************/
/* 関数名	  FPGAコンフィグレーションチェック								  */
/* 機能概要	  FPGAコンフィグレーションチェックを行う						  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  エラー発生時はリターンしない									  */
/* その他	  －															  */
/******************************************************************************/
void FPGAConfigChk(void)
{
	int		cnt = 0;

	/* コンフィグ完了チェック */
	while (FPGACONFIG_DONE != (RegRead(GPIO_FPGA_CONFIG) & FPGACONFIG_DONE))
	{
		cnt++;
		if (cnt > 2)	/* 30ms×2回NGなら、LED点滅 */
		{
			/* 検査結果LED点滅してリターンしない */
			GPIO_LedBlink();
		}
		msleep(30);
	}

	/* 青LED点灯 */
	RegWrite(FPGA_LED, LED_ON | LED_CTRL_LU | LED_BLUE);
}

/******************************************************************************/
/* 関数名	  FPGAコンフィグレーションチェック								  */
/* 機能概要	  FPGA-BUSチェックを行う										  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  エラー発生時はリターンしない									  */
/* その他	  －															  */
/******************************************************************************/
void FPGA_BusChk(void)
{
	/* バスチェック */
	if (RegRead(FPGA_BUS_CHK) != 0x00) {
		/* 検査結果LED点滅してリターンしない */
		GPIO_LedBlink();
	}

	RegWrite(FPGA_BUS_CHK, 0x55);
	if (RegRead(FPGA_BUS_CHK) != 0x55) {
		/* 検査結果LED点滅してリターンしない */
		GPIO_LedBlink();
	}

	RegWrite(FPGA_BUS_CHK, 0xAA);
	if (RegRead(FPGA_BUS_CHK) != 0xAA) {
		/* 検査結果LED点滅してリターンしない */
		GPIO_LedBlink();
	}
}

/******************************************************************************/
/* 関数名	  簡易メモリチェック											  */
/* 機能概要	  簡易メモリチェックを行う										  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  エラー発生時はリターンしない									  */
/* その他	  －															  */
/******************************************************************************/
void DDR_SimpleMemChk(void)
{
	DDRWrite(DDR_TOP_ADDR, 0x5555);
	DDRWrite(DDR_TAIL_ADDR, 0xaaaa);

	if (DDRRead(DDR_TOP_ADDR) != 0x5555) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TAIL_ADDR) != 0xaaaa) {
		FPGA_BlueLedBlink();
	}

	DDRWrite(DDR_TOP_ADDR+0x0002, 0x0001);
	DDRWrite(DDR_TOP_ADDR+0x0004, 0x0002);
	DDRWrite(DDR_TOP_ADDR+0x0006, 0x0004);
	DDRWrite(DDR_TOP_ADDR+0x0010, 0x0008);

	DDRWrite(DDR_TOP_ADDR+0x0040, 0x0010);
	DDRWrite(DDR_TOP_ADDR+0x0100, 0x0020);
	DDRWrite(DDR_TOP_ADDR+0x0400, 0x0040);
	DDRWrite(DDR_TOP_ADDR+0x1000, 0x0080);

	DDRWrite(DDR_TOP_ADDR+0x2000, 0x0100);
	DDRWrite(DDR_TOP_ADDR+0x8000, 0x0200);
	DDRWrite(DDR_TOP_ADDR+0x20000, 0x0400);
	DDRWrite(DDR_TOP_ADDR+0x80000, 0x0800);

	DDRWrite(DDR_TOP_ADDR+0x200000, 0x1000);
	DDRWrite(DDR_TOP_ADDR+0x800000, 0x2000);
	DDRWrite(DDR_TOP_ADDR+0x1000000, 0x4000);
	DDRWrite(DDR_TOP_ADDR+0x2000000, 0x8000);


	if (DDRRead(DDR_TOP_ADDR+0x0002) != 0x0001) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x0004) != 0x0002) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x0006) != 0x0004) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x0010) != 0x0008) {
		FPGA_BlueLedBlink();
	}

	if (DDRRead(DDR_TOP_ADDR+0x0040) != 0x0010) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x0100) != 0x0020) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x0400) != 0x0040) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x1000) != 0x0080) {
		FPGA_BlueLedBlink();
	}

	if (DDRRead(DDR_TOP_ADDR+0x2000) != 0x0100) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x8000) != 0x0200) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x20000) != 0x0400) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x80000) != 0x0800) {
		FPGA_BlueLedBlink();
	}

	if (DDRRead(DDR_TOP_ADDR+0x200000) != 0x1000) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x800000) != 0x2000) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x1000000) != 0x4000) {
		FPGA_BlueLedBlink();
	}
	if (DDRRead(DDR_TOP_ADDR+0x2000000) != 0x8000) {
		FPGA_BlueLedBlink();
	}
}


/******************************************************************************/
/* 関数名	  詳細メモリチェック											  */
/* 機能概要	  詳細メモリチェックを行う										  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  エラー発生時はリターンしない									  */
/* その他	  －															  */
/******************************************************************************/
void DDR_DetailMemChk(void)
{
	volatile unsigned short *p = NULL;

	p = (unsigned short *)DDR_TOP_ADDR;
	while (p <= (unsigned short *)DDR_TAIL_ADDR)
	{
		DDRWrite(p, 0x5555);
		if (DDRRead(p) != 0x5555) {
			FPGA_BlueLedBlink();
		}
		p++;
	}

	p = (unsigned short *)DDR_TOP_ADDR;
	while (p <= (unsigned short *)DDR_TAIL_ADDR)
	{
		DDRWrite(p, 0xaaaa);
		if (DDRRead(p) != 0xaaaa) {
			FPGA_BlueLedBlink();
		}
		p++;
	}
}

/******************************************************************************/
/* 関数名	  DDRメモリチェック												  */
/* 機能概要	  検査モードスイッチを参照し、簡易／詳細メモリチェックを行う	  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void DDR_MemChk(void)
{
	if ((RegRead(GPIO_DDRCHK_MODE) & DDRCHK_NORMAL) == DDRCHK_NORMAL)
	{	/* 通常モード */
		DDR_SimpleMemChk();
	}
	else
	{	/* 検査モード */
		RegWrite(GPIO_LED, RegRead(GPIO_LED) & ~GPIO_LED_OFF);		/* LED-ON */
		DDR_DetailMemChk();
	}
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  BootLoaderのソースファイル									  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
