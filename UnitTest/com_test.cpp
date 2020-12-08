/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  com_memのテストソースファイル									  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
//#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

#include "gtest/gtest.h"

extern "C" {
#include "def.h"
#ifdef TEMP_HEADER
#include "temp_header.h"
#include "temp_tmr_def.h"
#else
#include "str.h"
#include "tmr_def.h"
#endif
#include "com_mem.h"
#include "tftp_tsk.h"
#include "stub.h"
}

#define TEST_MEM_ADR	0x00000F00			/* 内蔵SRAMを用いて簡易デバッグする用途の物理アドレス */

/********************************************************************************/
/* テストケースの説明															*/
/*		ComMemLv01 は、引数テストやリソースを扱わないロジックテスト				*/
/*		ComMemLv02 は、ComMemLv01が完了して行うテスト							*/
/*		ComMemLv03 は、外部実行プログラムが必要な(整ってから実施する)テスト		*/
/*		ComMemLv04 は、ダウンロードしたファイルが必要な(整ってから実施する)テスト	*/
/*		ComMemLv05 は、threadにてメッセージ受信待ちが必要な(整ってから実施する)テスト	*/
/********************************************************************************/

/******************************************************************************/
/* テストクラス																  */
/******************************************************************************/
class ComMemLv01 : public ::testing::Test {
protected:
	ComMemLv01(){
	}
	~ComMemLv01(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
		com_memInit();
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

class ComMemLv02 : public ::testing::Test {
protected:
	ComMemLv02(){
	}
	~ComMemLv02(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
		com_memInit();
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

/******************************************************************************/
/* テストケース																  */
/******************************************************************************/
//TEST_F(ComMemLv01, com_memInit)
//{
// SetUp()に移動した
//	EXPECT_EQ(OK, com_memInit());
//}

TEST_F(ComMemLv01, com_memRead)
{
	uint32_t	data=0;

	//パラメータエラー確認
	EXPECT_EQ(NG, com_memRead(TEST_MEM_ADR, 0, 1, &data));
	EXPECT_EQ(NG, com_memRead(TEST_MEM_ADR, DWORD_WIDTH, 0, &data));
	EXPECT_EQ(NG, com_memRead(TEST_MEM_ADR, DWORD_WIDTH, 1, NULL));
}

TEST_F(ComMemLv01, com_memWrite)
{
	uint32_t	data=0;

	//パラメータエラー確認
	EXPECT_EQ(NG, com_memWrite(TEST_MEM_ADR, 0, 1, &data));
	EXPECT_EQ(NG, com_memWrite(TEST_MEM_ADR, DWORD_WIDTH, 0, &data));
	EXPECT_EQ(NG, com_memWrite(TEST_MEM_ADR, DWORD_WIDTH, 1, NULL));
}

TEST_F(ComMemLv02, com_memWrite_comMemRead)
{
	uint32_t	data=0x12345678;
	uint32_t	rd_data=0;

	EXPECT_EQ(OK, com_memWrite(TEST_MEM_ADR, DWORD_WIDTH, 1, &data));
	//リードバック確認
	EXPECT_EQ(OK, com_memRead(TEST_MEM_ADR, DWORD_WIDTH, 1, &rd_data));
	EXPECT_EQ(data, rd_data);
}

TEST_F(ComMemLv01, com_memUpdate)
{
	uint32_t	prev_data=0;
	uint32_t	data=0x98765432;

	//パラメータエラー確認
	EXPECT_EQ(NG, com_memUpdate(TEST_MEM_ADR, 0, 0xFFFFFFFF, data, &prev_data));
}

TEST_F(ComMemLv02, com_memUpdate)
{
	uint32_t	wt_data=0x12345678;
	uint32_t	data=0x98765432;
	uint32_t	prev_data=0;
	uint32_t	rd_data=0;

	//正常確認
	EXPECT_EQ(OK, com_memWrite(TEST_MEM_ADR, DWORD_WIDTH, 1, &wt_data));
	EXPECT_EQ(OK, com_memUpdate(TEST_MEM_ADR, DWORD_WIDTH, 0xFFFFFFFF, data, &prev_data));
	EXPECT_EQ(wt_data, prev_data);
	EXPECT_EQ(OK, com_memUpdate(TEST_MEM_ADR+4, DWORD_WIDTH, 0xFFFFFFFF, data, NULL));
	//リードバック確認
	EXPECT_EQ(OK, com_memRead(TEST_MEM_ADR, DWORD_WIDTH, 1, &rd_data));
	EXPECT_EQ(data, rd_data);
}

TEST_F(ComMemLv02, com_FpgaRegRead)
{
	uint8_t		rd_data[4] = {};

	EXPECT_EQ(OK, com_FpgaRegRead(FPGA_HWVER3, 4, &rd_data[0]));
	printf("FPGA_HWVER[3~0]=%02X,%02X,%02X,%02X\n", rd_data[0], rd_data[1], rd_data[2], rd_data[3]);
	rd_data[0] = 0xFF;
	EXPECT_EQ(OK, com_FpgaRegRead(FPGA_PKGVER, 1, &rd_data[0]));
	printf("FPGA_PKGVER=%02X\n", rd_data[0]);
}

TEST_F(ComMemLv02, com_FpgaRegWrite)
{
	// LED全点灯
	EXPECT_EQ(OK, com_FpgaRegWrite(FPGA_LED, LED_BLUE | LED_GREEN | LED_RED));
	sleep(3);
}

TEST_F(ComMemLv02, com_FpgaLED)
{
	// 青点滅
	EXPECT_EQ(OK, com_FpgaLED(ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, LED_BLINK | LED_LU | LED_BLUE, NULL));
	sleep(2);
	// 青+赤点灯
	EXPECT_EQ(OK, com_FpgaLED(ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, LED_ON | LED_LU | (LED_BLUE | LED_RED), NULL));
	sleep(2);
	EXPECT_EQ(OK, com_FpgaLED(ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, 0, NULL));
}

TEST_F(ComMemLv02, com_GpioRegRead)
{
	uint32_t	rd_data[2] = {};

	EXPECT_EQ(OK, com_GpioRegRead(GPIO_IN0_REG, FPGA_DONE | MODE_SW_ON, &rd_data[0]));
	EXPECT_EQ(OK, com_GpioRegRead(GPIO_IN0_REG, MODE_SW_ON, &rd_data[1]));
	EXPECT_EQ(rd_data[0] & MODE_SW_ON, rd_data[1]);
}

TEST_F(ComMemLv02, com_GpioRegUpdate)
{
	// 検査結果LED点灯
	EXPECT_EQ(OK, com_GpioRegUpdate(GPIO_OUT0_REG, GPIO_LED_MASK, ~GPIO_LED_OFF));
	sleep(3);
	EXPECT_EQ(OK, com_GpioRegUpdate(GPIO_OUT0_REG, GPIO_LED_MASK, GPIO_LED_OFF));
}

TEST_F(ComMemLv02, com_IPLVerGet)
{
	char	ipl_ver[5] = {};

	EXPECT_EQ(5, com_IPLVerGet(ipl_ver, sizeof(ipl_ver)));
	EXPECT_EQ(0, strcmp("1.00", ipl_ver));
}

TEST_F(ComMemLv01, com_SpiflashRead)
{
	uint16_t	data[512] = {};

	//パラメータエラー確認
	EXPECT_EQ(NG, com_SpiflashRead(0, 0, &data[0]));
	EXPECT_EQ(NG, com_SpiflashRead(0, 1, NULL));
}

TEST_F(ComMemLv02, com_SpiflashRead)
{
	uint16_t	data[512] = {};

	EXPECT_EQ(OK, com_SpiflashRead(0, 512, data));
	printf("SPI-FLASh dump[0~15]:%04X,%04X,%04X,%04X\n", data[0],data[1],data[2],data[3]);
}

TEST_F(ComMemLv01, seq_search)
{
	uint8_t		buff[20] ={"I have a PC."};

	//パラメータエラー確認
	EXPECT_EQ(NULL, seq_search(NULL, 10, "ave", 3));
	EXPECT_EQ(NULL, seq_search(buff, 0, "ave", 3));
	EXPECT_EQ(NULL, seq_search(buff, 12, NULL, 3));
	EXPECT_EQ(NULL, seq_search(buff, 12, "ave", 0));
	EXPECT_EQ(NULL, seq_search(buff, 3, "ave", 3));

	EXPECT_EQ(&buff[3], seq_search(buff, 12, "ave", 3));
}
