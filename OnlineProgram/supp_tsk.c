/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  サポートタスク機能のソースファイル							  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>		/* mkfifo(),open() */
#include <sys/stat.h>		/* mkfifo(),open() */
#include <fcntl.h>			/* open() */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>			/* read() */
#include <stdint.h>

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#define TEMP_HEADER
#ifdef TEMP_HEADER
#include "temp_header.h"
#else
#include "str.h"
#include "prot.h"
#endif

#include "com_mem.h"
#include "lulog_tsk.h"
#include "supp_tsk.h"

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

/* sshサービスファイル */
#define DEBUG_CFG			"/flsdata/debug.cfg"
#define SSH_ENABLE			"ssh:enable"
#define SSH_ENABLE_LEN		10

/* プロセル間通信FIFO */
#define SUPP_FIFO			"/tmp/support"
#define FIFO_REQ_STR		"logfile"
#define FIFO_REQ_LEN		7


/*** 自ファイル内でのみ使用する#define 関数マクロ ***/
/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/
/*** ファイル内で共有するstatic 変数宣言 ***/
static	char				tmp_buff[16];
/* errno文字列生成バッファ */
static	char				str_buff[16];

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
		sprintf(str_buff, "misc(%d)",err);
		return str_buff;
	}
}


/******************************************************************************/
/* 関数名	  サポートスレッド												  */
/* 機能概要	  sshサービス起動とアプリログファイル化要求を行う				  */
/* パラメータ arg : (in)	  未使用										  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
void support_thread(void *arg)
{
	int		retv = 0;
	int		fd = 0;

	/* ログレベル設定 */
	dbg_print_set(SUP_ID, LOG_LEVEL, LOG_PUTDISC);

	supp_sshChk();
	while (1)
	{
		if (mkfifo(SUPP_FIFO, S_IRWXU) == -1)
		{
			dbg_print(SUP_ID, LOG_WARNING, "mkfifo() Error:%s", mkstr_errno(errno));
			break;
		}
		while (1)
		{
			fd = open(SUPP_FIFO, O_RDONLY | O_SYNC);
			if (fd == -1)
			{
				dbg_print(SUP_ID, LOG_WARNING, "fifo open() Error:%s", mkstr_errno(errno));
				break;
			}

			while (1)
			{
				retv = read(fd, tmp_buff, 8);
				if (retv == -1)
				{
					dbg_print(SUP_ID, LOG_WARNING, "fifo read() Error:%s", mkstr_errno(errno));
					close(fd);
					break;
				}
				else if (retv != 0)
				{
					supp_AplLogReq(tmp_buff);
				}
			}
		}
	}
}

/******************************************************************************/
/* 関数名	  sshサービス起動判断処理										  */
/* 機能概要	  debug.cfgファイルを読み出して、sshサービス起動判断する		  */
/* パラメータ なし															  */
/* リターン	  なし															  */
/* 注意事項	  －															  */
/* その他	  設定ファイルについて											  */
/*			  ＜ssh設定ファイル＞											  */
/*				/etc/ssh/sshd_config										  */
/*			  ＜記述内容＞													  */
/*				Port 22														  */
/*				PermitRootLogin yes											  */
/*				PasswordAuthentication yes									  */
/******************************************************************************/
_ATTR_SYM void supp_sshChk(void)
{
	FILE	*fp = NULL;
	int		retv = 0;

	fp = fopen(DEBUG_CFG, "rt");
	if (fp == NULL)
	{
		dbg_print(SUP_ID, LOG_WARNING, "debug.cfg fopen() Error:%s", mkstr_errno(errno));
		return;
	}
	retv = fread(tmp_buff, 1, SSH_ENABLE_LEN, fp);
	if (retv < SSH_ENABLE_LEN)
	{
		if (ferror(fp) != 0)
		{
			dbg_print(SUP_ID, LOG_WARNING, "debug.cfg fread() Error:%s", mkstr_errno(errno));
		}
	}
	else
	{
		if (strncmp(tmp_buff, SSH_ENABLE, SSH_ENABLE_LEN) == 0)
		{
			retv = execlp("systemctl", "start", "sshd.service", NULL);
			if (retv == -1 )
			{
				dbg_print(SUP_ID, LOG_ERR, "ssh service Start Error:%s", mkstr_errno(errno));
			}
		}
	}
	fclose(fp);
}

/******************************************************************************/
/* 関数名	  アプリログファイル化要求処理									  */
/* 機能概要	  渡された文字列を判断して、アプリログファイル化要求MSGを送信する */
/* パラメータ str_p : (in)	  アプリログファイル化要求文字列				  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
_ATTR_SYM BYTE supp_AplLogReq(char *str_p)
{
	INNER_MSG	*msg_p = NULL;

	if (strncmp(str_p, FIFO_REQ_STR, FIFO_REQ_LEN) != 0)
	{
		return NG;
	}

	msg_p = (INNER_MSG*)com_poolget(POOL0);
	msg_p->msg_header.id	= SUP_ID;
	msg_p->msg_header.div	= IN_MSG;
	msg_p->msg_header.kind	= I_LOGWRITE;
	msg_p->msg_header.no	= 0;
	msg_p->msg_header.link	= NULL;

	com_sndmsg(LULOG_ECB, (BYTE*)msg_p);
	return OK;
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  サポートタスク機能のソースファイル							  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
