/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  com_memのテストソースファイル									  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
//#include <sys/types.h>
//#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/memfd.h>		/* memfd_create() */

#include "gtest/gtest.h"

extern "C" {
#include "def.h"
#define TEMP_HEADER
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
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

/******************************************************************************/
/* テストケース																  */
/******************************************************************************/
TEST_F(ComMemLv01, com_memInit)
{
	EXPECT_EQ(OK, com_memInit());
}

TEST_F(ComMemLv01, com_memRead)
{
	DWORD	data=0;
	DWORD	*ptr=NULL;

	int		fd = memfd_create("tmp_com_mem.txt", MFD_ALLOW_SEALING);
	write(fd, "1234567890", 10);
	ptr = mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, 0 & ~MAP_MASK);

	EXPECT_EQ(OK, com_memRead(ptr, DWORD_WIDTH, 1, &data));
	printf("data=%lX\n", data);
	munmap(ptr, MAP_SIZE);
	close(fd);
}

