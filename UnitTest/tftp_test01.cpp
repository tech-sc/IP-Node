#include "gtest/gtest.h"

extern "C" {
//#include <sys/types.h>	/* stat() */
//#include <sys/stat.h>		/* stat() */
//#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "def.h"
#define TEMP_HEADER
#ifdef TEMP_HEADER
#include "temp_header.h"
#include "temp_tmr_def.h"
#else
#include "str.h"
#include "tmr_def.h"
#endif
//#include "com_mem.h"
#include "tftp_tsk.h"
}

class TftpTaskLv01 : public ::testing::Test {
protected:
	TftpTaskLv01(){
	}
	~TftpTaskLv01(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

TEST_F(TftpTaskLv01, Test0101)
{
	BYTE	hex[32];
	EXPECT_EQ(OK, str2hex("FF00", hex, 2));
	EXPECT_EQ(0, memcmp("\xff\x00", hex, 2));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
