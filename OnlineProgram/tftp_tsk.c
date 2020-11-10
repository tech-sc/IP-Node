/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  TFTPタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>	/* stat() */
#include <sys/stat.h>		/* stat() */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#include <stdlib.h>
#include <errno.h>
#include <unistd.h>			/* execlp(), stat() */
#include <openssl/md5.h>	/* MD5_Init() */
#include <arpa/inet.h>	/* inet_ntop() */

/*** ユーザ作成ヘッダの取り込み ***/
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
#include "wave_sub.h"

#ifdef DEBUG
#define _ATTR_SYM
#else  // !DEBUG
#define _ATTR_SYM		static
#endif // DEBUG


/*** 自ファイル内でのみ使用する#define マクロ ***/
#ifdef DEBUG
	#define LOG_LEVEL		LOG_DEBUG			/* テスト時ログレベル */
	#define LOG_PUTDISC		LOGDST_CNS			/* テスト時ログ出力先 */
#else /* not DEBUG */
	#define LOG_LEVEL		LOG_ERR				/* 運用時ログレベル */
	#define LOG_PUTDISC		LOGDST_SYSLOG		/* 運用時ログ出力先 */
#endif /* end DEBUG */

/* 時間指定 */
#define FOREVER					0				/* 時間指定なし */
#define INTERVAL_TIMER			2				/* 周期タイマ */
#define T50MS_TIMER				2				/* 50ms */
#define TIMER_COUNT				(150/50)		/* 150msカウンタにする */

/* ダウンロード結果 */
#define TFTP_RES_OK				0				/* 正常終了 */
#define TFTP_RES_PARAM			1				/* パラメータ異常（TFTP未実施） */
#define TFTP_RES_FL_NOTFOUND	2				/* ファイルなし受信（TFTP未完了） */
#define TFTP_RES_SERVER			3				/* サーバ未応答（TFTP未完了） */
#define TFTP_RES_CRC			4				/* ファイルエラー：CRCエラー（TFTP未完了） */
#define TFTP_RES_STATE			5				/* 未実施（TFTP未完了）*/
#define TFTP_RES_MISC			6				/* その他エラー */

/* フォルダ名、ファイル名 */
#define MAX_PROGFILE_LEN		17
#define SAVE_FOLDER		"/flsdata/"
#define TMP_FOLDER		"/tmp/"
#define TAR_FILE		"IPCS_V4.tar"
#define ONL_PROG		"IPCS_V4_PROG.BIN"
#define IPL_PROG		"IPCS_V4_BOOT.BIN"
#define FPGA_PROG		"IPCS_V4_FPGA.BIN"
#define FILEINFO_FILE	"FILEINFO.txt"
#define CONFIG_FILE		"config.dat"
#define WAVE_FILE		"HOLD.wav"
#define CaData_FILE		"ca_data.dat"
#define LuStart_FILE	"lu_start.dat"

#define FPGA_FOOTER_SIZE		48					/* FPGAフッダサイズ */
#define FPGA_FOOTER_STR			"UF7200IPstep2"		/* FPGAフッダの先頭文字列 */

/* ダウンロード種別 */
#define PROG_DL					1
#define WAVE_DL					2

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/

/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/* 状態番号 */
typedef enum TFTP_STATE_e {
	STATE_IDLE = 0,					/* アイドル状態 */
	STATE_SERVER_DL,				/* ダウンロード監視状態 */
	STATE_CLIENT_DL,				/* ダウンロード中状態 */
	MAX_STATE_NO
}TFTP_STATE_e;

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/
/*** ファイル内で共有するstatic 変数宣言 ***/
/* 状態番号 */
_ATTR_SYM	TFTP_STATE_e		downld_state_no;
/* タイマ登録ID(1～254) */
_ATTR_SYM	pBYTE				downld_tim;
/* ダウンロード完了判断カウンタ */
_ATTR_SYM	uint16_t			downld_same_cnt;

/* TFTP起動前のLED状態 */
_ATTR_SYM	uint8_t			prev_LED;

/* errno文字列生成バッファ */
_ATTR_SYM	char		str_buff[16];

/* ダウンロード種別 */
_ATTR_SYM	uint16_t			downld_type;

/* ダウンロードファイルリスト型 */
typedef struct DL_FILE_t {
	const char	*fl_name;			/* パス付ファイル名 */
	uint32_t	exist;				/* ファイル有無/ファイルNo */
	off_t		size;				/* ファイルサイズ */
} DL_FILE_t;

/* ダウンロードファイルリスト数 */
#define MAX_FILELIST			(2+1)

/* ダウンロードファイルリストテーブル */
DL_FILE_t	dl_files[MAX_FILELIST] = {
	{TMP_FOLDER TAR_FILE,		false, 0},
	{TMP_FOLDER FILEINFO_FILE,	false, 0},
	{NULL, false, 0}
};

/* ダウンロードファイルリスト数 */
#define MAX_UNCOMP_FILELIST		(5+1)
#define FILEINFO_NO				1
#define CONFIGFILE_NO			2
#define WAVEFILE_NO				3
#define CADATAFILE_NO			4
#define LUSTARTFILE_NO			5

/* ダウンロードファイルリストテーブル */
DL_FILE_t	uncomp_files[MAX_UNCOMP_FILELIST] = {
	{TMP_FOLDER FILEINFO_FILE,	FILEINFO_NO,	0},
	{TMP_FOLDER CONFIG_FILE,	CONFIGFILE_NO,	0},
	{TMP_FOLDER WAVE_FILE,		WAVEFILE_NO,	0},
	{TMP_FOLDER CaData_FILE,	CADATAFILE_NO,	0},
	{TMP_FOLDER LuStart_FILE,	LUSTARTFILE_NO, 0},
	{NULL, false, 0}
};

/*** static 関数宣言 ***/
#ifndef DEBUG
static void downld_init(void);
static void downld_SttMng(INNER_MSG *msg_p);
static BYTE dl_IdleStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_IdleStt_Cl1Req( INNER_MSG *msg_p );
static BYTE dl_IdleStt_Cl2Req( INNER_MSG *msg_p );
static BYTE dl_SvrStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_SvrStt_Tmo( INNER_MSG *msg_p );
static BYTE dl_SvrStt_Cl1Req( INNER_MSG *msg_p );
static BYTE dl_SvrStt_Cl2Req( INNER_MSG *msg_p );
static BYTE dl_SvrStt_WriteResp( INNER_MSG *msg_p );
static BYTE dl_ClStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_ClStt_Cl1Req( INNER_MSG *msg_p );
static BYTE dl_ClStt_Cl2Req( INNER_MSG *msg_p );
static BYTE dl_ClStt_WriteResp( INNER_MSG *msg_p );
static bool dl_tmpfile_chk(void);
static BYTE str2hex(const char *str, BYTE *hex, size_t siz);
static int writer_FileInfoFile(void);
static int writer_OnlineProg(char *fileinfo);
static int writer_BootProg(char *fileinfo);
static BYTE *seq_search(BYTE *buff, size_t siz, char *target, size_t len);
static int writer_FpgaProg(void);
static int wt_ProgFileWrite(char *wt_dev, FILE *rd_fp);
static int wt_FpgaFileWrite(FILE *rd_fp);
static BYTE tftp(INNER_MSG *msg_p, uint16_t cnt);
#endif // !DEBUG

/** 状態管理テーブル型 */
typedef struct STATE_TABLE_t {
		BYTE		event_id;
		BYTE		(*event_proc)(INNER_MSG *msg_p);
} STATE_TABLE_t;

/** 状態管理テーブル */
STATE_TABLE_t	STATE_IDLE_Table[] = {
		{ I_SERVER_REQ,			dl_IdleStt_SvrReq },
		{ O_PGDOWNLOAD,			dl_IdleStt_Cl1Req },
		{ O_HORYDOWN,			dl_IdleStt_Cl2Req },
		{ 0, NULL }
};

STATE_TABLE_t	STATE_SERVER_DL_Table[] = {
		{ I_SERVER_REQ,			dl_SvrStt_SvrReq },
		{ TIM_OUT,				dl_SvrStt_Tmo },
		{ O_PGDOWNLOAD,			dl_SvrStt_Cl1Req },
		{ O_HORYDOWN,			dl_SvrStt_Cl2Req },
		{ I_WRITE_RESP,			dl_SvrStt_WriteResp },
		{ 0, NULL }
};

STATE_TABLE_t	STATE_CLIENT_DL_Table[] = {
		{ I_SERVER_REQ,			dl_ClStt_SvrReq },
		{ O_PGDOWNLOAD,			dl_ClStt_Cl1Req },
		{ O_HORYDOWN,			dl_ClStt_Cl2Req },
		{ I_WRITE_RESP,			dl_ClStt_WriteResp },
		{ 0, NULL }
};

STATE_TABLE_t	*RootStateTable[MAX_STATE_NO] = {
		STATE_IDLE_Table,
		STATE_SERVER_DL_Table,
		STATE_CLIENT_DL_Table,
};


/******************************************************************************/
/* 関数名	  errno文字列変換												  */
/* 機能概要	  引数のerrnoを文字化する										  */
/* パラメータ err : (in)	変換するerrno									  */
/* リターン	  変換した文字列												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define CASE_STR(a)	case a:	 return #a
_ATTR_SYM const char *mkstr_errno(int err)
{
	switch(err)
	{
	  CASE_STR(EACCES);
	  CASE_STR(ENOEXEC);
	  CASE_STR(ENOMEM);
	  CASE_STR(E2BIG);
	  default:
		sprintf(str_buff, "misc(%d)",err);
		return str_buff;
	}
}

/******************************************************************************/
/* 関数名	  文字列→HEX数値変換											  */
/* 機能概要	  文字列(null付)を16進数数値に変換するがノンエラーチェックである  */
/* パラメータ str : (in)	変換対象の文字列								  */
/*			  hex : (out)	変換した数値を格納する領域						  */
/*			  size: (in)	数値を格納する領域のサイズ						  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE str2hex(const char *str, BYTE *hex, size_t siz)
{
	char	ch  = 0;
	BYTE	val = 0;

	while (siz > 0)
	{
		ch = *str++;
		val = ch - 0x30;
		if (val >= 0x31)
		{	/* 小文字(a～) */
			val -= 0x27;
		}
		else if (val >= 0x11)
		{	/* 大文字(A～) */
			val -= 0x07;
		}
		if (val > 0x0f)
		{
			return NG;
		}
		*hex = val << 4;

		ch = *str++;
		val = ch - 0x30;
		if (val >= 0x31)
		{
			val -= 0x27;
		}
		else if (val >= 0x11)
		{
			val -= 0x07;
		}
		if (val > 0x0f)
		{
			return NG;
		}
		*hex |= val;
		siz--;
	}
	return OK;
}

/******************************************************************************/
/* 関数名	  ダウンロード完了通知メッセージ送信							  */
/* 機能概要	  LUMNG宛にダウンロード完了通知メッセージを送信する				  */
/* パラメータ result : (in)	  結果 TFTP_RES_OKなど							  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/*																			  */
/******************************************************************************/
_ATTR_SYM void dl_sndmsg(BYTE ecb, BYTE kind, BYTE result)
{
	INNER_MSG	*msg_p = NULL;

	msg_p = (INNER_MSG*)com_poolget(POOL0);

	msg_p->msg_header.id	= DWL_ID;
	msg_p->msg_header.div	= IN_MSG;
	msg_p->msg_header.kind	= kind;
	msg_p->msg_header.no	= result;
	msg_p->msg_header.link	= NULL;

	com_sndmsg(ecb, (BYTE*)msg_p);
}

/******************************************************************************/
/* 関数名	  downloadスレッド												  */
/* 機能概要	  保守動作向けTFTPサーバ機能および通常動作向けダウンロード機能	  */
/*			  を提供する													  */
/* パラメータ arg : (in)	  未使用										  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void downld_thread(void *arg)
{
	INNER_MSG	*msg_p = NULL;

	downld_init();
	/* writerスレッド起動 */
	com_threadstart(WRT_ID, NULL);
	while (1)
	{
		msg_p = (INNER_MSG*)com_rcvmsg(TFTP_ECB, FOREVER);

		downld_SttMng(msg_p);

		if (msg_p->msg_header.link != NULL)
		{
			com_poolput(POOL1, msg_p->msg_header.link);
		}
		com_poolput(POOL0, (BYTE*)msg_p);
	}
}

/******************************************************************************/
/* 関数名	  TFTP機能の初期設定											  */
/* 機能概要	  内部管理変数を初期設定する									  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM void downld_init(void)
{
	/* 変数初期化 */
	downld_state_no = STATE_IDLE;
	downld_tim		= 0;

	/* ログレベル設定 */
	dbg_print_set(DWL_ID, LOG_LEVEL, LOG_PUTDISC);
	dbg_print_set(WRT_ID, LOG_LEVEL, LOG_PUTDISC);
}

/******************************************************************************/
/* 関数名	  状態管理処理													  */
/* 機能概要	  downldスレッドの状態管理を行う								  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM void downld_SttMng(INNER_MSG *msg_p)
{
	STATE_TABLE_t	*p_tbl	 = NULL;
	BYTE			event_id = 0;

	if (downld_state_no >= MAX_STATE_NO)
	{
		return;		/* Bug */
	}

	if (msg_p->msg_header.div == TIM_OUT)
	{
		event_id = TIM_OUT;
	} else {
		event_id = msg_p->msg_header.kind;
	}

	p_tbl = RootStateTable[downld_state_no];
	while (p_tbl->event_proc != NULL)
	{
		if (p_tbl->event_id == event_id)
		{
			(*p_tbl->event_proc)(msg_p);
			break;
		}
		p_tbl++;
	}
}

/******************************************************************************/
/* 関数名	  IDLE状態でのTFTPサーバ要求処理								  */
/* 機能概要	  TFTPサーバ起動、タイマ要求、FPGA-LED制御を行う				  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_IdleStt_SvrReq(INNER_MSG *msg_p)
{
	int		retv = 0;

	retv = execlp("atftpd", "--daemon", "--pidfile", "/tmp/tftpd-pid.txt", "/tmp", NULL);
	if (retv == -1 )
	{
		dbg_print(DWL_ID, LOG_ERR, "TFTP Server Start Error:%s", mkstr_errno(errno));
		dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_SERVER);
		downld_state_no = STATE_IDLE;
		return NG;
	}

	downld_tim = tmr_entry(TFTP_ECB, T50MS_TIMER, INTERVAL_TIMER);
	tmr_start(downld_tim, TIMER_COUNT);

	com_FpgaLED(ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, LED_ON | LED_LU | LED_BLUE|LED_RED, &prev_LED);
	downld_state_no = STATE_SERVER_DL;
	return OK;
}

/******************************************************************************/
/* 関数名	  IDLE状態でのプログラムダウンロード要求処理					  */
/* 機能概要	  TFTPクライアントによるダウンロード処理を行う					  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_IdleStt_Cl1Req(INNER_MSG *msg_p)
{
	downld_type = PROG_DL;
	if (tftp(msg_p, PROG_DL) != OK)
	{
		dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_SERVER);
	}
	return OK;
}

/******************************************************************************/
/* 関数名	  IDLE状態での保留音ダウンロード要求処理						  */
/* 機能概要	  TFTPクライアントによるダウンロード処理を行う					  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_IdleStt_Cl2Req(INNER_MSG *msg_p)
{
	downld_type = WAVE_DL;
	if (tftp(msg_p, WAVE_DL) != OK)
	{
		dl_sndmsg(LUMNG_ECB, E_HORYDLEND, TFTP_RES_SERVER);
	}
	return OK;
}

/******************************************************************************/
/* 関数名	  Server動作中状態でのダウンローダ要求処理						  */
/* 機能概要	  ダウンローダ結果応答[エラー]を返送する						  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_SvrStt_SvrReq(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Server動作中状態でのタイムアウト処理							  */
/* 機能概要	  ダウンロードファイルのダウンロード終了を監視する				  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_SvrStt_Tmo(INNER_MSG *msg_p)
{
	if (dl_tmpfile_chk() != false)
	{
		/* ダウンロードファイルサイズが同一(変化なし) */
		downld_same_cnt++;
		if (downld_same_cnt >= 3)
		{
			/* ダウンロード終了と見なす */
			tmr_stop(msg_p->msg_header.kind);	
			tmr_delete(msg_p->msg_header.kind);

			dl_sndmsg(WRITER_ECB, I_WRITE_REQ, 0);
			downld_same_cnt = 0;				/* カウンタクリア */
		}
	}
	else
	{
		/* ダウンロードファイルサイズが変化している */
		downld_same_cnt = 0;					/* カウンタリセット */
	}
	return OK;
}

/******************************************************************************/
/* 関数名	  Server動作中状態でのプログラムダウンロード要求処理			  */
/* 機能概要	  プログラムダウンロード完了通知[エラー]を返送する				  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_SvrStt_Cl1Req(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Server動作中状態での保留音ダウンロード要求処理				  */
/* 機能概要	  保留音ダウンロード完了通知[エラー]を返送する				  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_SvrStt_Cl2Req(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, E_HORYDLEND, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Server動作中状態でのファイル書込み応答処理					  */
/* 機能概要	  ダウンロード結果をダウンローダ結果応答[結果]で返送する		  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_SvrStt_WriteResp(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, msg_p->msg_header.no);
	downld_state_no = STATE_IDLE;
	return OK;
}

/******************************************************************************/
/* 関数名	  Client動作中状態でのダウンローダ要求処理						  */
/* 機能概要	  ダウンローダ結果応答[エラー]を返送する						  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_ClStt_SvrReq(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Client動作中状態でのプログラムダウンロード要求処理			  */
/* 機能概要	  プログラムダウンロード完了通知[エラー]を返送する				  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_ClStt_Cl1Req(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Client動作中状態での保留音ダウンロード要求処理				  */
/* 機能概要	  保留音ダウンロード完了通知[エラー]を返送する					  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_ClStt_Cl2Req(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, E_HORYDLEND, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名	  Client動作中状態でのファイル書込み応答処理					  */
/* 機能概要	  ダウンロード結果をダウンローダ結果応答[結果]で返送する		  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE dl_ClStt_WriteResp(INNER_MSG *msg_p)
{
	if (downld_type == PROG_DL)
	{
		dl_sndmsg(LUMNG_ECB, I_PGDLCMP, msg_p->msg_header.no);
	}
	else
	{
		dl_sndmsg(LUMNG_ECB, E_HORYDLEND, msg_p->msg_header.no);
	}
	downld_state_no = STATE_IDLE;
	return OK;
}


/******************************************************************************/
/* 関数名	  ダウンロードファイルサイズ変化確認							  */
/* 機能概要	  ダウンロードファイルのファイルの有無とサイズ変化を調べる		  */
/* パラメータ なし															  */
/* リターン	  TRUE	: 変化なし												  */
/*			  FALSE : 変化あり												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM bool dl_tmpfile_chk(void)
{
	bool			same		= true;
	uint16_t		fl_cnt		= 0;
	DL_FILE_t		*fl_list_p	= dl_files;
	struct stat		fl_stat		= {};

	fl_cnt	  = 0;
	fl_list_p = dl_files;
	while (fl_list_p->fl_name != NULL)
	{
		if (stat(fl_list_p->fl_name, &fl_stat) == 0)
		{
			fl_cnt++;
			if ((fl_list_p->exist != true )||
				(fl_list_p->size != fl_stat.st_size))
			{
				same = false;
			}
			fl_list_p->exist = true;
			fl_list_p->size	 = fl_stat.st_size;
		}
		else
		{
			if ((fl_list_p->exist != false)||
				(fl_list_p->size != 0))
			{
				same = false;
			}
			fl_list_p->exist = false;
			fl_list_p->size	 = 0;
		}
		fl_list_p++;
	}

	if (fl_cnt != MAX_FILELIST -1)
	{
		same = false;
	}
	return same;
}

/******************************************************************************/
/* 関数名	  writerスレッド												  */
/* 機能概要	  ファイル書込み機能のスレッドである							  */
/* パラメータ arg : (in)	  未使用										  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void writer_thread(void *arg)
{
	INNER_MSG	*msg_p		= NULL;
	DL_FILE_t	*fl_list_p	= uncomp_files;
	struct stat	fl_stat		= {};
	int			retv		= 0;
	BYTE		result		= OK;

	while (1)
	{
		msg_p = (INNER_MSG*)com_rcvmsg(WRITER_ECB, FOREVER);

		fl_list_p = uncomp_files;
		while (fl_list_p->fl_name != NULL)
		{
			if (stat(fl_list_p->fl_name, &fl_stat) == 0)
			{
				switch (fl_list_p->exist)
				{
				  case FILEINFO_NO:
					retv = (int)writer_FileInfoFile();
					if (retv != OK)
					{
						result = (BYTE)retv;
					}
					execlp("rm", "-f", TMP_FOLDER FILEINFO_FILE, NULL);
					execlp("rm", "-f", TMP_FOLDER TAR_FILE, NULL);
					break;
				  case CONFIGFILE_NO:
					mnt_config_dat(fl_list_p->fl_name, 1);
					break;
				  case WAVEFILE_NO:
					if (wave_file_write(fl_list_p->fl_name) != OK)
					{
						result = NG;
					}
					break;
				default:
					if (execlp("mv", fl_list_p->fl_name, SAVE_FOLDER, NULL) != 0)
					{
						result = NG;
					}
					break;
				}
			}
		}
		dl_sndmsg(TFTP_ECB, I_WRITE_RESP, result);

		com_poolput(POOL0, (BYTE*)msg_p);
	}
}

/******************************************************************************/
/* 関数名	  FILEINFOファイルによるプログラムファイル書き込み				  */
/* 機能概要	  FILEINFOの内容により、プログラム系ファイルをFLASHに書き込む	  */
/* パラメータ なし															  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  FILEINFO書式：												  */
/*	IPCS_V4_PROG.BIN,04.00,2020/07/15 11:38:07,9999999,xxxxxxxxxxxxxxxxx	  */
/*	IPCS_V4_BOOT.BIN,04.00,2020/07/15 11:38:07,9999999,xxxxxxxxxxxxxxxxx	  */
/*	IPCS_V4_FPGA.BIN														  */
/******************************************************************************/
_ATTR_SYM int writer_FileInfoFile(void)
{
	int		result	= TFTP_RES_FL_NOTFOUND;
	int		retv	= 0;
	FILE	*fp		= NULL;
	char	*next_p	= NULL;
	char	buff[256];

	fp = fopen(TMP_FOLDER FILEINFO_FILE, "rt");
	if (fp != NULL)
	{
		while (feof(fp) == 0)
		{
			if (fgets(buff, sizeof(buff[256]), fp) != NULL)
			{
				next_p = strtok(buff, ",");
				/* 書込み対象のプログラムファイルだけを解凍する */
				retv = execlp("tar", "-xf", TMP_FOLDER TAR_FILE, buff, NULL);
				if (retv != 0)
				{
					return TFTP_RES_FL_NOTFOUND;
				}

				if (strncmp(buff, ONL_PROG, MAX_PROGFILE_LEN) == 0)
				{
					result = writer_OnlineProg(next_p);
					if (result != OK)
					{
						break;
					}
				}
				else if (strncmp(buff, IPL_PROG, MAX_PROGFILE_LEN) == 0)
				{
					result = writer_BootProg(next_p);
					if (result != OK)
					{
						break;
					}
				}
				else if (strncmp(buff, FPGA_PROG, MAX_PROGFILE_LEN) == 0)
				{
					result = writer_FpgaProg();
					if (result != OK)
					{
						break;
					}
				}
			}
		}
		fclose(fp);
	}
	return result;
}

/******************************************************************************/
/* 関数名	  オンラインプログラムの書き込み								  */
/* 機能概要	  オンラインプログラムの正常性確認後FLASHに書き込む				  */
/* パラメータ fileinfo : (in)	FILEINFOファイルの対象行					  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  FILEINFO書式：												  */
/*	IPCS_V4_PROG.BIN,04.00,2020/07/15 11:38:07,9999999,xxxxxxxxxxxxxxxxx	  */
/*					↑引数は、ココから渡される								  */
/******************************************************************************/
_ATTR_SYM int writer_OnlineProg(char *fileinfo)
{
	int		result	= TFTP_RES_FL_NOTFOUND;
	int		retv	= 0;
	FILE	*fp		= NULL;
	char	*next_p	= NULL;
	MD5_CTX	ctx		= {};
	BYTE	buff[512];
	BYTE	hash[16];

	/* バージョン比較 */
	next_p = strtok(fileinfo, ",");			/* next_pは日付を指す */
	if (next_p == NULL)
	{
		return TFTP_RES_MISC;
	}

	fp = fopen(TMP_FOLDER ONL_PROG, "rb");
	if (fp == NULL)
	{
		return TFTP_RES_FL_NOTFOUND;
	}
	while(1)
	{
		/* MD5計算開始 */
		if (MD5_Init(&ctx) == 0)
		{
			result = TFTP_RES_CRC;
			break;
		}

		result = TFTP_RES_OK;
		while (feof(fp) == 0)
		{
			retv = fread(buff, sizeof(buff[512]), 1, fp);
			if (retv > 0)
			{
				if (MD5_Update(&ctx, buff, retv) == 0)
				{
					result = TFTP_RES_CRC;
					break;
				}
			}
		}
		if (MD5_Final(buff, &ctx) == 0)			/* buffにハッシュ値(16byte)が格納される */
		{
			result = TFTP_RES_CRC;
		}
		if (result != TFTP_RES_OK)
		{
			break;
		}

		/* MD5比較 */
		next_p = strtok(next_p, ",");			/* next_pはサイズを指す */
		if (next_p == NULL)
		{
			result = TFTP_RES_MISC;
			break;
		}
		next_p = strtok(next_p, ",");			/* next_pはMD5ハッシュ値を指す */
		if (next_p == NULL)
		{
			result = TFTP_RES_MISC;
			break;
		}
		str2hex(next_p, hash, sizeof(hash[16]));
		if (memcmp(hash, buff, 16) != 0)
		{
			result = TFTP_RES_CRC;
			break;
		}

		/* プログラム書き込み処理 */
		//wt_ProgFileWrite()でシークするから不要
		//if (fseek(fp, 0, SEEK_SET) != 0)
		//{
		//	result = TFTP_RES_MISC;
		//	break;
		//}
		if (wt_ProgFileWrite("/dev/mtdblock1", fp) != OK)
		{
			result = TFTP_RES_MISC;
		}

		break;
	}
	fclose(fp);
	return result;
}

/******************************************************************************/
/* 関数名	  IPLプログラムの書き込み										  */
/* 機能概要	  IPL(BootLoader)プログラムの正常性確認後FLASHに書き込む		  */
/* パラメータ fileinfo : (in)	FILEINFOファイルの対象行					  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  FILEINFO書式：												  */
/*	IPCS_V4_BOOT.BIN,04.00,2020/07/15 11:38:07,9999999,xxxxxxxxxxxxxxxxx	  */
/*					↑引数は、ココから渡される								  */
/******************************************************************************/
_ATTR_SYM int writer_BootProg(char *fileinfo)
{
	int		result	= TFTP_RES_FL_NOTFOUND;
	int		retv	= 0;
	FILE	*fp		= NULL;
	char	*next_p	= NULL;
	MD5_CTX	ctx		= {};
	BYTE	buff[512];
	BYTE	hash[16];

	/* バージョン比較 */
	next_p = strtok(fileinfo, ",");			/* next_pは日付を指す */
	if (next_p == NULL)
	{
		return TFTP_RES_MISC;
	}

	fp = fopen(TMP_FOLDER IPL_PROG, "rb");
	if (fp == NULL)
	{
		return TFTP_RES_FL_NOTFOUND;
	}
	while(1)
	{
		/* MD5計算開始 */
		if (MD5_Init(&ctx) == 0)
		{
			result = TFTP_RES_CRC;
			break;
		}

		result = TFTP_RES_OK;
		while (feof(fp) == 0)
		{
			retv = fread(buff, sizeof(buff[512]), 1, fp);
			if (retv > 0)
			{
				if (MD5_Update(&ctx, buff, retv) == 0)
				{
					result = TFTP_RES_CRC;
					break;
				}
			}
		}
		if (MD5_Final(buff, &ctx) == 0)			/* buffにハッシュ値(16byte)が格納される */
		{
			result = TFTP_RES_CRC;
		}
		if (result != TFTP_RES_OK)
		{
			break;
		}

		/* MD5比較 */
		next_p = strtok(next_p, ",");			/* next_pはサイズを指す */
		if (next_p == NULL)
		{
			result = TFTP_RES_MISC;
			break;
		}
		next_p = strtok(next_p, ",");			/* next_pはMD5ハッシュ値を指す */
		if (next_p == NULL)
		{
			result = TFTP_RES_MISC;
			break;
		}
		str2hex(next_p, hash, sizeof(hash[16]));
		if (memcmp(hash, buff, 16) != 0)
		{
			result = TFTP_RES_CRC;
			break;
		}

		/* プログラム書き込み処理 */
		//wt_ProgFileWrite()でシークするから不要
		//if (fseek(fp, 0, SEEK_SET) != 0)
		//{
		//	result = TFTP_RES_MISC;
		//	break;
		//}
		if (wt_ProgFileWrite("/dev/mtdblock2", fp) != OK)
		{
			result = TFTP_RES_MISC;
		}

		break;
	}
	fclose(fp);
	return result;
}

/******************************************************************************/
/* 関数名	  シーケンシャル検索アルゴリズム								  */
/* 機能概要	  buff内からtargetに合致する部分検索を行う						  */
/* パラメータ buff   : (in)	検索領域のバッファ								  */
/*			  siz    : (in)	検索領域のバイト数								  */
/*			  target : (in)	検索するデータのポインタ						  */
/*			  len    : (in)	検索するデータのバイト長						  */
/* リターン	  targetに一致した検索領域内のポインタ							  */
/*			  NULLの場合、一致箇所なし										  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE *seq_search(BYTE *buff, size_t siz, char *target, size_t len)
{
	size_t		i    = 0;
	size_t		same = 0;
	BYTE		*p   = NULL;

	while (i < siz)
	{
		p = (BYTE*)target;
		while ((i < siz)&&(*buff != *p))
		{
			i++;
			buff++;
		}
		if (i >= siz)
		{
			return NULL;
		}
		same = 1;
		p++;
		while ((i < siz)&&(*buff == *p)&&(same != len))
		{
			i++;
			buff++;
			p++;
			same++;
		}
		if (same == len)
		{
			return buff - len;
		}
	}
	return NULL;
}

/******************************************************************************/
/* 関数名	  FPGAプログラムの書き込み										  */
/* 機能概要	  FPGAプログラムの正常性確認後FLASHに書き込む					  */
/* パラメータ fileinfo : (in)	FILEINFOファイルの対象行					  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  FILEINFO書式：												  */
/*	IPCS_V4_FPGA.BIN														  */
/*					↑引数は、ココから渡される								  */
/******************************************************************************/
_ATTR_SYM int writer_FpgaProg(void)
{
	int		result	= TFTP_RES_FL_NOTFOUND;
	int		rd_siz  = 0;
	FILE	*fp		= NULL;
	BYTE	*p		= NULL;
	BYTE	*ver_p	= NULL;
	BYTE	sum		= 0;
	BYTE	ver		= 0;
	WORD	i		= 0;
	struct stat		fl_stat		= {};
	BYTE	buff[512];

	/* FPGAプログラムサイズを基にSPI-FLASHからヘッダ情報辺りをリードして、版数を取得する */
	if (stat(TMP_FOLDER FPGA_PROG, &fl_stat) != 0)
	{
		return TFTP_RES_FL_NOTFOUND;
	}
	fl_stat.st_size -= sizeof(buff[512]);
	if (com_SpiflashRead(fl_stat.st_size, sizeof(buff[512])/sizeof(WORD), buff) == -1)
	{
		return TFTP_RES_FL_NOTFOUND;
	}
	p = seq_search(buff, 512, FPGA_FOOTER_STR, strlen(FPGA_FOOTER_STR));
	if (p == NULL)
	{
		return TFTP_RES_MISC;
	}
	ver = p[FPGA_FOOTER_SIZE -1];

	/* 書き込むファイルのチェック */
	fp = fopen(TMP_FOLDER FPGA_PROG, "rb");
	if (fp == NULL)
	{
		return TFTP_RES_FL_NOTFOUND;
	}
	while(1)
	{
		sum    = 0;
		result = TFTP_RES_OK;
		while (feof(fp) == 0)
		{
			rd_siz = fread(buff, sizeof(buff[512]), 1, fp);
			if (rd_siz > 0)
			{
				p = buff;
				for (i=0; i < rd_siz; i++)
				{
					sum += *p++;
				}
				ver_p = &buff[rd_siz -1];	/* フッタ情報内の版数の場所を仮保存する */
			}
		}

		/* チェックサム判定 */
		if (sum != 0)
		{
			result = TFTP_RES_CRC;
			break;
		}
		/* 版数が異なれば、プログラムを書き込む */
		if( ver != *ver_p )
		{
			/* プログラム書き込み処理 */
			if (wt_FpgaFileWrite(fp) != OK)
			{
				result = TFTP_RES_MISC;
			}
		}
		break;
	}
	fclose(fp);
	return result;
}

/******************************************************************************/
/* 関数名	  TIBINARYプログラムをFLASH(mtd1/mtd2)に書き込む				  */
/* 機能概要	  mtd1／mtd2にプログラムファイルを書き込む						  */
/* パラメータ wt_dev : (in)	書き込み先FLASHデバイス名						  */
/*            rd_fp  : (in)	書き込むプログラムファイル						  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM int wt_ProgFileWrite(char *wt_dev, FILE *rd_fp)
{
	int		result	= NG;
	int		retv	= 0;
	int		rd_sz	= 0;
	FILE	*fp	= NULL;
	char	buff[512];

	if (fseek(rd_fp, 10, SEEK_SET) != 0)
	{
		return NG;
	}

	fp = fopen(wt_dev, "wb");
	if (fp != NULL)
	{
		while (feof(rd_fp) == 0)
		{
			result = OK;
			rd_sz = fread(buff, sizeof(buff[512]), 1, rd_fp);
			if (rd_sz < 0)
			{
				result = NG;
				break;
			}
			else if (rd_sz > 0)
			{
				retv = fwrite(buff, rd_sz, 1, fp);
				if (retv != rd_sz)
				{
					result = NG;
					break;
				}
			}
		}
	}
	fclose(fp);
	return result;
}

/******************************************************************************/
/* 関数名	  FPGAプログラムの書き込み										  */
/* 機能概要	  SPI-FLASHにプログラムファイルを書き込む						  */
/* パラメータ rd_fp  : (in)	書き込むプログラムファイル						  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM int wt_FpgaFileWrite(FILE *rd_fp)
{
	int		result	= NG;
	int		retv	= 0;
	int		rd_sz	= 0;
	FILE	*fp	= NULL;
	char	buff[512];

	/* SPI-ROMゲート開放 */
	if (com_GpioRegUpdate(GPIO_OUT0_REG, SPI_GATE_MASK, ~SPI_GATE_CLOSE) != OK)
	{
		return NG;
	}

	/* 書き込むプログラムファイルを先頭にシークする */
	if (fseek(rd_fp, 0, SEEK_SET) != 0)
	{
		return NG;
	}

	fp = fopen("/dev/spi-flash", "wb");
	if (fp != NULL)
	{
		while (feof(rd_fp) == 0)
		{
			result = OK;
			rd_sz = fread(buff, sizeof(buff[512]), 1, rd_fp);
			if (rd_sz < 0)
			{
				result = NG;
				break;
			}
			else if (rd_sz > 0)
			{
				retv = fwrite(buff, rd_sz, 1, fp);
				if (retv != rd_sz)
				{
					result = NG;
					break;
				}
			}
		}
	}
	fclose(fp);

	if (com_GpioRegUpdate(GPIO_OUT0_REG, SPI_GATE_MASK, SPI_GATE_CLOSE) != OK)
	{
		return NG;
	}
	return result;
}

/******************************************************************************/
/* 関数名	  TFTPクライアントによるプログラムダウンロード					  */
/* 機能概要	  プログラムダウンロードMSGにより、TFTPサーバからファイルを		  */
/*			  ダウンロードする												  */
/* パラメータ msg_p : (in)	  内部受信メッセージ							  */
/*			  cnt   : (in)	  ダウンロード回数(=1:プログラム／=2:保留音)	  */
/* リターン	  OK : 正常終了													  */
/*			  NG : エラー													  */
/* 注意事項	  －															  */
/* その他	  二次リンク構造												  */
/*				FILE-NAME[配列]		DLプログラム名＋NULL					  */
/*				MODE[配列]			DLモード="octed"+NULL					  */
/*				IP_TYPE				0=IPv4									  */
/*				AP_ADDR[4]			IPv4アドレス							  */
/******************************************************************************/
_ATTR_SYM BYTE tftp(INNER_MSG *msg_p, uint16_t cnt)
{
	char	*fl_name		= NULL;
	char	*dl_mode		= NULL;
	BYTE	*ip_addr		= NULL;
	size_t	len				= 0;
	uint16_t	loop		= 0;
	int		retv			= 0;
	char	ipaddr_str[16]	= {};

	fl_name = (char*)msg_p->msg_header.link;
	while (loop < cnt)
	{
		len = strnlen(fl_name, 128+1);
		if ((len == 128+1)||(len == 0))
		{
			return NG;
		}

		dl_mode = fl_name + len +1;
		len = strnlen(dl_mode, 6+1);
		if (strcmp(dl_mode, "octed") != 0)
		{
			return NG;
		}

		ip_addr = (BYTE*)dl_mode + len +1 +1;		/* IP_TYPEは不要なので無視 */
		if (inet_ntop(AF_INET, ip_addr, ipaddr_str, sizeof(ipaddr_str[16])) == NULL)
		{
			return NG;
		}

		retv = execlp("atftp", "-g -r", fl_name, ipaddr_str, NULL);
		if (retv != 0)
		{
			return NG;
		}

		fl_name = (char*)ip_addr + 16;						/* IPアドレス領域は16byteある */
		loop++;
	}
	dl_sndmsg(WRITER_ECB, I_WRITE_REQ, 0);
	return OK;
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  TFTPタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
