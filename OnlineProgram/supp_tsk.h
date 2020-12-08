/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  サポートタスク機能のヘッダファイル							  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _SUPP_TSK_H_
#define _SUPP_TSK_H_

#ifdef DEBUG
#define _ATTR_SYM
#else  // !DEBUG
#define _ATTR_SYM		static
#endif // DEBUG

extern void support_thread(void *arg);
_ATTR_SYM void supp_sshChk(void);
_ATTR_SYM BYTE supp_AplLogReq(char *str_p);

#endif /* _SUPP_TSK_H_ */

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  サポートタスク機能のヘッダファイル							  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
