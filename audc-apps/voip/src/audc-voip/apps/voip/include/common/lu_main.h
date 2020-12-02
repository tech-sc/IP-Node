/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       LUメインスレッドヘッダ                                          */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

#ifndef _LU_MAIN_H
#define _LU_MAIN_H

/*** システムヘッダの取り込み ***/

/*** ユーザ作成ヘッダの取り込み ***/

/*** 自ファイル内でのみ使用する#define マクロ ***/

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/
#define IPCS_NW_IF_NAME		"eth0"


/*** 自ファイル内でのみ使用するtypedef 定義 ***/

/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/* MAIN エラーコード */
typedef enum {
	MAIN_ABORT_01= 0,	/* com_poolget メモリブロックID異常時 */
	MAIN_ABORT_02,		/* lu_data_init inet_ntop(IPアドレス)異常時 */
	MAIN_ABORT_03,		/* strncat inet_ntop異常時 */
	MAIN_ABORT_04,		/* lu_data_init inet_ntop(ゲートウェイアドレス)異常時 */
	MAIN_ABORT_05,		/* main msgget異常時 */
	MAIN_ABORT_06		/* main msgrcv異常時 */
} MAIN_ERROR_CODE;

/* IPCSメッセージ・キー */
typedef enum {
	MSG_KEY_LUPROC= 1000	/* lu_proc */
} IPCS_MSG_KEY;

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/

/*** ファイル内で共有するstatic 変数宣言 ***/

/*** static 関数宣言 ***/

/*** 変数定義 ***/

/*** プロトタイプ定義 ***/
int main(int argc, char **argv);
void lu_sigaction(void);
void lu_data_init(void);
void lu_flsdata_init(void);
void lu_thread_initialstart(void);

#endif	/* _LU_MAIN_H */

