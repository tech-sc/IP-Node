/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       スタブのヘッダファイル 		                                 */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _STUB_H_
#define _STUB_H_

#include "str_comdt.h"

extern COM_DATA		com_data;
extern uint8_t			pool_area[2][256];
extern uint8_t			msg_area[2][256];
/* パラメータ確認用変数 */
extern uint8_t			poolget_id;
extern uint8_t			poolput_id;
extern BYTE			*poolput_ptr;
extern uint8_t			sndmsg_ecb;
extern BYTE			*sndmsg_ptr;
extern uint8_t			rcvmsg_ecb;
extern WORD			rcvmsg_tim;
extern BYTE			threadstart_id;
extern void			*threadstart_ptr;

#endif /* _STUB_H_ */

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       スタブのヘッダファイル        		                          */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
