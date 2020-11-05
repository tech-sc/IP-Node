/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       TFTPタスク機能のソースファイル                                  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>	/* stat() */
#include <sys/stat.h>		/* stat() */
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>     	/* execlp(), stat() */

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

/*** 自ファイル内でのみ使用する#define マクロ ***/
#ifdef DEBUG
    #define LOG_LEVEL       LOG_DEBUG           /* テスト時ログレベル */
    #define LOG_PUTDISC     LOGDST_CNS          /* テスト時ログ出力先 */
#else /* not DEBUG */
    #define LOG_LEVEL       LOG_ERR             /* 運用時ログレベル */
    #define LOG_PUTDISC     LOGDST_SYSLOG       /* 運用時ログ出力先 */
#endif /* end DEBUG */

/* 時間指定 */
#define FOREVER                 0				/* 時間指定なし */
#define INTERVAL_TIMER          2				/* 周期タイマ */
#define T50MS_TIMER             2               /* 50ms */
#define TIMER_COUNT             (150/50)        /* 150msカウンタにする */

/* ダウンロード結果 */
#define TFTP_RES_OK             0               /* 正常終了 */
#define TFTP_RES_PARAM          1               /* パラメータ異常（TFTP未実施） */
#define TFTP_RES_FL_NOTFOUND    2               /* ファイルなし受信（TFTP未完了） */
#define TFTP_RES_SERVER         3               /* サーバ未応答（TFTP未完了） */
#define TFTP_RES_CRC            4               /* ファイルエラー：CRCエラー（TFTP未完了） */
#define TFTP_RES_STATE          5               /* 未実施（TFTP未完了）*/
#define TFTP_RES_MISC           6               /* その他エラー */

/* フォルダ名、ファイル名 */
#define SAVE_FOLDER     "/flsdata/"
#define TMP_FOLDER      "/tmp/"
#define TAR_FILE        "IPCS_V4.tar"
#define ONL_PROG        "IPCS_V4_PROG.BIN"
#define IPL_PROG        "IPCS_V4_BOOT.BIN"
#define FPGA_PROG       "IPCS_V4_FPGA.BIN"
#define CONFIG_FILE     "config.dat"
#define WAV_FILE        "HOLD.wav"
#define CaData_FILE     "ca_data.dat"
#define LuStart_FILE    "lu_start.dat"

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/

/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/** 状態管理テーブル型 */
typedef struct {
        BYTE		event_id;
        BYTE		(*event_proc)(INNER_MSG *msg_p);
} STATE_TABLE_t;

/* ダウンロードファイルリスト型 */
typedef struct DL_FILE_t {
    const char  *fl_name;           /* パス付ファイル名 */
    bool        exist;              /* ファイル有無 */
    off_t       size;               /* ファイルサイズ */
} DL_FILE_t;

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
static TFTP_STATE_e     downld_state_no;
/* タイマ登録ID(1～254) */
static BYTE             downld_tim;
/* ダウンロード完了判断カウンタ */
uint16_t                downld_same_cnt;

/* TFTP起動前のLED状態 */
static uint8_t          prev_LED;

/* errno文字列生成バッファ */
static char     str_buff[16];

/* ダウンロードファイルリスト数 */
#define MAX_FILELIST            9

/* ダウンロードファイルリストテーブル */
DL_FILE_t   dl_files[MAX_FILELIST] = {
    {TMP_FOLDER TAR_FILE,     false, 0},
    {TMP_FOLDER ONL_PROG,     false, 0},
    {TMP_FOLDER IPL_PROG,     false, 0},
    {TMP_FOLDER FPGA_PROG,    false, 0},
    {TMP_FOLDER CONFIG_FILE,  false, 0},
    {TMP_FOLDER WAV_FILE,     false, 0},
    {TMP_FOLDER CaData_FILE,  false, 0},
    {TMP_FOLDER LuStart_FILE, false, 0},
    {NULL, false, 0}
};

/*** static 関数宣言 ***/
static void downld_init(void);
static void downld_SttMng(INNER_MSG *msg_p);
static BYTE dl_IdleStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_IdleStt_ClReq( INNER_MSG *msg_p );
static BYTE dl_SvrStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_SvrStt_Tmo( INNER_MSG *msg_p );
static BYTE dl_SvrStt_ClReq( INNER_MSG *msg_p );
static BYTE dl_SvrStt_WriteResp( INNER_MSG *msg_p );
static BYTE dl_ClStt_SvrReq( INNER_MSG *msg_p );
static BYTE dl_ClStt_ClReq( INNER_MSG *msg_p );
static BYTE dl_ClStt_WriteResp( INNER_MSG *msg_p );
static bool dl_tmpfile_chk(void);

/** 状態管理テーブル */
STATE_TABLE_t   STATE_IDLE_Table[] = {
        { I_SERVER_REQ,         dl_IdleStt_SvrReq },
        { I_CLIENT_REQ,         dl_IdleStt_ClReq },
        { 0, NULL }
};

STATE_TABLE_t   STATE_SERVER_DL_Table[] = {
        { I_SERVER_REQ,         dl_SvrStt_SvrReq },
        { TIM_OUT,              dl_SvrStt_Tmo },
        { I_CLIENT_REQ,         dl_SvrStt_ClReq },
        { I_WRITE_RESP,         dl_SvrStt_WriteResp },
        { 0, NULL }
};

STATE_TABLE_t   STATE_CLIENT_DL_Table[] = {
        { I_SERVER_REQ,         dl_ClStt_SvrReq },
        { I_CLIENT_REQ,         dl_ClStt_ClReq },
        { I_WRITE_RESP,         dl_ClStt_WriteResp },
        { 0, NULL }
};

STATE_TABLE_t   *RootStateTable[MAX_STATE_NO] = {
        STATE_IDLE_Table,
        STATE_SERVER_DL_Table,
        STATE_CLIENT_DL_Table,
};


/******************************************************************************/
#define CASE_STR(a)	case a:  return #a
const char *mkstr_errno(int err)
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
/* 関数名     ダウンロード完了通知メッセージ送信                              */
/* 機能概要   LUMNG宛にダウンロード完了通知メッセージを送信する               */
/* パラメータ result : (in)   結果 TFTP_RES_OKなど                            */
/* リターン   なし                                                            */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/*                                                                            */
/******************************************************************************/
static void dl_sndmsg(BYTE ecb, BYTE kind, BYTE result)
{
    INNER_MSG   *msg_p = NULL;

    msg_p = (INNER_MSG*)com_poolget(POOL0);

    msg_p->msg_header.id    = DWL_ID;
    msg_p->msg_header.div   = IN_MSG;
    msg_p->msg_header.kind  = kind;
    msg_p->msg_header.no    = result;
    msg_p->msg_header.link  = NULL;

    com_sndmsg(ecb, (BYTE*)msg_p);
}

/******************************************************************************/
/* 関数名     downloadスレッド                                                */
/* 機能概要   保守動作向けTFTPサーバ機能および通常動作向けダウンロード機能    */
/*            を提供する                                                      */
/* パラメータ arg : (in)      未使用                                          */
/* リターン   なし                                                            */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
void downld_thread(void *arg)
{
    INNER_MSG   *msg_p = NULL;

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
/* 関数名     TFTP機能の初期設定                                              */
/* 機能概要   内部管理変数を初期設定する                                      */
/* パラメータ なし                                                            */
/* リターン   なし                                                            */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static void downld_init(void)
{
    /* 変数初期化 */
    downld_state_no = STATE_IDLE;
    downld_tim      = 0;

    /* ログレベル設定 */
    dbg_print_set(DWL_ID, LOG_LEVEL, LOG_PUTDISC);
    dbg_print_set(WRT_ID, LOG_LEVEL, LOG_PUTDISC);
}

/******************************************************************************/
/* 関数名     状態管理処理                                                    */
/* 機能概要   downldスレッドの状態管理を行う                                  */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   なし                                                            */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static void downld_SttMng(INNER_MSG *msg_p)
{
	STATE_TABLE_t	*p_tbl   = NULL;
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
/* 関数名     IDLE状態でのTFTPサーバ要求処理                                  */
/* 機能概要   TFTPサーバ起動、タイマ要求、FPGA-LED制御を行う                  */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_IdleStt_SvrReq(INNER_MSG *msg_p)
{
    int     retv = 0;

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
/* 関数名     IDLE状態でのダウンロード要求処理                                */
/* 機能概要   TFTPクライアントによるダウンロード処理を行う                    */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_IdleStt_ClReq(INNER_MSG *msg_p)
{
    return OK;
}

/******************************************************************************/
/* 関数名     Server動作中状態でのダウンローダ要求処理                        */
/* 機能概要   ダウンローダ結果応答[エラー]を返送する                          */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_SvrStt_SvrReq(INNER_MSG *msg_p)
{
    dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
    return OK;
}

/******************************************************************************/
/* 関数名     Server動作中状態でのタイムアウト処理                            */
/* 機能概要   ダウンロードファイルのダウンロード終了を監視する                */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_SvrStt_Tmo(INNER_MSG *msg_p)
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
/* 関数名     Server動作中状態でのプログラムダウンロード要求処理              */
/* 機能概要   プログラムダウンロード完了通知[エラー]を返送する                */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_SvrStt_ClReq(INNER_MSG *msg_p)
{
	dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名     Server動作中状態でのファイル書込み応答処理		              */
/* 機能概要   ダウンロード結果をダウンローダ結果応答[結果]で返送する          */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_SvrStt_WriteResp(INNER_MSG *msg_p)
{
    dl_sndmsg(LUMNG_ECB, I_PGDLCMP, msg_p->msg_header.no);
    downld_state_no = STATE_IDLE;
	return OK;
}

/******************************************************************************/
/* 関数名     Client動作中状態でのダウンローダ要求処理			              */
/* 機能概要   ダウンローダ結果応答[エラー]を返送する				          */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_ClStt_SvrReq(INNER_MSG *msg_p)
{
    dl_sndmsg(LUMNG_ECB, I_SERVER_RESP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名     Client動作中状態でのダウンロード要求処理			              */
/* 機能概要   プログラムダウンロード完了通知[エラー]を返送する		          */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_ClStt_ClReq(INNER_MSG *msg_p)
{
    dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_STATE);
	return OK;
}

/******************************************************************************/
/* 関数名     Client動作中状態でのファイル書込み応答処理		              */
/* 機能概要   ダウンロード結果をダウンローダ結果応答[結果]で返送する          */
/* パラメータ msg_p : (in)    内部受信メッセージ                              */
/* リターン   OK : 正常終了                                                   */
/*            NG : エラー                                                     */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static BYTE dl_ClStt_WriteResp(INNER_MSG *msg_p)
{
    dl_sndmsg(LUMNG_ECB, I_SERVER_RESP, msg_p->msg_header.no);
    downld_state_no = STATE_IDLE;
	return OK;
}


/******************************************************************************/
/* 関数名     ダウンロードファイルサイズ変化確認                              */
/* 機能概要   ダウンロードファイルのファイルの有無とサイズ変化を調べる        */
/* パラメータ なし								                              */
/* リターン   TRUE  : 変化なし                                                */
/*            FALSE : 変化あり                                                */
/* 注意事項   －                                                              */
/* その他     －                                                              */
/******************************************************************************/
static bool dl_tmpfile_chk(void)
{
    bool            same       	= true;
    DL_FILE_t       *fl_list_p 	= dl_files;
    struct stat     fl_stat 		= {};

    do
    {
        if (stat(fl_list_p->fl_name, &fl_stat) == 0)
        {
            if ((fl_list_p->exist != true )||
            	(fl_list_p->size != fl_stat.st_size))
            {
                same = false;
            }
            fl_list_p->exist = true;
            fl_list_p->size  = fl_stat.st_size;
        }
        else
        {
            if ((fl_list_p->exist != false)||
            	(fl_list_p->size != 0))
            {
                same = false;
            }
            fl_list_p->exist = false;
            fl_list_p->size  = 0;
        }
        fl_list_p++;
    } while( fl_list_p->fl_name != NULL );

	return same;
}


/******************************************************************************/
#include <sys/types.h>              /* opendir(), stat() */
#include <dirent.h>                 /* opendir() */
#include <sys/stat.h>               /* stat() */
BYTE FolderFilesGet(void)
{
    BYTE            retcd = OK;
    DIR             *dir_p = NULL;
    struct dirent   *dirent_p = NULL;
    struct stat     fl_stat;

    dir_p = opendir(TMP_FOLDER);
    if (dir_p == NULL)
    {
        dbg_print(DWL_ID, LOG_ERR, "not exist /tmp folder");
        dl_sndmsg(LUMNG_ECB, I_PGDLCMP, TFTP_RES_MISC);
        downld_state_no = STATE_IDLE;
        return NG;
    }

    do{
        dirent_p = readdir(dir_p);
        if( dirent_p != NULL )
        {
            if( stat(dirent_p->d_name, &fl_stat) != 0 )
            {
                retcd = NG;
                break;
            }
            ;
            ;
        }
    }while( dirent_p != NULL );

    closedir( dir_p );
    return retcd;
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
