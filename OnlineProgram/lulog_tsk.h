/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  LuLogタスク機能のヘッダファイル								  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _LULOG_TSK_H_
#define _LULOG_TSK_H_

#ifdef DEBUG
#define _ATTR_SYM
#include <stdio.h>
#else  // !DEBUG
#define _ATTR_SYM		static
#endif // DEBUG

/* アプリログ要求メッセージ型 */
#define APLLOGDATA_LEN		248
typedef struct APLLOG_MSG {
	DWORD	time;
	WORD	task_id;
	WORD	line;
	BYTE	data[APLLOGDATA_LEN];
}APLLOG_MSG;

/* 内部データ型 */
typedef struct lulog_t {
	uint16_t	LogCount;
	uint16_t	LogWriteIndex;
	uint16_t	LogReadIndex;
} lulog_t;

#ifdef DEBUG
/* デバッグ用に外部参照可能にする */
extern	lulog_t				lulog;
#define LOG_AREA_NUM		128
extern	APLLOG_MSG			lulog_LogArea[LOG_AREA_NUM];
#endif // DEBUG


extern void lulog_thread(void *arg);
_ATTR_SYM void lulog_init(void);
_ATTR_SYM BYTE lulog_Loggin(INNER_MSG *msg_p);
_ATTR_SYM BYTE lulog_LogWrite(void);
_ATTR_SYM BYTE lulog_LogWiteFile(FILE *fp);
void lulog_LogWiteFile(WORD task_id, WORD line, WORD len, BYTE *data_p);
#define com_LogWiteFile		lulog_LogWiteFile
#endif /* _LULOG_TSK_H_ */

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  LuLogタスク機能のヘッダファイル								  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
