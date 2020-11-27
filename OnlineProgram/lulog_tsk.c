/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  LuLogタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>		/* mkfifo(),open() */
#include <sys/stat.h>		/* mkfifo(),open() */
#include <fcntl.h>			/* open() */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>			/* stat() */
#include <stdint.h>
#include <sys/time.h>		/* gettimeofday() */

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#ifdef TEMP_HEADER
#include "temp_header.h"
#else
#include "str.h"
#include "prot.h"
#endif

#include "com_mem.h"
#include "lulog_tsk.h"

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
	#define LOG_LEVEL		LOG_INFO			/* 運用時ログレベル */
	#define LOG_PUTDISC		LOGDST_SYSLOG		/* 運用時ログ出力先 */
#endif /* end DEBUG */

#define FOREVER					0				/* 時間指定なし */

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/
/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/
/*** ファイル内で共有するstatic 変数宣言 ***/

/* 内部データ管理領域 */
_ATTR_SYM	lulog_t			lulog;

/* ログ領域 */
#define LOG_AREA_NUM		128
_ATTR_SYM	APLLOG_MSG		lulog_LogArea[LOG_AREA_NUM];

/* 内部処理バッファ */
static	char				tmp_buff[256];

/* errno文字列生成バッファ */
static	char				errno_str[16];
/* taskid文字列生成バッファ */
static	char				taskid_str[16];

/*** static 関数宣言 ***/

/******************************************************************************/
/* 関数名	  errno文字列変換												  */
/* 機能概要	  引数のerrnoを文字化する										  */
/* パラメータ err : (in)	変換するerrno									  */
/* リターン	  変換した文字列												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define CASE_STR(a)	case a:	 return #a
static const char *mkstr_errno(int err)
{
	switch(err)
	{
	  CASE_STR(EACCES);
	  CASE_STR(EDQUOT);
	  CASE_STR(EEXIST);
	  CASE_STR(ENAMETOOLONG);
	  CASE_STR(ENOENT);
	  CASE_STR(ENOSPC);
	  CASE_STR(ENOTDIR);
	  CASE_STR(EROFS);
	  CASE_STR(EFAULT);
	  CASE_STR(EINTR);
	  CASE_STR(EINVAL);
	  CASE_STR(ENFILE);
	  CASE_STR(ENOMEM);
	  default:
		sprintf(errno_str, "misc(%d)",err);
		return errno_str;
	}
}

/******************************************************************************/
/* 関数名	  taskid文字列変換												  */
/* 機能概要	  引数のtaskidを文字化する										  */
/* パラメータ taskidr : (in)	変換するtaskidg								  */
/* リターン	  変換した文字列												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
//#define CASE_STR(a)	case a:	 return #a
static const char *mkstr_taskid(WORD taskid)
{
	switch(taskid)
	{
	  CASE_STR(MON_ID);
	  CASE_STR(TMR_ID);
	  CASE_STR(CSCTL_ID);
	  CASE_STR(MDA_ID);
	  CASE_STR(LUMNG_ID);
	  CASE_STR(DOEPR_ID);
	  CASE_STR(DOEPS_ID);
	  CASE_STR(DWL_ID);
	  CASE_STR(MNT_ID);
	  CASE_STR(LULOG_ID);
	  CASE_STR(SUP_ID);
	  default:
		sprintf(taskid_str, "misc(%d)",taskid);
		return taskid_str;
	}
}


/******************************************************************************/
/* 関数名	  Lu_Logスレッド												  */
/* 機能概要	  アプリログのロギング処理とアプリログのファイル出力を行う		  */
/* パラメータ arg : (in)	  未使用										  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void lulog_thread(void *arg)
{
	INNER_MSG	*msg_p = NULL;

	lulog_init();
	while (1)
	{
		msg_p = (INNER_MSG*)com_rcvmsg(LULOG_ECB, FOREVER);

		switch(msg_p->msg_header.kind)
		{
		  case I_APLLOG:
			dbg_print(LULOG_ID, LOG_INFO, "rcvmsg I_APLLOG:%s", mkstr_taskid(msg_p->msg_header.id));
			lulog_Loggin(msg_p);
			break;
		  case I_LOGWRITE:
			dbg_print(LULOG_ID, LOG_INFO, "rcvmsg I_LOGWRITE:%s", mkstr_taskid(msg_p->msg_header.id));
			lulog_LogWrite();
			break;
		  default:
			dbg_print(LULOG_ID, LOG_WARNING, "rcvmsg unknown kind:%s", mkstr_taskid(msg_p->msg_header.id));
			break;
		}

		if (msg_p->msg_header.link != NULL)
		{
			com_poolput(POOL1, msg_p->msg_header.link);
		}
		com_poolput(POOL0, (BYTE*)msg_p);
	}
}

/******************************************************************************/
/* 関数名	  Lu_Log機能の初期設定											  */
/* 機能概要	  内部管理変数を初期設定する									  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM void lulog_init(void)
{
	lulog.LogCount = 0;
	lulog.LogWriteIndex = 0;
	lulog.LogReadIndex = 0;

	/* ログレベル設定 */
	dbg_print_set(LULOG_ID, LOG_LEVEL, LOG_PUTDISC);
}

/******************************************************************************/
/* 関数名	  アプリログのロギング処理										  */
/* 機能概要	  受信したアプリログMSGをログデータ領域に保存する				  */
/* パラメータ msg_p : (in)	  受信メッセージ								  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE lulog_Loggin(INNER_MSG *msg_p)
{
	BYTE		result = NG;
	APLLOG_MSG	*sub_p = (APLLOG_MSG*)msg_p->msg_header.link;

	if (sub_p != NULL)
	{
		if (lulog.LogWriteIndex >= LOG_AREA_NUM)
		{
			lulog.LogWriteIndex = 0;
		}
		memcpy( &lulog_LogArea[lulog.LogWriteIndex], sub_p, sizeof(APLLOG_MSG) );
		lulog.LogCount ++;
		lulog.LogWriteIndex ++;
		result = OK;
	}
	return result;
}

/******************************************************************************/
/* 関数名	  アプリログのファイル化処理									  */
/* 機能概要	  アプリログが保存されているログデータ領域をファイル出力する	  */
/* パラメータ なし															  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE lulog_LogWrite(void)
{
	BYTE		result = NG;
	int			loop = 0;
	FILE		*fp = NULL;
	struct timeval	timeval = {};
	struct stat		fl_stat = {};

	gettimeofday(&timeval, NULL);
	while(loop < 5)
	{
		sprintf(tmp_buff, "/tmp/alog%08ld.%06ldsec.log", timeval.tv_sec, timeval.tv_usec);
		if (stat(tmp_buff, &fl_stat) != 0)
		{
			fp = fopen(tmp_buff, "wt");
			if (fp == NULL)
			{
				dbg_print(LULOG_ID, LOG_ERR, "/tmp/alogXXXX.log fopen() Error:%s", mkstr_errno(errno));
			}
			else
			{
				result = lulog_LogWiteFile(fp);
				fclose(fp);
			}
			break;
		}
		timeval.tv_usec++;
		loop++;
	}
	return result;
}

/******************************************************************************/
/* 関数名	  ログデータをテキストファイル形式で書き込む					  */
/* 機能概要	  ログデータの各フィールドを文字化してライトする				  */
/* パラメータ fp : (in)	  書き込むファイルポインタ							  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE lulog_LogWiteFile(FILE *fp)
{
	BYTE		result = OK;
	uint16_t	idx = lulog.LogReadIndex;
	int			len = 0;

	if (idx >= LOG_AREA_NUM)
	{
		idx = 0;
		lulog.LogReadIndex = 0;
	}
	if (lulog.LogWriteIndex >= LOG_AREA_NUM)
	{
		lulog.LogWriteIndex = 0;
	}

	while (idx != lulog.LogWriteIndex)
	{
		fprintf(fp, "%08ld\t", lulog_LogArea[idx].time);
		fprintf(fp, "%s\t", mkstr_taskid(lulog_LogArea[idx].task_id));
		fprintf(fp, "%u\t", lulog_LogArea[idx].line);
		for(len=0; len < APLLOGDATA_LEN; len++)
		{
			fprintf(fp, "%02X ", lulog_LogArea[idx].data[len]);
		}
		if (fputc('\n', fp) == EOF)
		{
			dbg_print(LULOG_ID, LOG_ERR, "/tmp/alogXXXX.log fputc() Error:%s", mkstr_errno(errno));
			result = NG;
			break;
		}

		idx++;
		if (idx >= LOG_AREA_NUM)
		{
			idx = 0;
		}
		lulog.LogReadIndex = idx;
	}
	return result;
}

/******************************************************************************/
/* 関数名	  アプリログ保存API												  */
/* 機能概要	  Lu_Logタスクにアプリログ(I_APLLOG)MSGを送信する				  */
/* パラメータ task_id : (in)	タスク識別子								  */
/*			  line : (in)		ソースの行番号								  */
/*			  len : (in)		ログデータバイト数(最大248byte)				  */
/*			  data_p : (in)		ログデータ領域(最大248byte)					  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void lulog_AplLog(WORD task_id, WORD line, WORD len, BYTE *data_p)
{
	INNER_MSG	*msg_p = (INNER_MSG*)com_poolget(POOL0);
	APLLOG_MSG	*sub_p = (APLLOG_MSG*)com_poolget(POOL1);
	struct timeval	timeval = {};

	if ((msg_p != NULL)&&(sub_p != NULL)&&(len <= APLLOGDATA_LEN)&&(data_p != NULL))
	{
		gettimeofday(&timeval, NULL);

		sub_p->time = timeval.tv_sec * 1000;
		sub_p->time += (DWORD)(timeval.tv_usec / 1000);
		sub_p->task_id = task_id;
		sub_p->line = line;
		memcpy(sub_p->data, data_p, len);

		msg_p->msg_header.id	= task_id;
		msg_p->msg_header.div	= IN_MSG;
		msg_p->msg_header.kind	= I_APLLOG;
		msg_p->msg_header.no	= 0;
		msg_p->msg_header.link	= (BYTE*)sub_p;
		com_sndmsg(LULOG_ECB, (BYTE*)msg_p);
	}
	else
	{
		if (msg_p != NULL)
		{
			com_poolput(POOL0, (BYTE*)msg_p);
		}
		if (sub_p != NULL)
		{
			com_poolput(POOL1, (BYTE*)sub_p);
		}
	}
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  LuLogタスク機能のソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
