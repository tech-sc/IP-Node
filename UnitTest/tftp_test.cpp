/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  TFTPタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
//#include <sys/types.h>	/* stat() */
//#include <sys/stat.h>		/* stat() */
//#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "gtest/gtest.h"

extern "C" {
#include "def.h"
#include "str.h"
#include "prot.h"
#include "tmr_def.h"
#include "com_mem.h"
#include "tftp_tsk.h"
#include "stub.h"
}

/********************************************************************************/
/* テストケースの説明															*/
/*		TftpTaskLv01 は、引数テストやリソースを扱わないロジックテスト			*/
/*		TftpTaskLv02 は、TftpTaskLv01が完了して行うテスト						*/
/*		TftpTaskLv03 は、外部実行プログラムが必要な(整ってから実施する)テスト	*/
/*		TftpTaskLv04 は、ダウンロードしたファイルが必要な(整ってから実施する)テスト	*/
/*		TftpTaskLv05 は、threadにてメッセージ受信待ちが必要な(整ってから実施する)テスト	*/
/********************************************************************************/

/******************************************************************************/
/* テストクラス																  */
/******************************************************************************/
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

class TftpTaskLv02 : public ::testing::Test {
protected:
	TftpTaskLv02(){
	}
	~TftpTaskLv02(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

class TftpTaskLv03 : public ::testing::Test {
protected:
	TftpTaskLv03(){
	}
	~TftpTaskLv03(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

class TftpTaskLv04 : public ::testing::Test {
protected:
	TftpTaskLv04(){
	}
	~TftpTaskLv04(){
	}

	// TestCase名ごとの初期化処理。テストケース実行前に実行される。
	virtual void SetUp(){
	}
	// TestCase名ごとの後処理。テストケース実行後に実行される。
	virtual void TearDown(){
	}
};

class TftpTaskLv05 : public ::testing::Test {
protected:
	TftpTaskLv05(){
	}
	~TftpTaskLv05(){
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
TEST_F(TftpTaskLv01, str2hex)
{
	BYTE	hex[32];
	//printf("TestFunction:%s\n", ::testing::UnitTest::GetInstance()->current_test_info()->name());
	EXPECT_EQ(OK, str2hex("FF00", hex, 2));
	EXPECT_EQ(0,  memcmp("\xff\x00", hex, 2));
	EXPECT_EQ(OK, str2hex("ff0011", hex, 3));
	EXPECT_EQ(0,  memcmp("\xff\x00\x11", hex, 3));
	EXPECT_EQ(OK, str2hex("99aa99AA", hex, 4));
	EXPECT_EQ(0,  memcmp("\x99\xaa\x99\xAA", hex, 4));

	EXPECT_EQ(NG, str2hex("FG00", hex, 2));
	EXPECT_EQ(NG, str2hex("fg00", hex, 2));
}

TEST_F(TftpTaskLv01, dl_sndmsg)
{
	INNER_MSG	*msg_p = NULL;

	dl_sndmsg(TFTP_ECB, 0x01, OK);

	/* com_poolget()確認 */
	EXPECT_EQ(POOL0, poolget_id);

	/* com_sndmsg()確認 */
	EXPECT_EQ(TFTP_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(0x01,   msg_p->msg_header.kind);
	EXPECT_EQ(OK,     msg_p->msg_header.no);
	EXPECT_EQ(NULL,   msg_p->msg_header.link);
}

TEST_F(TftpTaskLv05, downld_thread)
{
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv01, downld_init)
{
	EXPECT_EQ(STATE_IDLE, downld_state_no);
	EXPECT_EQ(0, downld_tim);
}

TEST_F(TftpTaskLv02, downld_SttMng)
{
	// 各状態アクション関数のテスト完了後にテストする
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv02, dl_IdleStt_SvrReq)
{
	// atftpdの実行環境が整ってからテストする
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv02, dl_IdleStt_Cl1Req)
{
	// tftp()のテスト完了後にテストする
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv02, dl_IdleStt_Cl2Req)
{
	// tftp()のテスト完了後にテストする
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv01, dl_SvrStt_SvrReq)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_SvrStt_SvrReq(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP,      msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL,   msg_p->msg_header.link);
}

TEST_F(TftpTaskLv02, dl_SvrStt_Tmo)
{
	// dl_tmpfile_chk関数のテスト完了後にテストする
}

TEST_F(TftpTaskLv01, dl_SvrStt_Cl1Req)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_SvrStt_Cl1Req(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP,      msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL,   msg_p->msg_header.link);
}

TEST_F(TftpTaskLv01, dl_SvrStt_Cl2Req)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_SvrStt_Cl2Req(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(E_HORYDLEND,    msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL,   msg_p->msg_header.link);
}

TEST_F(TftpTaskLv01, dl_SvrStt_WriteResp)
{
	INNER_MSG	*msg_p = NULL;

	downld_state_no = MAX_STATE_NO;
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x00;

	EXPECT_EQ(OK, dl_SvrStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP, msg_p->msg_header.kind);
	EXPECT_EQ(0x00, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);

	EXPECT_EQ(STATE_IDLE, downld_state_no);

	/* 結果コード0x01テスト */
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x01;
	EXPECT_EQ(OK, dl_SvrStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(0x01, msg_p->msg_header.no);
}

TEST_F(TftpTaskLv01, dl_ClStt_SvrReq)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_ClStt_SvrReq(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP, msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);
}

TEST_F(TftpTaskLv01, dl_ClStt_Cl1Req)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_ClStt_Cl1Req(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP, msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);
}

TEST_F(TftpTaskLv01, dl_ClStt_Cl2Req)
{
	INNER_MSG	*msg_p = NULL;

	EXPECT_EQ(OK, dl_ClStt_Cl2Req(NULL));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(E_HORYDLEND, msg_p->msg_header.kind);
	EXPECT_EQ(TFTP_RES_STATE, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);
}

TEST_F(TftpTaskLv01, dl_ClStt_WriteResp)
{
	INNER_MSG	*msg_p = NULL;

	// プログラムダウンロードのパターン
	downld_type = PROG_DL;
	downld_state_no = MAX_STATE_NO;
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x00;

	EXPECT_EQ(OK, dl_ClStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(I_PGDLCMP, msg_p->msg_header.kind);
	EXPECT_EQ(0x00, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);

	EXPECT_EQ(STATE_IDLE, downld_state_no);

	/* 結果コード0x01テスト */
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x01;
	EXPECT_EQ(OK, dl_ClStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(0x01, msg_p->msg_header.no);
	
	// 保留音ダウンロードのパターン
	downld_type = WAVE_DL;
	downld_state_no = MAX_STATE_NO;
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x00;

	EXPECT_EQ(OK, dl_ClStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(DWL_ID, msg_p->msg_header.id);
	EXPECT_EQ(IN_MSG, msg_p->msg_header.div);
	EXPECT_EQ(E_HORYDLEND, msg_p->msg_header.kind);
	EXPECT_EQ(0x00, msg_p->msg_header.no);
	EXPECT_EQ(NULL, msg_p->msg_header.link);

	EXPECT_EQ(STATE_IDLE, downld_state_no);

	/* 結果コード0x01テスト */
	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.no = 0x01;
	EXPECT_EQ(OK, dl_ClStt_WriteResp(msg_p));

	EXPECT_EQ(LUMNG_ECB, sndmsg_ecb);
	msg_p = (INNER_MSG*)sndmsg_ptr;
	EXPECT_EQ(0x01, msg_p->msg_header.no);
}

TEST_F(TftpTaskLv01, dl_tmpfile_chk)
{
	// dl_files変数に定義されているファイルを/tmpに用意する必要がある
	// test1:IPCS_V4.tarとFILEINFO.txtが無ければ、false
	// test2:IPCS_V4.tarだけの場合、false
	// test3:FILEINFO.txtだけの場合、false
	// test4:IPCS_V4.tarとFILEINFO.txtが有れば、true
	EXPECT_EQ(0, dl_tmpfile_chk());

	printf("TBD:ファイルコピー\n");
}


TEST_F(TftpTaskLv05, writer_thread)
{
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv04, writer_FileInfoFile)
{
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv04, writer_OnlineProg)
{
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv04, writer_BootProg)
{
	printf("skip. TBD:No Confirmation Method.\n");
}

TEST_F(TftpTaskLv01, seq_search)
{
	BYTE	buff[512];
	BYTE	*p = buff;
	int16_t	i = 0;

	// バッファを汚した後、検索対象文字をセットする
	for(i=0; i < 512; i++)	*p++ = (BYTE)i;
	memcpy(&buff[100], "UF7200IPstep2", 13);

	EXPECT_EQ(&buff[100], seq_search(buff, 512, "UF7200IPstep2", 13));
}

TEST_F(TftpTaskLv02, wt_ProgFileWrite)
{
	printf("skip. TBD:No Confirmation Method.\n");
	// 何かしらのファイルが良い出来ればテストする
}

TEST_F(TftpTaskLv02, wt_FpgaFileWrite)
{
	printf("skip. TBD:No Confirmation Method.\n");
	// /dev/spi-flashファイルが良い出来ればテストする
}

TEST_F(TftpTaskLv03, tftp)
{
	printf("skip. TBD:No Confirmation Method.\n");
	/// TftpTaskLv04 の前段で実施するがダウンロードしたファイルが必要 */
}

/******************************************************************************/
/* メイン関数																  */
/******************************************************************************/
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  TFTPタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
