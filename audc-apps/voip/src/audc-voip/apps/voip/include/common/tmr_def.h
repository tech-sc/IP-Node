/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : TMR                                                      #*/
/*#                                                                         #*/
/*#     日本語名 : タイマ制御ヘッダファイル                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/20 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/05 IPCS_V4                                         #*/
/*#                                                                         #*/
/*###########################################################################*/

//extern	void	time_led(char);					/* IPCS_V4 DEL */
extern	DWORD	pool_get_err[];								/*# プール取得ＮＧログ #*/

#define		TMR_ENTSTS			0x01
#define		TMR_ACTSTS			0x02
#define		TMR_OUTSTS			0x04

#define		TMR_TMCBMAX_10MS	0x50				/* IPCS_V4 CHG */
#define		TMR_TMCBMAX_50MS	0xA0
#define		TMR_TMCBMAX_1S		0xFE

#define		TMR_5MS_CNT			0x01
#define		TMR_10MS_CNT		TMR_5MS_CNT			/* IPCS_V4 CHG */
#define		TMR_50MS_CNT		0x02
#define		TMR_1S_CNT			0x03

#define		TMR_EXENML			0x01
#define		TMR_EXECYC			0x02

/* make */
//typedef struct  TCS_MSGHD{
//  WORD seq;
//  WORD tim;
//  BYTE dpid;
//  BYTE spid;
//  WORD msgid;
//  BYTE prio;
//  BYTE rsv;
//  }LU_CS_MSGHD;
//
//typedef struct  TCS_APTMSG{
//  LU_CS_MSGHD cs_msghd;
//  WORD        timid;
//  }CS_APTMSG; 




