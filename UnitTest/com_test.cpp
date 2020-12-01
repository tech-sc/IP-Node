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
#define PFN_MASK_SIZE 8

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

	unsigned long virt2phy(const void *virtaddr)
	{
	    unsigned long page, physaddr, virt_pfn;
	    off_t offset;
	    int page_size = sysconf(_SC_PAGESIZE);
	    int fd, retval;

	    fd = open("/proc/self/pagemap", O_RDONLY);
	    if (fd < 0) {
	        printf("error: open");
	        return 0;
	    }

	    virt_pfn = (unsigned long)virtaddr / page_size;
	    offset = sizeof(uint64_t) * virt_pfn;
	    if (lseek(fd, offset, SEEK_SET) == (off_t) -1) {
	        printf("error: lseek\n");
	        close(fd);
	        return 0;
	    }

	    retval = read(fd, &page, PFN_MASK_SIZE);
	    close(fd);
	    if (retval < 0) {
	        printf("error: read\n");
	        return 0;
	    } else if (retval != PFN_MASK_SIZE) {
	        printf("error: read2\n");
	        return 0;
	    }

	    if ((page & 0x7fffffffffffffULL) == 0){
	        printf("pfn == 0\n");
	        printf("page = %016lX\n", page);
	        return 0;
	    }

	    physaddr = ((page & 0x7fffffffffffffULL) * page_size)
	        + ((unsigned long)virtaddr % page_size);

	    return physaddr;
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
	DWORD	*virt_ptr=NULL, *phy_ptr=NULL;

	virt_ptr = malloc(256);
	memcpy(virt_ptr, "1234567890", 10);

	phy_ptr = (DWORD*)virt2phy(virt_ptr);

	EXPECT_EQ(OK, com_memRead(phy_ptr, DWORD_WIDTH, 1, &data));
	printf("data=%lX\n", data);

	free(virt_ptr);
}

