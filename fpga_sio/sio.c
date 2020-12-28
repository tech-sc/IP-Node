/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
/* 概要       SIO割込み処理                                                   */
/*                                                                            */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/


/*------------------------------------------------------------------------------
  インクルードファイル
------------------------------------------------------------------------------*/

//#include "cd_def.h"                                       /* ＣＤＬＣ共通定義 */
//#include "cd_data.h"                                      /* グローバルデータ */
//#include "cd_func_extern.h"                   /* ＣＤＬＣ共通ｅｘｔｅｒｎ宣言 */

#include <linux/string.h>
#include <linux/wait.h>    									/* wait_queue_head_t */
#include <linux/sched.h>	 /* wait_event_interruptible, wake_up_interruptible */
#include <linux/interrupt.h>							/* IPCS_V4 PG 0007243  */

//#include "def.h"                                       			/* 共通定義 */
#include "sio_drv.h"                                       	/* FPGA SIO定義 */
#include "sio.h"                                       			/* SIO定義 */


/*------------------------------------------------------------------------------
  定数
------------------------------------------------------------------------------*/
/*############################################################################*/
/* ＳＩＯ処理関連 */
/*############################################################################*/

#define CD_BIT_SCR_TIE      7              /* ＳＣＲレジスタ ＴＩＥビット位置 */
#define CD_BIT_SCR_RIE      6              /* ＳＣＲレジスタ ＲＩＥビット位置 */
#define CD_BIT_SCR_TE       5                /* ＳＣＲレジスタ ＴＥビット位置 */
#define CD_BIT_SCR_RE       4                /* ＳＣＲレジスタ ＲＥビット位置 */
#define CD_BIT_SCR_TEIE     2            /* ＳＣＲレジスタ ＴＥＩＥビット位置 */

#define CD_BIT_SSR_TDRE     7            /* ＳＳＲレジスタ ＴＤＲＥビット位置 */
#define CD_BIT_SSR_RDRF     6            /* ＳＳＲレジスタ ＲＤＲＦビット位置 */
#define CD_BIT_SSR_ORER     5            /* ＳＳＲレジスタ ＯＲＥＲビット位置 */
#define CD_BIT_SSR_FER      4              /* ＳＳＲレジスタ ＦＥＲビット位置 */
#define CD_BIT_SSR_PER      3              /* ＳＳＲレジスタ ＰＥＲビット位置 */
#define CD_BIT_SSR_TEND     2            /* ＳＳＲレジスタ ＴＥＮＤビット位置 */

#define CD_SIO_ST_N         0                             /* ニュートラル状態 */
#define CD_SIO_ST_DLE1      1                                 /* ＤＬＥ１状態 */
#define CD_SIO_ST_H         2                                       /* Ｈ状態 */
#define CD_SIO_ST_A         3                                       /* Ａ状態 */
#define CD_SIO_ST_C         4                                       /* Ｃ状態 */
#define CD_SIO_ST_DATA      5                                 /* ＤＡＴＡ状態 */
#define CD_SIO_ST_ESC       6                                   /* ＥＳＣ状態 */
#define CD_SIO_ST_DLE2      7                                 /* ＤＬＥ２状態 */
#define CD_SIO_ST_ETX       8                                   /* ＥＴＸ状態 */
#define CD_SIO_ST_LRC       9                                   /* ＬＲＣ状態 */

/* 制御フィールド ブロック種別 */
#define CD_SP_C_LC          0xC0                        /* リンク制御ブロック */
#define CD_SP_C_I           0x00                            /* 情報ブロックＩ */
#define CD_SP_C_I0          0x00                      /* 情報ブロックＩ（０） */
#define CD_SP_C_I1          0x01                      /* 情報ブロックＩ（１） */
#define CD_SP_C_ACK0        0x40                  /* 監視ブロックＡＣＫ（０） */
#define CD_SP_C_ACK1        0x41                  /* 監視ブロックＡＣＫ（１） */
#define CD_SP_C_RNR0        0x50                  /* 監視ブロックＲＮＲ（０） */
#define CD_SP_C_RNR1        0x51                  /* 監視ブロックＲＮＲ（１） */
#define CD_SP_C_PL          0x60                          /* 監視ブロックＰＬ */

/* アドレスフィールド アドレス種別 */
#define CD_SP_A_AG          0x7F                        /* グローバルアドレス */
#define CD_SP_A_DAT         0x00                      /* データリンクアドレス */
#define CD_SP_A_MNT         0x07                /* メンテナンスリンクアドレス */

#define CD_DLE              0x10                              /* ＤＬＥコード */
#define CD_H                0x02                              /* ＳＴＸコード */
#define CD_ETX              0x03                              /* ＥＴＸコード */
#define CD_ESC              0x1B                              /* ＥＳＣコード */

#define CD_LOC_A      0                       /* アドレスフィールドの格納位置 */
#define CD_LOC_C      1                           /* 制御フィールドの格納位置 */
#define CD_LOC_DATA   2                     /* データフィールドの格納開始位置 */

/* キャラクタ桁間タイマ設定 */
#define CD_TIM03_STA        2                   /* キャラクタ桁間タイマの開始 */
#define CD_TIM03_END        0                   /* キャラクタ桁間タイマの停止 */


/*------------------------------------------------------------------------------
  構造体（実体）
------------------------------------------------------------------------------*/
wait_queue_head_t	rcv_cmp_wait_q;							/*# 受信完了待ちキュー #*/
wait_queue_head_t	snd_cmp_wait_q;							/*# 送信完了待ちキュー #*/

CD_RSIO				g_rsio;									/* SIO受信状態管理 */
CD_SSIO				g_ssio;									/* SIO送信状態管理 */

CD_RSIO_BUFF		r_sio_buf;								/* 受信リングバッファ */

CD_RSIO_ERR			g_rcv_err;								/* SIO統計情報 */

BYTE				g_tim03;								/* キャラクタ桁間タイマ */


/*------------------------------------------------------------------------------
  関数
------------------------------------------------------------------------------*/


//void sio_rcv_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* rdr_p );
//void sio_rcv_mtx( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_n( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_dle1( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_h( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_a( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_c( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_data( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_esc( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_dle2( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_etx( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_rcv_mtx_lrc( CD_RSIO* prsio, BYTE rcv_dt );
//void sio_snd_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* tdr_p );
//void sio_snd_mtx( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_n( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_dle1( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_h( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_a( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_c( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_data( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_esc( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_dle2( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_etx( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_snd_mtx_lrc( CD_SSIO* pssio, BYTE* snd_dt );
//void sio_err_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p );






/*# SOURCE_HEADER #*/
/*#######################################################################*/
/*#                                                                     #*/
/*# モジュール名 : BitSet                                               #*/
/*#                                                                     #*/
/*#     日本語名 : ビットセット処理                                     #*/
/*#     参照番号 : -                                                    #*/
/*#     単位区分 : 1                                                    #*/
/*#     入    力 : p      *BYTE  アドレス                               #*/
/*#                no      BYTE  ビット番号                             #*/
/*#     概    要 : ビットセットを行う。                                 #*/
/*#     作    成 : 00/03/06 - 澤村  岳志                                #*/
/*#     更    新 : 00/06/21 - 澤村  岳志                                #*/
/*#                                                                     #*/
/*#######################################################################*/
void BitSet( volatile BYTE *p, BYTE no)
  {
  *p |= (1 << no) ;
  }

/*# SOURCE_HEADER #*/
/*#######################################################################*/
/*#                                                                     #*/
/*# モジュール名 : BitClr                                               #*/
/*#                                                                     #*/
/*#     日本語名 : ビットクリア処理                                     #*/
/*#     参照番号 : -                                                    #*/
/*#     単位区分 : 1                                                    #*/
/*#     入    力 : p      *BYTE  アドレス                               #*/
/*#                no      BYTE  ビット番号                             #*/
/*#     概    要 : ビットクリアを行う。                                 #*/
/*#     作    成 : 00/03/06 - 澤村  岳志                                #*/
/*#     更    新 : 00/06/21 - 澤村  岳志                                #*/
/*#                                                                     #*/
/*#######################################################################*/
void BitClr(volatile BYTE *p, BYTE no)
  {
  *p &= (~(1 << no)) ;
  }

/*# SOURCE_HEADER #*/
/*#######################################################################*/
/*#                                                                     #*/
/*# モジュール名 : BitTst                                               #*/
/*#                                                                     #*/
/*#     日本語名 : ビットテスト処理                                     #*/
/*#     参照番号 : -                                                    #*/
/*#     単位区分 : 1                                                    #*/
/*#     入    力 : p      *BYTE  アドレス                               #*/
/*#                n       BYTE  ビット番号                             #*/
/*#     概    要 : ビットテストを行う。                                 #*/
/*#     作    成 : 00/03/06 - 澤村  岳志                                #*/
/*#     更    新 : 05/08/01 - 澤村  岳志                                #*/
/*#                                                                     #*/
/*#######################################################################*/
BYTE BitTst(volatile BYTE *p, BYTE no)
  {
  return((*p >> no) & 1) ;
  }

/******************************************************************************/
/* 関数名     sio_init                                                        */
/* 機能概要   内部変数の初期設定                                              */
/*            リソースや内部変数の初期設定を行う                              */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/* 注意事項                                                                   */
/* その他                                                                     */
/*                                                                            */
/******************************************************************************/
void	sio_drv_init()
{
	
	init_waitqueue_head( &rcv_cmp_wait_q );			/*# 受信完了待ちキューの初期化 #*/
	init_waitqueue_head( &snd_cmp_wait_q );			/*# 送信完了待ちキューの初期化 #*/
	
	memset( &g_rsio , 0, sizeof(CD_RSIO) );			/*# SIO受信状態管理領域初期化 #*/
	memset( &g_ssio , 0, sizeof(CD_SSIO) );			/*# SIO受信状態管理領域初期化 #*/
	
	memset( &g_rcv_err , 0 , sizeof(CD_RSIO_ERR) );	/*# SIO統計情報領域初期化 #*/
	
	memset( &r_sio_buf ,0 , sizeof(CD_RSIO_BUFF) );	/*# 受信リングバッファ初期化 #*/
}


/******************************************************************************/
/* 関数名     sio_rcv_inte                                                    */
/* 機能概要   FPGA_SIO割込みハンドラ                                          */
/*            FPGA_SIO割込み要因ごとの処理を行う                              */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/* 注意事項                                                                   */
/* その他                                                                     */
/*                                                                            */
/******************************************************************************/
irqreturn_t	sio_intr_handler(int irq, void *dev)						/* IPCS_V4 PG 0007243  */
{
volatile	BYTE	*pReg;
			BYTE	read_dt;
	
	pReg  = SSR_REG;			/*# ステータスレジスタアドレス設定 #*/
	read_dt = *pReg;			/*# ステータスレジスタ読み込み #*/
	
	if( read_dt & 0x38 )
	{/*# エラー割込み要因あり #*/
		sio_err_inte( SCR_REG , SSR_REG );				/*# エラー割込み処理 #*/
	}
	else if( read_dt & 0x40 )
	{/*# 受信割込み要因あり #*/
		sio_rcv_inte( SCR_REG , SSR_REG , RDR_REG );	/*# 受信割込み処理 #*/
	}
	else
	{        
		/* DO NOTHING */
	}
	if( read_dt & 0x84 )
	{/*# 送信割込み要因あり #*/
		sio_snd_inte( SCR_REG , SSR_REG , TDR_REG );	/*# 送信割込み処理 #*/
	}
	return IRQ_HANDLED;									/* IPCS_V4 PG 0007243  */
}


/*----------------------------------------------------------------------------*/
/**
 * ＳＩＯ受信割り込み処理
 * 
 * @param scr_p ＳＣＲレジスタ
 * @param ssr_p ＳＳＲレジスタ
 * @param rdr_p ＲＤＲレジスタ
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* rdr_p )
{
    CD_RSIO* prsio;                              /* ＳＩＯ受信状態管理 作業用 */
    BYTE	 cnt;
//    BYTE rdrf;                               /* ＳＳＲレジスタ ＲＤＲＦビット */
//    WORD ret;                                                         /* 戻値 */
//    BYTE rcv_dt;

                                                        /* 受信エラーチェック */
//    if( (*ssr_p & 0x38) != 0 )
//    {                                                       /* 受信エラーあり */
//                                                    /* ＳＩＯ障害割り込み処理 */
//        sio_err_inte( scr_p, ssr_p );
//        return;
//    }
//                                     /* 受信データ？（ＳＳＲ ＲＤＲＥビット） */
//    rdrf = BitTst( ssr_p, CD_BIT_SSR_RDRF );
//    if( rdrf == 0 )
//    {                                                   /* 受信データ格納なし */
//        return;
//    }
//                                                                /* 実装表は？ */
//    if( g_act_sby_inf[bs_no] == OFF )
//    {               /* 未実装なので破棄（ＲＤＲＦビットクリアは一応しておく） */
//                                               /* ＳＳＲ ＲＤＲＦビットクリア */
//        BitClr( ssr_p, CD_BIT_SSR_RDRF );
//        return;
//    }
                                                        /* ＳＩＯ受信状態管理 */
    prsio = &g_rsio;
                                      /* 受信状態番号の更新と受信データの格納 */
    sio_rcv_mtx( prsio, *rdr_p );
                                                          /* 受信状態番号は？ */
    if( prsio->rstn == CD_SIO_ST_LRC )
    {                                         /* ＬＲＣ：１フレーム分受信完了 */
		/*# １フレーム分をリングバッファに転送 #*/
		for( cnt=0 ; prsio->rsize < 0 ; cnt++  )
		{/*# 受信サイズ分行う（リングバッファなのでmemcpyは使わない） #*/
			r_sio_buf.rcv_buf[r_sio_buf.rcv_write] = prsio->rbuf[cnt];	/*# １バイト分をリングバッファに転送 #*/
			prsio->rsize--;												/*# 受信サイズ減算 #*/
			r_sio_buf.rcv_write++;										/*# リングバッファ書き込み位置更新 #*/
		}
		wake_up_interruptible( &rcv_cmp_wait_q );		/*# 受信待ちを起こす #*/


//        CD_MSG_SIO_RCV_IND *psio_rcv_ind;                       /* 獲得メモリ */
//
//                                      /* ＳＩＯ受信通知メッセージのメモリ獲得 */
//        ret = get_mpf( CD_MPID_RCV_SIO_MSG, 
//            CD_BLKSZ_RCV_SIO_MSG, (void**)&psio_rcv_ind );
//        if( ret != E_OK )
//        {
//                      /* 取得できない場合は異常処理でなく受信データを破棄する */
//            /*cdlc_err_proc( CD_ERRC_SVC_GET_MPF, CD_ERRL_SIO_RCV_INTE, 4 );*/
//        }
//        else
//        {
//                                            /* ＳＩＯ受信通知メッセージの作成 */
//                                                          /* メッセージヘッダ */
//            psio_rcv_ind->msghd.msgid = CD_MSGID_SIORCVIND;
//            psio_rcv_ind->msghd.spid  = CD_PID_SPRO_TSK;
//            psio_rcv_ind->msghd.prio  = PRI_NML;
//            psio_rcv_ind->msghd.seq   = 0;
//            psio_rcv_ind->msghd.tim   = 0;
//                                                                  /* ＢＳ番号 */
//            psio_rcv_ind->bs = bs_no;
//                                                            /* バイトカウント */
//            psio_rcv_ind->bc = prsio->rsize;
//                                                          /* ＳＩＯ受信データ */
//            MemCpy( psio_rcv_ind->data, prsio->rbuf, prsio->rsize );
//                                            /* ＳＩＯ受信通知メッセージの送信 */
//            ret = snd_mbx( CD_PID_SPRO_TSK, (MSGHD*)psio_rcv_ind );
//            if( ret != E_OK )
//            {                                                  /* snd_mbx失敗 */
//                                                                  /* 異常処理 */
//                cdlc_err_proc( CD_ERRC_SVC_SND_MBX, CD_ERRL_SIO_RCV_INTE, 5 );
//            }
//        }
                                        /* 次の処理で初期化するので状態を変更 */
        prsio->rstn = CD_SIO_ST_N;
    }
                                                          /* 受信状態番号は？ */
    if( prsio->rstn != CD_SIO_ST_N )
    {                                             /* ニュートラル以外：受信中 */
                                                /* キャラクタ桁間タイマの開始 */
        g_tim03 = CD_TIM03_STA;
    }
    else
    {                                               /* ニュートラル：受信終了 */
                                                /* キャラクタ桁間タイマの停止 */
        g_tim03 = CD_TIM03_END;
                                                /* ＳＩＯ受信状態管理の初期化 */
                                                              /* 受信状態番号 */
        prsio->rstn = CD_SIO_ST_N;
                                                            /* 受信ＬＲＣ情報 */
        prsio->rlrc = 0;
                                                          /* 受信キャラクタ数 */
        prsio->rsize = 0;
    }
    
                                               /* ＳＳＲ ＲＤＲＦビットクリア */
    BitClr( ssr_p, CD_BIT_SSR_RDRF );

}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                        /* 受信状態番号別処理 */
    switch( prsio->rstn )
    {
                                                          /* ニュートラル状態 */
    case CD_SIO_ST_N:
        sio_rcv_mtx_n( prsio, rcv_dt );
        break;
                                                              /* ＤＬＥ１状態 */
    case CD_SIO_ST_DLE1:
        sio_rcv_mtx_dle1( prsio, rcv_dt );
        break;
                                                                    /* Ｈ状態 */
    case CD_SIO_ST_H:
        sio_rcv_mtx_h( prsio, rcv_dt );
        break;
                                                                    /* Ａ状態 */
    case CD_SIO_ST_A:
        sio_rcv_mtx_a( prsio, rcv_dt );
        break;
                                                                    /* Ｃ状態 */
                                                              /* ＤＡＴＡ状態 */
                                                                /* ＥＳＣ状態 */
    case CD_SIO_ST_C:
    case CD_SIO_ST_DATA:
    case CD_SIO_ST_ESC:
        sio_rcv_mtx_c( prsio, rcv_dt );
        break;
                                                              /* ＤＬＥ２状態 */
    case CD_SIO_ST_DLE2:
        sio_rcv_mtx_dle2( prsio, rcv_dt );
        break;
                                                                /* ＥＴＸ状態 */
    case CD_SIO_ST_ETX:
        sio_rcv_mtx_etx( prsio, rcv_dt );
        break;
                                                                  /* 異常状態 */
    default:
                                                                  /* 異常処理 */
//            cdlc_err_proc( CD_ERRC_ETC, CD_ERRL_SIO_RCV_INTE, 7 );
        break;
    }

}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ニュートラル）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_n( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                      /* 受信データはＤＬＥ？ */
    if( rcv_dt == CD_DLE )
    {                                                           /* ＤＬＥ受信 */
                                                 /* 受信状態番号遷移 ＤＬＥ１ */
        prsio->rstn = CD_SIO_ST_DLE1;
                                                              /* ＬＲＣの計算 */
        prsio->rlrc ^= rcv_dt;
    }
    else
    {                                                   /* ＤＬＥ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ＤＬＥ１）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_dle1( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                          /* 受信データはＨ？ */
    if( rcv_dt == CD_H )
    {                                                               /* Ｈ受信 */
                                                       /* 受信状態番号遷移 Ｈ */
        prsio->rstn = CD_SIO_ST_H;
                                                              /* ＬＲＣの計算 */
        prsio->rlrc ^= rcv_dt;
    }
    else
    {                                                       /* Ｈ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：Ｈ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_h( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                          /* 受信データはＡ？ */
    if( rcv_dt == CD_SP_A_AG ||
        rcv_dt == CD_SP_A_DAT ||
        rcv_dt == CD_SP_A_MNT )
    {                                                               /* Ａ受信 */
                                                       /* 受信状態番号遷移 Ａ */
        prsio->rstn = CD_SIO_ST_A;
                                              /* 受信キャラクタバッファに格納 */
        prsio->rbuf[prsio->rsize] = rcv_dt;
                                                    /* 受信キャラクタ数の更新 */
        prsio->rsize++;
                                                              /* ＬＲＣの計算 */
        prsio->rlrc ^= rcv_dt;
    }
    else
    {                                                       /* Ａ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：Ａ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_a( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                          /* 受信データはＣ？ */
    if( rcv_dt == CD_SP_C_I0 || 
        rcv_dt == CD_SP_C_I1 || 
        rcv_dt == CD_SP_C_ACK0 || 
        rcv_dt == CD_SP_C_ACK1 || 
        rcv_dt == CD_SP_C_RNR0 || 
        rcv_dt == CD_SP_C_RNR1 || 
        rcv_dt == CD_SP_C_PL || 
        rcv_dt == CD_SP_C_LC )
    {                             /* Ｃ（Ｉ、ＡＣＫ、ＲＮＲ、ＰＬ、ＬＣ）受信 */
                                                       /* 受信状態番号遷移 Ｃ */
        prsio->rstn = CD_SIO_ST_C;
                                              /* 受信キャラクタバッファに格納 */
        prsio->rbuf[prsio->rsize] = rcv_dt;
                                                    /* 受信キャラクタ数の更新 */
        prsio->rsize++;
                                                              /* ＬＲＣの計算 */
        prsio->rlrc ^= rcv_dt;
    }
    else
    {                                                       /* Ｃ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：Ｃ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_c( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                            /* 受信データは？ */
    if( rcv_dt == CD_DLE )
    {                                                           /* ＤＬＥ受信 */
                                                 /* 受信状態番号遷移 ＤＬＥ２ */
        prsio->rstn = CD_SIO_ST_DLE2;
                                                              /* ＬＲＣの計算 */
        prsio->rlrc ^= rcv_dt;
    }
    else
    {                                                           /* ＤＬＥ以外 */
                                                      /* 受信キャラクタ数は？ */
        if( prsio->rsize < sizeof(prsio->rbuf) )
        {                                                 /* バッファ容量未満 */
                                                          /* Ｃフィールドは？ */
            if( prsio->rbuf[CD_LOC_C] == CD_SP_C_I0 ||
                prsio->rbuf[CD_LOC_C] == CD_SP_C_I1 )
            {                                                     /* Ｃ（Ｉ） */
                                                 /* 受信状態番号遷移 ＤＡＴＡ */
                prsio->rstn = CD_SIO_ST_DATA;
                                              /* 受信キャラクタバッファに格納 */
                prsio->rbuf[prsio->rsize] = rcv_dt;
                                                    /* 受信キャラクタ数の更新 */
                prsio->rsize++;
                                                              /* ＬＲＣの計算 */
                prsio->rlrc ^= rcv_dt;
            }
            else
            if( prsio->rbuf[CD_LOC_C] == CD_SP_C_LC )
            {                                                   /* Ｃ（ＬＣ） */
                                                            /* 受信データは？ */
                if( rcv_dt == CD_SP_A_DAT || rcv_dt == CD_SP_A_MNT )
                {         /* データリンクアドレスorメンテナンスリンクアドレス */
                                                 /* 受信状態番号遷移 ＤＡＴＡ */
                    prsio->rstn = CD_SIO_ST_DATA;
                                              /* 受信キャラクタバッファに格納 */
                    prsio->rbuf[prsio->rsize] = rcv_dt;
                                                    /* 受信キャラクタ数の更新 */
                    prsio->rsize++;
                                                              /* ＬＲＣの計算 */
                    prsio->rlrc ^= rcv_dt;
                }
                else
                {                                 /* 知らないアドレス「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
                    prsio->rstn = CD_SIO_ST_N;
                }
            }
            else
            {                                  /* Ｃ（Ｉ、ＬＣ） 以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
                prsio->rstn = CD_SIO_ST_N;
            }
        }
        else
        {                                     /* バッファ容量オーバー「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
            prsio->rstn = CD_SIO_ST_N;
        }
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ＤＡＴＡ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_data( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                        /* 状態Ｃと同等の処理 */
    sio_rcv_mtx_c( prsio, rcv_dt );
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ＥＳＣ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_esc( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                        /* 状態Ｃと同等の処理 */
    sio_rcv_mtx_c( prsio, rcv_dt );
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ＤＬＥ２）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_dle2( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                            /* 受信データは？ */
    if( rcv_dt == CD_ETX )
    {                                                           /* ＥＴＸ受信 */
                                                          /* Ｃフィールドは？ */
        if( prsio->rbuf[CD_LOC_C] == CD_SP_C_I0 || 
            prsio->rbuf[CD_LOC_C] == CD_SP_C_I1 || 
            prsio->rbuf[CD_LOC_C] == CD_SP_C_LC )
        {                                                   /* Ｃ（Ｉ、ＬＣ） */
                                                      /* 受信キャラクタ数は？ */
                        /* （Ｉ、ＬＣはＡとＣとＤＡＴＡの３バイト以上のはず） */
            if( prsio->rsize >= 3 )
            {                                                       /* ０以上 */
                                                   /* 受信状態番号遷移 ＥＸＴ */
                prsio->rstn = CD_SIO_ST_ETX;
                                                              /* ＬＲＣの計算 */
                prsio->rlrc ^= rcv_dt;
            }
            else
            {                                                   /* ０「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
                prsio->rstn = CD_SIO_ST_N;
            }
        }
        else
        {                                       /* Ｃ（ＡＣＫ，ＲＮＲ，ＰＬ） */
                                                      /* 受信キャラクタ数は？ */
                          /* （ＡＣＫ，ＲＮＲ，ＰＬはＡとＣの２バイトのはず） */
            if( prsio->rsize == 2 )
            {                                                           /* ２ */
                                                   /* 受信状態番号遷移 ＥＴＸ */
                prsio->rstn = CD_SIO_ST_ETX;
                                                              /* ＬＲＣの計算 */
                prsio->rlrc ^= rcv_dt;
            }
            else
            {                                               /* ２以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
                prsio->rstn = CD_SIO_ST_N;
            }
        }
    }
    else
    if( rcv_dt == CD_ESC )
    {                                                           /* ＥＳＣ受信 */
                                                      /* 受信キャラクタ数は？ */
        if( prsio->rsize < sizeof(prsio->rbuf) )
        {                                                 /* バッファ容量未満 */
                                                   /* 受信状態番号遷移 ＥＳＣ */
            prsio->rstn = CD_SIO_ST_ESC;
                                              /* 受信キャラクタバッファに格納 */
            prsio->rbuf[prsio->rsize] = CD_DLE;
                                                    /* 受信キャラクタ数の更新 */
            prsio->rsize++;
                                                              /* ＬＲＣの計算 */
            prsio->rlrc ^= rcv_dt;
        }
        else
        {                                     /* バッファ容量オーバー「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
            prsio->rstn = CD_SIO_ST_N;
        }
    }
    else
    {                                           /* ＥＸＴ、ＥＳＣ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}

/*----------------------------------------------------------------------------*/
/**
 * 受信状態番号の遷移処理（状態：ＥＴＸ）
 * 
 * @param prsio ＳＩＯ受信状態管理のポインタ (I/O)
 * @param rcv_dt 受信データ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_mtx_etx( CD_RSIO* prsio, BYTE rcv_dt )
{
                                                      /* 受信データはＬＲＣ？ */
    if( prsio->rlrc == rcv_dt )
    {                                                           /* ＬＲＣ受信 */
                                                   /* 受信状態番号遷移 ＬＲＣ */
        prsio->rstn = CD_SIO_ST_LRC;
    }
    else
    {                                                   /* ＬＲＣ以外「異常」 */
                                             /* 受信状態番号遷移 ニュートラル */
        prsio->rstn = CD_SIO_ST_N;
    }
}











/*----------------------------------------------------------------------------*/
/** 
 * ＳＩＯ送信開始（第１キャラクタ送信）
 * 
 * @param data  送信データが格納されているポインタ (I)
 * @param size  送信サイズ (I)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_sta( BYTE* pdata, BYTE size )
{
    CD_SSIO* pssio;                              /* ＳＩＯ送信状態管理 作業用 */
//    WORD ret;                                                         /* 戻値 */

	BitClr( SCR_REG , CD_BIT_SCR_TIE);		/*# TXI送信割込み禁止 #*/
	
                                                    /* ＳＩＯ送信割り込み禁止 */
//    if( bs_no == 0 )
//        ret = dis_int( INO_TXI0 );
//    else
//        ret = dis_int( INO_TXI1 );
//    if( ret != E_OK )
//    {
//                                                                  /* 異常処理 */
//        cdlc_err_proc( CD_ERRC_SVC_DIS_INT, CD_ERRL_SIO_SND_INTE, 1 );
//    }
                                                        /* ＳＩＯ送信状態管理 */
    pssio = &g_ssio;
                                                            /* 送信データ格納 */
                                                              /* 送信位置情報 */
    pssio->sndpoint = 0;
                                                          /* 送信キャラクタ数 */
    pssio->ssize = size;
                                                    /* 送信キャラクタバッファ */
    memcpy( pssio->sbuf, pdata, size );
                                                              /* 送信状態番号 */
    pssio->sstn = CD_SIO_ST_N;
                                                            /* 送信ＬＲＣ情報 */
    pssio->slrc = 0;
                                                              /* ＢＳ番号は？ */
//    if( bs_no == 0 )
//    {                                                              
                        /* ＳＩＯ送信割り込み関数経由でＳＩＯ送信処理を実施 */
        sio_snd_inte(SCR_REG , SSR_REG , TDR_REG);
//    }
//    else
//    {                                                               /* ＢＳ１ */
//                        /* ＳＩＯ１送信割り込み関数経由でＳＩＯ送信処理を実施 */
//        sio_snd_sci1();
//    }
//                                                    /* ＳＩＯ送信割り込み許可 */
//    if( bs_no == 0 )
//        ret = ena_int( INO_TXI0 );
//    else
//        ret = ena_int( INO_TXI1 );
//    if( ret != E_OK )
//    {
//                                                                  /* 異常処理 */
//        cdlc_err_proc( CD_ERRC_SVC_ENA_INT, CD_ERRL_SIO_SND_INTE, 2 );
//    }
	BitSet( SCR_REG , CD_BIT_SCR_TIE);		/*# TXI送信割込み許可 #*/
}

/*----------------------------------------------------------------------------*/
/** 
 * ＳＩＯ送信中チェック
 * 
 * @retval 0以外 送信処理中
 * @retval 0 送信処理中でない
 * 
 */
/*----------------------------------------------------------------------------*/
BYTE sio_snd_chk(  )
{
    /* バッファサイズを返信 */
    return( g_ssio.ssize );
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** 
 * ＳＩＯ送信割り込み関数（ポート０）
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_snd_sci0()
//{
//                                                      /* ループバック試験中？ */
//    if( g_cdlc_sts != CD_DCH_LOP_TEST_STS )
//    {                                               /* ループバック試験中以外 */
//                                                    /* ＳＩＯ送信割り込み処理 */
//        sio_snd_inte( 0, /* ＢＳ０ */
//            (REGST)CD_CUP_A_SCR_SCI0, 
//            (REGST)CD_CUP_A_SSR_SCI0, 
//            (REGST)CD_CUP_A_TDR_SCI0 );
//    }
//    else
//    {                                                   /* ループバック試験中 */
//                                  /* ループバック処理－ＳＩＯ送信割り込み処理 */
//        lpb_sio_snd_inte( 0, /* ＢＳ０ */
//            (REGST)CD_CUP_A_SCR_SCI0, 
//            (REGST)CD_CUP_A_SSR_SCI0, 
//            (REGST)CD_CUP_A_TDR_SCI0 );
//    }
//}

/*----------------------------------------------------------------------------*/
/** 
 * ＳＩＯ送信割り込み関数（ポート１）
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_snd_sci1()
//{
//                                                      /* ループバック試験中？ */
//    if( g_cdlc_sts != CD_DCH_LOP_TEST_STS )
//    {                                               /* ループバック試験中以外 */
//                                                    /* ＳＩＯ送信割り込み処理 */
//        sio_snd_inte( 1, /* ＢＳ１ */
//            (REGST)CD_CUP_A_SCR_SCI1, 
//            (REGST)CD_CUP_A_SSR_SCI1, 
//            (REGST)CD_CUP_A_TDR_SCI1 );
//    }
//    else
//    {                                                   /* ループバック試験中 */
//                                  /* ループバック処理－ＳＩＯ送信割り込み処理 */
//        lpb_sio_snd_inte( 1, /* ＢＳ１ */
//            (REGST)CD_CUP_A_SCR_SCI1, 
//            (REGST)CD_CUP_A_SSR_SCI1, 
//            (REGST)CD_CUP_A_TDR_SCI1 );
//    }
//}

/*----------------------------------------------------------------------------*/
/** 
 * ＳＩＯ割り込み処理
 * 
 * @param scr_p ＳＣＲレジスタ (I)
 * @param ssr_p ＳＳＲレジスタ (I)
 * @param tdr_p ＴＤＲレジスタ (I)
 * @retval -
 * @note ・ＳＣＲ ＴＩＥビットの設定（割り込み禁止）
 * @note ・送信状態番号の更新
 * @note ・送信データをＳＩＯに書き込み
 * @note ・データ送信が完了した場合はＳＩＯ送信完了メッセージを
 *         Ｓプロ制御タスクへ通知
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_inte(  volatile BYTE* scr_p, volatile BYTE* ssr_p, volatile BYTE* tdr_p )
{
    CD_SSIO* pssio;                              /* ＳＩＯ送信状態管理 作業用 */
//    WORD ret;                                                         /* 戻値 */
    BYTE tdre;                               /* ＳＳＲレジスタ ＴＤＲＥビット */
    BYTE snd_dt;                                                /* 送信データ */

                                 /* ＳＣＲ ＴＩＥビットの設定（割り込み禁止） */
    BitClr( scr_p, CD_BIT_SCR_TIE );
                             /* 有効な送信データは？（ＳＳＲ ＴＤＲＥビット） */
    tdre = BitTst( ssr_p, CD_BIT_SSR_TDRE );
    if( tdre == 0 )
    {                                                   /* 有効な送信データ有 */
        return;
    }
                                                                /* 実装表は？ */
//    if( g_act_sby_inf[bs_no] == OFF )
//    {               /* 未実装なので破棄（ＴＤＲＥビットクリアは一応しておく） */
//                                             /* ＳＳＲ ＴＤＲＥビットのクリア */
//        BitClr( ssr_p, CD_BIT_SSR_TDRE );
//        return;
//    }
                                                    /* 送信ＳＩＯ情報テーブル */
    pssio = &g_ssio;
                                                          /* 送信バッファは？ */
    if( pssio->ssize == 0 )
    {                                                 /* バッファにデータなし */
                                             /* ＳＳＲ ＴＤＲＥビットのクリア */
        BitClr( ssr_p, CD_BIT_SSR_TDRE );
        return;
    }
                                      /* 送信状態番号の更新と送信データの獲得 */
    sio_snd_mtx( pssio, &snd_dt );
                                                          /* 送信状態番号は？ */
    if( pssio->sstn != CD_SIO_ST_N )
    {                                     /* ニュートラル以外：送信データあり */
                                              /* 送信データをＳＩＯに書き込み */
        *tdr_p = snd_dt;
                                             /* ＳＳＲ ＴＤＲＥビットのクリア */
        BitClr( ssr_p, CD_BIT_SSR_TDRE );
                                 /* ＳＣＲ ＴＩＥビットの設定（割り込み許可） */
        BitSet( scr_p, CD_BIT_SCR_TIE );
    }
    else
    {                                         /* ニュートラル：データ送信完了 */
		wake_up_interruptible( &snd_cmp_wait_q );	/*# 送信完了待ちを起こす #*/
//        CD_MSG_SIO_SNDCMP_IND *psio_smdcmp_ind;                 /* 獲得メモリ */
//
                                                /* 送信キャラクタ数を０に更新 */
        pssio->ssize = 0;
//                                      /* ＳＩＯ送信完了メッセージのメモリ獲得 */
//        ret = get_mpf( CD_MPID_TSK_MSG, 
//            CD_BLKSZ_TSK_MSG, (void**)&psio_smdcmp_ind );
//        if( ret != E_OK )
//        {
//                                                                  /* 異常処理 */
//            cdlc_err_proc( CD_ERRC_SVC_GET_MPF, CD_ERRL_SIO_SND_INTE, 3 );
//        }
//                                            /* ＳＩＯ送信完了メッセージの作成 */
//                                                          /* メッセージヘッダ */
//        psio_smdcmp_ind->msghd.msgid = CD_MSGID_SIOSNDCMP;
//        psio_smdcmp_ind->msghd.spid  = CD_PID_SPRO_TSK;
//        psio_smdcmp_ind->msghd.prio  = PRI_NML;
//        psio_smdcmp_ind->msghd.seq   = 0;
//        psio_smdcmp_ind->msghd.tim   = 0;
//                                                                  /* ＢＳ番号 */
//        psio_smdcmp_ind->bs = bs_no;
//                                            /* ＳＩＯ送信完了メッセージの送信 */
//        ret = snd_mbx( CD_PID_SPRO_TSK, (MSGHD*)psio_smdcmp_ind );
//        if( ret != E_OK )
//        {                                                      /* snd_mbx失敗 */
//                                                                  /* 異常処理 */
//            cdlc_err_proc( CD_ERRC_SVC_SND_MBX, CD_ERRL_SIO_SND_INTE, 4 );
//        }
    }
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                        /* 送信状態番号別処理 */
    switch( pssio->sstn )
    {
                                                          /* ニュートラル状態 */
    case CD_SIO_ST_N:
        sio_snd_mtx_n( pssio, snd_dt );
        break;
                                                              /* ＤＬＥ１状態 */
    case CD_SIO_ST_DLE1:
        sio_snd_mtx_dle1( pssio, snd_dt );
        break;
                                                                    /* Ｈ状態 */
    case CD_SIO_ST_H:
        sio_snd_mtx_h( pssio, snd_dt );
        break;
                                                                    /* Ａ状態 */
    case CD_SIO_ST_A:
        sio_snd_mtx_a( pssio, snd_dt );
        break;
                                                                    /* Ｃ状態 */
    case CD_SIO_ST_C:
        sio_snd_mtx_c( pssio, snd_dt );
        break;
                                                              /* ＤＡＴＡ状態 */
    case CD_SIO_ST_DATA:
        sio_snd_mtx_data( pssio, snd_dt );
        break;
                                                                /* ＥＳＣ状態 */
    case CD_SIO_ST_ESC:
        sio_snd_mtx_esc( pssio, snd_dt );
        break;
                                                              /* ＤＬＥ２状態 */
    case CD_SIO_ST_DLE2:
        sio_snd_mtx_dle2( pssio, snd_dt );
        break;
                                                                /* ＥＴＸ状態 */
    case CD_SIO_ST_ETX:
        sio_snd_mtx_etx( pssio, snd_dt );
        break;
                                                                /* ＬＲＣ状態 */
    case CD_SIO_ST_LRC:
        sio_snd_mtx_lrc( pssio, snd_dt );
        break;
                                                                  /* 異常状態 */
    default:
                                                                  /* 異常処理 */
//            cdlc_err_proc( CD_ERRC_ETC, CD_ERRL_SIO_SND_INTE, 5 );
        break;
    }
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ニュートラル）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_n( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                /* ＤＬＥ送出 */
    *snd_dt = CD_DLE;
                                                 /* 送信状態番号遷移 ＤＬＥ１ */
    pssio->sstn = CD_SIO_ST_DLE1;
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＤＬＥ１）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_dle1( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                    /* Ｈ送出 */
    *snd_dt = CD_H;
                                                               /* 状態遷移 Ｈ */
    pssio->sstn = CD_SIO_ST_H;
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：Ｈ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_h( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                    /* Ａ送出 */
                                            /* （Ａは［０］に格納されている） */
    *snd_dt = pssio->sbuf[pssio->sndpoint];
                                                            /* 送信位置の更新 */
    pssio->sndpoint++;
                                                       /* 送信状態番号遷移 Ａ */
    pssio->sstn = CD_SIO_ST_A;
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：Ａ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_a( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                    /* Ｃ送出 */
                                            /* （Ａは［１］に格納されている） */
    *snd_dt = pssio->sbuf[pssio->sndpoint];
                                                            /* 送信位置の更新 */
    pssio->sndpoint++;
                                                       /* 送信状態番号遷移 Ｃ */
    pssio->sstn = CD_SIO_ST_C;
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：Ｃ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_c( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                            /* 送出データは？ */
    if( pssio->sndpoint < pssio->ssize )
    {                                                                 /* あり */
                                                                    /* Ｄ送出 */
        *snd_dt = pssio->sbuf[pssio->sndpoint];
                                                            /* 送信位置の更新 */
        pssio->sndpoint++;
                                                 /* 送信状態番号遷移 ＤＡＴＡ */
        pssio->sstn = CD_SIO_ST_DATA;
    }
    else
    {                                                                 /* なし */
                                                                /* ＤＬＥ送出 */
        *snd_dt = CD_DLE;
                                                 /* 送信状態番号遷移 ＤＬＥ２ */
        pssio->sstn = CD_SIO_ST_DLE2;
    }
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＤＡＴＡ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_data( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                      /* 前回の送出データは？ */
    if( pssio->sbuf[pssio->sndpoint-1] != CD_DLE )
    {                                                     /* ＤＬＥとは違う値 */
                                                              /* ＤＡＴＡ送出 */
                            /* 送信状態番号の遷移処理（状態：Ｃ）と同等の処理 */
        sio_snd_mtx_c( pssio, snd_dt );
    }
    else
    {                                                       /* ＤＬＥと同じ値 */
                                                                /* ＥＳＣ送出 */
        *snd_dt = CD_ESC;
                                                   /* 送信状態番号遷移 ＥＳＣ */
        pssio->sstn = CD_SIO_ST_ESC;
                                                              /* ＬＲＣの計算 */
        pssio->slrc ^= *snd_dt;
    }
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＥＳＣ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_esc( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                              /* ＤＡＴＡ送出 */
                            /* 送信状態番号の遷移処理（状態：Ｃ）と同等の処理 */
    sio_snd_mtx_c( pssio, snd_dt );
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＤＬＥ２）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_dle2( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                /* ＥＴＸ送出 */
    *snd_dt = CD_ETX;
                                                   /* 送信状態番号遷移 ＥＴＸ */
    pssio->sstn = CD_SIO_ST_ETX;
                                                              /* ＬＲＣの計算 */
    pssio->slrc ^= *snd_dt;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＥＴＸ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_etx( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                                /* ＬＲＣ送出 */
    *snd_dt = pssio->slrc;
                                                   /* 送信状態番号遷移 ＬＲＣ */
    pssio->sstn = CD_SIO_ST_LRC;
}

/*----------------------------------------------------------------------------*/
/** 
 * 送信状態番号の遷移処理（状態：ＬＲＣ）
 * 
 * @param pssio ＳＩＯ送信状態管理のポインタ (I/O)
 * @param snd_dt 送信データを格納するポインタ (O)
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_snd_mtx_lrc( CD_SSIO* pssio, BYTE* snd_dt )
{
                                                       /* 送信状態番号遷移 Ｎ */
    pssio->sstn = CD_SIO_ST_N;
}

/*----------------------------------------------------------------------------*/
/** 
 * @file  cd_sio_rcv_inte.c
 * @brief ＳＩＯ受信割込み処理
 *
 * @author tokunaga724
 * @date 2006-08-30
 * @version $Id: cd_sio_rcv_inte.c,v 1.2 2006/08/30 06:11:57 pdr2f Exp $
 *
 * Copyright 2006 Oki Electric Industry Co.,Ltd.
 * Post Officestage 2CDLC Firmware Project Files
 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
 * ＳＩＯ受信割り込み関数（ポート０）
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_rcv_inte_sci0()
//{
//    BYTE bs_no = 0;                                               /* ＢＳ番号 */
//    REGST scr_p;                                            /* ＳＣＲレジスタ */
//    REGST ssr_p;                                            /* ＳＳＲレジスタ */
//    REGST rdr_p;                                            /* ＲＤＲレジスタ */
//
//                                                        /* ＳＣＲレジスタ設定 */
//    scr_p = (REGST)CD_CUP_A_SCR_SCI0;
//                                                        /* ＳＳＲレジスタ設定 */
//    ssr_p = (REGST)CD_CUP_A_SSR_SCI0;
//                                                        /* ＲＤＲレジスタ設定 */
//    rdr_p = (REGST)CD_CUP_A_RDR_SCI0;
//                                                      /* ループバック試験中？ */
//    if( g_cdlc_sts != CD_DCH_LOP_TEST_STS )
//    {                                               /* ループバック試験中以外 */
//                                                    /* ＳＩＯ受信割り込み処理 */
//        sio_rcv_inte( bs_no, scr_p, ssr_p, rdr_p );
//    }
//    else
//    {                                                   /* ループバック試験中 */
//                                  /* ループバック処理－ＳＩＯ受信割り込み処理 */
//        lpb_sio_rcv_inte( bs_no, scr_p, ssr_p, rdr_p );
//    }
//}

/*----------------------------------------------------------------------------*/
/**
 * ＳＩＯ受信割り込み関数（ポート１）
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_rcv_inte_sci1()
//{
//    BYTE bs_no = 1;                                               /* ＢＳ番号 */
//    REGST scr_p;                                            /* ＳＣＲレジスタ */
//    REGST ssr_p;                                            /* ＳＳＲレジスタ */
//    REGST rdr_p;                                            /* ＲＤＲレジスタ */
//
//                                                        /* ＳＣＲレジスタ設定 */
//    scr_p = (REGST)CD_CUP_A_SCR_SCI1;
//                                                        /* ＳＳＲレジスタ設定 */
//    ssr_p = (REGST)CD_CUP_A_SSR_SCI1;
//                                                        /* ＲＤＲレジスタ設定 */
//    rdr_p = (REGST)CD_CUP_A_RDR_SCI1;
//                                                      /* ループバック試験中？ */
//    if( g_cdlc_sts != CD_DCH_LOP_TEST_STS )
//    {                                               /* ループバック試験中以外 */
//                                                    /* ＳＩＯ受信割り込み処理 */
//        sio_rcv_inte( bs_no, scr_p, ssr_p, rdr_p );
//    }
//    else
//    {                                                   /* ループバック試験中 */
//                                  /* ループバック処理－ＳＩＯ受信割り込み処理 */
//        lpb_sio_rcv_inte( bs_no, scr_p, ssr_p, rdr_p );
//    }
//}
//
/*----------------------------------------------------------------------------*/
/**
 * キャラクタ桁間タイマのタイムアウト処理
 * 
 * @param bs_no ＢＳ番号
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_rcv_to_tim03( BYTE bs_no )
{
//    WORD ret;                                                         /* 戻値 */

#if 0 /* 割り込み中の割り込み制御はＮＧ */
                                                /* ＳＩＯ受信割り込み禁止開始 */
    if( bs_no == 0 )
        ret = dis_int( INO_RXI0 );
    else
        ret = dis_int( INO_RXI1 );
    if( ret != E_OK )
    {
                                                                  /* 異常処理 */
        cdlc_err_proc( CD_ERRC_SVC_DIS_INT, CD_ERRL_SIO_RCV_INTE, 1 );
    }
#endif
                                                      /* 受信状態番号の初期化 */
    g_rsio.rstn = CD_SIO_ST_N;
                                                    /* 受信ＬＲＣ情報の初期化 */
    g_rsio.rlrc = 0;
                                                  /* 受信キャラクタ数の初期化 */
    g_rsio.rsize = 0;
	
    if( g_rcv_err.t_o != 0xFF )
    {
                                                /* ＳＩＯ受信エラー情報の更新 */
        g_rcv_err.t_o++;
    }
#if 0 /* 割り込み中の割り込み制御はＮＧ */
                                                /* ＳＩＯ受信割り込み禁止解除 */
    if( bs_no == 0 )
        ret = ena_int( INO_RXI0 );
    else
        ret = ena_int( INO_RXI1 );
    if( ret != E_OK )
    {
                                                                  /* 異常処理 */
        cdlc_err_proc( CD_ERRC_SVC_ENA_INT, CD_ERRL_SIO_RCV_INTE, 2 );
    }
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** 
 * @file  cd_sio_err_inte.c
 * @brief ＳＩＯ障害割込み処理
 *
 * @author tokunaga724
 * @date 2006-08-30
 * @version $Id: cd_sio_err_inte.c,v 1.2 2006/08/30 06:11:57 pdr2f Exp $
 *
 * Copyright 2006 Oki Electric Industry Co.,Ltd.
 * Post Officestage 2CDLC Firmware Project Files
 */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  インクルードファイル
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  定数
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  構造体
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
  関数
------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** ＳＩＯ障害割り込み関数（ポート０）
 *
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_err_inte_sci0( void )
//{
//    BYTE bs_no = 0;                                               /* ＢＳ番号 */
//    REGST scr_p;                                            /* ＳＣＲレジスタ */
//    REGST ssr_p;                                            /* ＳＳＲレジスタ */
//
//                                                        /* ＳＣＲポインタ設定 */
//    scr_p = (REGST)CD_CUP_A_SCR_SCI0;
//                                                        /* ＳＳＲポインタ設定 */
//    ssr_p = (REGST)CD_CUP_A_SSR_SCI0;
//                                                    /* ＳＩＯ障害割り込み処理 */
//    sio_err_inte( bs_no, scr_p, ssr_p );
//}

/*----------------------------------------------------------------------------*/
/** ＳＩＯ障害割り込み関数（ポート１）
 *
 * 
 * @param -
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
//void sio_err_inte_sci1( void )
//{
//    BYTE bs_no = 1;                                               /* ＢＳ番号 */
//    REGST scr_p;                                            /* ＳＣＲレジスタ */
//    REGST ssr_p;                                            /* ＳＳＲレジスタ */
//
//                                                        /* ＳＣＲポインタ設定 */
//    scr_p = (REGST)CD_CUP_A_SCR_SCI1;
//                                                        /* ＳＳＲポインタ設定 */
//    ssr_p = (REGST)CD_CUP_A_SSR_SCI1;
//                                                    /* ＳＩＯ障害割り込み処理 */
//    sio_err_inte( bs_no, scr_p, ssr_p );
//}
//
/*----------------------------------------------------------------------------*/
/** ＳＩＯ障害割り込み処理
 *
 * 
 * @param scr_p ＳＣＲレジスタ
 * @param ssr_p ＳＳＲレジスタ
 * @retval -
 * 
 */
/*----------------------------------------------------------------------------*/
void sio_err_inte( volatile BYTE* scr_p, volatile BYTE* ssr_p )
{
    BYTE orer;                               /* ＳＳＲレジスタ ＯＲＥＲビット */
    BYTE fer;                                  /* ＳＳＲレジスタ ＦＥＲビット */
    BYTE per;                                  /* ＳＳＲレジスタ ＰＥＲビット */
    
                                                                /* 実装表は？ */
//    if( g_act_sby_inf[bs_no] == OFF )
//   {               /* 未実装なので破棄（エラービットのクリアは一応しておく） */
//                                                      /* エラービットのクリア */
//        BitClr( ssr_p, CD_BIT_SSR_ORER );
//        BitClr( ssr_p, CD_BIT_SSR_FER );
//        BitClr( ssr_p, CD_BIT_SSR_PER );
//        return;
//    }
                                                      /* オーバーランエラー？ */
    orer = BitTst( ssr_p, CD_BIT_SSR_ORER );
    if( orer == 1 )
    {                                               /* オーバーランエラー発生 */
        if( g_rcv_err.ov_run != 0xFF )
        {
                                                /* ＳＩＯ受信エラー情報の更新 */
            g_rcv_err.ov_run++;
        }
                                                      /* エラービットのクリア */
        BitClr( ssr_p, CD_BIT_SSR_ORER );
    }
    
                                                      /* フレーミングエラー？ */
    fer = BitTst( ssr_p, CD_BIT_SSR_FER );
    if( fer == 1 )
    {                                               /* フレーミングエラー発生 */
        if( g_rcv_err.flm_eff != 0xFF )
        {
                                                /* ＳＩＯ受信エラー情報の更新 */
            g_rcv_err.flm_eff++;
        }
                                                      /* エラービットのクリア */
        BitClr( ssr_p, CD_BIT_SSR_FER );
    }
    
                                                          /* パリティエラー？ */
    per = BitTst( ssr_p, CD_BIT_SSR_PER );
    if( per == 1 )
    {                                                   /* パリティエラー発生 */
        if( g_rcv_err.pari_err != 0xFF )
        {
                                                /* ＳＩＯ受信エラー情報の更新 */
            g_rcv_err.pari_err++;
        }
                                                      /* エラービットのクリア */
        BitClr( ssr_p, CD_BIT_SSR_PER );
    }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */


