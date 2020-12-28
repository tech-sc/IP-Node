/* This is free software, licensed under the GNU General Public License v2. */

/* See /LICENSE for more information. */

/******************************************************************************/

/* 概要      SIO割込み機能のヘッダファイル                                  */

/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */

/******************************************************************************/

#ifndef _SIO_INT_H_

#define _SIO_INT_H_

typedef	unsigned char	BYTE;

/*------------------------------------------------------------------------------
  定数
------------------------------------------------------------------------------*/
#define CD_SPFRAME_SIZE     18          /* Ｓプロフレームのデータの最大サイズ */


/*------------------------------------------------------------------------------
  構造体
------------------------------------------------------------------------------*/
/* SIO受信状態管理 */
typedef struct{
    BYTE        rstn;                                         /* 受信状態番号 */
    BYTE        rlrc;                                       /* 受信ＬＲＣ情報 */
    BYTE        rsize;                                    /* 受信キャラクタ数 */
    BYTE        rbuf[CD_SPFRAME_SIZE];              /* 受信キャラクタバッファ */
} CD_RSIO;

/* SIO送信状態管理 */
typedef struct {
    BYTE        sstn;                                         /* 送信状態番号 */
    BYTE        slrc;                                       /* 送信ＬＲＣ情報 */
    BYTE        sndpoint;                                     /* 送信位置情報 */
    BYTE        ssize;                                    /* 送信キャラクタ数 */
    BYTE        sbuf[CD_SPFRAME_SIZE];              /* 送信キャラクタバッファ */
} CD_SSIO;

/* SIO受信リングバッファ */
typedef struct{
    BYTE        rcv_read;                                 	/* 読込位置 */
    BYTE        rcv_write;                                 	/* 書込位置 */
    BYTE        rcv_buf[256];              					/* 受信リングバッファ */
} CD_RSIO_BUFF;

/* SIO統計情報 */
typedef struct{
    BYTE        t_o      ;                                    /* タイムアウト */
    BYTE        dta_umh  ;                                /* データアンマッチ */
    BYTE        flm_eff  ;                              /* フレーミングエラー */
    BYTE        ov_run   ;                              /* オーバーランエラー */
    BYTE        pari_err ;                                  /* バリティエラー */
} CD_RSIO_ERR;



/*------------------------------------------------------------------------------
  構造体（extern）
------------------------------------------------------------------------------*/
extern	wait_queue_head_t	rcv_cmp_wait_q;							/*# 受信完了待ちキュー #*/
extern	wait_queue_head_t	snd_cmp_wait_q;							/*# 送信完了待ちキュー #*/

extern	CD_RSIO				g_rsio;									/* SIO受信状態管理 */
extern	CD_SSIO				g_ssio;									/* SIO送信状態管理 */

extern	CD_RSIO_BUFF		r_sio_buf;								/* 受信リングバッファ */

extern	CD_RSIO_ERR			g_rcv_err;								/* SIO統計情報 */


/*------------------------------------------------------------------------------
  関数
------------------------------------------------------------------------------*/


void sio_rcv_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* rdr_p );
void sio_rcv_mtx( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_n( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_dle1( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_h( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_a( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_c( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_data( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_esc( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_dle2( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_etx( CD_RSIO* prsio, BYTE rcv_dt );
void sio_rcv_mtx_lrc( CD_RSIO* prsio, BYTE rcv_dt );
void sio_snd_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* tdr_p );
void sio_snd_mtx( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_n( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_dle1( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_h( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_a( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_c( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_data( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_esc( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_dle2( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_etx( CD_SSIO* pssio, BYTE* snd_dt );
void sio_snd_mtx_lrc( CD_SSIO* pssio, BYTE* snd_dt );
void sio_err_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p );
void sio_drv_init(void);
void sio_snd_sta( BYTE* pdata, BYTE size );
BYTE sio_snd_chk(void);


#define INT_REG		(volatile BYTE*)(0x1C000014)	/*# INTRレジスタアドレス #*/

#define SCR_REG		(volatile BYTE*)(0x1C000021)	/*# SCRレジスタアドレス #*/
#define TDR_REG		(volatile BYTE*)(0x1C000022)	/*# TDRレジスタアドレス #*/
#define SSR_REG		(volatile BYTE*)(0x1C000023)	/*# SSRレジスタアドレス #*/
#define RDR_REG		(volatile BYTE*)(0x1C000024)	/*# RSRレジスタアドレス #*/




#endif /* _SIO_INT_H_ */



/* This is free software, licensed under the GNU General Public License v2. */

/* See /LICENSE for more information. */

/******************************************************************************/

/* 概要       SIO割込み機能機能のヘッダファイル                                  */

/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */

/******************************************************************************/
