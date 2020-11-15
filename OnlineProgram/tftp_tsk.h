/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       TFTPタスク機能のヘッダファイル                                  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#ifndef _TFTP_TSK_H_
#define _TFTP_TSK_H_

/* TFTPタスクのスレッド */
extern void downld_thread( void *arg );
extern void writer_thread( void *arg );

#ifdef DEBUG
#ifndef TFTP_RES_OK
/* ダウンロード結果 */
#define TFTP_RES_OK				0				/* 正常終了 */
#define TFTP_RES_PARAM			1				/* パラメータ異常（TFTP未実施） */
#define TFTP_RES_FL_NOTFOUND	2				/* ファイルなし受信（TFTP未完了） */
#define TFTP_RES_SERVER			3				/* サーバ未応答（TFTP未完了） */
#define TFTP_RES_CRC			4				/* ファイルエラー：CRCエラー（TFTP未完了） */
#define TFTP_RES_STATE			5				/* 未実施（TFTP未完了）*/
#define TFTP_RES_MISC			6				/* その他エラー */
/* ダウンロード種別 */
#define PROG_DL					1
#define WAVE_DL					2
#endif //TFTP_RES_OK

/* 状態番号 */
typedef enum TFTP_STATE_e {
	STATE_IDLE = 0,					/* アイドル状態 */
	STATE_SERVER_DL,				/* ダウンロード監視状態 */
	STATE_CLIENT_DL,				/* ダウンロード中状態 */
	MAX_STATE_NO
}TFTP_STATE_e;
extern TFTP_STATE_e		downld_state_no;
/* タイマ登録ID(1～254) */
extern BYTE				downld_tim;
/* ダウンロード完了判断カウンタ */
extern uint16_t			downld_same_cnt;
/* TFTP起動前のLED状態 */
extern uint8_t			prev_LED;
/* errno文字列生成バッファ */
extern char		str_buff[16];
/* ダウンロード種別 */
extern uint16_t			downld_type;
/* 内部関数 */
extern void downld_init(void);
extern void dl_sndmsg(BYTE ecb, BYTE kind, BYTE result);
extern void downld_SttMng(INNER_MSG *msg_p);
extern BYTE dl_IdleStt_SvrReq( INNER_MSG *msg_p );
extern BYTE dl_IdleStt_Cl1Req( INNER_MSG *msg_p );
extern BYTE dl_IdleStt_Cl2Req( INNER_MSG *msg_p );
extern BYTE dl_SvrStt_SvrReq( INNER_MSG *msg_p );
extern BYTE dl_SvrStt_Tmo( INNER_MSG *msg_p );
extern BYTE dl_SvrStt_Cl1Req( INNER_MSG *msg_p );
extern BYTE dl_SvrStt_Cl2Req( INNER_MSG *msg_p );
extern BYTE dl_SvrStt_WriteResp( INNER_MSG *msg_p );
extern BYTE dl_ClStt_SvrReq( INNER_MSG *msg_p );
extern BYTE dl_ClStt_Cl1Req( INNER_MSG *msg_p );
extern BYTE dl_ClStt_Cl2Req( INNER_MSG *msg_p );
extern BYTE dl_ClStt_WriteResp( INNER_MSG *msg_p );
extern bool dl_tmpfile_chk(void);
extern BYTE str2hex(const char *str, BYTE *hex, size_t siz);
extern int writer_FileInfoFile(void);
extern int writer_OnlineProg(char *fileinfo);
extern int writer_BootProg(char *fileinfo);
extern int writer_FpgaProg(void);
extern int wt_ProgFileWrite(char *wt_dev, FILE *rd_fp);
extern int wt_FpgaFileWrite(FILE *rd_fp);
extern BYTE tftp(INNER_MSG *msg_p);
#endif // !DEBUG

#endif /* _TFTP_TSK_H_ */

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       TFTPタスク機能のヘッダファイル                                  */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
