/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       ＭＤＡプログラム                                                */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <errno.h>   
#include <string.h>
#include <unistd.h>

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#include "str.h"
#include "str_comdt.h"
#include "prot.h"
#include "str_boot.h"
#include "str_start.h"
#include "mda.h"
#include "h323_def.h"
#include "tmr_def.h"

#include "acl_voip_conf.h"
#include "acGatewayDefs.h"
#include "acGatewayAPI.h"
#include "acl_dsp_telephony_commands.h"

/*** 自ファイル内でのみ使用する#define マクロ ***/

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/

/*** 自ファイル内でのみ使用するtypedef 定義 ***/

/*** 自ファイル内でのみ使用するenum タグ定義 ***/

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/

/*** ファイル内で共有するstatic 変数宣言 ***/

/*** static 関数宣言 ***/

/*** 関数定義 ***/
void mda_data_init(void);
int mda_dsp_init(void);
void mda_thread_start(void);
void mda_inmsgexe(INNER_MSG *msgp);
void mda_luodrexe(INNER_MSG *msgp);
void mda_luevtexe(INNER_MSG *msgp);
void mda_timeoutexe(INNER_MSG *msgp);
MdaFuncAddr mda_msganl(MDAMSG_ATTR *arrttbl_p, BYTE kind);
void mda_dtmfcomp(INNER_MSG *msgp);
void dspmng_pbsndend(DWORD cnno);
void mdamsg_evtsend(INNER_MSG *msgp);
void mda_muonptset(INNER_MSG *msgp);
void mda_horycomp(INNER_MSG *msgp);
void mda_dtmfabt(INNER_MSG *msgp);
void dspmng_dsp_errstat(INNER_MSG	*msg_ptr);
void dspmng_make_dsperror(int error,short dsp_id,short irq_flg,BYTE tsk_id);
void dspmng_dspreinit(INNER_MSG	*msg_ptr);
void dspmng_dsperror(int error_no,short dsp_id,short irq_flg, BYTE	tsk_id);
DWORD	dspmng_ipaddrck( LUORD_PASSCTLREQ *ordp , DWORD  cnno , DWORD  ckptn);
void	dspmng_ordpass( INNER_MSG *msgp);
void	dspmng_pass00( INNER_MSG *msgp );
void	dspmng_pass01( INNER_MSG *msgp );
void	dspmng_pass02( INNER_MSG *msgp );
void	dspmng_pass10( INNER_MSG *msgp );
void	dspmng_pass11( INNER_MSG *msgp );
void	dspmng_pass12( INNER_MSG *msgp );
void	dspmng_pass20( INNER_MSG *msgp );
void	dspmng_pass21( INNER_MSG *msgp );
void	dspmng_pass30( INNER_MSG *msgp );
void	dspmng_ordtone(INNER_MSG *msgp);
void	dspmng_tonesnd( INNER_MSG *msgp );
void	dspmng_tonecut( INNER_MSG *msgp );
void	dspmng_tonesave( INNER_MSG *msgp );

void dspmng_dspclose(BYTE	ctlptn,BYTE	cnno);
void dspmng_passfree(short dsp_id,short	irq_flg,BYTE tsk_id);
void dspmng_h323cut( BYTE cnno, BYTE ctlptn );
void dspmng_timout( INNER_MSG *msgp);
void dspmng_helchk_timout(DWORD time_id , DWORD time_kind , DWORD cnno);
void dspmng_tonetime( DWORD time_id , DWORD time_kind , DWORD cnno );
void dspmng_ordhory( INNER_MSG *msgp);
void dspmng_ordpbsnd( INNER_MSG *msgp);
void dspmng_ordpbcut( INNER_MSG *msgp);
void dspmng_voicecut( INNER_MSG *msgp);
void dspmng_helchk_rcv(INNER_MSG *msg_ptr);
void	dspmng_orderr( INNER_MSG *msgp);

void dspevt(void *arg);
void mdamsg_inmsend(BYTE kind, DWORD cnno);
void dsp2lan(void *arg);
void lan2dsp(void *arg);
void mda_rcvpacket(BYTE sock_kind, DWORD cnno, fd_set *readfds);
void mda_fd_init(void);

voip_dsp_advanced_channel_configuration_t   AdvancedChannelConfiguration;
Tac49xCallProgressDeviceConfigurationAttr   CallProgress_Attr;

void mda_path_set( DWORD	ctlptn, DWORD cnno,BYTE codec_change);
void mda_set_RTPstream(acgTRTPStreamParameters *acgTRTPStreamParameters_p, DSPMNG_PASSCTL *dspmng_passctl_p);
void mda_set_gain(acgTGainParameters *acgTGainParameters_p, DSPMNG_PASSCTL *dspmng_passctl_p);
void mda_set_JBparam(acgTJitterBufferParameters *acgTJitterBufferParameters_p, RXBUF *rxbuf_p);
void dspmng_make_dsprepair(short dsp_id,char irq);

/*** 変数定義 ***/

extern	COM_DATA	com_data;								/*# システム共通メモリ #*/
extern	DSPMNG_PASSCTL	dspmng_passctl[CNMAX];				/*# パス制御用メモリ #*/
extern	DSPMNG_TONECTL	dspmng_tonectl[CNMAX];				/*# トーン制御用メモリ #*/
extern	DSPMNG_SHFTCTL	dspmng_shftctl;						/*# シフト監視用メモリ #*/
extern	DSPINT_SCANCTL	dspint_scanctl[CNMAX];				/*# 監視制御用メモリ #*/
extern	DSPINT_VOICECTL	dspint_voicectl[CNMAX];				/*# 音声制御用メモリ #*/
extern	DWORD pool_get_err[];
extern struct	H323_DATA	h323_snd_data[CNMAX];
extern struct	H323_DATA	h323_rcv_data[CNMAX];

extern  acgTChannelParameters ChannelParameters[CONFIG_RG_VOIP_MAX_NUMBER_OF_LINES][NUMBER_OF_CHANNELS_PER_LINE]; 

extern int Default_Coder;
extern int Default_Payload_Type;


DSP_DATA dsp_data[DSP_MAX];
Tac49xSetupDeviceAttr Device_Attr;

RXBUF	rx_buf_tbl[V2_1_CCU_BUFFNUMMAX];		//DSP用バッファ面管理テーブル(10)
DWORD	hpireply_count[2];

/* タイムアウトメッセージ分析テール */
MDAMSG_ATTR mda_timeoutattr[MDA_TIMEOUT_MAX];

MDA_SOCKET_CTL_T mda_socket_ctl_table[CNMAX];  /* MDAソケット管理テーブル */
int max_rcv_sd; /* 受信用最大ソケットディスクリプタ */
int notice_sd_s;  /* 通知用サーバソケットディスクリプタ */
struct sockaddr_in notice_addr_s; /* 通知用サーバアドレス */
int notice_sd_c;  /* 通知用クライアントソケットディスクリプタ */
struct sockaddr_in notice_addr_c; /* 通知用クライアントアドレス */
fd_set mda_readfds; /* 受信用ソケットディスクリプタ群 */
char mda_dummy_msg_rcv[POOL3_SIZE];

regional_settings_params_t *regionalSettings = NULL;
/* 内部メッセージ分析テール */
MDAMSG_ATTR mda_inmsgattr[10] = {{I_VOSTOP,{0,0,0},h323},{I_DEVICE_STATUS,{0,0,0},dspmng_helchk_rcv},{I_DSPERROR,{0,0,0},dspmng_dspreinit},
	{I_DTMFCOMPLETE,{0,0,0},mda_dtmfcomp},{I_SDESRCV,{0,0,0},h323},{I_BYERCV,{0,0,0},h323},{I_MUONPTNSET,{0,0,0},mda_muonptset},
	{I_HORYCOMPLETE,{0,0,0},mda_horycomp},{I_DTMFABORT,{0,0,0},mda_dtmfabt},{KIND_OF_STOPPER,{0,0,0},NULL}};
/* オーダメッセージ分析テール */
MDAMSG_ATTR mda_luodrattr[8] = {{O_PASSCTLREQ,{0,0,0},dspmng_ordpass},{O_TONECTLREQ,{0,0,0},dspmng_ordtone},{O_HORYCTLREQ,{0,0,0},dspmng_ordhory},
	{O_PBSNDSTART,{0,0,0},dspmng_ordpbsnd},{O_PBSNDSTOP,{0,0,0},dspmng_ordpbcut},{O_OBSNTFYREQ,{0,0,0},dspmng_dsp_errstat},{O_AQACT,{0,0,0},h323},
	{KIND_OF_STOPPER,{0,0,0},NULL}};
/* イベントメッセージ分析テール */
MDAMSG_ATTR mda_luevtattr[1] = {{KIND_OF_STOPPER,{0,0,0},NULL}};

/*# パス制御オーダ処理ＪＵＭＰテーブル #*/
const	MdaFuncAddr dspmng_passtbl[3][4] = {
		{dspmng_pass00,dspmng_pass01,dspmng_pass02,dspmng_pass30},
		{dspmng_pass10,dspmng_pass11,dspmng_pass11,dspmng_pass30},
		{dspmng_pass20,dspmng_pass21,dspmng_orderr,dspmng_pass30}
};
/*# トーン制御オーダ処理ＪＵＭＰテーブル #*/
const	MdaFuncAddr dspmng_tonetbl[4] = {
		dspmng_tonesnd,
		dspmng_tonesnd,
		dspmng_tonecut,
		dspmng_tonesave
};

//オーダで指定されるCODECにあわせたLengthとMsec
const CODEC_INFO	lu_codec[]={
//	len,	msec
	{20		,30},
	{24		,30},
	{10		,10},
	{80		,10},
	{80		,10},
	{0		,0},
	{0		,0},
	{80		,10}
};	



/******************************************************************************/
/* 関数名     ＭＤＡスレッド                                                  */
/* 機能概要   LUプロセスのメイン処理を行う。                                  */
/* パラメータ arg  : (in)   動作モード格納領域ポインタ配列                    */
/* リターン   -                                                               */
/******************************************************************************/

void mda_thread(void *arg)
{
	INNER_MSG	*msgp; /*# ＬＵメッセージポインタ #*/
	DWORD	msg_div;   /*# メッセージ区分 #*/
	DWORD	msg_kind;  /*# メッセージ種別 #*/
	DWORD	loop_index;/*# ループインデックス #*/
	INNER_MSG *msg_ptr;

	/* 引数(動作モード)が通常モード以外の時は、エラーメッセージを出力後、5秒sleepしながら無限ループする */
	if (*(int *)arg == LUMODE_MAINTE) {
		while(1) {
			dbg_print(MDA_ID, LOG_ERR, "%s Illeagal operation mode %d\n", __func__, *(int *)arg);
			sleep(5);
		}
	}
	/* データ初期化を行う */
	mda_data_init();
	/* lu_mngから無音設定オーダメッセージを待つ */
	while (1) {
		/*# メッセージ受信 #*/
		msgp =(INNER_MSG *) com_rcvmsg( MDA_ECB , 0 );
		msg_kind = (DWORD)msgp->msg_header.kind;			/*# メッセージ種別セーブ #*/
		msg_div = (DWORD)msgp->msg_header.div;				/*# メッセージ区分セーブ #*/
		/*# メッセージ解放処理 #*/
		if(msgp->msg_header.link != NULL){
			/*# ２次メッセージ解放 #*/
			com_poolput( POOL1 , (BYTE *)msgp->msg_header.link );
		}
		/*# 内部（１次）メッセージ解放 #*/
		com_poolput( POOL0 , (BYTE *)msgp );

		if ((msg_div == IN_MSG) && (msg_kind == I_MUONPTNSET)) {
			/* 符号化則データの更新を行う */
			if(com_data.silent_ptn == VO_SLTPTN_U){
				/* μ_Law */
				Default_Coder = ACG_CODER__G711MULAW;
				Default_Payload_Type = 0;
			}
			else {
				/* A_Law */
				Default_Coder = ACG_CODER__G711ALAW;
				Default_Payload_Type = 8;
			}
			/* voip_conf_initを起動し、SDKで使用するVoiP関連データを初期化する  */
			voip_conf_init();
		}
		break;
	}
	/* mda_dsp_initを起動し、内蔵DSPの初期化を行う */
	if (mda_dsp_init() == ACG_FAILURE) {
		/* 実行結果が失敗(ACG_FAILURE)の場合、DSP初期化エラー発生(dsp_data[i].init_error)を設定する */
		for (loop_index=0;loop_index<DSP_MAX;++loop_index) {
			dsp_data[loop_index].init_error = DSP_INIT_ERROR;
		}
	}
	/* LUMNGにDSP起動完了通知メッセージ送信 */
	{
		/* メッセージ取得 */
		while(1) {
			msg_ptr = ( INNER_MSG *)com_ipoolget( POOL0 );		/*# メモリ確保 #*/
			if(msg_ptr == ( INNER_MSG *)0xffffffff)				/*# プール取得OK？ #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				sleep(1);
			}
			else {
				break; /* whileループを抜ける */
			}
		}
		msg_ptr->msg_header.id		= MDA_ID;				/*# 送信元ID(LU管理)セット #*/
		msg_ptr->msg_header.div 	= IN_MSG;				/*# メッセージ区分セット #*/
		msg_ptr->msg_header.kind 	= I_DSPCMP;				/*# DSP起動完了通知 #*/
		msg_ptr->msg_header.link 	= NULL;					/*# 2次リンクなし #*/

		//メッセージ送信
		com_sndmsg(LUMNG_ECB, ( BYTE * )msg_ptr );				/*#	メッセージ送出 #*/
	}
	/* 関連スレッドの生成と初期化を行う */
	mda_thread_start();
	/* メッセージを受信し処理する */
	while (1) {
		/*# メッセージ受信 #*/
		msgp =(INNER_MSG *) com_rcvmsg( MDA_ECB , 0 );
		/* メッセージ区分で分岐処 */
		switch (msg_ptr->msg_header.div) {
		case IN_MSG: /* 内部メッセージ */
			mda_inmsgexe(msgp);
			break;
		case LU_ODR: /* ＬＵオーダ */
			mda_luodrexe(msgp);
			break;
		case LU_EVT: /* ＬＵイベント */
			mda_luodrexe(msgp);
			break;
		case TIM_OUT: /* タイムアウトメッセージ */
			mda_timeoutexe(msgp);
			break;
		default:
			dbg_print(MDA_ID, LOG_ERR, "%s Illeagal msg_header.div=%d\n", __func__, (int)msgp->msg_header.div);
			break;
		}
		/*# メッセージ解放処理 #*/
		if(msgp->msg_header.link != NULL){
			/*# ２次メッセージ解放 #*/
			com_poolput( POOL1 , (BYTE *)msgp->msg_header.link );
		}
		/*# 内部（１次）メッセージ解放 #*/
		com_poolput( POOL0 , (BYTE *)msgp );
	}

}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_data_init                                            #*/
/*#                                                                         #*/
/*#     日本語名 : mdaデータ初期設定                                        #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : mda内のデータを初期化する。                              #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_data_init(void)
{
	int		cnidx; /*# チャネルインデックス #*/
	int		loop_index; /*# ループインデックス #*/
	int		timeout_index=0;
	
	/* タイムアウトメッセージ分析テールを初期化する */
	for(loop_index=0;loop_index<MDA_TIMEOUT_MAX;++loop_index) {
		mda_timeoutattr[loop_index].kind = KIND_OF_STOPPER;
		mda_timeoutattr[loop_index].func_adr = NULL;
	}
	for(cnidx=0; cnidx<CNMAX; cnidx++){
		/* 共通データを初期化する */
		{
			com_data.voice_start[cnidx] = 1;/*# デフォルト値 #*/
			com_data.voice_stop[cnidx] = -1;/*# デフォルト値 #*/
		}
		/* パス制御用メモリ(dspmng_passctl)を初期化する */
		{
			memset((void *)&dspmng_passctl[cnidx], 0, sizeof(DSPMNG_PASSCTL));
			dspmng_passctl[cnidx].codec=0xff;
		}
		/* トーン制御用メモリ(dspmng_tonectl)を初期化する */
		{
			memset((void *)&dspmng_tonectl[cnidx], 0, sizeof(DSPMNG_TONECTL));
			dspmng_tonectl[cnidx].tonetim_id1 =					/*# トーンタイマID_1取得 #*/
										(DWORD)tmr_entry(MDA_ECB,TMR_5MS_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)dspmng_tonectl[cnidx].tonetim_id1;
			mda_timeoutattr[timeout_index].func_adr = dspmng_timout;
			++timeout_index;
			dspmng_tonectl[cnidx].tonetim_id2 =					/*# トーンタイマID_2取得 #*/
										(DWORD)tmr_entry(MDA_ECB,TMR_5MS_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)dspmng_tonectl[cnidx].tonetim_id2;
			mda_timeoutattr[timeout_index].func_adr = dspmng_timout;
			++timeout_index;
		}
		/* 音声制御用メモリ(dspint_voicectl)を初期化する */
		{
			memset((void *)&dspint_voicectl[cnidx], 0, sizeof(DSPINT_VOICECTL));
		}
		/* 監視制御用メモリ(dspint_scanctl)を初期化する */
		{
			memset((void *)&dspint_scanctl[cnidx], 0, sizeof(DSPINT_SCANCTL));
		}
		/* 送信制御データを初期化する */
		{
			h323_sdata_cls(cnidx);
			/* SDES送信タイマ登録 */
			h323_snd_data[cnidx].timerid = tmr_entry(MDA_ECB,TMR_1S_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)h323_snd_data[cnidx].timerid;
			mda_timeoutattr[timeout_index].func_adr = h323;
			++timeout_index;
		}
		/* 受信制御データを初期化する */
		{
			h323_rdata_cls(cnidx);
			/* SDES受信タイマ登録 */
			h323_rcv_data[cnidx].timerid = tmr_entry(MDA_ECB,TMR_1S_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)h323_rcv_data[cnidx].timerid;
			mda_timeoutattr[timeout_index].func_adr = h323;
			++timeout_index;
			/* RR送信タイマ登録 */
			h323_rcv_data[cnidx].timerid2 = tmr_entry(MDA_ECB,TMR_1S_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)h323_rcv_data[cnidx].timerid2;
			mda_timeoutattr[timeout_index].func_adr = h323;
			++timeout_index;
		}
		/* チャネル情報(h323_channel_info)を初期化する */
		{
			h323_channel_info_cls(cnidx);
		}
	}
	for (loop_index=0;loop_index<DSP_MAX;++loop_index) {
		/* DSP管理データ(dsp_data)を初期化する */
		{
			memset((void *)&dsp_data[loop_index], 0, sizeof(DSP_DATA));
			/* 状態を障害に設定する */
			dsp_data[loop_index].status = XC_FAULT;
			/* DSPヘルスチェックタイムアウトタイマID取得 */
			dsp_data[loop_index].helouttim_id = (int)tmr_entry(MDA_ECB,TMR_1S_CNT,TMR_EXENML);
			mda_timeoutattr[timeout_index].kind = (BYTE)dspmng_tonectl[loop_index].tonetim_id2;
			mda_timeoutattr[timeout_index].func_adr = dspmng_timout;
			++timeout_index;
		}
	}
	/* 受信用ファイルディスクリプタを初期化する */
	mda_fd_init();

	memset((void *)hpireply_count, 0, sizeof(hpireply_count));
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_dsp_init                                          #*/
/*#                                                                         #*/
/*#     日本語名 : mdaDSP初期設定                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 内部DSPを初期化する。                                   #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : int : 実行結果(ACG_SUCCESS/ACG_FAILURE)            #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
int mda_dsp_init(void)
{
	/* DSPドライバ(ac49x_dsp.ko)をinsmodする */
	system("insmod /lib/modules/3.10.49/ac49x_dsp.ko");
	/* DSP関連コンフィグレーションデータを設定する */
	voip_conf_dsp_params_get(&regionalSettings, &AdvancedChannelConfiguration);
	/* DSPを立ち上げる */
	return(dsp_init(regionalSettings, &AdvancedChannelConfiguration));
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_thread_start                                         #*/
/*#                                                                         #*/
/*#     日本語名 : mdaスレッド開始                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : mda関連スレッドの生成と起動を行う。                      #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_thread_start(void)
{
	/* dspevtスレッドの生成と起動を行う */
	com_threadstart(DSPEVT_ID, (void *)&com_data.operation_mode);
	/* dsp2lanスレッドの生成と起動を行う */
	com_threadstart(DSP2LAN_ID, (void *)&com_data.operation_mode);
	/* lan2dspスレッドの生成と起動を行う */
	com_threadstart(LAN2DSP_ID, (void *)&com_data.operation_mode);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_inmsgexe                                             #*/
/*#                                                                         #*/
/*#     日本語名 : mda内部メッセージ実行                                    #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 内部メッセージ受信処理を実行する。                       #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_inmsgexe(INNER_MSG *msgp)
{
	MdaFuncAddr func_adr; /*  関数アドレス*/
	/* 実行関数アドレスを求める */
	func_adr = mda_msganl(mda_inmsgattr, msgp->msg_header.kind);
	if(func_adr!= NULL) {
		/* 実行関数アドレスがNULL以外の場合は、実行関数を起動する */
		func_adr(msgp);
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_luodrexe                                             #*/
/*#                                                                         #*/
/*#     日本語名 : mdaオーダ実行                                            #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : オーダメセージ受信処理を実行する。                       #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_luodrexe(INNER_MSG *msgp)
{
	MdaFuncAddr func_adr; /*  関数アドレス*/
	/* 実行関数アドレスを求める */
	func_adr = mda_msganl(mda_luodrattr, msgp->msg_header.kind);
	if(func_adr!= NULL) {
		/* 実行関数アドレスがNULL以外の場合は、実行関数を起動する */
		func_adr(msgp);
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_luevtexe                                             #*/
/*#                                                                         #*/
/*#     日本語名 : mdaイベント実行                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : イベントメセージ受信処理を実行する。                     #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_luevtexe(INNER_MSG *msgp)
{
	MdaFuncAddr func_adr; /*  関数アドレス*/
	/* 実行関数アドレスを求める */
	func_adr = mda_msganl(mda_luevtattr, msgp->msg_header.kind);
	if(func_adr!= NULL) {
		/* 実行関数アドレスがNULL以外の場合は、実行関数を起動する */
		func_adr(msgp);
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_timeoutexe                                           #*/
/*#                                                                         #*/
/*#     日本語名 : mdaタイムアウト実行                                      #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : タイムアウトメセージ受信処理を実行する。                 #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_timeoutexe(INNER_MSG *msgp)
{
	MdaFuncAddr func_adr; /*  関数アドレス*/
	/* 実行関数アドレスを求める */
	func_adr = mda_msganl(mda_timeoutattr, msgp->msg_header.kind);
	if(func_adr!= NULL) {
		/* 実行関数アドレスがNULL以外の場合は、実行関数を起動する */
		func_adr(msgp);
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_msganl                                               #*/
/*#                                                                         #*/
/*#     日本語名 : mdaメッセージ分析処理                                    #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 入力テーブルのメッセージ種別と一致する実行関数を求める。 #*/
/*#     入    力 : MDAMSG_ATTR *arrttbl_p、BYTE kind                  #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
MdaFuncAddr mda_msganl(MDAMSG_ATTR *arrttbl_p, BYTE kind)
{
	while(arrttbl_p->kind != KIND_OF_STOPPER) {
		if (arrttbl_p->kind == kind) {
			return (void *)arrttbl_p->func_adr;
		}
	}
	return ((MdaFuncAddr)NULL);
}


/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_dtmfcomp                                             #*/
/*#                                                                         #*/
/*#     日本語名 : DTMF送信完了処理                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DTMF送信完了処理を行う。                                 #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_dtmfcomp(INNER_MSG *msgp)
{
	DWORD	cnno; /*# ＣＮ番号 #*/
	BYTE pbdata;
	LONG set_pad_l,set_pad_h;
	acgTTone ToneSpec;
	WORD timedata;
	BYTE codec_change;
	
	// CN番号を抽出する
	cnno = msgp->msg_header.no;
	
	// PB送出中でなければ、リターンする
	if (dspmng_tonectl[cnno].send_sts != SNDPB) {
		return;
	}
	//残ＰＢあれば送出、なければ完了イベント送出
	if(dspmng_tonectl[cnno].pbcount!=0){
		
		pbdata = dspmng_tonectl[cnno].pb_data[dspmng_tonectl[cnno].pbread++];
		/*# ＰＢデータが数字の場合 #*/
		if((pbdata & PB_CHECK) == 0){
			pbdata = pbdata & PB_MASK;/*# 数字データ抽出 #*/
			/*# 低群レベル読み出し #*/
			set_pad_l = dspmng_tonectl[cnno].pb_pad_l;
			/*# 高群レベル読み出し #*/
			set_pad_h = dspmng_tonectl[cnno].pb_pad_h;
			// UserDefinedTones属性を変更する
			Device_Attr.UserDefinedTones.Signal[0].HighEnergyThreshold = (int)(-set_pad_h);
			Device_Attr.UserDefinedTones.Signal[0].LowEnergyThreshold = (int)(-set_pad_l);
			
			// DTMF信要求を行う
			ToneSpec.ToneElement.Type = ACG_TONE_TYPE__DTMF;
			ToneSpec.ToneElement.ToneIndex = pbdata;
			ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
			ToneSpec.OnDuration = (int)dspmng_tonectl[cnno].pb_tim;
			ToneSpec.OffDuration = (int)dspmng_tonectl[cnno].pb_min;
			if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #1\n", __func__);
			}
			dspmng_tonectl[cnno].pbtim_sts = PBNUM_STS;/*# ＰＢタイマ状態ポーズ設定 #*/
		}
		else
		{ /*# 送出ータがポーズデータの場合 #*/
			pbdata = pbdata & POSE_MASK; /*# ポーズデータ抽出 #*/
			/*# ポーズ時間算出 #*/
			timedata = (WORD)(pbdata * T100MS );
			dspmng_tonectl[cnno].pbtim_sts = POSE_STS;
			// DTMF信要求を行う
			ToneSpec.ToneElement.Type = ACG_TONE_TYPE__DTMF;
			ToneSpec.ToneElement.ToneIndex = (int)timedata | 0x80000000;
			ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
			ToneSpec.OnDuration = (int)dspmng_tonectl[cnno].pb_tim;
			ToneSpec.OffDuration = (int)dspmng_tonectl[cnno].pb_min;
			if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #2\n", __func__);
			}
		}
		//ＰＢ送出ＩＮＤＥＸが最後まで行った場合は最初に戻る
		if(dspmng_tonectl[cnno].pbread==PBDATAMAX)
		{
			dspmng_tonectl[cnno].pbread=0;
			//ＰＢ書き込みＩＮＤＥＸがＰＢ送出ＩＮＤＥＸをこえてたら
			//追い越しなくなるのでフラグオフ
			if(dspmng_tonectl[cnno].oikosi==ON)
				dspmng_tonectl[cnno].oikosi=OFF;
			
			//残ＰＢ数設定
			dspmng_tonectl[cnno].pbcount=(dspmng_tonectl[cnno].pbwrite-dspmng_tonectl[cnno].pbread);
		}
		else	//ＰＢ送出ＩＮＤＥＸが最後まで行ってない場合
		{
			//ＰＢ送出ＩＮＤＥＸがＰＢ書き込みＩＮＤＥＸより大きい場合→追い越してる場合です
			if((dspmng_tonectl[cnno].pbread>dspmng_tonectl[cnno].pbwrite)&&(dspmng_tonectl[cnno].oikosi==ON))
			{
				dspmng_tonectl[cnno].pbcount=(PBDATAMAX-dspmng_tonectl[cnno].pbread)+dspmng_tonectl[cnno].pbwrite;
			}
			//ＰＢ送出ＩＮＤＥＸがＰＢ書き込みＩＮＤＥＸより小さい場合
			else if(dspmng_tonectl[cnno].pbread<=dspmng_tonectl[cnno].pbwrite)
			{
				dspmng_tonectl[cnno].pbcount=(dspmng_tonectl[cnno].pbwrite-dspmng_tonectl[cnno].pbread);
			}
			//ＰＢ送出ＩＮＤＥＸとＰＢ書き込みＩＮＤＥＸが等しい場合は追い越し有り無しに関わらず
			//何もしない。この後ＰＢがなければ最後のＰＢ送出完了時にイベントを上げる
		}
	}
	else		//残ＰＢなし→ＰＢ送出完了イベント送信する？
	{
														/*# 送出状態を空きに設定 #*/
		dspmng_tonectl[cnno].send_sts = NOSND;
														/*# ＰＢタイマ状態クリア #*/
		dspmng_tonectl[cnno].pbtim_sts = CLR;
														/*# 送出ＰＢデータ数クリア #*/
		dspmng_tonectl[cnno].pb_num = CLR;
														/*# ＰＢ読み出し位置クリア #*/
		dspmng_tonectl[cnno].pb_cnt = CLR;
		
		dspmng_tonectl[cnno].pbread=CLR;
		
		dspmng_tonectl[cnno].pbwrite=CLR;
		
		dspmng_tonectl[cnno].pbcount=CLR;
		
		dspmng_tonectl[cnno].pb_acm_flg=CLR;
		
		dspmng_tonectl[cnno].oikosi=CLR;
		
//		//SS9100 2004.11.15 IPstage対応D01側も必要
//		dsp_mute(dsp_id,cnno,XC_OFF);
		
		//DSPクローズ判定
		//送話or受話パス接続中でなければDSPクローズしてリターン
		if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
		{
			dspmng_dspclose(TONEPBSND,cnno);
		}
		else
		{
			//ボコーダOFFで送受話パスどちらか接続中の場合はモードを変更
			if(dspmng_passctl[cnno].snd_sts!=DISCON)
			{
				codec_change=ON;
				mda_path_set(SNDPASS,cnno,codec_change);
			}
			if(dspmng_passctl[cnno].rcv_sts!=DISCON)
			{
				if(codec_change==ON)
				{
					codec_change=OFF;
				}
				else
				{
					codec_change=ON;
				}
				
				mda_path_set(RCVPASS,cnno,codec_change);
			}
		}
		dspmng_pbsndend( cnno ); /*# ＰＢ送信完了イベント送信処理 #*/
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pbsndend(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－ＰＢ送信完了イベント送信処理                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ＰＢ送信完了イベントの作成／送信を行う。                 #*/
/*#     入    力 : DWORD    cnno : ＣＮ番号                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/28 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_pbsndend(DWORD cnno)
{
LUEVT_PBSNDEND	*evtp;										/*# ＰＢ送出完了イベントポインタ #*/
INNER_MSG	*msgp;											/*# ＬＵメッセージポインタ #*/

															/*# ＰＢ送信完了イベント作成 #*/
															/*# イベント用プール獲得 #*/
	evtp = (LUEVT_PBSNDEND *)com_poolget(POOL1);
	if(evtp == (LUEVT_PBSNDEND *)0xffffffff)				/*# POOL取得ＯＫ？ (V1) #*/
		return;
															/*# インタフェーサ部作成 #*/
	evtp->lu_header.type = SNDID_LU;
	evtp->bc = 0x03;										/*# ＢＣセット #*/
	evtp->code = E_PBSNDEND;								/*# ＰＢ受信イベントコードセット #*/
	evtp->cnno = (BYTE)cnno;								/*# ＣＮ番号セット #*/
															/*# 内部メッセージ用プール獲得 #*/
	msgp = (INNER_MSG *)com_poolget(POOL0);
	if(msgp == (INNER_MSG *)0xffffffff){					/*# メモリ確保ＯＫ?(V1) #*/
		pool_get_err[POOL0]++;								/*# プール取得ＮＧログ #*/
		com_poolput( POOL1 ,(BYTE *)evtp );
		return;
	}
															/*# ＬＵ内部共通ヘッダ作成 #*/
	msgp->msg_header.kind = E_PBSNDEND;						/*# メッセージ種別 #*/
	msgp->msg_header.link = (BYTE *)evtp;					/*# イベントポインタ接続 #*/
	mdamsg_evtsend( msgp );									/*# イベント送信処理 #*/
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mdamsg_evtsend                                           #*/
/*#                                                                         #*/
/*#     日本語名 : mdaイベント送信                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : doep_sにイベントメッセージを送信する。                   #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mdamsg_evtsend(INNER_MSG *msgp)
{	
	LU_ODEV*	evt_p;
	/*# ＬＵ内部共通ヘッダ作成 #*/
	msgp->msg_header.id = MDA_ID;				/*# 送信元ＩＤ設定 #*/
	msgp->msg_header.div = LU_EVT;				/*# メッセージ区分設定 #*/
	msgp->msg_header.no = CLR;					/*# ＰＫＧ番号クリア #*/
	evt_p = (LU_ODEV*)msgp->msg_header.link;
	evt_p->lu_header.pkg_no = FROM_SLAVE;		/*# ＰＫＧ番号1固定 #*///IP-CS対応追加 2004.04
	com_sndmsg( DOEP_ECB , (BYTE *)msgp );		/*# メッセージ送信 #*///IP-CS対応追加 2004.04
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_muonptset                                            #*/
/*#                                                                         #*/
/*#     日本語名 : 無音パターン設定処理                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 無音パターンオーダ受信時処理を行う。                     #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_muonptset(INNER_MSG *msgp)
{	
	dbg_print(MDA_ID, LOG_DEBUG, "%s activated\n", __func__);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_horycomp                                             #*/
/*#                                                                         #*/
/*#     日本語名 : 保留音出力完了処理                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 保留音出力完了処理を行う。                               #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_horycomp(INNER_MSG *msgp)
{	
	DWORD	cnno; /*# ＣＮ番号 #*/
	
	// CN番号を抽出する
	cnno = msgp->msg_header.no;
	
	// PB送出中でなければ、リターンする
	if (dspmng_tonectl[cnno].send_sts != SNDPB) {
		return;
	}
	dbg_print(MDA_ID, LOG_DEBUG, "%s activated\n", __func__);
	dspmng_tonectl[cnno].send_sts = NOSND;		/*# 空き状態設定 #*/

	dspint_voicectl[cnno].hory_sts = OFF;		/*# ＤＳＰ割り込み処理内の保留音＿ＦＬＧにＯＦＦ設定 #*/

	if(dspmng_passctl[cnno].snd_sts==ON){	//送話パス接続中？
		mda_path_set(SNDPASS, cnno, ON);		/*# 送話パス復旧 #*/
	}
	//DSPクローズ判定
	//送話or受話パス接続中か？
	if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
	{
		dspmng_dspclose(TONEPBSND,cnno);
	}
	//D01の場合は保留音接続はパス上書き扱いなので受話側のLANパスを切断する。
	dspmng_h323cut(cnno,RCVPASS);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_dtmfabt                                              #*/
/*#                                                                         #*/
/*#     日本語名 : DTMF送信アボート処理                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DTMF送信アボート処理を行う。                             #*/
/*#     入    力 : INNER_MSG *msgp                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_dtmfabt(INNER_MSG *msgp)
{	
	DWORD	cnno; /*# ＣＮ番号 #*/
	BYTE   codec_change;
	
	// CN番号を抽出する
	cnno = msgp->msg_header.no;
	
	// 保留音送出中でなければ、リターンする
	if (dspmng_tonectl[cnno].send_sts != SNDPB) {
		return;
	}
	dbg_print(MDA_ID, LOG_DEBUG, "%s activated\n", __func__);
	/*# 送出状態を空きに設定 #*/
	dspmng_tonectl[cnno].send_sts = NOSND;
	/*# ＰＢタイマ状態クリア #*/
	dspmng_tonectl[cnno].pbtim_sts = CLR;
	/*# 送出ＰＢデータ数クリア #*/
	dspmng_tonectl[cnno].pb_num = CLR;
	/*# ＰＢ読み出し位置クリア #*/
	dspmng_tonectl[cnno].pb_cnt = CLR;
	
	dspmng_tonectl[cnno].pbread=CLR;
	
	dspmng_tonectl[cnno].pbwrite=CLR;
	
	dspmng_tonectl[cnno].pbcount=CLR;
	
	dspmng_tonectl[cnno].pb_acm_flg=CLR;
	
	dspmng_tonectl[cnno].oikosi=CLR;
	
//		//SS9100 2004.11.15 IPstage対応D01側も必要
//		dsp_mute(dsp_id,cnno,XC_OFF);
	
	//DSPクローズ判定
	//送話or受話パス接続中でなければDSPクローズしてリターン
	if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
	{
		dspmng_dspclose(TONEPBSND,cnno);
	}
	else
	{
		//ボコーダOFFで送受話パスどちらか接続中の場合はモードを変更
		if(dspmng_passctl[cnno].snd_sts!=DISCON)
		{
			codec_change=ON;
			mda_path_set(SNDPASS,cnno,codec_change);
		}
		if(dspmng_passctl[cnno].rcv_sts!=DISCON)
		{
			if(codec_change==ON)
			{
				codec_change=OFF;
			}
			else
			{
				codec_change=ON;
			}
			
			mda_path_set(RCVPASS,cnno,codec_change);
		}
	}
	dspmng_pbsndend( cnno ); /*# ＰＢ送信完了イベント送信処理 #*/
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_dsp_errstat                                       #*/
/*#                                                                         #*/
/*#     日本語名 : 障害情報通知要求受信処理                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 障害情報通知要求を受信時にフラグを操作し、               #*/
/*#                障害通知を送出する。                                     #*/
/*#     作    成 : 03/12/09 - S.A                                           #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : INNER_MSG *msg_ptr: 内部MSGへのポインタ					#*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/12/09 - S.A                                           #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_dsp_errstat(INNER_MSG	*msg_ptr)
{
	BYTE	i;
	
	for(i=0;i<DSP_MAX;i++)
	{
		//DSPエラーフラグON
		dsp_data[i].errstat=ON;

		/*圧縮LU対応。タイマ開始前に必ず全タイマを停止すること*/
		//DSPヘルス受信タイムアウトタイマ
		tmr_stop(dsp_data[i].helouttim_id);

		//DSP初期化エラーチェック
		//DSPがオンラインでなく、エラーが発生している場合は障害あげてDSP動作停止
		if(dsp_data[i].init_error!=0)
		{
			//DSP障害通知送出
			dspmng_make_dsperror(dsp_data[i].init_error,i,OFF,MDA_ID);
			
			//DSP初期化エラー値クリア
			dsp_data[i].init_error=CLR;
		}
		else
		{
			if(dsp_data[i].status!=XC_FAULT)
			{
				//DSPヘルス受信タイムアウトタイマ開始
				tmr_start(dsp_data[i].helouttim_id,18);
			}
		}
	}
}
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_make_dsperror	                                    #*/
/*#                                                                         #*/
/*#     日本語名 : DSP障害発生イベント送信                                  #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : CCUへDSP障害発生イベントを送信する                       #*/
/*#     入    力 : int	error_no チェック結果                               #*/
/*#                short dsp_id   DSP_ID                                    #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/07 - 薄井  正明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_make_dsperror(int error_no,short dsp_id,short irq_flg,BYTE tsk_id)
{
	INNER_MSG *msg_ptr;
	LUEVT_DSPERROR	*evtp;
	
	union{
		int	work_i;
		char work_c[4];
	}chg_worki;

	//障害情報通知オーダ受信済み？
	if(dsp_data[dsp_id].errstat!=ON)
	{
		return;
	}

	if(irq_flg==ON){
		//メッセージヘッダ取得
		msg_ptr = ( INNER_MSG *)com_ipoolget( POOL0 );		/*# メモリ確保 #*/
		if(msg_ptr == ( INNER_MSG *)0xffffffff)				/*# プール取得OK？ #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return ;
		}
	
		//イベント部取得
		evtp = ( LUEVT_DSPERROR *)com_ipoolget( POOL1 );
		if(evtp == ( LUEVT_DSPERROR *)0xffffffff){/*# メモリ確保OK?(V1) #*/
			com_ipoolput( POOL0 ,(BYTE *)msg_ptr );
			return;
		}
	}else{
		//メッセージヘッダ取得
		msg_ptr = ( INNER_MSG *)com_poolget( POOL0 );		/*# メモリ確保 #*/
		if(msg_ptr == ( INNER_MSG *)0xffffffff)				/*# プール取得OK？ #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return ;
		}
	
		//イベント部取得
		evtp = ( LUEVT_DSPERROR *)com_poolget( POOL1 );
		if(evtp == ( LUEVT_DSPERROR *)0xffffffff){/*# メモリ確保OK?(V1) #*/
			com_poolput( POOL0 ,(BYTE *)msg_ptr );
			return;
		}
	}
		//イベント組み立て

	evtp->lu_header.pkg_no = FROM_SLAVE;//2004.04 IP-CS追加
	evtp->bc 		= 0x07;								/*# B.C #*/
	evtp->code 	= E_DSP_ERROR_DETECT;						/*# メッセージ区分 #*/
	if(dsp_id==0x00)
		evtp->cnno 	= 0x00;
	else
		evtp->cnno 	= 0x04;
	
	chg_worki.work_i=error_no;
	evtp->result[3]=chg_worki.work_c[0];
	evtp->result[2]=chg_worki.work_c[1];
	evtp->result[1]=chg_worki.work_c[2];
	evtp->result[0]=chg_worki.work_c[3];
	
	msg_ptr->msg_header.id		= tsk_id;				/*# 送信元ID(LU管理)セット #*/
	msg_ptr->msg_header.div 	= LU_EVT;				/*# メッセージ区分セット #*/
	msg_ptr->msg_header.kind 	= 0;					/*# #*/
	msg_ptr->msg_header.link 	= (BYTE *)evtp;		/*# 2次リンクあり #*/

	//メッセージ送信
	com_sndmsg( DOEP_ECB, ( BYTE * )msg_ptr );		/*#	メッセージ送出 #*/
}
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_dspreinit       	                                #*/
/*#                                                                         #*/
/*#     日本語名 : DSP再初期化処理                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DSPの再初期化処理を行う                                  #*/
/*#     入    力 : INNER_MSG *msg_ptr 内部MSGへのポインタ                   #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/07 - 薄井  正明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_dspreinit(INNER_MSG	*msg_ptr)
{
	int 	result;
	short	dsp_id;
	regional_settings_params_t *regionalSettings = NULL;
	int		error_no;
	
	//エラー情報取り込み;
	dsp_id=msg_ptr->user_data.errorinfo.dsp_id;
	
	dsp_data[dsp_id].error_count++;

	/* DSP状態を更新する */
	if (dsp_data[dsp_id].status!=XC_FAULT)
	{
		dsp_data[0].status=XC_NORMAL;
	}
	
	/* DSPとの通信をクローズする */
	result = acgCloseDSP();
	/* DSPドライバをrmmodする */
	system("rmmod /lib/modules/3.10.49/ac49x_dsp.ko");
	/* コンフィグレーションファイルを初期化する */
    voip_conf_dsp_params_get(&regionalSettings, &AdvancedChannelConfiguration);
    /* DSPを再立ち上げする */
	result = dsp_init(regionalSettings, &AdvancedChannelConfiguration);
	if (result == ACG_SUCCESS)
	{
		//DSP障害復旧イベント送信
		dspmng_make_dsprepair(dsp_id,OFF);
				
		//エラーカウンタクリア
		dsp_data[dsp_id].error_count=CLR;
			
		//DSPヘルス受信タイムアウトタイマ開始
		tmr_start(dsp_data[dsp_id].helouttim_id,18);

		//DSP状態正常設定
		dsp_data[dsp_id].status=XC_NORMAL;
	}
	else {
		error_no=DSP_INIT_ERROR;

		//エラー発生回数インクリメント
		dsp_data[dsp_id].error_count++;
		
		error_no=(error_no<<16)&0xffff0000;
		
		//エラー発生回数が1回以上の場合は再初期化しない
		if(dsp_data[dsp_id].error_count<2)
		{
			dspmng_dsperror(error_no,dsp_id,OFF,MDA_ID);
		}
		else//障害通知だけ送信する
		{
			if(dsp_data[dsp_id].errstat!=ON)
			{
				dsp_data[dsp_id].init_error=error_no;
			}
			else
			{
				dspmng_make_dsperror(error_no,dsp_id,OFF,MDA_ID);
			}
		}
	}
	/* デバイスコンフィグレーションを取得する */
	if (acgGetDeviceAttr(&Device_Attr) != ACG_SUCCESS) {
		dbg_print(MDA_ID, LOG_ERR, "%s acgGetDeviceAttr abnormal end\n", __func__);
	}
}
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_dsperror                                          #*/
/*#                                                                         #*/
/*#     日本語名 : DSPエラー処理                                            #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DSPエラー発生時の処理を行う                              #*/
/*#     入    力 : int	error_no  チェック結果                              #*/
/*#                short dsp_id   DSP_ID                                    #*/
/*#                short irq_flg エラー発生箇所(ON:割込み内、OFF:それ以外)  #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/07 - 薄井  正明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_dsperror(int error_no,short dsp_id,short irq_flg, BYTE	tsk_id)
{
	INNER_MSG	*msg_ptr;
	
	//ヘルス受信タイムアウトタイマ停止
	tmr_stop(dsp_data[dsp_id].helouttim_id);

	dsp_data[dsp_id].status=XC_FAULT;

	dsp_data[dsp_id].error_count++;
	
	//パス解放
	dspmng_passfree(dsp_id,irq_flg,tsk_id);

	dspmng_make_dsperror(error_no,dsp_id,irq_flg,tsk_id);
	
	msg_ptr = ( INNER_MSG *)com_poolget( POOL0 );	/*#	LUメッセージ用プール取得 #*/

	if(msg_ptr == ( INNER_MSG *)0xffffffff){			/*# プール取得OK？ #*/
		return;
	}

	msg_ptr -> msg_header.id = tsk_id;				/*# 送信元IDセット #*/
	msg_ptr -> msg_header.div = IN_MSG;				/*# メッセージ区分セット #*/
	msg_ptr -> msg_header.kind = I_DSPERROR;		/*# メッセージ種別セット #*/
	msg_ptr -> msg_header.no = CLR;					/*# ＣＮ番号クリア #*/
	msg_ptr -> msg_header.link = (BYTE*)NULL;		/*# ２次リンククリア #*/

	msg_ptr -> user_data.errorinfo.dsp_id=dsp_id;
	msg_ptr -> user_data.errorinfo.error=(short)error_no;
	
	com_sndmsg( MDA_ECB, ( BYTE *)msg_ptr );	
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ordpass(V1)                                       #*/
/*#                                                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : パス制御オーダの処理を行う。                             #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : 03/09/01 -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_ordpass( INNER_MSG *msgp)
{
LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
DWORD	passctl;											/*# パス制御内容 #*/
DWORD	passdirt;											/*# パス方向 #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
															/*# パラメータが正常の場合 #*/
	if((ordp->ctl < PASS_CTLERR) && (ordp->pass < PASS_PASSERR)){
		passctl = (DWORD)ordp->ctl;							/*# 制御種別セーブ #*/
		passdirt = (DWORD)ordp->pass;						/*# パス方向セーブ #*/
															/*# 処理内容でＪＵＭＰ #*/
		(*dspmng_passtbl[passdirt][passctl])( msgp );
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ipaddrck(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＩＰアドレスチェック                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : オーダと内部のＩＰ／マルチアドレスが同一かチェックする   #*/
/*#     入    力 : LUORD_PASSCTLREQ *ordp : パス制御オーダ                  #*/
/*#                BYTE    cnno  : ＣＮ番号                                 #*/
/*#                BYTE    ckptn :  チェックアドレス識別子                  #*/
/*#                                    ＝１：送話先ＩＰアドレス             #*/
/*#                                    ＝２：受話元ＩＰアドレス             #*/
/*#                                    ＝３：送話先マルチキャストアドレス   #*/
/*#                                    ＝４：受話元マルチキャストアドレス   #*/
/*#     出    力 : BYTE    result : チェック結果                            #*/
/*#                                    ＝０  ：ＯＫ                         #*/
/*#                                    ＝ＦＦ：ＮＧ                         #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/25 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	dspmng_ipaddrck( LUORD_PASSCTLREQ *ordp , DWORD  cnno , DWORD  ckptn)
{

DWORD	result;												/*# IPアドレスチェック結果 #*/
DWORD	ipcnt;												/*# IPアドレスチェックカウンタ #*/
DWORD	cnt;												/*# カウンタ #*/

	result = OK;											/*# 判定結果にOK設定 #*/
															/*# IPタイプチェック #*/
	if(ordp->iptype == IPV4){								/*# IPがV4の場合 #*/
		ipcnt = IPV4NUM;									/*# IPV4アドレス数設定 #*/
	}
	else{													/*# IPがV6の場合 #*/
		ipcnt = IPV6NUM;									/*# IPV6アドレス数設定 #*/
	}
	switch(ckptn){											/*# 識別子チェック #*/
		case SNDIP :										/*# 送話ＩＰの場合 #*/
															/*# 新／旧 IPタイプチェック #*/
			if(dspmng_passctl[cnno].snd_iptype != (DWORD)ordp->iptype){
				result = NG;								/*# タイプが違う場合は、結果にNG設定 #*/
			}
			else{											/*# タイプが同じ場合は、アドレスチェック #*/
				for(cnt = 0; cnt < ipcnt; cnt++ ){
					if(dspmng_passctl[cnno].snd_ipaddr[cnt] != ordp->ipadd[cnt]){
						result = NG;						/*# IPアドレスが違う場合は、結果にNG設定 #*/
						break;
					}
				}
			}
			break;
		case RCVIP :											/*# 受話ＩＰの場合 #*/
														/*# 新／旧 IPタイプチェック #*/
			if(dspmng_passctl[cnno].rcv_iptype != (DWORD)ordp->iptype){
				result = NG;				/*# タイプが違う場合は、結果にNG設定 #*/
			}
			else{						/*# タイプが同じ場合は、アドレスチェック #*/
				for(cnt=0; cnt<ipcnt; cnt++ ){
					if(dspmng_passctl[cnno].rcv_ipaddr[cnt] != ordp->ipadd[cnt]){
						result = NG;	/*# IPアドレスが違う場合は、結果にNG設定 #*/
						break;
					}
				}
			}
			break;
		case SNDMULTIP :								/*# 送話マルチＩＰの場合 #*/
														/*# 新／旧 IPタイプチェック #*/
			if(dspmng_passctl[cnno].snd_iptype != (DWORD)ordp->iptype){
				result = NG;					/*# タイプが違う場合は、結果にNG設定 #*/
			}
			else{							/*# タイプが同じ場合は、アドレスチェック #*/
				for(cnt=0; cnt<ipcnt; cnt++ ){
					if(dspmng_passctl[cnno].snd_multaddr[cnt] != ordp->multadd[cnt]){
						result = NG;	/*# マルチアドレスが違う場合は、結果にNG設定 #*/
						break;
					}
				}
			}
			break;
		case RCVMULTIP :									/*# 受話マルチＩＰの場合 #*/
															/*# 新／旧 IPタイプチェック #*/
			if(dspmng_passctl[cnno].rcv_iptype != (DWORD)ordp->iptype){
				result = NG;					/*# タイプが違う場合は、結果にNG設定 #*/
			}
			else{							/*# タイプが同じ場合は、アドレスチェック #*/
				for(cnt=0; cnt<ipcnt; cnt++ ){
					if(dspmng_passctl[cnno].rcv_ipaddr[cnt] != ordp->ipadd[cnt]){
						result = NG;		/*# IPアドレスが違う場合は、結果にNG設定 #*/
						break;
					}
				}
				for(cnt=0; cnt<ipcnt; cnt++ ){
					if(dspmng_passctl[cnno].rcv_multaddr[cnt] != ordp->multadd[cnt]){
						result = NG;	/*# マルチアドレスが違う場合は、結果にNG設定 #*/
						break;
					}
				}
			}
			break;
		default :												/*# その他識別子の場合 #*/
			break;
	}
	return(result);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジューzル名 : dspmng_pass00(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（送話／接続）                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 送話／接続のパス制御を行う。                             #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass00( INNER_MSG *msgp )
{
	LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	
//	DWORD	cnt;												/*# カウンタ #*/
	DWORD	result;												/*# IPアドレスチェック結果 #*/
	DWORD	ckptn;												/*# IPアドレスチェック方向 #*/
	DWORD	cnno;												/*# ＣＮ番号 #*/
	DWORD	ctlptn;												/*# 制御内容 #*/
	WORD	myportck;											/*# 自ポートチェック用 #*/
	WORD	portck;												/*# 相手ポートチェック用 #*/
	WORD	sendtime;
	short	frame_time;
	BYTE	codec_change;
	union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		WORD	chg_w;
		BYTE	chg_b[2];
	}chg_work;
	
	union{														/*# CHAR->SHORT変換用 #*/
			SHORT	chg_s;
			CHAR	chg_c[2];
	
	}chg_work2;
	
	DWORD	cont;
															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;

	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/

	//コーデック判定
	if(dspmng_passctl[cnno].codec!=ordp->codec)
	{
		codec_change=ON;
	}
	else
	{
		codec_change=OFF;
		
	}
	
	dspmng_passctl[cnno].sendcount=CLR;		//送信周期カウンタクリア

	//タイムスタンプクリア
	com_data.rtp_time[cnno]=CLR;
	
	//RTPシーケンスクリア
	com_data.rtp_sndseq[cnno]=CLR;

	/*# 送話パス接続中の場合 #*/
	if(dspmng_passctl[cnno].snd_sts != DISCON)
	{
															/*# 通常パス指定の場合 #*/
		if(ordp->ipmult == MULT_OFF){
															/*# 送信先IPアドレスチェック指定 #*/
			ckptn = SNDIP;
		}
		else{
															/*# 送信先マルチアドレスチェック指定 #*/
			ckptn = SNDMULTIP;
		}
															/*# ＩＰ／マルチアドレスチェック #*/
		result = dspmng_ipaddrck( ordp , cnno , ckptn );
															/*# 送話自ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
		myportck = com_chgword ( chg_work.chg_w );
															/*# 送話相手ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		portck = com_chgword ( chg_work.chg_w );

		/*# 送話自／送信先ポート番号、送信先IP/マルチアドレスが違う場合 #*/
		if((dspmng_passctl[cnno].snd_myport != myportck)
			||(dspmng_passctl[cnno].snd_port != portck)||(result != OK)){
															/*# 音声通信ストップ(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)				/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
															/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = SNDDIR;
															/*# 送話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
															/*# 送話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == PASS ){
															/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{											/*# マルチ送話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
															
			h323(innp);		/*# H323制御起動 #*/
		}
		else	/*# 送話自／送信先ポート番号、送信先IPアドレスが同じ場合 #*/
		{
			chg_work2.chg_c[1]=ordp->pad[0];
			chg_work2.chg_c[0]=ordp->pad[1];
			dspmng_passctl[cnno].snd_pad = (LONG)chg_work2.chg_s;
			//送話ＰＡＤ値は他のエリアから読む
	/* 下記1行は既存では有効になっているが、send_padを設定している箇所がない */
//			dspmng_passctl[cnno].snd_pad=send_pad[cnno];

															/*# THRU変更設定 #*/
			dspmng_passctl[cnno].thru_mode = (DWORD)ordp->thru;
															/*# THRUデータ作成 #*/
			if(dspmng_passctl[cnno].thru_mode==THRU_ON)
			{
				dspmng_passctl[cnno].snd_pad = 0x00;
			}
			//CODEC取得
			dspmng_passctl[cnno].codec=(DWORD)ordp->codec;
			//送信データ周期チェック(最大及び最小)
			switch(dspmng_passctl[cnno].codec){
		
				//送信周期は５msを基本とする為CCUから指定された値を２倍する
				//dspmng_passctl[cnno].send_periodは５ｍｓ割り込みでつかうので単位を５ｍｓにする
				//ordp->send_periodはH323部でRTPパケット送信時のサイズ設定に使う
				case 2:		//G729
					if((ordp->send_period == 0) ||  (ordp->send_period > G729_SEND_PERIOD_MAX)){
						dspmng_passctl[cnno].send_period=G729_SEND_PERIOD_MAX*2;
					}
					else
					{
						dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period*2;
					}
					
					break;
		
				default:	//G711A/G711μ
					
					if((ordp->send_period == 0) ||  (ordp->send_period > SEND_PERIOD_MAX)){
																			/*# 送話データ送信最大周期設定 #*/
						dspmng_passctl[cnno].send_period=SEND_PERIOD_MAX;
					}
					else
					{													/*# 送話データ送信周期がＯＫの場合 #*/
																			/*# オーダの送話データ送信周期設定 #*/
						dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period;
						//G711で5ms単位指定があった場合。(奇数で送信周期指定されたとき)
						if(dspmng_passctl[cnno].send_period % 2)
						{
							dspmng_passctl[cnno].send_period++;		//送信周期修正(+1)				
						}
					}
			}
		
			frame_time=lu_codec[dspmng_passctl[cnno].codec].msec;
			
			//送信フレーム数取得
			sendtime=dspmng_passctl[cnno].send_period*5;
			dspmng_passctl[cnno].send_framecount=(sendtime/frame_time);
		
			ctlptn = SNDPASS;								/*# 制御内容に送話パスセット #*/
			mda_path_set(ctlptn ,cnno,codec_change);		/*# パス設定実行 #*/
			
			return;											/*# 処理終了 #*/
		}
	}
															/*# 空き／再接続時の送話パス接続処理 #*/
															/*# 送話パス接続クリア #*/
	dspmng_passctl[cnno].snd_sts = DISCON;
															/*# 送話自ポート番号設定 #*/
	chg_work.chg_b[0] = ordp->myport[0];
	chg_work.chg_b[1] = ordp->myport[1];
	dspmng_passctl[cnno].snd_myport = com_chgword ( chg_work.chg_w );
															/*# 送話相手ポート番号設定 #*/
	chg_work.chg_b[0] = ordp->port[0];
	chg_work.chg_b[1] = ordp->port[1];
	dspmng_passctl[cnno].snd_port = com_chgword ( chg_work.chg_w );
															/*# 送話IPタイプ設定 #*/
	dspmng_passctl[cnno].snd_iptype = (DWORD)ordp->iptype;

//圧縮LU対応。オーダのPAD値が符号ありに変更。2003.09.09
															/*# 送話PAD変更設定 #*/
	chg_work2.chg_c[1]=ordp->pad[0];
	chg_work2.chg_c[0]=ordp->pad[1];
	dspmng_passctl[cnno].snd_pad = (LONG)chg_work2.chg_s;
	////送話ＰＡＤ値は他のエリアから読む
	/* 下記1行は既存では有効になっているが、send_padを設定している箇所がない */
//	dspmng_passctl[cnno].snd_pad=send_pad[cnno];
															/*# THRU変更設定 #*/
	dspmng_passctl[cnno].thru_mode = (DWORD)ordp->thru;

//圧縮LU対応。スルーON時はPAD=0dbで動作する。2003.09.09。
	if(dspmng_passctl[cnno].thru_mode==THRU_ON)
	{
		dspmng_passctl[cnno].snd_pad = 0x00;
	}
	
	//CODEC取得
	dspmng_passctl[cnno].codec=(DWORD)ordp->codec;
	
	//送信データ周期チェック(最大及び最小)
	switch(dspmng_passctl[cnno].codec){

		//送信周期は５msを基本とする為CCUから指定された値を２倍する
		//dspmng_passctl[cnno].send_periodは５ｍｓ割り込みでつかうので単位を５ｍｓにする
		//ordp->send_periodはH323部でRTPパケット送信時のサイズ設定に使う
		case 2:		//G729
			if((ordp->send_period == 0) ||  (ordp->send_period > G729_SEND_PERIOD_MAX)){
				dspmng_passctl[cnno].send_period=G729_SEND_PERIOD_MAX*2;
			}
			else
			{
				dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period*2;
			}
			
			break;

		default:	//G711A/G711μ
			
			if((ordp->send_period == 0) ||  (ordp->send_period > SEND_PERIOD_MAX)){
																	/*# 送話データ送信最大周期設定 #*/
				dspmng_passctl[cnno].send_period=SEND_PERIOD_MAX;
			}
			else
			{													/*# 送話データ送信周期がＯＫの場合 #*/
																	/*# オーダの送話データ送信周期設定 #*/
				dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period;
				//G711で5ms単位指定があった場合。(奇数で送信周期指定されたとき)
				if(dspmng_passctl[cnno].send_period % 2)
				{
					dspmng_passctl[cnno].send_period++;		//送信周期修正(+1)				
					ordp->send_period =  dspmng_passctl[cnno].send_period;
				}
			}
	}

	frame_time=lu_codec[dspmng_passctl[cnno].codec].msec;
	
	//送信フレーム数取得
	sendtime=dspmng_passctl[cnno].send_period*5;
	dspmng_passctl[cnno].send_framecount=(sendtime/frame_time);
	
																/*# マルチキャスト指定でない場合 #*/
	if(ordp->ipmult == MULT_OFF){
															/*# 送話パス接続設定 #*/
		dspmng_passctl[cnno].snd_sts = PASS;
															/*# 送話先IPアドレス設定 #*/
		for(cont=0; cont<IPADDRMAX; cont++ ){
			dspmng_passctl[cnno].snd_ipaddr[cont] = ordp->ipadd[cont];
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cont=0; cont<IPADDRMAX; cont++ ){
			dspmng_passctl[cnno].snd_multaddr[cont] = CLR;
		}
	}
	else{													/*# マルチキャスト指定の場合 #*/
															/*# マルチ送話パス接続設定 #*/
		dspmng_passctl[cnno].snd_sts = MULTI;
															/*# 送話先IPアドレス設定 #*/
		//IPCS IPstage対応(start)変数名誤りcnt→cont 2004.8.31
		for(cont=0; cont<IPADDRMAX; cont++ ){
			dspmng_passctl[cnno].snd_ipaddr[cont] = CLR;
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cont=0; cont<IPADDRMAX; cont++ ){
			/* 下記1行は既存では有効になっているが、cntを設定している箇所がない=>このルートは通らない？ */
//			dspmng_passctl[cnno].snd_multaddr[cont] = ordp->multadd[cnt];
			dspmng_passctl[cnno].snd_multaddr[cont] = ordp->multadd[cont];
		}
		//IPCS IPstage対応(end) 2004.8.31
	}
															/*# 音声通信スタート(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
	innp = (INNER_MSG *)com_poolget(POOL0);
	if(innp == (INNER_MSG *)0xffffffff)						/*# POOL取得ＮＧ？ (V1) #*/
	{
		pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
		return;
	}
	innp->msg_header.id = MDA_ID;						/*# 送信元ID設定 #*/
	innp->msg_header.div = IN_MSG;							/*# メッセージ区分設定 #*/
	innp->msg_header.kind = I_VOSTART;						/*# メッセージID設定 #*/
	innp->msg_header.no = ordp->cnno;						/*# CN番号設定 #*/
	innp->msg_header.link = (BYTE *)ordp;					/*# リンクアドレスセット #*/
															/*# H323制御へメッセージ送信 #*/
	h323(innp);		/*# H323制御起動 #*/
	msgp->msg_header.link = (BYTE *)CLR;					/*# リンククリア #*/	// 99.12.14

	ctlptn = SNDPASS;								/*# 制御内容に送話パスセット #*/
	mda_path_set(ctlptn ,cnno,codec_change);		/*# パス設定実行 #*/
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass01(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（送話／切断）                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 送話／切断のパス制御を行う。                             #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass01( INNER_MSG *msgp )
{
	LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	
	DWORD	cnno;												/*# ＣＮ番号 #*/
	DWORD	ctlptn;												/*# 制御内容 #*/
	DWORD	cnt;												/*# カウンタ #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
															/*# 送話パスが接続中の場合 #*/

	if( dspmng_passctl[cnno].snd_sts != DISCON ){

															/*# 音声通信ストップ(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = SNDDIR;
															/*# 送話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
															/*# 送話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].snd_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ送話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 送話パス空き設定 #*/
		dspmng_passctl[cnno].snd_sts = DISCON;
															/*# 送話自ポート番号クリア #*/
		dspmng_passctl[cnno].snd_myport = CLR;
															/*# 送話相手ポート番号クリア #*/
		dspmng_passctl[cnno].snd_port = CLR;
															/*# 送話IPタイプクリア #*/
		dspmng_passctl[cnno].snd_iptype = CLR;
															/*# 送話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].snd_pad = CLR;
															/*# 送話IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
		}
															
		dspmng_passctl[cnno].send_period = CLR; 			/*# 送話データ送信周期クリア #*/

		if(dspmng_passctl[cnno].rcv_sts == DISCON){			/*# 受話パス未接続の場合 #*/

			dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
															
			dspmng_passctl[cnno].thru_mode = OFF;			/*# スルーモード：通常セット #*/
		}

		dspmng_passctl[cnno].send_framecount=CLR;

		ctlptn=SNDPASS;
		
		//パスカット待ちがないときはDPSのモードを変更する
		if(dspmng_passctl[cnno].sndcut_wait!=ON){
			dspmng_dspclose(ctlptn,cnno);
		}
	}
}
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass02(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（送話／確認切断）                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 送話／確認切断のパス制御を行う。                         #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass02( INNER_MSG *msgp )
{
	LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	
	DWORD	cnno;												/*# ＣＮ番号 #*/
	DWORD	ctlptn;												/*# 制御内容 #*/
	DWORD	cnt;												/*# カウンタ #*/
	DWORD	result;												/*# IPアドレスチェック結果 #*/
	DWORD	ckptn;												/*# IPアドレスチェック方向 #*/
	WORD	portck;												/*# 相手ポートチェック用 #*/
	union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		WORD	chg_w;
		BYTE	chg_b[2];
	}chg_work;

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
															/*# 送話パスが接続中の場合 #*/
															//or D01フラグがONの場合
	if( dspmng_passctl[cnno].snd_sts != DISCON ){
										/*# 通常パス指定の場合 #*/
		if(ordp->ipmult == MULT_OFF){
															/*# 送信先IPアドレスチェック指定 #*/
			ckptn = SNDIP;
		}
		else{
															/*# 送信先マルチアドレスチェック指定 #*/
			ckptn = SNDMULTIP;
		}
															/*# ＩＰ／マルチアドレスチェック #*/
		result = dspmng_ipaddrck( ordp , cnno , ckptn );
															/*# 送話自ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
															/*# 送話相手ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		portck = com_chgword ( chg_work.chg_w );

		/*# 送信先ポート番号、送信先IP/マルチアドレスが同じ場合 #*/
		//2004.1.6 条件文がorになっていたので修正
		if((dspmng_passctl[cnno].snd_port == portck)&&(result == OK)){

																/*# 音声通信ストップ(送話）メッセージ送信 #*/
																/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
																/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
																/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
																/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
																/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
																/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
																/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = SNDDIR;
																/*# 送話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
																/*# 送話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == PASS ){
																/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{												/*# マルチ送話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
																/*# H323制御へメッセージ送信 #*/
			h323(innp);		/*# H323制御起動 #*/
																/*# 送話パス空き設定 #*/
			dspmng_passctl[cnno].snd_sts = DISCON;
																/*# 送話自ポート番号クリア #*/
			dspmng_passctl[cnno].snd_myport = CLR;
																/*# 送話相手ポート番号クリア #*/
			dspmng_passctl[cnno].snd_port = CLR;
																/*# 送話IPタイプクリア #*/
			dspmng_passctl[cnno].snd_iptype = CLR;
																/*# 送話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].snd_pad = CLR;
																/*# 送話IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
			}
																/*# 送話先マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
			}
																
			dspmng_passctl[cnno].send_period = CLR; 			/*# 送話データ送信周期クリア #*/												
																/*# エコーキャンセル状態をＯＦＦ設定 #*/
			if(dspmng_passctl[cnno].rcv_sts == DISCON){			/*# 受話パス未接続の場合 #*/

				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
																
				dspmng_passctl[cnno].thru_mode = OFF;			/*# スルーモード：通常セット #*/
			}

			ctlptn=SNDPASS;
			
			// 受話パスがない場合は、パス切断する
			if( dspmng_passctl[cnno].rcv_sts == DISCON ) {
				dspmng_dspclose(ctlptn,cnno);
			}
		}
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass10(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（受話／接続）                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 受話／接続のパス制御を行う。                             #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass10( INNER_MSG *msgp )
{
LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/

DWORD	cnt;												/*# カウンタ #*/
DWORD	result;												/*# IPアドレスチェック結果 #*/
DWORD	ckptn;												/*# IPアドレスチェック方向 #*/
DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	ctlptn;												/*# 制御内容 #*/
WORD	myportck;											/*# 自ポートチェック用 #*/
WORD	portck;												/*# 相手ポートチェック用 #*/
BYTE	codec_change;
WORD	sendtime;
short	frame_time;
union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		WORD	chg_w;
		BYTE	chg_b[2];
}chg_work;
union{														/*# CHAR->SHORT変換用 #*/
//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
		SHORT	chg_s;
		CHAR	chg_c[2];
}chg_work2;

															/*# 2006/8/25 IPstage-FOMA対応 #*/
BYTE	send_period_set;									/*# 送信周期設定切り分け #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;

	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
	
	//RTPシーケンスクリア
	com_data.rtp_rcvseq[cnno]=CLR;

	//3リンク不具合対応 2004.3.31
	if(dspmng_passctl[cnno].codec!=ordp->codec)
	{
		codec_change=ON;
	}
	else
	{
		codec_change=OFF;
	}

	// D01系はこれまで通り（送信周期を処理させる）。
		send_period_set = ON;
															/*# 受話パスが接続中の場合 #*/
	if(dspmng_passctl[cnno].rcv_sts != DISCON){

		if(ordp->ipmult == MULT_OFF){
															/*# 受信元IPアドレスチェック指定 #*/
			ckptn = RCVIP;
		}
		else{
															/*# 受信元マルチアドレスチェック指定 #*/
			ckptn = RCVMULTIP;
		}
															/*# ＩＰ／マルチアドレスチェック #*/
		result = dspmng_ipaddrck( ordp , cnno , ckptn);
															/*# 受話自ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
		myportck = com_chgword ( chg_work.chg_w );
															/*# 受話相手ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		portck = com_chgword ( chg_work.chg_w );
															/*# 受話自／送信元ポート番号、送信元IPアドレスが違う場合 #*/
		if((dspmng_passctl[cnno].rcv_myport != myportck)
						||(dspmng_passctl[cnno].rcv_port != portck)
																||(result != OK)){
															/*# 音声通信ストップ(受話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)				/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
															/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = RCVDIR;
															/*# 受話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
															/*# 受話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].rcv_sts == PASS ){
															/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{											/*# マルチ受話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
															/*# H323制御へメッセージ送信 #*/
			h323(innp);		/*# H323制御起動 #*/
		}
		else
		{												/*# 受話自／送信元ポート番号、送信元IPアドレスが同じ場合 #*/

//圧縮LU対応。オーダのPAD値が符号ありに変更。

															/*# 受話PAD変更設定 #*/
			chg_work2.chg_c[1]=ordp->pad[0];
			chg_work2.chg_c[0]=ordp->pad[1];
			dspmng_passctl[cnno].rcv_pad = (LONG)chg_work2.chg_s;

															/*# ECHO変更設定 #*/
			dspmng_passctl[cnno].echo = (DWORD)ordp->echo;
															/*# THRU変更設定 #*/
			dspmng_passctl[cnno].thru_mode = (DWORD)ordp->thru;
//圧縮LU対応。スルーON時はPAD=0dbで動作する。2003.09.09。
			if(dspmng_passctl[cnno].thru_mode==THRU_ON)
			{
				dspmng_passctl[cnno].rcv_pad = 0x00;		
			}
			//CODEC取得
			dspmng_passctl[cnno].codec=(DWORD)ordp->codec;
			
			//受信バッファ制御設定
			//sendcount=(short)ordp->send_period;		//送信周期取得
			//CODEC取得
			dspmng_passctl[cnno].codec=(DWORD)ordp->codec;

			// 2006/8/25 IPstage-FOMA対応（FOMA以外でも指定の送信周期にならないフィールドクレームあり）
			if( send_period_set == ON ){
				//送信データ周期チェック(最大及び最小)
				switch(dspmng_passctl[cnno].codec){
			
					//送信周期は５msを基本とする為CCUから指定された値を２倍する
					//dspmng_passctl[cnno].send_periodは５ｍｓ割り込みでつかうので単位を５ｍｓにする
					//ordp->send_periodはH323部でRTPパケット送信時のサイズ設定に使う
					case 2:		//G729
						if((ordp->send_period == 0) ||  (ordp->send_period > G729_SEND_PERIOD_MAX)){
							dspmng_passctl[cnno].send_period=G729_SEND_PERIOD_MIN*2;
						}
						else
						{
							dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period*2;
						}
						
						break;
			
					default:	//G711A/G711μ
						
						if((ordp->send_period == 0) ||  (ordp->send_period > SEND_PERIOD_MAX)){
																				/*# 送話データ送信最小周期設定 #*/
							dspmng_passctl[cnno].send_period=SEND_PERIOD_MIN;
						}
						else
						{													/*# 送話データ送信周期がＯＫの場合 #*/
																				/*# オーダの送話データ送信周期設定 #*/
							dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period;
							//G711で5ms単位指定があった場合。(奇数で送信周期指定されたとき)
							if(dspmng_passctl[cnno].send_period % 2)
							{
								dspmng_passctl[cnno].send_period++;		//送信周期修正(+1)				
							}
						}
				}
			}
		
			frame_time=lu_codec[dspmng_passctl[cnno].codec].msec;
			
			//送信フレーム数取得
			sendtime=dspmng_passctl[cnno].send_period*5;
			dspmng_passctl[cnno].send_framecount=(sendtime/frame_time);			
			
			
			//上書き時、相手同じでFAX動作中は呼種別切り替え待ってからDSPのモード変更
			if(dspmng_passctl[cnno].fax_sts!=ON)
			{
				ctlptn = RCVPASS;								/*# 制御内容に受話パスセット #*/
				mda_path_set( ctlptn , cnno, codec_change);		/*# パス設定実行 #*/
			}
			return;											/*# 処理終了 #*/
		}
	}
															/*# 空き／再接続時の受話パス接続処理 #*/
															/*# 受話パス接続クリア #*/
	dspmng_passctl[cnno].rcv_sts = DISCON;
															/*# 受話自ポート番号設定 #*/
	chg_work.chg_b[0] = ordp->myport[0];
	chg_work.chg_b[1] = ordp->myport[1];
	dspmng_passctl[cnno].rcv_myport = com_chgword ( chg_work.chg_w );
															/*# 受話相手ポート番号設定 #*/
	chg_work.chg_b[0] = ordp->port[0];
	chg_work.chg_b[1] = ordp->port[1];
	dspmng_passctl[cnno].rcv_port = com_chgword ( chg_work.chg_w );
															/*# 受話IPタイプ設定 #*/
	dspmng_passctl[cnno].rcv_iptype = (DWORD)ordp->iptype;

//圧縮LU対応。オーダのPAD値が符号ありに変更。
															/*# 受話PAD変更設定 #*/
	chg_work2.chg_c[1]=ordp->pad[0];
	chg_work2.chg_c[0]=ordp->pad[1];
	dspmng_passctl[cnno].rcv_pad = (LONG)chg_work2.chg_s;

															/*# ECHO変更設定 #*/
	dspmng_passctl[cnno].echo = (DWORD)ordp->echo;
															/*# THRU変更設定 #*/
	dspmng_passctl[cnno].thru_mode = (DWORD)ordp->thru;

//圧縮LU対応。スルーON時はPAD=0dbで動作する。2003.09.09。
	if(dspmng_passctl[cnno].thru_mode==THRU_ON)
	{
		dspmng_passctl[cnno].rcv_pad = 0x00;		
	}
	
	//CODEC取得
	dspmng_passctl[cnno].codec=(DWORD)ordp->codec;
	
	//for debug 20040302
	//CODEC取得
	dspmng_passctl[cnno].codec=(DWORD)ordp->codec;

	// 2006/8/25 IPstage-FOMA対応（FOMA以外でも指定の送信周期にならないフィールドクレームあり）
	if( send_period_set == ON ){
		//送信データ周期チェック(最大及び最小)
		switch(dspmng_passctl[cnno].codec){

			//送信周期は５msを基本とする為CCUから指定された値を２倍する
			//dspmng_passctl[cnno].send_periodは５ｍｓ割り込みでつかうので単位を５ｍｓにする
			//ordp->send_periodはH323部でRTPパケット送信時のサイズ設定に使う
			case 2:		//G729
				if((ordp->send_period == 0) ||  (ordp->send_period > G729_SEND_PERIOD_MAX)){
					dspmng_passctl[cnno].send_period=G729_SEND_PERIOD_MIN*2;
				}
				else
				{
					dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period*2;
				}
				
				break;

			default:	//G711A/G711μ
				
				if((ordp->send_period == 0) ||  (ordp->send_period > SEND_PERIOD_MAX)){
																		/*# 送話データ送信最小周期設定 #*/
					dspmng_passctl[cnno].send_period=SEND_PERIOD_MIN;
				}
				else
				{													/*# 送話データ送信周期がＯＫの場合 #*/
																		/*# オーダの送話データ送信周期設定 #*/
					dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period;
					//G711で5ms単位指定があった場合。(奇数で送信周期指定されたとき)
					if(dspmng_passctl[cnno].send_period % 2)
					{
						dspmng_passctl[cnno].send_period++;		//送信周期修正(+1)
					}
				}
		}
	}

	frame_time=lu_codec[dspmng_passctl[cnno].codec].msec;
	
	//送信フレーム数取得
	sendtime=dspmng_passctl[cnno].send_period*5;
	dspmng_passctl[cnno].send_framecount=(sendtime/frame_time);

															/*# マルチキャスト指定でない場合 #*/
	if(ordp->ipmult == MULT_OFF){
															/*# 受話パス接続設定 #*/
		dspmng_passctl[cnno].rcv_sts = PASS;
															/*# 受話元IPアドレス設定 #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = ordp->ipadd[cnt];
		}
															/*# 受話元マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
		}
	}
	else{													/*# マルチキャスト指定の場合 #*/
															/*# マルチ受話パス接続設定 #*/
		dspmng_passctl[cnno].rcv_sts = MULTI;
															/*# 受話元IPアドレス設定 #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = ordp->ipadd[cnt];
		}
															/*# 受話元マルチアドレス設定 #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = ordp->multadd[cnt];
		}
	}
															/*# 音声通信スタート(受話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
	innp = (INNER_MSG *)com_poolget(POOL0);
	if(innp == (INNER_MSG *)0xffffffff)						/*# POOL取得ＮＧ？ (V1) #*/
	{
		pool_get_err[POOL0]++;								/*# プール取得ＮＧログ #*/
		return;
	}
	innp->msg_header.id = MDA_ID;						/*# 送信元ID設定 #*/
	innp->msg_header.div = IN_MSG;							/*# メッセージ区分設定 #*/
	innp->msg_header.kind = I_VOSTART;						/*# メッセージID設定 #*/
	innp->msg_header.no = ordp->cnno;						/*# CN番号設定 #*/
	innp->msg_header.link = (BYTE *)ordp;					/*# リンクアドレスセット #*/
															/*# H323制御へメッセージ送信 #*/
	h323(innp);		/*# H323制御起動 #*/
	msgp->msg_header.link = (BYTE *)CLR;					/*# リンククリア #*/	// 99.12.14

	//FAX動作中のDSPモード変更は行わない。呼種別切り替えを待つ
	if(dspmng_passctl[cnno].fax_sts!=ON)
	{
		ctlptn = RCVPASS;										/*# 制御内容に受話パスセット #*/
		mda_path_set( ctlptn , cnno, codec_change);		/*# パス設定実行 #*/
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass11(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（受話／切断）                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 受話／切断のパス制御を行う。                             #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass11( INNER_MSG *msgp )
{
LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	ctlptn;												/*# 制御内容 #*/
DWORD	cnt;												/*# カウンタ #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
															/*# 受話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].rcv_sts != DISCON ){

		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = RCVDIR;
															/*# 受話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
															/*# 受話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].rcv_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ受話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 受話パス空き設定 #*/
		dspmng_passctl[cnno].rcv_sts = DISCON;
															/*# 受話自ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_myport = CLR;
															/*# 受話相手ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_port = CLR;
															/*# 受話IPタイプクリア #*/
		dspmng_passctl[cnno].rcv_iptype = CLR;
															/*# 受話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].rcv_pad = CLR;
															/*# ECHO停止設定 #*/
		dspmng_passctl[cnno].echo = CLR;
															/*# 受話元IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
		}
															/*# 受話元マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
		}
															/*# 送話パス未接続の場合 #*/
		if(dspmng_passctl[cnno].snd_sts == DISCON){
															/*# スルーモード：通常セット #*/
			dspmng_passctl[cnno].thru_mode = OFF;
			dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
		}
		
		ctlptn=RCVPASS;
		// 送話パスがない場合は、パス切断する
		if( dspmng_passctl[cnno].snd_sts != DISCON ) {
			dspmng_dspclose(ctlptn,cnno);
		}
	}
	else
	{
		//パス切断受信時パスがなく、トーン送出中であればトーン停止
		if((dspmng_tonectl[cnno].send_sts==SNDTONE)
			||(dspmng_tonectl[cnno].send_sts==SNDPULS)){
			
			acgStopTone((int)cnno); /* トーン停止 */
			
			dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
			
			dspmng_tonectl[cnno].send_sts = NOSND;				/*# 送出中トーンステータスクリア #*/
			dspmng_tonectl[cnno].tone_id = CLR;					/*# 送出トーンＩDクリア #*/
			dspmng_tonectl[cnno].tonetim_idptn = CLR;			/*# 使用中トーンタイマID種別クリア #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id1);			/*# トーンタイマ＿１停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id2);			/*# トーンタイマ＿２停止処理 #*/
		}
		
		//パス切断受信時パスがなく保留音送出中であれば保留音停止。
		//逆パスはないはずなので即座にパス切断
		if(dspint_voicectl[cnno].hory_sts!= CLR)
		{	
			dspmng_tonectl[cnno].send_sts=NOSND;
			dspint_voicectl[cnno].hory_sts=CLR;
			
			acgPlaybackEnd((int)cnno); // 保留音停止
			ctlptn=RCVPASS;
			dspmng_dspclose(ctlptn,cnno); // パス切断
		}
	}
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass12(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（受話／確認切断）                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 受話／確認切断のパス制御を行う。                         #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass12( INNER_MSG *msgp )
{
	LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/

	DWORD	cnno;												/*# ＣＮ番号 #*/
	DWORD	ctlptn;												/*# 制御内容 #*/
	DWORD	cnt;												/*# カウンタ #*/
	DWORD	result;												/*# IPアドレスチェック結果 #*/
	DWORD	ckptn;												/*# IPアドレスチェック方向 #*/
	WORD	portck;												/*# 相手ポートチェック用 #*/
	union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		WORD	chg_w;
		BYTE	chg_b[2];
	}chg_work;

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
															/*# 受話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].rcv_sts != DISCON ){

		
		if(ordp->ipmult == MULT_OFF){
															/*# 受信元IPアドレスチェック指定 #*/
			ckptn = RCVIP;
		}
		else{
															/*# 受信元マルチアドレスチェック指定 #*/
			ckptn = RCVMULTIP;
		}
															/*# ＩＰ／マルチアドレスチェック #*/
		result = dspmng_ipaddrck( ordp , cnno , ckptn);
															/*# 受話自ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
															/*# 受話相手ポート番号読み込み #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		portck = com_chgword ( chg_work.chg_w );
															/*# 受話相手ポート番号、送信元IPアドレスが同じ場合 #*/
		if((dspmng_passctl[cnno].rcv_port == portck)&&(result == OK)){
			
																/*# 音声通信ストップ(受話）メッセージ送信 #*/
																/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
																/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
																/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
																/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
																/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
																/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
																/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = RCVDIR;
																/*# 受話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
																/*# 受話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].rcv_sts == PASS ){
																/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{												/*# マルチ受話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
																/*# H323制御へメッセージ送信 #*/
			h323(innp);		/*# H323制御起動 #*/
																/*# 受話パス空き設定 #*/
			dspmng_passctl[cnno].rcv_sts = DISCON;
																/*# 受話自ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_myport = CLR;
																/*# 受話相手ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_port = CLR;
																/*# 受話IPタイプクリア #*/
			dspmng_passctl[cnno].rcv_iptype = CLR;
																/*# 受話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].rcv_pad = CLR;
																/*# ECHO停止設定 #*/
			dspmng_passctl[cnno].echo = CLR;
																/*# 受話元IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
			}
																/*# 受話元マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
			}
			if(dspmng_passctl[cnno].snd_sts == DISCON){
																/*# スルーモード：通常セット #*/
				dspmng_passctl[cnno].thru_mode = OFF;
				
				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
			}
			

			ctlptn=RCVPASS;
			// 送話パスがない場合は、パス切断する
			if( dspmng_passctl[cnno].snd_sts != DISCON ) {
				dspmng_dspclose(ctlptn,cnno);
			}
		}
	}
	
	//パス切断受信時パスがなく、トーン送出中であればトーン停止
	if((dspmng_tonectl[cnno].send_sts==SNDTONE)
		||(dspmng_tonectl[cnno].send_sts==SNDPULS)){
		acgStopTone((int)cnno); /* トーン停止 */
		dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
		dspmng_tonectl[cnno].send_sts = NOSND;				/*# 送出中トーンステータスクリア #*/
		dspmng_tonectl[cnno].tone_id = CLR;					/*# 送出トーンＩDクリア #*/
		dspmng_tonectl[cnno].tonetim_idptn = CLR;			/*# 使用中トーンタイマID種別クリア #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id1);			/*# トーンタイマ＿１停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id2);			/*# トーンタイマ＿２停止処理 #*/
	}
		
	//パス切断受信時パスがなく保留音送出中であれば保留音停止。
	//逆パスはないはずなので即座にパス切断
	if(dspint_voicectl[cnno].hory_sts!= CLR)
	{	
		dspmng_tonectl[cnno].send_sts=NOSND;
		dspint_voicectl[cnno].hory_sts=CLR;
		//チャネルモード待機状態(PB監視及びトーン監視は待機後再開)
		acgPlaybackEnd((int)cnno); // 保留音停止
		ctlptn=RCVPASS;
		dspmng_dspclose(ctlptn,cnno); // パス切断
	}
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass20(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（双方向／接続）                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 双方向／接続のパス制御を行う。                           #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/24 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass20( INNER_MSG *msgp )
{
	LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	
	DWORD	cnt;												/*# カウンタ #*/
	DWORD	cnno;												/*# ＣＮ番号 #*/
	DWORD	ctlptn;												/*# 制御内容 #*/
	
	WORD	sendtime;
	
	union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
			WORD	chg_w;
			BYTE	chg_b[2];
	}chg_work;
	
	union{														/*# CHAR->SHORT変換用 #*/
			SHORT	chg_s;
			CHAR	chg_c[2];
	
	}chg_work2;
															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;

	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/

	if(ordp->ipmult == MULT_OFF){							/*# マルチキャスト指定でない場合 #*/
		
		
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  送話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 送話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].snd_sts != DISCON){

															/*# 音声通信ストップ(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)				/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
															/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = SNDDIR;
															/*# 送話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
															/*# 送話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == PASS ){
															/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{											/*# マルチ送話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
															/*# H323制御へメッセージ送信 #*/
			h323(innp);		/*# H323制御起動 #*/
		}
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  受話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 受話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].rcv_sts != DISCON){

															/*# 音声通信ストップ(受話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)				/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;						/*# プール取得ＮＧログ #*/
				return;
			}
															/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
			innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = RCVDIR;
															/*# 受話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
															/*# 受話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].rcv_sts == PASS ){
															/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{											/*# マルチ受話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
															/*# H323制御へメッセージ送信 #*/
			h323(innp);		/*# H323制御起動 #*/
		}

			/*#///////////////  2002.10.01 保留音鳴動保持（ソフトバグ？）対策  ここから  //////#*/
		else{													/* 受話パス未接続からの接続の場合 */

			if(dspmng_tonectl[cnno].send_sts == SNDHRY){		/*# 保留音鳴動中の場合 #*/
															/*# 空き状態設定 #*/
				dspmng_tonectl[cnno].send_sts = NOSND;
															/*# ＤＳＰ割り込み処理内の保留音＿ＦＬＧにＯＦＦ設定 #*/
				dspint_voicectl[cnno].hory_sts = OFF;
			}
		}
			/*#//////////////  2002.10.01 保留音鳴動保持（ソフトバグ？）対策  ここまで  //////#*/


	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  送受話パス設定処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 送話パス接続設定 #*/
		dspmng_passctl[cnno].snd_sts = PASS;
															/*# 受話パス接続設定 #*/
		dspmng_passctl[cnno].rcv_sts = PASS;
															/*# 送話自ポート番号設定 #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
		dspmng_passctl[cnno].snd_myport = com_chgword ( chg_work.chg_w );
															/*# 送話相手ポート番号設定 #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		dspmng_passctl[cnno].snd_port = com_chgword ( chg_work.chg_w );
															/*# 送話IPタイプ設定 #*/
		dspmng_passctl[cnno].snd_iptype = (DWORD)ordp->iptype;

//圧縮LU対応。オーダのPAD値が符号ありに変更。
															/*# 送話PAD変更設定 #*/
		chg_work2.chg_c[1]=ordp->pad[0];
		chg_work2.chg_c[0]=ordp->pad[1];
		dspmng_passctl[cnno].snd_pad = (LONG)chg_work2.chg_s;
		//CODEC取得
		dspmng_passctl[cnno].codec=(DWORD)ordp->codec;
																/*# 送話データ送信周期がＮＧの場合 #*/
		if((ordp->send_period == 0) ||  (ordp->send_period > SEND_PERIOD_MAX)){
																/*# 送話データ送信最小周期設定 #*/
	
			dspmng_passctl[cnno].send_period=SEND_PERIOD_MIN;
	//		dspint_voicectl[cnno].snd_num = SEND_PERIOD_MIN;
			ordp->send_period =  SEND_PERIOD_MIN;				/*# H.323でのガード不良救済 #*/	// 2000.08.31 変更
	
		}
		else{													/*# 送話データ送信周期がＯＫの場合 #*/
																/*# オーダの送話データ送信周期設定 #*/
			dspmng_passctl[cnno].send_period=(DWORD)ordp->send_period;
			//dspint_voicectl[cnno].snd_num = (DWORD)ordp->send_period;
		}
		
		if(dspmng_passctl[cnno].codec<3)	//送信周期を5ms単位に変換
											//G711以外は10ms単位で来るので5ms単位に直す
		{
			dspmng_passctl[cnno].send_period = dspmng_passctl[cnno].send_period*2;
		}
		else
		{
			//G711で5ms単位指定があった場合。(奇数で送信周期指定されたとき)
			if(dspmng_passctl[cnno].send_period % 2)
				dspmng_passctl[cnno].send_period++;		//送信周期修正(+1)
		}
		//送信フレーム数取得
		sendtime=dspmng_passctl[cnno].send_period*5;
		dspmng_passctl[cnno].send_framecount=sendtime/lu_codec[dspmng_passctl[cnno].codec].msec;

															/*# 送話先IPアドレス設定 #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_ipaddr[cnt] = ordp->ipadd[cnt];
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
		}
															/*# 受話自ポート番号設定 #*/
		chg_work.chg_b[0] = ordp->myport[0];
		chg_work.chg_b[1] = ordp->myport[1];
		dspmng_passctl[cnno].rcv_myport = com_chgword ( chg_work.chg_w );
															/*# 受話相手ポート番号設定 #*/
		chg_work.chg_b[0] = ordp->port[0];
		chg_work.chg_b[1] = ordp->port[1];
		dspmng_passctl[cnno].rcv_port = com_chgword ( chg_work.chg_w );
															/*# 受話IPタイプ設定 #*/
		dspmng_passctl[cnno].rcv_iptype = (DWORD)ordp->iptype;

//圧縮LU対応。オーダのPAD値が符号ありに変更。
															/*# 受話PAD変更設定 #*/
		chg_work2.chg_c[1]=ordp->pad[0];
		chg_work2.chg_c[0]=ordp->pad[1];
		dspmng_passctl[cnno].rcv_pad = (LONG)chg_work2.chg_s;

															/*# ECHO変更設定 #*/
		dspmng_passctl[cnno].echo = (DWORD)ordp->echo;
															/*# 受話元IPアドレス設定 #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = ordp->ipadd[cnt];
		}
															/*# 受話元マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
		}
															/*# THRU変更設定 #*/
		dspmng_passctl[cnno].thru_mode = (DWORD)ordp->thru;
//圧縮LU対応。スルーON時はPAD=0dbで動作する。2003.09.09。
		if(dspmng_passctl[cnno].thru_mode==THRU_ON)
		{
			dspmng_passctl[cnno].rcv_pad = 0x00;		
		}
															/*# 音声通信スタート(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
		innp->msg_header.id = MDA_ID;					/*# 送信元ID設定 #*/
		innp->msg_header.div = IN_MSG;						/*# メッセージ区分設定 #*/
		innp->msg_header.kind = I_VOSTART;					/*# メッセージID設定 #*/
		innp->msg_header.no = ordp->cnno;					/*# CN番号設定 #*/
		innp->msg_header.link = (BYTE *)ordp;				/*# リンクアドレスセット #*/
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
		msgp->msg_header.link = (BYTE *)CLR;				/*# リンククリア #*/	// 99.12.14

		ctlptn = BOTHPASS;									/*# 送受話パス制御指示設定 #*/
		mda_path_set(PASS_NEW_WRITE, ctlptn , cnno);		/*# パス設定実行 #*/
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass21                                            #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（双方向／切断）                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 双方向／切断のパス制御を行う。                           #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/01/26 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass21( INNER_MSG *msgp )
{
LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	ctlptn;												/*# 制御内容 #*/
DWORD	cnt;												/*# カウンタ #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  送話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 送話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].snd_sts != DISCON ){
															/*# 音声通信ストップ(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = SNDDIR;
															/*# 送話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
															/*# 送話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].snd_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ送話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 送話パス空き設定 #*/
		dspmng_passctl[cnno].snd_sts = DISCON;
															/*# 送話自ポート番号クリア #*/
		dspmng_passctl[cnno].snd_myport = CLR;
															/*# 送話相手ポート番号クリア #*/
		dspmng_passctl[cnno].snd_port = CLR;
															/*# 送話IPタイプクリア #*/
		dspmng_passctl[cnno].snd_iptype = CLR;
															/*# 送話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].snd_pad = CLR;
															/*# 送話IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
		}
															/*# DAVIS-LU-CT 99-5-31 #*/
															/*# 送話データ送信周期クリア #*/
		//dspint_voicectl[cnno].snd_num = CLR;
		dspmng_passctl[cnno].send_period = CLR; 															
	}
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  受話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 受話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].rcv_sts != DISCON ){

															/*# 音声通信ストップ(受話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = RCVDIR;
															/*# 受話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
															/*# 受話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].rcv_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ受話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 受話パス空き設定 #*/
		dspmng_passctl[cnno].rcv_sts = DISCON;
															/*# 受話自ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_myport = CLR;
															/*# 受話相手ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_port = CLR;
															/*# 受話IPタイプクリア #*/
		dspmng_passctl[cnno].rcv_iptype = CLR;
															/*# 受話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].rcv_pad = CLR;
															/*# ECHO停止設定 #*/
		dspmng_passctl[cnno].echo = CLR;
															/*# 受話元IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
		}
															/*# 受話元マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
		}
	}
															/*# エコーキャンセル状態をＯＦＦ設定 #*/
	dspmng_passctl[cnno].echo_sts = OFF;


//エコーキャンセラーは処理未定
//	dspmng_echoordset(getp->data+oakcnt,ECHO_OFF_ORD,cnno);		//2000.12.15 V2.2変更
//	oakcnt ++;												/*# オーダ数更新（＋１） #*/
															/*# スルーモード：通常セット #*/
//圧縮LU対応(end)。

//スルーモード時は処理特殊
	dspmng_passctl[cnno].thru_mode = OFF;

	ctlptn=BOTHPASS;
//DSPクローズ処理
	dspmng_dspclose(ctlptn,cnno);
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_pass30                                            #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パス制御（全切断）                     		#*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 全切断のパス制御を行う。                           		#*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/01/26 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_pass30( INNER_MSG *msgp )
{
LUORD_PASSCTLREQ *ordp;										/*# パス制御オーダポインタ #*/
INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	ctlptn;												/*# 制御内容 #*/
DWORD	cnt;												/*# カウンタ #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PASSCTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  送話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 送話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].snd_sts != DISCON ){
															/*# 音声通信ストップ(送話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = SNDDIR;
															/*# 送話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
															/*# 送話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].snd_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ送話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 送話パス空き設定 #*/
		dspmng_passctl[cnno].snd_sts = DISCON;
															/*# 送話自ポート番号クリア #*/
		dspmng_passctl[cnno].snd_myport = CLR;
															/*# 送話相手ポート番号クリア #*/
		dspmng_passctl[cnno].snd_port = CLR;
															/*# 送話IPタイプクリア #*/
		dspmng_passctl[cnno].snd_iptype = CLR;
															/*# 送話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].snd_pad = CLR;
															/*# 送話IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
		}
															/*# 送話先マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
		}
															/*# 送話データ送信周期クリア #*/
		dspmng_passctl[cnno].send_period = CLR; 															
	}
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊  受話パス制御処理  ＊＊＊＊＊＊＊ #*/
	/*# ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ #*/
															/*# 受話パスが接続中の場合 #*/
	if( dspmng_passctl[cnno].rcv_sts != DISCON ){
															/*# 音声通信ストップ(受話）メッセージ送信 #*/
															/*# 内部メッセージプール獲得 #*/
		innp = (INNER_MSG *)com_poolget(POOL0);
		if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return;
		}
															/*# 送信元ID設定 #*/
		innp->msg_header.id = MDA_ID;
															/*# メッセージ区分設定 #*/
		innp->msg_header.div = IN_MSG;
															/*# メッセージID設定 #*/
		innp->msg_header.kind = I_VOSTOP;
															/*# CN番号設定 #*/
		innp->msg_header.no = ordp->cnno;
															/*# リンクアドレスクリア #*/
		innp->msg_header.link = (BYTE *)CLR;
															/*# 通話パス方向設定 #*/
		innp->user_data.voice_ctl2.direct = RCVDIR;
															/*# 受話自ポート番号設定 #*/
		innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
															/*# 受話パス接続中の場合 #*/
		if(dspmng_passctl[cnno].rcv_sts == PASS ){
															/*# 通常パス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
		}
		else{												/*# マルチ受話パス接続中の場合 #*/
															/*# マルチパス指定設定 #*/
			innp->user_data.voice_ctl2.ip_mult = MULT_ON;
		}
															/*# H323制御へメッセージ送信 #*/
		h323(innp);		/*# H323制御起動 #*/
															/*# 受話パス空き設定 #*/
		dspmng_passctl[cnno].rcv_sts = DISCON;
															/*# 受話自ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_myport = CLR;
															/*# 受話相手ポート番号クリア #*/
		dspmng_passctl[cnno].rcv_port = CLR;
															/*# 受話IPタイプクリア #*/
		dspmng_passctl[cnno].rcv_iptype = CLR;
															/*# 受話ＰＡＤ値クリア #*/
		dspmng_passctl[cnno].rcv_pad = CLR;
															/*# ECHO停止設定 #*/
		dspmng_passctl[cnno].echo = CLR;
															/*# 受話元IPアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
		}
															/*# 受話元マルチアドレスクリア #*/
		for(cnt=0; cnt<IPADDRMAX; cnt++ ){
			dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
		}
	}
	
	dspmng_passctl[cnno].codec=0xff;
	
	dspmng_passctl[cnno].thru_mode = OFF;
	
	dspmng_passctl[cnno].sendcount=CLR;
	
	dspmng_passctl[cnno].send_framecount=CLR;

	//トーン送出中
	if((dspmng_tonectl[cnno].send_sts==SNDTONE)
		||(dspmng_tonectl[cnno].send_sts==SNDPULS)){
		acgStopTone((int)cnno); /* トーン停止 */
		dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
		dspmng_tonectl[cnno].send_sts = NOSND;				/*# 送出中トーンステータスクリア #*/
		dspmng_tonectl[cnno].tone_id = CLR;					/*# 送出トーンＩDクリア #*/
		dspmng_tonectl[cnno].tonetim_idptn = CLR;			/*# 使用中トーンタイマID種別クリア #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id1);			/*# トーンタイマ＿１停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id2);			/*# トーンタイマ＿２停止処理 #*/
	}
	//保留音切断
	if(dspint_voicectl[cnno].hory_sts!= CLR)
	{	
		acgPlaybackEnd((int)cnno); // 保留音停止
		dspmng_tonectl[cnno].send_sts=NOSND;
		dspint_voicectl[cnno].hory_sts=CLR;
	}

	//全切断時は定義BOTHPASS使用
	ctlptn=BOTHPASS;
	//DSPクローズ処理
	dspmng_dspclose(ctlptn,cnno);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ordtone                                           #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－トーン制御(V1)                               #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : トーン制御オーダの処理を行う。                           #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_ordtone(INNER_MSG *msgp)
{
LUORD_TONECTLREQ *ordp;										/*# トーン制御オーダポインタ #*/
BYTE	tonectl;											/*# トーン制御内容 #*/

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_TONECTLREQ *)msgp->msg_header.link;
															/*# 制御種別が正常の場合 #*/
	if(ordp->ctl < TONE_PARAERR){
		tonectl = ordp->ctl;								/*# 制御種別セーブ #*/
															/*# 処理内容でＪＵＭＰ #*/
		(*dspmng_tonetbl[tonectl])( msgp );
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_tonesnd(V1)                                       #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－トーン制御（トーン／パルス接続）             #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : トーン送出中のトーン制御を行う。                         #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/18 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_tonesnd( INNER_MSG *msgp )
{
LUORD_TONECTLREQ *ordp;										/*# トーン制御オーダポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
//DWORD	set_ord;											/*# ＯＡＫオーダ組立ワーク #*/
DWORD	set_pad;											/*# ＯＡＫオーダ設定ＰＡＤ値 #*/
DWORD	set_on1;											/*# ＯＡＫオーダ設定ＯＮ１時間値 #*/
DWORD	set_off1;											/*# ＯＡＫオーダ設定ＯＦＦ１時間値 #*/
DWORD	set_on2;											/*# ＯＡＫオーダ設定ＯＮ２時間値 #*/
DWORD	set_off2;											/*# ＯＡＫオーダ設定ＯＦＦ２時間値 #*/
DWORD	set_on3;											/*# ＯＡＫオーダ設定ＯＮ３時間値 #*/
DWORD	set_off3;											/*# ＯＡＫオーダ設定ＯＦＦ３時間値 #*/
DWORD	set_f1;												/*# ＯＡＫオーダ設定Ｆ１周波数値 #*/
DWORD	set_f2;												/*# ＯＡＫオーダ設定Ｆ２周波数値 #*/
DWORD	set_fam;											/*# ＯＡＫオーダ設定ＦＡＭ周波数値 #*/
DWORD	set_fk;												/*# ＯＡＫオーダ設定ＦＫ周波数値 #*/
DWORD	time_data=0;										/*# パルス１周期分時間値 #*/
//DWORD	ctlptn;												/*# 制御内容 #*/

BYTE	send_ptn;

union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		WORD	chg_w;
		BYTE	chg_b[2];
}chg_work;

union{														/*# CHAR->SHORT変換用 #*/
//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
		SHORT	chg_s;
		CHAR	chg_c[2];
}chg_work2;

short	amari=CLR;

Tac49xCallProgressSignalAttr *CallProgressSignalAttr_p;
acgTTone ToneSpec;
															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_TONECTLREQ *)msgp->msg_header.link;
															/*# パラメータチェック #*/
															/*# パラメータが有効の場合 #*/
	if((ordp->ptn < TONE_PARAERR) && (ordp->toneid < TONE_PARAERR)){
		cnno = (DWORD)ordp->cnno;							/*# CN番号セーブ #*/

		// 既に起動中の機能を停止する
		switch (dspmng_tonectl[cnno].send_sts) {
		case SNDTONE:
		case SNDPULS:
		case SNDPB:
			// トーン送出を停止する
			// トーン停止
			if (acgStopTone((int)cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
			}
			/* acgUpdateChannel()APIを起動し、PAD値(Gain)をもとに戻す */
			if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
			}
			break;
		case SNDHRY:
			// 保留音送出中は、停止する。
			// 保留音停止
			if (acgPlaybackEnd((int)cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackEnd abnormal end #1\n", __func__);
			}
			/* acgUpdateChannel()APIを起動し、PAD値(Gain)をもとに戻す */
			if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
			}
			break;
		default:
			break;
		}

//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
															/*# ＰＡＤ読み出し／セーブ #*/
		chg_work2.chg_c[1] = ordp->pad[0];
		chg_work2.chg_c[0] = ordp->pad[1];
		set_pad = (LONG)chg_work2.chg_s ;
			//出力ゲインが最小値より小さい場合は補正する
			if (set_pad < ACG_VOICE_GAIN__minus31_DB) {
				set_pad = ACG_VOICE_GAIN__minus31_DB;
			}
			//出力ゲインが最大値より大きい場合は補正する
			if (set_pad > ACG_VOICE_GAIN__plus31_DB) {
				set_pad = ACG_VOICE_GAIN__plus31_DB;
			}
		dspmng_tonectl[cnno].tone_pad = set_pad;
		
		/*# ＯＮ１時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->on1[0];
		chg_work.chg_b[1] = ordp->on1[1];
		set_on1 = (DWORD)com_chgword ( chg_work.chg_w );
		
		//オーダで10ms単位以下で来たときは繰り上げ
		amari=set_on1%10;
		if(amari)
			set_on1=set_on1+(10-amari);
		
		if(set_on1>10000)
			set_on1=10000;
		
		//連続送出時のダミー時間。"0"だと鳴動しない為。
		if((ordp->ctl==0)&&(ordp->ptn==0))
		{	
			set_on1=10;
		}

		dspmng_tonectl[cnno].on1_tim = set_on1;
		
		/*# ＯＦＦ１時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->off1[0];
		chg_work.chg_b[1] = ordp->off1[1];
		set_off1 = (DWORD)com_chgword ( chg_work.chg_w );

		amari=set_off1%10;
		if(amari)
			set_off1=set_off1+(10-amari);
		
		if(set_off1>10000)
			set_off1=10000;
		
		dspmng_tonectl[cnno].off1_tim = set_off1;
		
		/*# ＯＮ２時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->on2[0];
		chg_work.chg_b[1] = ordp->on2[1];
		set_on2 = (DWORD)com_chgword ( chg_work.chg_w );

		amari=set_on2%10;
		if(amari)
			set_on2=set_on2+(10-amari);

		if(set_on2>10000)
			set_on2=10000;

		dspmng_tonectl[cnno].on2_tim = set_on2;
		
		/*# ＯＦＦ２時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->off2[0];
		chg_work.chg_b[1] = ordp->off2[1];
		set_off2 = (DWORD)com_chgword ( chg_work.chg_w );

		amari=set_off2%10;
		if(amari)
			set_off2=set_off2+(10-amari);

		if(set_off2>10000)
			set_off2=10000;

		dspmng_tonectl[cnno].off2_tim = set_off2;
		
		/*# ＯＮ３時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->on3[0];
		chg_work.chg_b[1] = ordp->on3[1];
		set_on3 = (DWORD)com_chgword ( chg_work.chg_w );

		amari=set_on3%10;
		if(amari)
			set_on3=set_on3+(10-amari);

		if(set_on3>10000)
			set_on3=10000;

		dspmng_tonectl[cnno].on3_tim = set_on3;
		
		/*# ＯＦＦ３時間読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->off3[0];
		chg_work.chg_b[1] = ordp->off3[1];
		set_off3 = (DWORD)com_chgword ( chg_work.chg_w );
		amari=set_off3%10;
		if(amari)
			set_off3=set_off3+(10-amari);
		
		if(set_off3>10000)
			set_off3=10000;
			
		dspmng_tonectl[cnno].off3_tim = set_off3;

															/*# Ｆ１周波数読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->f1[0];
		chg_work.chg_b[1] = ordp->f1[1];
		set_f1 = (DWORD)com_chgword ( chg_work.chg_w );
		if (set_f1 < 300) {
			set_f1 = 300;
		}
		if (set_f1 > 1980) {
			set_f1 = 1980;
		}
		dspmng_tonectl[cnno].tone_f1 = set_f1;
															/*# Ｆ２周波数読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->f2[0];
		chg_work.chg_b[1] = ordp->f2[1];
		set_f2 = (DWORD)com_chgword ( chg_work.chg_w );
		if (set_f2 < 300) {
			set_f2 = 300;
		}
		if (set_f2 > 1980) {
			set_f2 = 1980;
		}
		dspmng_tonectl[cnno].tone_f2 = set_f2;
															/*# ＦＡＭ周波数読み出し／セーブ #*/
		chg_work.chg_b[0] = ordp->fam[0];
		chg_work.chg_b[1] = ordp->fam[1];
		set_fam = (DWORD)com_chgword ( chg_work.chg_w );
		if (set_fam < 300) {
			set_fam = 300;
		}
		if (set_fam > 1980) {
			set_fam = 1980;
		}

		//for debug
		if(ordp->toneid!=2)
		{
			set_fam=CLR;
		}
															/*# ＦＫ周波数読み出し／セーブ #*/
		dspmng_tonectl[cnno].tone_fam = set_fam;
		
		chg_work.chg_b[0] = ordp->fk[0];
		chg_work.chg_b[1] = ordp->fk[1];
		set_fk = (DWORD)com_chgword ( chg_work.chg_w );
		dspmng_tonectl[cnno].tone_fk = set_fk;
															/*# パルス時間値算出 #*/
		switch(ordp->ptn){
			case TONE_PTN3 :								/*# パターン＿３の場合 #*/
				time_data += set_on3;
				time_data += set_off3;
			case TONE_PTN2 :								/*# パターン＿２の場合 #*/
				time_data += set_on2;
				time_data += set_off2;
			case TONE_PTN1 :								/*# パターン＿１の場合 #*/
				time_data += set_off1;
			default :										/*# パターン＿０の場合 #*/
				time_data += set_on1;
																/*# 送出パターンセーブ #*/
				dspmng_tonectl[cnno].tone_ptn = (DWORD)ordp->ptn;
															/*# パルス時間セーブ #*/
				dspmng_tonectl[cnno].tone_tim = (DWORD)time_data/5;
			break;
		}

		if(dspmng_tonectl[cnno].crint_tone != NULL){											// 2000.01.18 追加
															/*# CRINT待ちメッセージ解放 #*/		// 2000.01.18 追加
			com_poolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );						// 2000.01.18 追加
		}
															/*# CRINT待ちクリア #*/
		dspmng_tonectl[cnno].crint_tone = (BYTE *)CLR;
															/*# ＰＢタイマ状態クリア #*/
		dspmng_tonectl[cnno].pbtim_sts = CLR;
															/*# 送出ＰＢデータ数クリア #*/
		dspmng_tonectl[cnno].pb_num = CLR;
															/*# ＰＢ読み出し位置クリア #*/
		dspmng_tonectl[cnno].pb_cnt = CLR;
		
		dspmng_tonectl[cnno].pbread=CLR;
		
		dspmng_tonectl[cnno].pbwrite=CLR;
		
		dspmng_tonectl[cnno].pbcount=CLR;
		
		dspmng_tonectl[cnno].pb_acm_flg=CLR;
		
		dspmng_tonectl[cnno].oikosi=CLR;
		
		//IPCS IPstage対応(start) 交番状態クリア追加 2004.8.31
		dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
		//IPCS IPstage対応(end) 2004.8.31
															/*# トーンタイマ＿１停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id1);
															/*# トーンタイマ＿２停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id2);
															/*# ＤＳＰ割り込み処理内の保留音＿ＦＬＧにＯＦＦ設定 #*/
		dspint_voicectl[cnno].hory_sts = OFF;
															/*# エコーキャンセル状態をＯＦＦ設定 #*/
		dspmng_passctl[cnno].echo_sts = OFF;
															/*# ＯＡＫオーダ組立 #*/

//		if(dspmng_passctl[cnno].echo!=NULL){
//			dspmng_echoordset(ECHO_OFF,cnno);		//2000.12.15 V2.2変更
//		}

		send_ptn=ordp->ptn;

		if(ordp->toneid!=3)
		{
			//交番以外の処理
			/* DSP CAllProgress属性を設定する */
			Device_Attr.CallProgress.NumberOfSignals = 32;
			dspmng_tonectl[cnno].tone_id = 16+cnno*4; /*# 送出トーンＩＤセーブ #*/
			CallProgressSignalAttr_p = (Tac49xCallProgressSignalAttr *)&Device_Attr.CallProgress.Signal[dspmng_tonectl[cnno].tone_id];
			memset((void *)CallProgressSignalAttr_p, 0, sizeof(Tac49xCallProgressSignalAttr));
			if ((ordp->ctl == TONE_SEND) && (send_ptn == 0)) {
				/* 制御種別接続 */
				CallProgressSignalAttr_p->Type = CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
			}
			else {
				/* 制御種別パルス接続 */
				CallProgressSignalAttr_p->Type = CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
			}
			CallProgressSignalAttr_p->ToneAFrequency = set_f1;
			if (ordp->toneid == 1) {
				CallProgressSignalAttr_p->ToneB_OrAmpModulationFrequency = (int)set_f2;
			}
			if (ordp->toneid == 2) {
				CallProgressSignalAttr_p->ToneB_OrAmpModulationFrequency = (int)set_fam;
				CallProgressSignalAttr_p->AmFactor = 50;
			}
			CallProgressSignalAttr_p->TwistThreshold = 10;
			CallProgressSignalAttr_p->HighEnergyThreshold = 0; /* dbm  */
			CallProgressSignalAttr_p->LowEnergyThreshold = 35; /* -dbm */
			CallProgressSignalAttr_p->SignalToNoiseRatioThreshold = 15;
			CallProgressSignalAttr_p->FrequencyDeviationThreshold = 10;
			CallProgressSignalAttr_p->ToneALevel = (int)set_pad + MDA_GAIN_RADIX;
			if (ordp->toneid != 0) {
				CallProgressSignalAttr_p->ToneBLevel = (int)set_pad + MDA_GAIN_RADIX;
			}
			if (send_ptn > 0) {
				CallProgressSignalAttr_p->CadenceFirstOffDuration = set_off1;
			}
			CallProgressSignalAttr_p->DetectionTimeOrCadenceFirstOnOrBurstDuration = set_on1;
			if (send_ptn > 1) {
				CallProgressSignalAttr_p->CadenceSecondOnDuration = set_on2;
				CallProgressSignalAttr_p->CadenceSecondOffDuration = set_off2;
			}
			if (send_ptn > 2) {
				CallProgressSignalAttr_p->CadenceThirdOnDuration = set_on3;
				CallProgressSignalAttr_p->CadenceThirdOffDuration = set_off3;
			}
			if(ordp->ctl == TONE_SEND){
																/*# トーン送出設定 #*/
				dspmng_tonectl[cnno].send_sts = SNDTONE;
			}
			else
			{												/*# ＣＣＵオーダ指示がパルス接続の場合 #*/
																/*# パルス送出設定 #*/
				dspmng_tonectl[cnno].send_sts = SNDPULS;
			}
			// CallProgressトーン送信要求を行う
			ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
			ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
			ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
			ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
			if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #1\n", __func__);
			}
		}
		else {
			//交番出力
			//周波数→時間変換
			time_data=1000/dspmng_tonectl[cnno].tone_fk;	//周波数から1単位時間取得
			time_data=(time_data/10)*10;		//1の位切り捨て
			/* DSP CAllProgress属性を設定する */
			Device_Attr.CallProgress.NumberOfSignals = 32;
			dspmng_tonectl[cnno].tone_id = 16+cnno*4+1; /*# 送出トーンＩＤセーブ #*/
			CallProgressSignalAttr_p = (Tac49xCallProgressSignalAttr *)&Device_Attr.CallProgress.Signal[dspmng_tonectl[cnno].tone_id];
			memset((void *)CallProgressSignalAttr_p, 0, sizeof(Tac49xCallProgressSignalAttr));
			/* 交番の最初の周波数設定 */
			CallProgressSignalAttr_p->Type = CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
			CallProgressSignalAttr_p->ToneAFrequency = set_f1;
			CallProgressSignalAttr_p->TwistThreshold = 10;
			CallProgressSignalAttr_p->HighEnergyThreshold = 0; /* dbm  */
			CallProgressSignalAttr_p->LowEnergyThreshold = 35; /* -dbm */
			CallProgressSignalAttr_p->SignalToNoiseRatioThreshold = 15;
			CallProgressSignalAttr_p->FrequencyDeviationThreshold = 10;
			CallProgressSignalAttr_p->ToneALevel = (int)set_pad + MDA_GAIN_RADIX;
			CallProgressSignalAttr_p->DetectionTimeOrCadenceFirstOnOrBurstDuration = (int)time_data;
			/* 交番の2番目の周波数設定 */
			++CallProgressSignalAttr_p;
			CallProgressSignalAttr_p->Type = CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
			CallProgressSignalAttr_p->ToneAFrequency = set_f2;
			CallProgressSignalAttr_p->TwistThreshold = 10;
			CallProgressSignalAttr_p->HighEnergyThreshold = 0; /* dbm  */
			CallProgressSignalAttr_p->LowEnergyThreshold = 35; /* -dbm */
			CallProgressSignalAttr_p->SignalToNoiseRatioThreshold = 15;
			CallProgressSignalAttr_p->FrequencyDeviationThreshold = 10;
			CallProgressSignalAttr_p->ToneALevel = (int)set_pad + MDA_GAIN_RADIX;
			CallProgressSignalAttr_p->DetectionTimeOrCadenceFirstOnOrBurstDuration = (int)time_data;
			// CallProgressトーン送信要求を行う
			ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
			ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
			ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
			ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
			if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #1\n", __func__);
			}
			dspmng_tonectl[cnno].send_sts = SNDTONE; 		/*# トーン送出設定 #*/
			dspmng_tonectl[cnno].fk_phase = TOME_PH_1; /* 交番トーンフェーズ */
			tmr_start(dspmng_tonectl[cnno].tonetim_id1 , (WORD)(time_data/5)); /* 交番フェーズ切替タイマ */
			dspmng_tonectl[cnno].fk_tim = (DWORD)time_data;/* 交番フェース切替タイミング */
			if(send_ptn>0){
				tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)dspmng_tonectl[cnno].on1_tim/5 ); /* 交番ON/OFF切替タイマ */
			}
			//交番状態セット
			dspmng_tonectl[cnno].fk_sts=FK_TONE_ON1;
		}
		//D01の場合はトーン接続はパス上書き扱いなので受話側のLANパスを切断する。
		dspmng_h323cut(cnno,RCVPASS);
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_tonecut(V1)                                       #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－トーン制御（トーン切断）                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : トーン切断のトーン制御を行う。                           #*/
/*#     作    成 : 99/06/22 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージ                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/22 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_tonecut( INNER_MSG *msgp )
{
LUORD_TONECTLREQ *ordp;										/*# トーン制御オーダポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
//SHORT	type;
SHORT	dsp_id;
															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_TONECTLREQ *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/
	if(dspmng_tonectl[cnno].crint_tone != NULL){											// 2000.01.18 追加
														/*# CRINT待ちメッセージ解放 #*/		// 2000.01.18 追加
		com_poolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );						// 2000.01.18 追加
	}
															/*# CRINT待ちクリア #*/
	dspmng_tonectl[cnno].crint_tone = (BYTE *)CLR;

	dsp_id=0;
															/*# トーン／パルス送出中の場合 #*/
	if((dspmng_tonectl[cnno].send_sts == SNDTONE) ||(dspmng_tonectl[cnno].send_sts == SNDPULS)){
															/*# 空き状態設定 #*/
		dspmng_tonectl[cnno].send_sts = NOSND;
															/*# 使用中トーンタイマID種別クリア #*/
		dspmng_tonectl[cnno].tonetim_idptn = CLR;
		
		dspmng_tonectl[cnno].tone_id = CLR;					/*# 送出トーンＩDクリア #*/

		dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
															/*# トーンタイマ＿１停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id1);
															/*# トーンタイマ＿２停止処理 #*/
		tmr_stop(dspmng_tonectl[cnno].tonetim_id2);
	
		// トーン停止
		if (acgStopTone((int)cnno) != ACG_SUCCESS) {
			dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
		}
		
		//DSPクローズ判定
		//送話or受話パス接続中か？
		if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
		{
			dspmng_dspclose(TONEPBSND,cnno);
		}
		
		//D01の場合はトーン接続はパス上書き扱いなので受話側のLANパスを切断する。
		dspmng_h323cut(cnno,RCVPASS);
	}
	else
	{
		//D01の場合はトーン接続はパス上書き扱いなので受話側のLANパスを切断する。
		dspmng_h323cut(cnno,RCVPASS);
		//保留音送出中の場合は保留音停止
		if(dspmng_tonectl[cnno].send_sts == SNDHRY)
		{	
			dspmng_tonectl[cnno].send_sts=NOSND;
			// 保留音停止
			if (acgPlaybackEnd((int)cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackEnd abnormal end #1\n", __func__);
			}
			/* acgUpdateChannel()APIを起動し、PAD値(Gain)をもとに戻す */
			if (acgUpdateChannel(cnno, &ChannelParameters[dsp_id][cnno]) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
			}
		}
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_tonesave                                          #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－トーン制御（同期接続）                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 同期接続のトーン制御を行う。（オーダセーブ）             #*/
/*#     入    力 : INNER_MSG *msgp : ＬＵメッセージポインタ                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     追    記 : D01モードでは動作しない                                  #*/
/*#     作    成 : 99/06/22 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_tonesave( INNER_MSG *msgp )
{
	dbg_print(MDA_ID, LOG_DEBUG, "%s avtivate\n", __func__);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ordhory(V1)                                       #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－保留音制御                                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 保留音制御オーダの処理を行う。                           #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_ordhory( INNER_MSG *msgp)
{
LUORD_HORYCTLREQ *ordp;										/*# 保留音制御オーダポインタ #*/
//INNER_MSG	*innp;

DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	set_pad;											/*# ＯＡＫオーダ設定ＰＡＤ値 #*/

union{														/*# CHAR->SHORT変換用 #*/
		SHORT	chg_s;
		CHAR	chg_c[2];

}chg_work2;

acgECoder coder;
														/*# リンクオーダ取り込み #*/
	ordp = (LUORD_HORYCTLREQ *)msgp->msg_header.link;
															/*# パラメータが正常の場合 #*/
	if(ordp->ctl < HORY_CTLERR){
		cnno = (DWORD)ordp->cnno;							/*# CN番号セーブ #*/
		if(ordp->ctl == 0){								/*# 制御：接続の場合 #*/
															/*# 使用中トーンタイマID種別クリア #*/
			dspmng_tonectl[cnno].tonetim_idptn = CLR;
			if(dspmng_tonectl[cnno].crint_tone != NULL){											// 2000.01.18 追加
																/*# CRINT待ちメッセージ解放 #*/		// 2000.01.18 追加
				com_poolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );						// 2000.01.18 追加
			}
			/* トーン送出中は、acgStopTone()APIを起動し、該当チャネルのトーン送出をストップする */
			if((dspmng_tonectl[cnno].send_sts == SNDTONE) || (dspmng_tonectl[cnno].send_sts == SNDPULS) ||
			   (dspmng_tonectl[cnno].send_sts == SNDPB)) {
				// トーン停止
				if (acgStopTone((int)cnno) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
				}
			}
			/* 保留音送出中は、acgPlaybackEnd()PAIを起動し、保留音送出を停止する */
			if(dspmng_tonectl[cnno].send_sts == SNDHRY) {
				// 保留音停止
				if (acgPlaybackEnd((int)cnno) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackEnd abnormal end #1\n", __func__);
				}
			}
															/*# CRINT待ちクリア #*/
			dspmng_tonectl[cnno].crint_tone = (BYTE *)CLR;
															/*# 使用中PBタイマID種別クリア #*/
			dspmng_tonectl[cnno].pbtim_idptn = CLR;
															/*# ＰＢタイマ状態クリア #*/
			dspmng_tonectl[cnno].pbtim_sts = CLR;
															/*# 送出ＰＢデータ数クリア #*/
			dspmng_tonectl[cnno].pb_num = CLR;
															/*# ＰＢ読み出し位置クリア #*/
			dspmng_tonectl[cnno].pb_cnt = CLR;

			dspmng_tonectl[cnno].pbread=CLR;
			
			dspmng_tonectl[cnno].pbwrite=CLR;
			
			dspmng_tonectl[cnno].pbcount=CLR;
			
			dspmng_tonectl[cnno].pb_acm_flg=CLR;
			
			dspmng_tonectl[cnno].oikosi=CLR;

			//IPCS IPstage対応(start)	交番状態クリア追加 2004.7.26
			dspmng_tonectl[cnno].fk_sts=CLR;
			//IPCS IPstage対応(end) 2004.7.26
															/*# トーンタイマ＿１停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id1);
															/*# トーンタイマ＿２停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id2);

			dspmng_tonectl[cnno].fk_tim=0; /* 交番フェース切替タイミング */
			dspmng_tonectl[cnno].fk_phase=CLR; /* 交番トーンフェーズ */
			//2001.01.17 V2.2追加（start）
//			if(ordp->ptn == HORYPTN_HOLD){
//				com_data.horyud_ptr[cnno] = (BYTE *)((DWORD)com_data.horyu_data_top & 0xfffffffc);  //2001.01.31 V2.2変更
//				com_data.horyu_rdmax[cnno]=(com_data.horyu_chunk_size/4)+1;      /*# 保留音ＭＡＸフレーム数設定 #*/
//			}
//			else{
//				com_data.horyud_ptr[cnno] = (BYTE *)((DWORD)com_data.talky_data_top & 0xfffffffc);  //2001.01.31 V2.2変更
//				com_data.horyu_rdmax[cnno]=(com_data.talky_chunk_size/4)+1;      /*# 保留音ＭＡＸフレーム数設定 #*/
//			}

//			com_data.horyu_rdcnt[cnno] = CLR;														//2001.01.31 V2.2追加

															/*# ＰＡＤ読み出し／セーブ #*/
			chg_work2.chg_c[1]=ordp->pad[0];
			chg_work2.chg_c[0]=ordp->pad[1];
			set_pad = chg_work2.chg_s;
			//出力ゲインが最小値より小さい場合は補正する
			if (set_pad < ACG_VOICE_GAIN__minus31_DB) {
				set_pad = ACG_VOICE_GAIN__minus31_DB;
			}
			//出力ゲインが最大値より大きい場合は補正する
			if (set_pad > ACG_VOICE_GAIN__plus31_DB) {
				set_pad = ACG_VOICE_GAIN__plus31_DB;
			}
			dspmng_h323cut(cnno,RCVPASS);	//IPstage,D01ともに上書き時は受話パス切断
			// ゲイン値を設定
			dspmng_tonectl[cnno].hory_pad = set_pad;
			ChannelParameters[XC_DSP_ID][cnno].GainParameters.VoiceOutputGain = (short)set_pad + MDA_GAIN_RADIX;
			//保留音用CODECに切り替え
			if(com_data.silent_ptn == VO_SLTPTN_U){		/*# ＣＯＤＥＣ：μ－ｌａｗ指定の場合 #*/
				
				dspmng_passctl[cnno].horyu_codec=XC_DSP_TYPE_G711_U;
				coder = ACG_CODER__G711MULAW;
			}
			else										/*# ＣＯＤＥＣ：Ａ－ｌａｗ指定の場合 #*/
			{	
				dspmng_passctl[cnno].horyu_codec=XC_DSP_TYPE_G711_A;
				coder = ACG_CODER__G711ALAW;
			}
			/* acgUpdateChannel()APIを起動し、PAD値(Gain)を変更する */
			if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
			}
			/* acgPlaybackStart()APIを起動し、保留音送出を開始する */
			if (acgPlaybackStart(cnno, 0 /* To TDM */, coder, "/flsdata/HOLD.wav") != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackStart abnormal end\n", __func__);
			}
			
			dspmng_tonectl[cnno].send_sts = SNDHRY;
		}
		else
		{												/*# 制御：切断の場合 #*/
			if(dspmng_tonectl[cnno].send_sts == SNDHRY){	//保留音送出時以外は無処理
				// 保留音停止
				if (acgPlaybackEnd((int)cnno) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackEnd abnormal end #1\n", __func__);
				}
				dspmng_tonectl[cnno].send_sts = NOSND;		/*# 空き状態設定 #*/
															
				dspint_voicectl[cnno].hory_sts = OFF;		/*# ＤＳＰ割り込み処理内の保留音＿ＦＬＧにＯＦＦ設定 #*/
				
				if(dspmng_passctl[cnno].snd_sts==ON){	//送話パス接続中？
					mda_path_set(SNDPASS, cnno, ON);		/*# 送話パス復旧 #*/
				}
				//DSPクローズ判定
				//送話or受話パス接続中か？
				if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
				{
					dspmng_dspclose(TONEPBSND,cnno);
				}
			}
			//D01の場合は保留音接続はパス上書き扱いなので受話側のLANパスを切断する。
			dspmng_h323cut(cnno,RCVPASS);
			//トーン切断
			if((dspmng_tonectl[cnno].send_sts==SNDTONE)
				||(dspmng_tonectl[cnno].send_sts==SNDPULS)){
				// トーン停止
				if (acgStopTone((int)cnno) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #2\n", __func__);
				}
				
				dspmng_tonectl[cnno].fk_sts=CLR;					/*# 交番出力状態クリア #*/
				
				dspmng_tonectl[cnno].send_sts = NOSND;				/*# 送出中トーンステータスクリア #*/
		
				dspmng_tonectl[cnno].tone_id = CLR;					/*# 送出トーンＩDクリア #*/
																	
				dspmng_tonectl[cnno].tonetim_idptn = CLR;			/*# 使用中トーンタイマID種別クリア #*/
																	
				tmr_stop(dspmng_tonectl[cnno].tonetim_id1);			/*# トーンタイマ＿１停止処理 #*/
																	
				tmr_stop(dspmng_tonectl[cnno].tonetim_id2);			/*# トーンタイマ＿２停止処理 #*/
				dspmng_tonectl[cnno].fk_tim=0; /* 交番フェース切替タイミング */
				dspmng_tonectl[cnno].fk_phase=CLR; /* 交番トーンフェーズ */
			}
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ordpbsnd(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－ＰＢ送信                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ＰＢ送信オーダの処理を行う。                             #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/25 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_ordpbsnd( INNER_MSG *msgp)
{
LUORD_PBSNDSTART *ordp;										/*# ＰＢ送信オーダポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
SHORT	set_pad_l;											/*# ＯＡＫオーダ設定ＰＡＤ値（低群） #*/
SHORT	set_pad_h;											/*# ＯＡＫオーダ設定ＰＡＤ値（高群） #*/
//DWORD	ctlptn;												/*# 制御内容 #*/
BYTE	pbdata;												/*# 送出ＰＢデータ判定 #*/
WORD	timedata;											/*# ポーズ時間値 #*/
union{														/*# ＢＹＴＥ－＞ＷＯＲＤ変換用 #*/
		SHORT	chg_s;
		CHAR	chg_c[2];
}chg_work2;

acgTTone ToneSpec;

															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PBSNDSTART *)msgp->msg_header.link;
															/*# パラメータチェック #*/
															/*# 送出ＰＢデータが有る場合 #*/
	if(ordp->num != 0){
		cnno = (DWORD)ordp->cnno;							/*# CN番号セーブ #*/

		/* トーン送出中は、acgStopTone()APIを起動し、該当チャネルのトーン送出をストップする */
		if((dspmng_tonectl[cnno].send_sts == SNDTONE) || (dspmng_tonectl[cnno].send_sts == SNDPULS)) {
			// トーン停止
			if (acgStopTone((int)cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
			}
		}
		/* 保留音送出中は、acgPlaybackEnd()PAIを起動し、保留音送出を停止する */
		if(dspmng_tonectl[cnno].send_sts == SNDHRY) {
			// 保留音停止
			if (acgPlaybackEnd((int)cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlaybackEnd abnormal end #1\n", __func__);
			}
		}

		/*# ＰＢ送出設定 #*/
		if(dspmng_tonectl[cnno].pb_acm_flg!=ON){
			dspmng_tonectl[cnno].send_sts = SNDPB;
																/*# 使用中トーンタイマID種別クリア #*/
			dspmng_tonectl[cnno].tonetim_idptn = CLR;
			if(dspmng_tonectl[cnno].crint_tone != NULL){											// 2000.01.18 追加
																/*# CRINT待ちメッセージ解放 #*/		// 2000.01.18 追加
				com_poolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );						// 2000.01.18 追加
			}
																/*# CRINT待ちクリア #*/
			dspmng_tonectl[cnno].crint_tone = (BYTE *)CLR;
																/*# トーンパターンクリア #*/
			dspmng_tonectl[cnno].tone_ptn = CLR;
																/*# ＰＢタイマ状態クリア #*/
			dspmng_tonectl[cnno].pbtim_sts = CLR;
		}														/*# 低群レベル読み出し／セーブ #*/
		chg_work2.chg_c[0] = ordp->pad_l[1];
		chg_work2.chg_c[1] = ordp->pad_l[0];
		set_pad_l = chg_work2.chg_s;
		//ゲインが最小値より小さい場合は補正する
		if (set_pad_l < ACG_VOICE_GAIN__minus31_DB) {
			set_pad_l = ACG_VOICE_GAIN__minus31_DB;
		}
		//ゲインが最大値より大きい場合は補正する
		if (set_pad_l > ACG_VOICE_GAIN__plus31_DB) {
			set_pad_l = ACG_VOICE_GAIN__plus31_DB;
		}
		dspmng_tonectl[cnno].pb_pad_l = set_pad_l;
															/*# 高群レベル読み出し／セーブ #*/
		chg_work2.chg_c[0] = ordp->pad_h[1];
		chg_work2.chg_c[1] = ordp->pad_h[0];
		set_pad_h = chg_work2.chg_s;
		//ゲインが最小値より小さい場合は補正する
		if (set_pad_h < ACG_VOICE_GAIN__minus31_DB) {
			set_pad_h = ACG_VOICE_GAIN__minus31_DB;
		}
		//ゲインが最大値より大きい場合は補正する
		if (set_pad_h > ACG_VOICE_GAIN__plus31_DB) {
			set_pad_h = ACG_VOICE_GAIN__plus31_DB;
		}
		dspmng_tonectl[cnno].pb_pad_h = set_pad_h;
		
			
		if(dspmng_tonectl[cnno].pbwrite==PBDATAMAX)
		{
			//ＰＢ書き込みＩＮＤＥＸ最初に戻る
			dspmng_tonectl[cnno].pbwrite=0;
			//追い越しフラグＯＮセット
			dspmng_tonectl[cnno].oikosi=ON;
			//バッファに指定ＰＢ書き込み
			dspmng_tonectl[cnno].pb_data[dspmng_tonectl[cnno].pbwrite++]=ordp->digit[0];
			//残送出ＰＢ数設定
			dspmng_tonectl[cnno].pbcount=(PBDATAMAX-dspmng_tonectl[cnno].pbread)+1;
		}
		else
		{
			//ＰＢ書き込みＩＮＤＥＸがＰＢ送出ＩＮＤＥＸと同じかそれより小さいときは
			if(dspmng_tonectl[cnno].pbwrite==dspmng_tonectl[cnno].pbread)
			{
				//追い越しフラグを参照し追い越してる場合はＰＢを書き込まない？
				if(dspmng_tonectl[cnno].oikosi!=ON){
					dspmng_tonectl[cnno].pb_data[dspmng_tonectl[cnno].pbwrite++]=ordp->digit[0];
				}
			}
			else
			{
				dspmng_tonectl[cnno].pb_data[dspmng_tonectl[cnno].pbwrite++]=ordp->digit[0];
			}
							//残送出ＰＢ数設定
			dspmng_tonectl[cnno].pbcount=dspmng_tonectl[cnno].pbwrite-dspmng_tonectl[cnno].pbread;
			//dspmng_tonectl[cnno].pbwrite++;
		}
															/*# 送出メーク時間設定 #*/
		dspmng_tonectl[cnno].pb_tim = (DWORD)(ordp->sndtim * T20MS);
															/*# 送出ブレイク時間設定 #*/
		dspmng_tonectl[cnno].pb_min = (DWORD)(ordp->sndmin * T20MS);
		//PB送信オーダ受信１回目のみここで送出。以降はオーダ受信で送出しない。
		//フラグが落ちるのは完了イベント送出タイミング
		if(dspmng_tonectl[cnno].pb_acm_flg!=ON)
		{
			//PB送出中フラグON
			dspmng_tonectl[cnno].pb_acm_flg=ON;
																/*# トーンタイマ＿１停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id1);
																/*# トーンタイマ＿２停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id2);
			dspmng_tonectl[cnno].fk_tim=0; /* 交番フェース切替タイミング */
			dspmng_tonectl[cnno].fk_phase=CLR; /* 交番トーンフェーズ */
																/*# ＤＳＰ割り込み処理内の保留音＿ＦＬＧにＯＦＦ設定 #*/
			dspint_voicectl[cnno].hory_sts = OFF;
																/*# 送出ＰＢデータ（第１データ）読み出し #*/
			pbdata = dspmng_tonectl[cnno].pb_data[0];
			
																/*# 第１データが数字の場合 #*/
			if((pbdata & PB_CHECK) == 0x00){
				pbdata = pbdata & PB_MASK;						/*# 数字データ抽出 #*/
																/*# 低群レベル読み出し #*/
				set_pad_l = dspmng_tonectl[cnno].pb_pad_l;
																/*# 高群レベル読み出し #*/
				set_pad_h = dspmng_tonectl[cnno].pb_pad_h;
																/*# ＯＡＫオーダ組立 #*/
				//本来の使い方ではないがイベント受信部で参照するため値を設定
				dspmng_tonectl[cnno].pbtim_sts = PBNUM_STS;		/*# ＰＢタイマ状態ポーズ設定 #*/
				
				// UserDefinedTones属性を変更する
				Device_Attr.UserDefinedTones.Signal[0].HighEnergyThreshold = (int)(-set_pad_h);
				Device_Attr.UserDefinedTones.Signal[0].LowEnergyThreshold = (int)(-set_pad_l);
				
				// DTMF信要求を行う
				ToneSpec.ToneElement.Type = ACG_TONE_TYPE__DTMF;
				ToneSpec.ToneElement.ToneIndex = pbdata;
				ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
				ToneSpec.OnDuration = (int)dspmng_tonectl[cnno].pb_tim;
				ToneSpec.OffDuration = (int)dspmng_tonectl[cnno].pb_min;
				if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #1\n", __func__);
				}
				//
				dspmng_tonectl[cnno].pbcount--;
			}
			else
			{
															/*# 第１データがポーズデータの場合 #*/
				pbdata = pbdata & POSE_MASK;				/*# ポーズデータ抽出 #*/
															/*# ポーズ時間算出 #*/
				timedata = (WORD)(pbdata * T100MS );
				
				dspmng_tonectl[cnno].pbtim_sts = POSE_STS;
				// DTMF信要求を行う
				ToneSpec.ToneElement.Type = ACG_TONE_TYPE__DTMF;
				ToneSpec.ToneElement.ToneIndex = (int)timedata | 0x80000000;
				ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
				ToneSpec.OnDuration = (int)dspmng_tonectl[cnno].pb_tim;
				ToneSpec.OffDuration = (int)dspmng_tonectl[cnno].pb_min;
				if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #2\n", __func__);
				}
			}
			dspmng_tonectl[cnno].pbread++;
		}
	}
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_ordpbcut(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－ＰＢ送信停止                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ＰＢ送信停止オーダの処理を行う。                         #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/25 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_ordpbcut( INNER_MSG *msgp)
{
LUORD_PBSNDSTOP *ordp;										/*# ＰＢ送信停止オーダポインタ #*/

DWORD	cnno;												/*# ＣＮ番号 #*/
BYTE	codec_change=CLR;
															/*# リンクオーダ取り込み #*/
	ordp = (LUORD_PBSNDSTOP *)msgp->msg_header.link;
	cnno = (DWORD)ordp->cnno;								/*# CN番号セーブ #*/

															/*# ＰＢ送出状態の場合 #*/
	if(dspmng_tonectl[cnno].send_sts == SNDPB){
															/*# 送出状態を空きに設定 #*/
		dspmng_tonectl[cnno].send_sts = NOSND;
															/*# ＰＢタイマ状態クリア #*/
		dspmng_tonectl[cnno].pbtim_sts = CLR;
															/*# 送出ＰＢデータ数クリア #*/
		dspmng_tonectl[cnno].pb_num = CLR;
															/*# ＰＢ読み出し位置クリア #*/
		dspmng_tonectl[cnno].pb_cnt = CLR;

		dspmng_tonectl[cnno].pbread=CLR;
		
		dspmng_tonectl[cnno].pbwrite=CLR;
		
		dspmng_tonectl[cnno].pbcount=CLR;
		
		dspmng_tonectl[cnno].pb_acm_flg=CLR;

		// トーン停止
		if (acgStopTone((int)cnno) != ACG_SUCCESS) {
			dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
		}

		//DSPクローズ判定
		//送話or受話パス接続中でなければDSPクローズしてリターン
		if((dspmng_passctl[cnno].rcv_sts==DISCON)&&(dspmng_passctl[cnno].snd_sts==DISCON))
		{
			dspmng_dspclose(TONEPBSND,cnno);
			return;
		}
		// 送受話パスどちらか接続中の場合はモードを変更
		if(dspmng_passctl[cnno].snd_sts!=DISCON)
		{
			codec_change=ON;
			mda_path_set(SNDPASS,cnno,codec_change);
		}
		if(dspmng_passctl[cnno].rcv_sts!=DISCON)
		{
			if(codec_change==ON)
			{
				codec_change=OFF;
			}
			else
			{
				codec_change=ON;
			}
			mda_path_set(RCVPASS,cnno,codec_change);
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_voicecut(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－音声通信切断通知                             #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 音声通信切断通知メッセージ（内部）の処理を行う。         #*/
/*#     入    力 : INNER_MSG*  msgp : 受信内部メッセージ                    #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/28 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_voicecut( INNER_MSG *msgp)
{
	DWORD	cnno;											/*# ＣＮ番号 #*/
	DWORD	cnt;											/*# カウンタ #*/
	DWORD	result=NG;										/*# 処理実行の有無 #*/

															/*# 理由がＵＤＰオープンエラーの場合 #*/
	if(msgp->user_data.voice_ctl3.result == H323_OPENNG){/* IPCS_V4 <PD><CHG> *//* 実際の値はあっているが、デファイン名が違う！！ */
		cnno = (DWORD)msgp->msg_header.no;					/*# CN番号読み出し #*/
															/*# 送話自ポート番号が同じ場合 #*/
		if(dspmng_passctl[cnno].snd_myport == msgp->user_data.voice_ctl3.port){
															/*# 送話パス空き設定 #*/
			dspmng_passctl[cnno].snd_sts = DISCON;
															/*# 送話自ポート番号クリア #*/
			dspmng_passctl[cnno].snd_myport = CLR;
															/*# 送話相手ポート番号クリア #*/
			dspmng_passctl[cnno].snd_port = CLR;
															/*# 送話IPタイプクリア #*/
			dspmng_passctl[cnno].snd_iptype = CLR;
															/*# 送話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].snd_pad = CLR;
															/*# 送話IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
			}
															/*# 送話先マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
			}
			
			dspmng_passctl[cnno].send_period = CLR; 			/*# 送話データ送信周期クリア #*/												

			if(dspmng_passctl[cnno].rcv_sts == DISCON){			/*# 受話パス未接続の場合 #*/

				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
																
				dspmng_passctl[cnno].thru_mode = OFF;			/*# スルーモード：通常セット #*/
			}

			result = OK;									/*# 処理実行に有り設定 #*/
		}
															/*# 受話自ポート番号が同じ場合 #*/
		if(dspmng_passctl[cnno].rcv_myport == msgp->user_data.voice_ctl3.port){
															/*# 受話パス空き設定 #*/
			dspmng_passctl[cnno].rcv_sts = DISCON;
															/*# 受話自ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_myport = CLR;
															/*# 受話相手ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_port = CLR;
															/*# 受話IPタイプクリア #*/
			dspmng_passctl[cnno].rcv_iptype = CLR;
															/*# 受話IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
			}
															/*# 受話先マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
			}
																/*# 送話パス未接続の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == DISCON){
																/*# スルーモード：通常セット #*/
				dspmng_passctl[cnno].thru_mode = OFF;
				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
			}
			result = OK;									/*# 処理実行に有り設定 #*/
		}

		if(result == OK)
		{													/*# 処理実行が有りの場合 #*/
															/*# 送受話パス未接続の場合 #*/
			if((dspmng_passctl[cnno].snd_sts == DISCON) && 
									(dspmng_passctl[cnno].rcv_sts == DISCON)){
															/*# スルーモード：通常セット #*/
				dspmng_passctl[cnno].thru_mode = OFF;
				// RTPストリーム処理を停止する
				if (acgDeactivateRTP(cnno) != ACG_SUCCESS) {
					dbg_print(MDA_ID, LOG_ERR, "%s acgDeactivateRTP abnormal end\n", __func__);
				}
			}
		}
	}
	/*# ＊＊＊ ＵＤＰクローズエラー、ＳＤＥＳ３０ｓタイムアウト、ＢＹＥ受信は無視 ＊＊＊ #*/
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_helchk_rcv                                        #*/
/*#                                                                         #*/
/*#     日本語名 : HPIエコーリプライ受信                                    #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : HPIエコーリプライ受信内容を判定する                      #*/
/*#     入    力 : INNER_MSG*  msgp :                                       #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/04 - S.A                                           #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_helchk_rcv(INNER_MSG *msg_ptr)
{
	short dsp_id;

	dsp_id=0x00;
	
	//for debug HPI_REPLYカウンター
	hpireply_count[dsp_id]++;

	//タイマスタート
	tmr_start(dsp_data[dsp_id].helouttim_id,18);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_orderr(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : 無効オーダ処理                                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 無効オーダは未処理とする。                               #*/
/*#     入    力 : INNER_MSG*  msgp :                                       #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/23 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dspmng_orderr( INNER_MSG *msgp)
{
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_passfree                                          #*/
/*#                                                                         #*/
/*#     日本語名 : DSPパス解放処理                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DSP障害により該当するDSPのパスを全解放する               #*/
/*#     入    力 : short   dsp_id: DSP_ID                                   #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/07 - 薄井  正明                                    #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_passfree(short dsp_id,short	irq_flg,BYTE tsk_id)
{
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	INNER_MSG *innp2;											/*# ＬＵ内部メッセージポインタ #*/

	DWORD	cnno;
	DWORD	cnno_start;												/*# ＣＮ番号 #*/
	DWORD	cnno_stop;
	DWORD	cnt;												/*# カウンタ #*/
	
	cnno_start=0;
	cnno_stop=3;
	
	//ヘルスタイマは状態に関わらず停止する。
	tmr_stop(dsp_data[dsp_id].helouttim_id);

	for(cnno=cnno_start;cnno<=cnno_stop;cnno++)
	{
		if( dspmng_passctl[cnno].snd_sts != DISCON ){				/*# 送話パスが接続中の場合 #*/
																/*# 音声通信ストップ(送話）メッセージ送信 #*/
			if(irq_flg==ON)
			{
				innp = (INNER_MSG *)com_ipoolget(POOL0);				/*# 内部メッセージプール獲得 #*/
			}else{
				innp = (INNER_MSG *)com_poolget(POOL0);				/*# 内部メッセージプール獲得 #*/
			}
			
			
			if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
			innp->msg_header.id = tsk_id;					/*# 送信元ID設定 #*/
			innp->msg_header.div = IN_MSG;						/*# メッセージ区分設定 #*/
			innp->msg_header.kind = I_VOSTOP;					/*# メッセージID設定 #*/
			innp->msg_header.no = cnno;					/*# CN番号設定 #*/
			innp->msg_header.link = (BYTE *)CLR;				/*# リンクアドレスクリア #*/
			innp->user_data.voice_ctl2.direct = SNDDIR;			/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;	/*# 送話自ポート番号設定 #*/
			if(dspmng_passctl[cnno].snd_sts == PASS ){			/*# 送話パス接続中の場合 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;	/*# 通常パス指定設定 #*/
			}
			else{												/*# マルチ送話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
			
			h323(innp);		/*# H323制御起動 #*/

																/*# 送話パス空き設定 #*/
			dspmng_passctl[cnno].snd_sts = DISCON;
																/*# 送話自ポート番号クリア #*/
			dspmng_passctl[cnno].snd_myport = CLR;
																/*# 送話相手ポート番号クリア #*/
			dspmng_passctl[cnno].snd_port = CLR;
																/*# 送話IPタイプクリア #*/
			dspmng_passctl[cnno].snd_iptype = CLR;
																/*# 送話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].snd_pad = CLR;
																/*# 送話IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
			}
																/*# 送話先マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
			}
			dspmng_passctl[cnno].send_period=CLR;
		}
																/*# 受話パスが接続中の場合 #*/
		if( dspmng_passctl[cnno].rcv_sts != DISCON ){
																/*# 音声通信ストップ(受話）メッセージ送信 #*/
			if(irq_flg==ON){										/*# 内部メッセージプール獲得 #*/
				innp2 = (INNER_MSG *)com_poolget(POOL0);
			}else{
				innp2 = (INNER_MSG *)com_ipoolget(POOL0);
			}
			
			if(innp2 == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
																/*# 送信元ID設定 #*/
			innp2->msg_header.id = tsk_id;
																/*# メッセージ区分設定 #*/
			innp2->msg_header.div = IN_MSG;
																/*# メッセージID設定 #*/
			innp2->msg_header.kind = I_VOSTOP;
																/*# CN番号設定 #*/
			innp2->msg_header.no = cnno;
																/*# リンクアドレスクリア #*/
			innp2->msg_header.link = (BYTE *)CLR;
																/*# 通話パス方向設定 #*/
			innp2->user_data.voice_ctl2.direct = RCVDIR;
																/*# 受話自ポート番号設定 #*/
			innp2->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
																/*# 受話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].rcv_sts == PASS ){
																/*# 通常パス指定設定 #*/
				innp2->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{												/*# マルチ受話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp2->user_data.voice_ctl2.ip_mult = MULT_ON;
			}
			
			h323(innp2);		/*# H323制御起動 #*/
																/*# 受話パス空き設定 #*/
			dspmng_passctl[cnno].rcv_sts = DISCON;
																/*# 受話自ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_myport = CLR;
																/*# 受話相手ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_port = CLR;
																/*# 受話IPタイプクリア #*/
			dspmng_passctl[cnno].rcv_iptype = CLR;
																/*# 受話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].rcv_pad = CLR;
																/*# 受話元IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
			}
																/*# 受話元マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
			}
		}
																/*# エコーキャンセル状態をＯＦＦ設定 #*/
		dspmng_passctl[cnno].echo_sts = OFF;
		
		dspmng_passctl[cnno].echo=CLR;
		
		//スルーモード時は処理特殊
		dspmng_passctl[cnno].thru_mode = OFF;

		//CODECデフォルトに
		dspmng_passctl[cnno].codec=0xff;
		
		//IPCS IPstage対応(start) CRINT待ち解除処理追加　2004.9.1
		if(dspmng_tonectl[cnno].crint_tone != NULL){
			if(irq_flg==ON){	//割り込み処理中？
				com_ipoolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );	/*# CRINT待ちメッセージ解放 #*/
			}
			else{
				com_poolput( POOL1 , (BYTE *)dspmng_tonectl[cnno].crint_tone );	/*# CRINT待ちメッセージ解放 #*/
			}
		}
		dspmng_tonectl[cnno].crint_tone = (BYTE *)CLR;		/*# CRINT待ちクリア #*/
		//IPCS IPstage対応(end) 2004.9.1

		//稼働タイマありでトーン送出状態である
		//全トーンタイマ停止
		if((dspmng_tonectl[cnno].tonetim_idptn >0 ) && (dspmng_tonectl[cnno].send_sts > 0)){
			
			dspmng_tonectl[cnno].send_sts = NOSND;
			
			dspmng_tonectl[cnno].fk_sts = CLR;
															/*# 使用中トーンタイマID種別クリア #*/
			dspmng_tonectl[cnno].tonetim_idptn = CLR;
															/*# トーンタイマ＿１停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id1);
															/*# トーンタイマ＿２停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id2);
		}
		
		//保留音送出状態である場合
		if(dspmng_tonectl[cnno].send_sts == SNDHRY )
		{
			dspint_voicectl[cnno].hory_sts=CLR;

			//保留音ステータスクリア
			dspmng_tonectl[cnno].send_sts = CLR;
		}
		
		if(dspmng_tonectl[cnno].send_sts == SNDPB){
															/*# 送出状態を空きに設定 #*/
			dspmng_tonectl[cnno].send_sts = NOSND;
																/*# 使用中PBタイマID種別クリア #*/
																/*# ＰＢタイマ状態クリア #*/
			dspmng_tonectl[cnno].pbtim_sts = CLR;
																/*# 送出ＰＢデータ数クリア #*/
			dspmng_tonectl[cnno].pb_num = CLR;
																/*# ＰＢ読み出し位置クリア #*/
			dspmng_tonectl[cnno].pb_cnt = CLR;
			
			dspmng_tonectl[cnno].pbread=CLR;
			
			dspmng_tonectl[cnno].pbwrite=CLR;
			
			dspmng_tonectl[cnno].pbcount=CLR;
			
			dspmng_tonectl[cnno].pb_acm_flg=CLR;
		}
		
	}
	//IPCS IPstage対応(end)　2004.9.1
	
	for(cnno=0;cnno<4;cnno++){
			//該当DSPチャネル初期化
		dsp_data[dsp_id].channel[cnno].mode=XC_DSP_MODE_VOICE; //BV初期値
		dsp_data[dsp_id].channel[cnno].type=XC_DSP_TYPE_VOCODER_OFF; //BV初期値
		//値未定
		dsp_data[dsp_id].channel[cnno].AGC_target_gain=XC_AGC_GAIN_DEFAULT;//DSPデフォルト
		dsp_data[dsp_id].channel[cnno].dtmf_detect_length=0x01;
		dsp_data[dsp_id].channel[cnno].faxtone_ced_length=500;
		dsp_data[dsp_id].channel[cnno].faxtone_pre_length=600;

		dsp_data[dsp_id].channel[cnno].calltone_detect_time=BIT09;//BV初期値
		dsp_data[dsp_id].channel[cnno].calltone_on_time=100;//かってに入れた
		dsp_data[dsp_id].channel[cnno].calltone_off_time=100;//かってに入れた
		dsp_data[dsp_id].channel[cnno].option=CLR;//BV初期値
		dsp_data[dsp_id].channel[cnno].rxbuf.max  =16000;//BV初期値
		dsp_data[dsp_id].channel[cnno].rxbuf.start=500;//BV初期値
		dsp_data[dsp_id].channel[cnno].rxbuf.stop =200;//BV初期値
		dsp_data[dsp_id].channel[cnno].dtmf_para.upper_level=-8;//BV初期値
		dsp_data[dsp_id].channel[cnno].dtmf_para.lower_level=-9;//BV初期値
		dsp_data[dsp_id].channel[cnno].dtmf_para.continuous_length=100;//かってに入れた
		dsp_data[dsp_id].channel[cnno].dtmf_para.space_length=100;//かってに入れた
	}
	
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_h323cut                                           #*/
/*#                                                                         #*/
/*#     日本語名 : H323パス切断通知                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : H323部へ音声切断MSGを通知する                            #*/
/*#                PID          xxxxxxx                                     #*/
/*#                Priorty      xxxxxxx                                     #*/
/*#                Queue Kind   FIFOQ                                       #*/
/*#     作    成 : 03/12/09 - S.A                                           #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : BYTE cnno : チャネル番号(0～7)                           #*/
/*#                BYTE ctlptn : 制御種別(0:送信,1:受信,2:双方向=未使用)    #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_h323cut( BYTE cnno, BYTE ctlptn )
{
	INNER_MSG *innp;											/*# ＬＵ内部メッセージポインタ #*/
	
	DWORD	cnt;												/*# カウンタ #*/

	if(ctlptn==SNDPASS){	//送話パス

		if( dspmng_passctl[cnno].snd_sts != DISCON ){
																/*# 音声通信ストップ(送話）メッセージ送信 #*/
																/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
																/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
																/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
																/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
																/*# CN番号設定 #*/
			innp->msg_header.no = cnno;
																/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
																/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = SNDDIR;
																/*# 送話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].snd_myport;
																/*# 送話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == PASS ){
																/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{												/*# マルチ送話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}

			h323(innp);		/*# H323制御起動 #*/
																/*# 送話パス空き設定 #*/
			dspmng_passctl[cnno].snd_sts = DISCON;
																/*# 送話自ポート番号クリア #*/
			dspmng_passctl[cnno].snd_myport = CLR;
																/*# 送話相手ポート番号クリア #*/
			dspmng_passctl[cnno].snd_port = CLR;
																/*# 送話IPタイプクリア #*/
			dspmng_passctl[cnno].snd_iptype = CLR;
																/*# 送話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].snd_pad = CLR;
																/*# 送話IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_ipaddr[cnt] = CLR;
			}
																/*# 送話先マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].snd_multaddr[cnt] = CLR;
			}
																
			dspmng_passctl[cnno].send_period = CLR; 			/*# 送話データ送信周期クリア #*/												
			
			if(dspmng_passctl[cnno].rcv_sts==DISCON)
			{
				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
			}
																
			if(dspmng_passctl[cnno].rcv_sts == DISCON){			/*# 受話パス未接続の場合 #*/
				dspmng_passctl[cnno].thru_mode = OFF;			/*# スルーモード：通常セット #*/
			}
		}
	}
	else if(ctlptn==RCVPASS)
	{
															/*# 受話パスが接続中の場合 #*/
		if( dspmng_passctl[cnno].rcv_sts != DISCON ){	
																/*# 音声通信ストップ(受話）メッセージ送信 #*/
																/*# 内部メッセージプール獲得 #*/
			innp = (INNER_MSG *)com_poolget(POOL0);
			if(innp == (INNER_MSG *)0xffffffff)					/*# POOL取得ＮＧ？ (V1) #*/
			{
				pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
				return;
			}
																/*# 送信元ID設定 #*/
			innp->msg_header.id = MDA_ID;
																/*# メッセージ区分設定 #*/
			innp->msg_header.div = IN_MSG;
																/*# メッセージID設定 #*/
			innp->msg_header.kind = I_VOSTOP;
																/*# CN番号設定 #*/
			innp->msg_header.no = cnno;
																/*# リンクアドレスクリア #*/
			innp->msg_header.link = (BYTE *)CLR;
																/*# 通話パス方向設定 #*/
			innp->user_data.voice_ctl2.direct = RCVDIR;
																/*# 受話自ポート番号設定 #*/
			innp->user_data.voice_ctl2.port = dspmng_passctl[cnno].rcv_myport;
																/*# 受話パス接続中の場合 #*/
			if(dspmng_passctl[cnno].rcv_sts == PASS ){
																/*# 通常パス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_OFF;
			}
			else{												/*# マルチ受話パス接続中の場合 #*/
																/*# マルチパス指定設定 #*/
				innp->user_data.voice_ctl2.ip_mult = MULT_ON;
			}

			h323(innp);		/*# H323制御起動 #*/
																/*# 受話パス空き設定 #*/
			dspmng_passctl[cnno].rcv_sts = DISCON;
																/*# 受話自ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_myport = CLR;
																/*# 受話相手ポート番号クリア #*/
			dspmng_passctl[cnno].rcv_port = CLR;
																/*# 受話IPタイプクリア #*/
			dspmng_passctl[cnno].rcv_iptype = CLR;
																/*# 受話ＰＡＤ値クリア #*/
			dspmng_passctl[cnno].rcv_pad = CLR;
																/*# ECHO停止設定 #*/
			dspmng_passctl[cnno].echo = CLR;
																/*# 受話元IPアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_ipaddr[cnt] = CLR;
			}
																/*# 受話元マルチアドレスクリア #*/
			for(cnt=0; cnt<IPADDRMAX; cnt++ ){
				dspmng_passctl[cnno].rcv_multaddr[cnt] = CLR;
			}
			
			if(dspmng_passctl[cnno].snd_sts==DISCON)
			{
				dspmng_passctl[cnno].codec = 0xff;		//CCUからオーダで"0"がくるので仕方なくFFにする
			}
																/*# 送話パス未接続の場合 #*/
			if(dspmng_passctl[cnno].snd_sts == DISCON){
																/*# スルーモード：通常セット #*/
				dspmng_passctl[cnno].thru_mode = OFF;
	
			}
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_dspclose                                          #*/
/*#                                                                         #*/
/*#     日本語名 : DSPチャネルクローズ                                      #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : パス切断時にエリアを開放し、DSPの指定チャネルを          #*/
/*#                音声/ボコーダOFFに設定する。                             #*/
/*#     作    成 : 03/09/09 - 薄井  正明                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : BYTE	ctlptn   : 制御種別                                 #*/
/*#                DWORD   cnno  : ＣＮ番号                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/09 - 薄井  正明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_dspclose(BYTE	ctlptn,BYTE	cnno)
{
	short	dsp_id;
	short	dsp_channel;
	short	pass_cut;

	dsp_id = 0x00;
	dsp_channel = (short)cnno;
	if(dspmng_tonectl[cnno].send_sts!=SNDPB){

		pass_cut=OFF;		//パスカットフラグ初期化

		switch(ctlptn){
		
			case SNDPASS:		//制御が送話パス
				//受話パスが未接続な場合。
				if(dspmng_passctl[cnno].rcv_sts==DISCON){
					//LU 1コールバグ。2004.1.8 送話パス切断時にトーン/保留御の状態を判定していなかった。
					if((dspmng_tonectl[cnno].send_sts != SNDTONE)&&
						(dspmng_tonectl[cnno].send_sts != SNDPULS)&&
							(dspmng_tonectl[cnno].send_sts != SNDHRY)){
						pass_cut=ON;
					}
				}
				break;
			
			case RCVPASS:		//制御が受話パス
				//送話パスが未接続な場合
				if(dspmng_passctl[cnno].snd_sts==DISCON){
					pass_cut=ON;
				}
				break;

			default:			//保留音からの場合(保留音は切断→戻しの作業があるので完全な切断以外はここにこない)
				pass_cut=ON;
				break;
		}

		if(pass_cut==ON)
		{//パスカットフラグON時のみ
			//DSPチャネル情報初期化
			//1フレームサイズ初期化
			dsp_data[dsp_id].channel[dsp_channel].frame1_size=CLR;
			
			//DTMF検出設定判定
			//BIT立っていなければクリアする
			if((dsp_data[dsp_id].channel[dsp_channel].option & BIT02)==CLR){
				dsp_data[dsp_id].channel[dsp_channel].dtmf_detect_length=CLR;
			}
			
//			//呼制御トーン検出設定判定
//			//BIT立っていなければクリアする
//			if(dsp_data[dsp_id].channel[dsp_channel].option&BIT08==CLR){
//				dsp_data[dsp_id].channel[dsp_channel].calltone_band=0x01;
//				dsp_data[dsp_id].channel[dsp_channel].calltone_on_time=CLR;
//				dsp_data[dsp_id].channel[dsp_channel].calltone_off_time=CLR;
//				dsp_data[dsp_id].channel[dsp_channel].calltone_detect_time=500;//DSPデフォルト
//			}
			
			//PAD設定クリア
			dsp_data[dsp_id].channel[dsp_channel].PAD_Tx=CLR;
			dsp_data[dsp_id].channel[dsp_channel].PAD_Rx=CLR;
			
			//送出中トーン状態クリア
			dsp_data[dsp_id].channel[dsp_channel].xmit_DTMF=CLR;
			dsp_data[dsp_id].channel[dsp_channel].xmit_tone=CLR;
			dsp_data[dsp_id].channel[dsp_channel].xmit_status=CLR;
			
			//受信バッファ回復制御OFF
			dsp_data[dsp_id].channel[dsp_channel].option&=~BIT09;
			dsp_data[dsp_id].channel[dsp_channel].rxbuf.max  =500;//BV初期値
			dsp_data[dsp_id].channel[dsp_channel].rxbuf.start=DEFAULT_VOICE_JITTER_BUFFER_MAX_DELAY;
			dsp_data[dsp_id].channel[dsp_channel].rxbuf.stop =DEFAULT_VOICE_JITTER_BUFFER_MIN_DELAY;
			
			//DTMF送出パラメータ初期化
			dsp_data[dsp_id].channel[dsp_channel].dtmf_para.upper_level=-8;//BV初期値
			dsp_data[dsp_id].channel[dsp_channel].dtmf_para.lower_level=-9;//BV初期値
			dsp_data[dsp_id].channel[dsp_channel].dtmf_para.continuous_length=100;//かってに入れた
			dsp_data[dsp_id].channel[dsp_channel].dtmf_para.space_length=100;//かってに入れた
			
			//AGC初期化
			dsp_data[dsp_id].channel[dsp_channel].AGC_target_gain=XC_AGC_GAIN_DEFAULT;//DSPデフォルト
			
			// RTPストリーム処理を停止する
			if (acgDeactivateRTP(cnno) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgDeactivateRTP abnormal end\n", __func__);
			}
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_timout(V1)                                        #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－タイムアウト処理                             #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ＯＳタイマのタイムアウト処理を行う。                     #*/
/*#     入    力 : INNER_MSG*  msgp : 受信メッセージ                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/28 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_timout( INNER_MSG *msgp)
{

DWORD	time_idck;											/*# タイマＩＤチェックデータ #*/
DWORD	cnno;												/*# ＣＮ番号 #*/
DWORD	cnt;												/*# カウンタ #*/
DWORD	time_id;											/*# タイマＩＤ #*/
DWORD	time_kind;											/*# タイマ種別 #*/
DWORD	result = NG;										/*# 判定結果 #*/

	time_idck = (DWORD)msgp->msg_header.kind;				/*# ID取り込み #*/
	if(time_idck == (DWORD)dsp_data[XC_DSP_ID].helouttim_id){
		time_kind = DSPHEL_TIME;								/*# タイマ種別にDSPヘルスチェックタイマ設定 #*/
		time_id = ID_1;											/*# タイマIＤ＿１設定 #*/
		cnno = CLR;												/*# ＣＮ番号（ダミー）設定 #*/
		dspmng_helchk_timout( time_id , time_kind , cnno );
	}
	else {
		for( cnt = 0 ; cnt < com_data.ins_chnum ; cnt ++ ){
															/*# トーンタイマID＿１の場合 #*/
			if( time_idck == dspmng_tonectl[cnt].tonetim_id1 ){
				time_kind = TONE_TIME;						/*# タイマ種別にトーンタイマ設定 #*/
				time_id = ID_1;								/*# タイマIＤ＿１設定 #*/
				cnno = cnt;									/*# ＣＮ番号設定 #*/
				result = OK;								/*# 判定結果にOK設定 #*/
			}
															/*# トーンタイマID＿2の場合 #*/
			else if( time_idck == dspmng_tonectl[cnt].tonetim_id2 ){
				time_kind = TONE_TIME;						/*# タイマ種別にトーンタイマ設定 #*/
				time_id = ID_2;								/*# タイマIＤ＿２設定 #*/
				cnno = cnt;									/*# ＣＮ番号設定 #*/
				result = OK;								/*# 判定結果にOK設定 #*/
			}
		}
		if( result == OK ){										/*# 判定結果がOKの場合 #*/
																/*# タイマ種別でＪＵＭＰ #*/
			dspmng_tonetime( time_id , time_kind , cnno );
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_helchk_timout                                     #*/
/*#                                                                         #*/
/*#     日本語名 : DSPヘルスチェックタイムアウト処理                        #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : DSPへのヘルスチェック要求を送信する                      #*/
/*#     入    力 : time_id : タイムアウトタイマID                           #*/
/*#                time_kind : タイマ種別（＝１：DSPヘルスチェック）        #*/
/*#                cnno : ＣＮ番号                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 03/09/07 - S.A                                    		#*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_helchk_timout(DWORD time_id , DWORD time_kind , DWORD cnno)
{
	short	dsp_id;
	
	dsp_id=0x00;

	dspmng_dsperror(HPI_HELTH_TIMOUT_ERROR,dsp_id,OFF,MDA_ID);
	
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_tonetime(V1)                                      #*/
/*#                                                                         #*/
/*#     日本語名 : ＤＳＰ状態－パルス送出終了制御                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : トーンタイマ（パルス用）タイムアウトの制御を行う。       #*/
/*#     作    成 : 99/06/28 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : time_id : タイムアウトタイマID                           #*/
/*#                time_kind : タイマ種別（＝０：トーンタイマ）             #*/
/*#                cnno : ＣＮ番号                                          #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 99/06/28 - 青木  康浩                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_tonetime( DWORD time_id , DWORD time_kind , DWORD cnno )
{

	DWORD	time_data;
	acgTTone ToneSpec;
	
	/*# タイマIDで分岐 */
	if (time_id == ID_1) {
		/* 交番トーンフェーズ切替用タイマ */
		/* トーン送出中は、フェーズを切り替える */
		if ((dspmng_tonectl[cnno].fk_sts == FK_TONE_ON1) ||
		    (dspmng_tonectl[cnno].fk_sts == FK_TONE_ON2) ||
		    (dspmng_tonectl[cnno].fk_sts == FK_TONE_ON3)) {
		    /* フェースで分岐 */
		    if (dspmng_tonectl[cnno].fk_phase == TOME_PH_1) {
				// 交番の2番めの周波数設定を行う
				ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
				ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id+1;
				ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
				ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
				dspmng_tonectl[cnno].fk_phase = TOME_PH_2; /* 交番トーンフェーズ */
			}
		    else {
				// 交番の最初の周波数設定を行う
				ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
				ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
				ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
				ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
				dspmng_tonectl[cnno].fk_phase = TOME_PH_1; /* 交番トーンフェーズ */
		    }
			// CallProgressトーン送信要求を行う
			if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
				dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #1\n", __func__);
			}
		    // フェーズ切替タイマスタート
			tmr_start(dspmng_tonectl[cnno].tonetim_id1 , (WORD)(dspmng_tonectl[cnno].fk_tim/5)); /* 交番フェーズ切替タイマ */
		}
	}
	else {
		// 交番トーン　ON/OFF切替タイマ
		if(dspmng_tonectl[cnno].fk_sts!=CLR){	//交番出力中？
			dspmng_tonectl[cnno].tonetim_idptn = CLR;		/*# 使用中トーンタイマID種別クリア #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id1);		/*# トーンタイマ＿１停止処理 #*/
			tmr_stop(dspmng_tonectl[cnno].tonetim_id2);		/*# トーンタイマ＿２停止処理 #*/
			switch(dspmng_tonectl[cnno].fk_sts){//交番の状態により処理分岐
				case FK_TONE_ON1:
					// トーン停止
					if (acgStopTone((int)cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #1\n", __func__);
					}
					dspmng_tonectl[cnno].fk_sts=FK_TONE_OFF1;	//状態遷移
					time_data=dspmng_tonectl[cnno].off1_tim;	//OFF1時間セット
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;
				case FK_TONE_OFF1:
					//トーンパターン判定
					//トーンパターンが2以降はON2へ遷移
					//それ以外はON1へ遷移
					if(dspmng_tonectl[cnno].tone_ptn>TONE_PTN1){
						dspmng_tonectl[cnno].fk_sts=FK_TONE_ON2;	//状態遷移
						
						time_data=dspmng_tonectl[cnno].on2_tim;		//ON2時間セット
					}
					else{
						dspmng_tonectl[cnno].fk_sts=FK_TONE_ON1;	//状態遷移

						time_data=dspmng_tonectl[cnno].on1_tim;		//ON1時間セット
					}
					// 交番の最初の周波数設定を行う
					ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
					ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
					ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
					ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
					dspmng_tonectl[cnno].fk_phase = TOME_PH_1; /* 交番トーンフェーズ */
					// CallProgressトーン送信要求を行う
					if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #2\n", __func__);
					}
				    // フェーズ切替タイマスタート
					tmr_start(dspmng_tonectl[cnno].tonetim_id1 , (WORD)(dspmng_tonectl[cnno].fk_tim/5)); /* 交番フェーズ切替タイマ */
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;
				case FK_TONE_ON2:
					// トーン停止
					if (acgStopTone((int)cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #2\n", __func__);
					}
					dspmng_tonectl[cnno].fk_sts=FK_TONE_OFF2;	//状態遷移
					time_data=dspmng_tonectl[cnno].off2_tim;	//OFF2時間セット
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;

				case FK_TONE_OFF2:
					//トーンパターン判定
					//トーンパターンが3以降はON3へ遷移
					//それ以外はON1へ遷移
					if(dspmng_tonectl[cnno].tone_ptn>TONE_PTN2){
						dspmng_tonectl[cnno].fk_sts=FK_TONE_ON3;	//状態遷移
						
						time_data=dspmng_tonectl[cnno].on3_tim;		//ON3時間セット
					}
					else{
						dspmng_tonectl[cnno].fk_sts=FK_TONE_ON1;	//状態遷移
						
						time_data=dspmng_tonectl[cnno].on1_tim;		//ON1時間セット
					}
					// 交番の最初の周波数設定を行う
					ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
					ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
					ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
					ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
					dspmng_tonectl[cnno].fk_phase = TOME_PH_1; /* 交番トーンフェーズ */
					// CallProgressトーン送信要求を行う
					if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #3\n", __func__);
					}
				    // フェーズ切替タイマスタート
					tmr_start(dspmng_tonectl[cnno].tonetim_id1 , (WORD)(dspmng_tonectl[cnno].fk_tim/5)); /* 交番フェーズ切替タイマ */
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;
				case FK_TONE_ON3:
					// トーン停止
					if (acgStopTone((int)cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgStopTone abnormal end #3\n", __func__);
					}
					dspmng_tonectl[cnno].fk_sts=FK_TONE_OFF3;	//状態遷移
					time_data=dspmng_tonectl[cnno].off3_tim;	//OFF3時間セット
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;
				case FK_TONE_OFF3:
					dspmng_tonectl[cnno].fk_sts=FK_TONE_ON1;	//状態遷移
					time_data=dspmng_tonectl[cnno].on1_tim;		//ON1時間セット
					// 交番の最初の周波数設定を行う
					ToneSpec.ToneElement.Type = ACG_TONE_TYPE__CALL_PROGRESS_TONE;
					ToneSpec.ToneElement.ToneIndex = dspmng_tonectl[cnno].tone_id;
					ToneSpec.Direction = ACG_DIRECTION__TO_LOCAL_TDM;
					ToneSpec.OnDuration = ToneSpec.OffDuration = 0;
					dspmng_tonectl[cnno].fk_phase = TOME_PH_1; /* 交番トーンフェーズ */
					// CallProgressトーン送信要求を行う
					if (acgPlayTone(cnno, &ToneSpec) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgPlayTone abnormal end #4\n", __func__);
					}
				    // フェーズ切替タイマスタート
					tmr_start(dspmng_tonectl[cnno].tonetim_id1 , (WORD)(dspmng_tonectl[cnno].fk_tim/5)); /* 交番フェーズ切替タイマ */
				    /* 交番ON/OFF切替タイマ */
					tmr_start(dspmng_tonectl[cnno].tonetim_id2 , (WORD)(time_data/5) );
					break;
				default:
					dspmng_tonectl[cnno].fk_sts=CLR;	//トーン送出状態クリア
					break;
			}
		}
	}
	//IPCS IPstage対応(end) 2004.9.3
}


/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_path_set                                             #*/
/*#                                                                         #*/
/*#     日本語名 : MDA音声パス設定                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 音声パスを設定する                                       #*/
/*#     入    力 :                                                          #*/
/*#                DWORD   ctlptn : 制御内容                                #*/
/*#                                         １：送話パス                    #*/
/*#                                         ２：受話パス                    #*/
/*#                                         ３：送受話パス                  #*/
/*#                DWORD   cnno  : ＣＮ番号                                 #*/
/*#                BYTE    codec_change  : コーデック変更有無               #*/
/*#                                        (ON:有り/OFF：なし)              #*/
/*#                                                                         #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_path_set( DWORD	ctlptn, DWORD cnno,BYTE codec_change)
{
																/*# 制御内容チェック #*/
	switch(ctlptn){
		case SNDPASS :											/*# 送話パスの場合 #*/
			//送話パス接続時のみ
			if(dspmng_tonectl[cnno].send_sts != SNDPB)
			{
				//PB送出中でない
				// RTPストリームパラメータを設定する。
				mda_set_RTPstream(&ChannelParameters[XC_DSP_ID][cnno].RTPStreamParameters, &dspmng_passctl[cnno]);
				// ゲインパラメータを設定する。
				mda_set_gain(&ChannelParameters[XC_DSP_ID][cnno].GainParameters, &dspmng_passctl[cnno]);
				// ジッタバッファパラメータ設定を行う。
				mda_set_JBparam(&ChannelParameters[XC_DSP_ID][cnno].JitterBufferParameters, &rx_buf_tbl[dspmng_passctl[cnno].buf_num]);
				if(codec_change==ON)
				{
					// トーン送出中状態をチェックする。
					if ((dspmng_passctl[cnno].rcv_sts == DISCON) &&
					    ((dspmng_tonectl[cnno].send_sts == SNDTONE)||(dspmng_tonectl[cnno].send_sts == SNDPULS))) {
					    	// トーン送出を継続する。
					    	dbg_print(MDA_ID, LOG_DEBUG, "%s Continue to send tones\n", __func__);
					}
					else {
//						// トーン送出中以外の場合にパスの再設定を行う。
//						if (dspmng_tonectl[cnno].send_sts == SNDHRY) {
//							// 保留音送出中は、停止する。
//							mda_hold_stop(cnno);
//						}
//						else {
//							// トーン送出を停止する
//							mda_tone_stop(cnno);
//						}
						// RTPストリーム処理を停止する
						if (acgDeactivateRTP(cnno) != ACG_SUCCESS) {
							dbg_print(MDA_ID, LOG_ERR, "%s acgDeactivateRTP abnormal end\n", __func__);
						}
						// チャネル属性を更新する
						if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
							dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
						}
						// RTPストリーム処理を開始する
						if (acgActivateRTP(cnno) != ACG_SUCCESS) {
							dbg_print(MDA_ID, LOG_ERR, "%s acgActivateRTP abnormal end\n", __func__);
						}
					}
				}
			}
			break;

		case RCVPASS :/*# 受話パスの場合 #*/
			//受話パス接続時のみ
			if(dspmng_tonectl[cnno].send_sts != SNDPB)
			{
				// RTPストリームパラメータを設定する。
				mda_set_RTPstream(&ChannelParameters[XC_DSP_ID][cnno].RTPStreamParameters, &dspmng_passctl[cnno]);
				// ゲインパラメータを設定する。
				mda_set_gain(&ChannelParameters[XC_DSP_ID][cnno].GainParameters, &dspmng_passctl[cnno]);
				// ジッタバッファパラメータ設定を行う。
				mda_set_JBparam(&ChannelParameters[XC_DSP_ID][cnno].JitterBufferParameters, &rx_buf_tbl[dspmng_passctl[cnno].buf_num]);
				//パスモード変更はCODEC変更されてる場合のみ
				if(codec_change==ON)
				{
//					// トーン送信状態をチェックする
//					if (dspmng_tonectl[cnno].send_sts == SNDHRY) {
//						// 保留音送出中は、停止する。
//						mda_hold_stop(cnno);
//					}
//					else {
//						// トーン送出を停止する
//						mda_tone_stop(cnno);
//					}
					// RTPストリーム処理を停止する
					if (acgDeactivateRTP(cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgDeactivateRTP abnormal end\n", __func__);
					}
					// チャネル属性を更新する
					if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
					}
					// RTPストリーム処理を開始する
					if (acgActivateRTP(cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_DEBUG, "%s acgActivateRTP abnormal end\n", __func__);
					}
				}
			}
			break;
		case BOTHPASS :												/*# 双方向パスの場合 #*/
#ifdef BOTH_PASS_ON
			/* 非送信明らかにBOTH_PASS_ONは定義されてファイル化されることはない、何故ならば、pass_statusが未定義でコンパイルエラー＾となるから */

			if(dspmng_tonectl[cnno].send_sts == NOSND ){			/*# トーン／パルス／ＰＢ／保留音送出中でない場合 #*/
				// RTPストリームパラメータを設定する。
				mda_set_RTPstream(&ChannelParameters[XC_DSP_ID][cnno].RTPStreamParameters, &dspmng_passctl[cnno]);
				// ゲインパラメータを設定する。
				mda_set_gain(&ChannelParameters[XC_DSP_ID][cnno].GainParameters, &dspmng_passctl[cnno]);
				// ジッタバッファパラメータ設定を行う。
				mda_set_JBparam(&ChannelParameters[XC_DSP_ID][cnno].JitterBufferParameters, &rx_buf_tbl[dspmng_passctl[cnno].buf_num]);
				//新規パスorPB/トーン切断→接続時のみチャネルモード設定する。上書き時はPADとECHOの設定のみ。
				if(pass_status==PASS_NEW_WRITE){
					// RTPストリーム処理を停止する
					if (acgDeactivateRTP(cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgDeactivateRTP abnormal end\n", __func__);
					}
					// チャネル属性を更新する
					if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
					}
					// RTPストリーム処理を開始する
					if (acgActivateRTP(cnno) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgActivateRTP abnormal end\n", __func__);
					}
				}
			}
			else
			{								/*# トーン／パルス／ＰＢ／保留音送出中の場合 #*/
				/*# ＰＢ送出中でない場合 #*/
				if(dspmng_tonectl[cnno].send_sts != SNDPB ){
					// チャネル属性を更新する
					if (acgUpdateChannel(cnno, &ChannelParameters[XC_DSP_ID][cnno]) != ACG_SUCCESS) {
						dbg_print(MDA_ID, LOG_ERR, "%s acgUpdateChannel abnormal end\n", __func__);
					}
				}																	// 2000.01.28 追加
			}
#endif /* BOTH_PASS_ON */
		break;

		default :
				dbg_print(MDA_ID, LOG_DEBUG, "%s Illegal ctlptn(%d)", __func__, (int)ctlptn);
				break;
		}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_set_RTPstream                                        #*/
/*#                                                                         #*/
/*#     日本語名 : MDARTPストリーム設定                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : RTPストリーム属性をDSPチャネルパラメータに設定する。     #*/
/*#     入    力 : RTPストリームパラメータアドレス                          #*/
/*#                パス制御用メモリアドレス                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_set_RTPstream(acgTRTPStreamParameters *acgTRTPStreamParameters_p, DSPMNG_PASSCTL *dspmng_passctl_p)
{
	int	PayloadType = PT_G711u;
	// コーデック種別を設定し、同時にペイロードタイプを求める
	switch(dspmng_passctl_p->codec){
		case 2:		//G729
			acgTRTPStreamParameters_p->Coder = ACG_CODER__G729;
			PayloadType = PT_G729;
			break;
		case 3:		//G711u
			acgTRTPStreamParameters_p->Coder = ACG_CODER__G711MULAW;
			PayloadType = PT_G711u;
			break;
		case 4:		//G711A
			acgTRTPStreamParameters_p->Coder = ACG_CODER__G711ALAW;
			PayloadType = PT_G711a;
			break;
		case 7:		//ダウンロード値
			if(com_data.lu_download.law == CODEC_U){	/*# ＣＯＤＥＣ：μ－ｌａｗ指定の場合 #*/
				acgTRTPStreamParameters_p->Coder = ACG_CODER__G711MULAW;
				PayloadType = PT_G711u;
			}
			else										/*# ＣＯＤＥＣ：Ａ－ｌａｗ指定の場合 #*/
			{
				acgTRTPStreamParameters_p->Coder = ACG_CODER__G711ALAW;
				PayloadType = PT_G711a;
			}
			break;
		default:	//その他（未サポート）
			dbg_print(MDA_ID, LOG_ERR, "%s Illegal codec(%d)\n", __func__, dspmng_passctl_p->codec);
			break;
	}
	// RTP送信周期を設定
	acgTRTPStreamParameters_p->PTime = dspmng_passctl_p->send_period*2; /* dspmng_passctl内は、5ms単位なので、2倍する */
	// ペイロードタイプを設定する
	acgTRTPStreamParameters_p->TxVoicePayloadType = acgTRTPStreamParameters_p->RxVoicePayloadType = PayloadType;
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_set_gain                                             #*/
/*#                                                                         #*/
/*#     日本語名 : MDAゲイン設定                                            #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ゲインをDSPチャネルパラメータに設定する。                #*/
/*#     入    力 : AGCパラメータアドレス                                    #*/
/*#                パス制御用メモリアドレス                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_set_gain(acgTGainParameters *acgTGainParameters_p, DSPMNG_PASSCTL *dspmng_passctl_p)
{

	// スルーモードが有効時は、ゲインを0とし、リターンする。
	if (dspmng_passctl_p->thru_mode == THRU_ON) {
		acgTGainParameters_p->VoiceOutputGain = acgTGainParameters_p->InputGain = ACG_VOICE_GAIN__0_DB;
		return;
	}
	//未設定の場合は、デフォルト値を設定
	if (dspmng_passctl_p->snd_pad == 0) {
		acgTGainParameters_p->VoiceOutputGain =ACG_VOICE_GAIN__0_DB;
	}
	if (dspmng_passctl_p->rcv_pad == 0) {
		acgTGainParameters_p->InputGain =ACG_VOICE_GAIN__0_DB;
	}
	//出力ゲインを設定する。
	acgTGainParameters_p->VoiceOutputGain = (short)dspmng_passctl_p->snd_pad + MDA_GAIN_RADIX;
	//出力ゲインが最小値より小さい場合は補正する
	if (acgTGainParameters_p->VoiceOutputGain < ACG_VOICE_GAIN__minus31_DB) {
		acgTGainParameters_p->VoiceOutputGain = ACG_VOICE_GAIN__minus31_DB;
	}
	//出力ゲインが最大値より大きい場合は補正する
	if (acgTGainParameters_p->VoiceOutputGain > ACG_VOICE_GAIN__plus31_DB) {
		acgTGainParameters_p->VoiceOutputGain = ACG_VOICE_GAIN__plus31_DB;
	}
	//入力ゲインを設定する。
	acgTGainParameters_p->InputGain = (short)dspmng_passctl_p->rcv_pad + MDA_GAIN_RADIX;
	//入力ゲインが最小値より小さい場合は補正する
	if (acgTGainParameters_p->InputGain < ACG_VOICE_GAIN__minus31_DB) {
		acgTGainParameters_p->InputGain = ACG_VOICE_GAIN__minus31_DB;
	}
	//入力ゲインが最大値より大きい場合は補正する
	if (acgTGainParameters_p->InputGain > ACG_VOICE_GAIN__plus31_DB) {
		acgTGainParameters_p->InputGain = ACG_VOICE_GAIN__plus31_DB;
	}
}
				
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_set_JBparam                                          #*/
/*#                                                                         #*/
/*#     日本語名 : MDAジッタバッファパラメータ設定                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ジッタバッファパラメータ属性を設定する。                 #*/
/*#     入    力 : ジッタバッファパラメータアドレス                         #*/
/*#                受信バッファメモリアドレス                               #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_set_JBparam(acgTJitterBufferParameters *acgTJitterBufferParameters_p, RXBUF *rxbuf_p)
{
	acgTJitterBufferParameters_p->VoiceJitterBufferMinDelay = rxbuf_p->start;
	acgTJitterBufferParameters_p->VoiceJitterBufferMaxDelay = rxbuf_p->stop;
}

/******************************************************************************/
/* 関数名     DSPイベント受信スレッド                                         */
/* 機能概要   DSPよりイベントを受信し、処理する。                             */
/* パラメータ arg  : (in)   動作モード格納領域ポインタ配列                    */
/* リターン   -                                                               */
/******************************************************************************/

void dspevt(void *arg)
{
	int 	status;
	acgTEventInfo 		DspEvent;
	BYTE	cnno;
	
	/* 引数(動作モード)が通常モード以外の時は、エラーメッセージを出力後、5秒sleepしながら無限ループする */
	if (*(int *)arg == LUMODE_MAINTE) {
		while(1) {
			dbg_print(MDA_ID, LOG_ERR, "%s Illeagal operation mode %d\n", __func__, *(int *)arg);
			sleep(5);
		}
	}
	// イベント受信ループ
	while (1) {
		/* イベント受信 */
		status = acgGetEvent(&DspEvent, ACG_POLLING_METHOD__BLOCKING);
		// 受信結果で分岐
		switch(status)
		{
			case ACG_SUCCESS:
				// CN番号を抽出する
				cnno = DspEvent.Channel;
				// イベント種別で分岐
				switch(DspEvent.EventType) {
				case ACG_EVENT_TYPE__SYSYEM_ERROR:
					/* DSP_SYSTEM_ERRORでdspmng_dsperrorを起動する */
					dspmng_dsperror(DSP_SYSTEM_ERROR, 0, OFF, MDA_ID);
					break;
				case ACG_EVENT_TYPE__IBS_STRING_GENERATION_ENDED:
					/* DTMF送出完了 (I_DTMFCOMPLETE)を通知する */
					mdamsg_inmsend(I_DTMFCOMPLETE, (DWORD)cnno); 
					break;
				case ACG_EVENT_TYPE__PLAYBACK_ENDED:
					/* 保留音送出完了 (I_HORYCOMPLETE)を通知する */
					mdamsg_inmsend(I_HORYCOMPLETE, (DWORD)cnno); 
					break;
				case ACG_EVENT_TYPE__IBS_STRING_GENERATION_ABORTED:
					/* DTMF送出アボート (I_DTMFABORT)を通知する */
					mdamsg_inmsend(I_DTMFABORT, (DWORD)cnno); 
					break;
				case ACG_EVENT_TYPE__JITTER_BUFFER_STATUS:
					/* 情報収集機能削除のため、処理なし。 */
					break;
				case ACG_EVENT_TYPE__REGULAR_CHANNEL_PACKETIZER_ERROR:
					/* パケットエラーを通知する */
					h323_socketng_post(cnno,H323_ERRPKT);
					break;
				case ACG_EVENT_TYPE__DEVICE_STATUS:
					/* Device Status受信(I_DEVICE_STATUS)を通知 */
					mdamsg_inmsend(I_DEVICE_STATUS, (DWORD)cnno); 
					break;
				case ACG_EVENT_TYPE__RTCP_INFO:
					/* 情報収集機能削除のため、処理なし。 */
					break;
				default:
					break;
					
				}
				break;

			case ACG_FAILURE:
				dbg_print(MDA_ID, LOG_DEBUG, "%s acgGetEvent - status = ACG_FAILURE\n",__func__);
				dspmng_dsperror(DSP_EVTRCV_ERROR, 0, OFF, MDA_ID);
				while (1) {
					// 100スリープする
					usleep(100*1000);
					// DSP状態チェック
					if (dsp_data[XC_DSP_ID].status == XC_FAULT) {
						// 障害復旧したら、ループに戻る
						break;
					}
				}
				break;

			case ACG_NO_PACKET:
				dbg_print(MDA_ID, LOG_DEBUG, "%s acgGetEvent - status = ACG_NO_PACKET\n",__func__);
				break;

			default:
				dbg_print(MDA_ID, LOG_ERR, "%s acgGetEvent - status = %d unknown\n",__func__, status);
				break;
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mdamsg_inmsend                                           #*/
/*#                                                                         #*/
/*#     日本語名 : MDA内部メッセージ送信処理                                #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 内部メッセージの作成／送信を行う。                       #*/
/*#     入    力 : BYTE  kind : メッセージ種別                              #*/
/*#              : DWORD    cnno : ＣＮ番号                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mdamsg_inmsend(BYTE kind, DWORD cnno)
{
	INNER_MSG	*msgp; /*# ＬＵメッセージポインタ #*/

	/*# プール獲得 #*/
	msgp = (INNER_MSG *)com_poolget(POOL0);
	if(msgp == (INNER_MSG *)0xffffffff){/*# メモリ確保ＯＫ? #*/
		pool_get_err[POOL0]++;/*# プール取得ＮＧログ #*/
		dbg_print(MDA_ID, LOG_ERR, "%s cn=%d com_poolget(POOL0) NG\n", __func__, cnno);
		return;
	}
	/*# ＬＵ内部共通ヘッダ作成 #*/
	msgp->msg_header.id = MDA_ID;				/*# 送信元ＩＤ設定 #*/
	msgp->msg_header.div = IN_MSG;				/*# メッセージ区分設定 #*/
	msgp->msg_header.kind = kind;				/*# メッセージ種別 #*/
	msgp->msg_header.no = cnno;					/*# チャネル番号設定 #*/
	msgp->msg_header.link = NULL;
	
	com_sndmsg( MDA_ECB , (BYTE *)msgp );		/*# メッセージ送信 #*/
}

/******************************************************************************/
/* 関数名     DDSPtoLAN                                                       */
/* 機能概要   DSPよりRTP/RTCPパケット受信し処理する。                         */
/* パラメータ arg  : (in)   動作モード格納領域ポインタ配列                    */
/* リターン   -                                                               */
/******************************************************************************/

void dsp2lan(void *arg)
{
	int 			status;
	acgTGetMedia	GetMedia;
	BYTE			cnno;
	INNER_MSG		*msg1;
	RTPDAT			*msg2;
	
	/* 引数(動作モード)が通常モード以外の時は、エラーメッセージを出力後、5秒sleepしながら無限ループする */
	if (*(int *)arg == LUMODE_MAINTE) {
		while(1) {
			dbg_print(MDA_ID, LOG_ERR, "%s Illeagal operation mode %d\n", __func__, *(int *)arg);
			sleep(5);
		}
	}
	// イベント受信ループ
	while (1) {
		/* イベント受信 */
		memset((void *)&GetMedia, 0, sizeof(GetMedia));
		GetMedia.PollingMethod = ACG_POLLING_METHOD__BLOCKING;
		status = acgGetMedia(&GetMedia);
		// 受信結果で分岐
		switch(status)
		{
			case ACG_SUCCESS:
				// CN番号を抽出する
				cnno = GetMedia.Channel;
				//  RTPパケットの場合受信パケットを内部メッセージに組み立てて、h323_voicesndを起動する
			if (GetMedia.MediaProtocol == ACG_PROTOCOL__RTP) {
				/*# RTP用プール獲得 #*/
				msg2 = (RTPDAT *)com_poolget(POOL4);
				if(msg2 == (RTPDAT *)0xffffffff){/*# メモリ確保ＯＫ? #*/
					pool_get_err[POOL4]++;/*# プール取得ＮＧログ #*/
					dbg_print(MDA_ID, LOG_ERR, "%s cn=%d com_poolget(POOL4) NG\n", __func__, cnno);
					break;
				}
				/*# 一次メッセージ用プール獲得 #*/
				msg1 = (INNER_MSG *)com_poolget(POOL0);
				if(msg1 == (INNER_MSG *)0xffffffff){/*# メモリ確保ＯＫ? #*/
					pool_get_err[POOL0]++;/*# プール取得ＮＧログ #*/
					com_poolput( POOL4 ,(BYTE *)msg2 );
					dbg_print(MDA_ID, LOG_ERR, "%s cn=%d com_poolget(POOL0) NG\n", __func__, cnno);
					break;
				}
				/*# ＬＵ内部共通ヘッダ作成 #*/
				msg1->msg_header.id = MDA_ID;		/*# 送信元ＩＤ設定 #*/
				msg1->msg_header.div = IN_MSG;		/*# メッセージ区分設定 #*/
				msg1->msg_header.kind = I_VOICESND;	/*# メッセージ種別 #*/
				msg1->msg_header.no = cnno;			/*# チャネル番号設定 #*/
				msg1->msg_header.link = (BYTE *)msg2;
				// RTPパケットをコピーする
				if (GetMedia.Len > POOL4_SIZE) {
					dbg_print(MDA_ID, LOG_ERR, "%s cn=%d RTP Len exseeded(%d) %d\n", __func__, cnno, POOL4_SIZE, GetMedia.Len);
					GetMedia.Len = POOL4_SIZE;
				}
				memcpy((void *)msg2, (const void *)GetMedia.Buff, GetMedia.Len);
				// h323_voicesndを起動する
				h323_voicesnd((BYTE *)msg1);
				/*# メッセージ解放処理 #*/
				if(msg1->msg_header.link != NULL){
					/*# ２次メッセージ解放 #*/
					com_poolput( POOL4 , (BYTE *)msg1->msg_header.link );
				}
				/*# 内部（１次）メッセージ解放 #*/
				com_poolput( POOL0 , (BYTE *)msg1 );
			}
				break;

			case ACG_FAILURE:
				dbg_print(MDA_ID, LOG_DEBUG, "%s acgGetMedia - status = ACG_FAILURE\n",__func__);
				dspmng_dsperror(DSP_EVTRCV_ERROR, 0, OFF, MDA_ID);
				while (1) {
					// 100スリープする
					usleep(100*1000);
					// DSP状態チェック
					if (dsp_data[XC_DSP_ID].status == XC_FAULT) {
						// 障害復旧したら、ループに戻る
						break;
					}
				}
				break;

			case ACG_NO_PACKET:
				dbg_print(MDA_ID, LOG_DEBUG, "%s acgGetEvent - status = ACG_NO_PACKET\n",__func__);
				break;

			default:
				dbg_print(MDA_ID, LOG_ERR, "%s acgGetEvent - status = %d unknown\n",__func__, status);
				break;
		}
	}
}

/******************************************************************************/
/* 関数名     LANtoDSP                                                        */
/* 機能概要   NWよりRTP/RTCPパケット受信し処理する。                          */
/* パラメータ arg  : (in)   動作モード格納領域ポインタ配列                    */
/* リターン   -                                                               */
/******************************************************************************/

void lan2dsp(void *arg)
{
	fd_set				readfds;
	int 				status;
	int					ch_no;
	struct sockaddr_in senderinfo;
	socklen_t         addrlen;
	
	/* 引数(動作モード)が通常モード以外の時は、エラーメッセージを出力後、5秒sleepしながら無限ループする */
	if (*(int *)arg == LUMODE_MAINTE) {
		while(1) {
			dbg_print(MDA_ID, LOG_ERR, "%s Illeagal operation mode %d\n", __func__, *(int *)arg);
			sleep(5);
		}
	}
	// イベント受信ループ
	while (1) {
		/* リード用fdワークを設定する */
		readfds = mda_readfds;
		status = select(max_rcv_sd+1, &readfds, NULL, NULL, NULL);
		/* selectの実行結果で分岐する */
		if (select(max_rcv_sd+1, &readfds, NULL, NULL, NULL) <= 0) {
			dbg_print(MDA_ID, LOG_ERR, "%s select NG(%d) %s\n", __func__, errno, strerror(errno));
			if ((status < 0) && (errno != EBADF)) {
				/* EBADF以外の場合com_syserrを起動する */
				com_syserr(MDA_ID, MDA_ABORT_01);
			}
		}
		else {
			/* 受信したソケット数分処理する */
			for (ch_no=0;ch_no<CNMAX_IPCS;++ch_no) {
				/* RTP送信用ソケット処理 */
				mda_rcvpacket(S_RTP_SND, ch_no, &readfds);
				/* RTP受信用ソケット処理 */
				mda_rcvpacket(S_RTP_RCV, ch_no, &readfds);
				/* RTCP送信用ソケット処理 */
				mda_rcvpacket(S_RTCP_SND, ch_no, &readfds);
				/* RTCP受信用ソケット処理 */
				mda_rcvpacket(S_RTCP_RCV, ch_no, &readfds);
			}
			/* 通知用ダミーメッセージの場合は、読み捨て */
			if (FD_ISSET(notice_sd_s, &readfds)) {
				addrlen = sizeof(senderinfo);
				memset((void *)&senderinfo, 0, sizeof(senderinfo));
				/* パケットを受信 */
				recvfrom(notice_sd_s, (void *)mda_dummy_msg_rcv, POOL3_SIZE, 0, (struct sockaddr *)&senderinfo, &addrlen);
			}
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_rcvpacket                                            #*/
/*#                                                                         #*/
/*#     日本語名 : MDAパケット受信処理                                      #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : NWよりパケットを受信し処理する。                         #*/
/*#     入    力 : BYTE  kind : ソケット種別                                #*/
/*#              : DWORD    cnno : ＣＮ番号                                 #*/
/*#              : fd_set   *readfds : リード用fdワークポインタ             #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_rcvpacket(BYTE sock_kind, DWORD cnno, fd_set *readfds)
{
	RTPDAT				*msg2;
	MDA_SOCKET_CTL_T	*socket_ctlp;
	int 				sockfd;
	ssize_t				rcv_len;
	struct sockaddr_in senderinfo;
	socklen_t         addrlen;


	socket_ctlp = &mda_socket_ctl_table[cnno];
	/* ソケットディスクリプタを抽出 */
	switch (sock_kind) {
	case S_RTP_SND:
		if ((socket_ctlp->create_flag[0] & S_RTP_MASK) == S_RTP_MASK) {
			sockfd = socket_ctlp->rtp_sd[0];
		}
		else {
			return;
		}
		break;
	case S_RTP_RCV:
		if ((socket_ctlp->create_flag[1] & S_RTP_MASK) == S_RTP_MASK) {
			sockfd = socket_ctlp->rtp_sd[1];
		}
		else {
			return;
		}
		break;
	case S_RTCP_SND:
		if ((socket_ctlp->create_flag[0] & S_RTCP_MASK) == S_RTCP_MASK) {
			sockfd = socket_ctlp->rtcp_sd[0];
		}
		else {
			return;
		}
		break;
	default: /* S_RTCP_RCV */
		if ((socket_ctlp->create_flag[1] & S_RTCP_MASK) == S_RTCP_MASK) {
			sockfd = socket_ctlp->rtcp_sd[0];
		}
		else {
			return;
		}
		break;
	}

	/* 受信したかをチェック */
	if (FD_ISSET(sockfd, readfds)) {
		/*# RTP用プール獲得 #*/
		msg2 = (RTPDAT *)com_poolget(POOL3);
		if(msg2 == (RTPDAT *)0xffffffff){/*# メモリ確保ＯＫ? #*/
			pool_get_err[POOL3]++;/*# プール取得ＮＧログ #*/
			dbg_print(MDA_ID, LOG_ERR, "%s cn=%d com_poolget(POOL3) NG\n", __func__, cnno);
			return;
		}
		memset((void *)&senderinfo, 0, sizeof(senderinfo));
		/* パケットを受信 */
		addrlen = sizeof(senderinfo);
		rcv_len = recvfrom(sockfd, msg2, POOL3_SIZE, 0, (struct sockaddr *)&senderinfo, &addrlen);
		if (rcv_len > 0) {
			/* 受信成功 */
			if ((sock_kind == S_RTP_SND) || (sock_kind == S_RTP_RCV)) {
				/* RTP*/
				h323_rcvcallback(sockfd,senderinfo.sin_addr.s_addr,senderinfo.sin_port,(BYTE *)msg2,rcv_len);
			}
			else {
				/* RTCP */
				h323_rcvcallback_rtcp(sockfd,senderinfo.sin_addr.s_addr,senderinfo.sin_port,(BYTE *)msg2,rcv_len);
			}
		}
		/*# メッセージ解放 #*/
		com_poolput( POOL3 , (BYTE *)msg2 );
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : mda_fd_init                                              #*/
/*#                                                                         #*/
/*#     日本語名 : mdaFD初期化                                              #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : select用ファイルディスクリプタを初期化する。             #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
void mda_fd_init(void)
{
	int port;
	int ch_no;
	
	/* lan2dspスレッド通知送信用ソケット(AF_INET/UDP)を取得し、mda内部データに設定する */
	notice_sd_s = socket(AF_INET,SOCK_DGRAM,0);
	if (notice_sd_s < 0) {
		/* 取得NG時は、com_syserrを起動する */
		dbg_print(MDA_ID, LOG_ERR, "%s socket abnormal end #1 %s\n", __func__, strerror(errno));
		com_syserr(MDA_ID, MDA_ABORT_02);
	}
	/* 受信用最大ソケットディスクリプタを設定 */
	max_rcv_sd = notice_sd_s;
	notice_sd_c = socket(AF_INET,SOCK_DGRAM,0);
	if (notice_sd_c < 0) {
		/* 取得NG時は、com_syserrを起動する */
		dbg_print(MDA_ID, LOG_ERR, "%s socket abnormal end #2 %s\n", __func__, strerror(errno));
		com_syserr(MDA_ID, MDA_ABORT_03);
	}
	/* lan2dspスレッド通知受信用ソケットに、IPアドレス(127.0.0.1)、ポート(50000以降)をbindする */
	notice_addr_s.sin_family = AF_INET;
	notice_addr_s.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local Host */
	for (port=50000;port<65535;++port) {
		notice_addr_s.sin_port = htons(port);
		if (bind(notice_sd_s, (const struct sockaddr *)&notice_addr_s, sizeof(notice_addr_s)) == -1) {
			dbg_print(MDA_ID, LOG_ERR, "%s bind abnormal end #1 %s\n", __func__, strerror(errno));
			if (errno != EADDRINUSE) {
				/* EADDRINUSE以外で異常終了の場合は、com_syserrを起動する */
				com_syserr(MDA_ID, MDA_ABORT_04);
			}
		}
		else {
			/* bind成功 */
			break;
		}
	}
	/* 50000から65535の範囲でバインドできない場合は、com_syserrを起動する */
	if (port > 65533) {
		dbg_print(MDA_ID, LOG_ERR, "%s bind port over #1 %d\n", __func__, port);
		com_syserr(MDA_ID, MDA_ABORT_06);
	}
	notice_addr_c.sin_family = AF_INET;
	notice_addr_c.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Local Host */
	for (port+=1;port<65535;++port) {
		notice_addr_c.sin_port = htons(port);
		if (bind(notice_sd_c, (const struct sockaddr *)&notice_addr_c, sizeof(notice_addr_c)) == -1) {
			dbg_print(MDA_ID, LOG_ERR, "%s bind abnormal end #2 %s\n", __func__, strerror(errno));
			if (errno != EADDRINUSE) {
				/* EADDRINUSE以外で異常終了の場合は、com_syserrを起動する */
				com_syserr(MDA_ID, MDA_ABORT_05);
			}
		}
		else {
			/* bind成功 */
			break;
		}
	}
	/* 50000から65535の範囲でバインドできない場合は、com_syserrを起動する */
	if (port > 65534) {
		dbg_print(MDA_ID, LOG_ERR, "%s bind port over #2 %d\n", __func__, port);
		com_syserr(MDA_ID, MDA_ABORT_07);
	}
	/* mda受信用ファイルディスクリプタテーブルをクリアする */
	FD_ZERO(&mda_readfds);
	/* mda受信用ファイルディスクリプタテーブルにlan2dspスレッド通知受信用ソケットをセットする */
	FD_SET(notice_sd_s, &mda_readfds);
	/* mdaソケット管理テーブルを初期化する */
	for (ch_no=0;ch_no<CNMAX;++ch_no) {
		memset((void *)&mda_socket_ctl_table[ch_no], 0, sizeof(MDA_SOCKET_CTL_T));
		mda_socket_ctl_table[ch_no].rtp_sd[0] = mda_socket_ctl_table[ch_no].rtp_sd[1] =
		mda_socket_ctl_table[ch_no].rtcp_sd[0] = mda_socket_ctl_table[ch_no].rtcp_sd[1] = 0xffffffff;
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dspmng_make_dsprepair	                            #*/
/*#                                                                         #*/
/*#     日本語名 : DSP障害復旧イベント送信   　　　　                      #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : CCUへDSP障害復旧イベントを送信する                     #*/
/*#     作    成 : 03/09/07 - 薄井  正明                                    #*/
/*#     入    力 : short dsp_id  DSP_ID                                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void dspmng_make_dsprepair(short dsp_id,char irq)
{
	INNER_MSG *msg_ptr;
	LUEVT_DSPREPAIR	*evtp;

	if(irq==ON)
	{
		//メッセージヘッダ取得
		msg_ptr = ( INNER_MSG *)com_ipoolget( POOL0 );		/*# メモリ確保 #*/
		if(msg_ptr == ( INNER_MSG *)0xffffffff)				/*# プール取得OK？ #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return ;
		}
	
		//イベント部取得
		evtp = ( LUEVT_DSPREPAIR *)com_ipoolget( POOL1 );
		if(evtp == ( LUEVT_DSPREPAIR *)0xffffffff){/*# メモリ確保OK?(V1) #*/
			com_ipoolput( POOL0 ,(BYTE *)msg_ptr );
			return;
		}
	}
	else
	{
	//メッセージヘッダ取得
		msg_ptr = ( INNER_MSG *)com_poolget( POOL0 );		/*# メモリ確保 #*/
		if(msg_ptr == ( INNER_MSG *)0xffffffff)				/*# プール取得OK？ #*/
		{
			pool_get_err[POOL0]++;							/*# プール取得ＮＧログ #*/
			return ;
		}
	
		//イベント部取得
		evtp = ( LUEVT_DSPREPAIR *)com_poolget( POOL1 );
		if(evtp == ( LUEVT_DSPREPAIR *)0xffffffff){/*# メモリ確保OK?(V1) #*/
			com_poolput( POOL0 ,(BYTE *)msg_ptr );
			return;
		}
	}

	//イベント組み立て
	evtp->lu_header.pkg_no = FROM_SLAVE;//2004.04 IP-CS追加
	
	evtp->bc 		= 0x04;								/*# B.C #*/
	evtp->code 	= E_DSP_REPAIR_DETECT;						/*# メッセージ区分 #*/
	evtp->dspno 	= dsp_id;
									
	//メッセージヘッダ設定
	msg_ptr->msg_header.id		= MDA_ID;				/*# 送信元ID(LU管理)セット #*/
	msg_ptr->msg_header.div 	= LU_EVT;				/*# メッセージ区分セット #*/
	msg_ptr->msg_header.kind 	= CLR;					/*# #*/
	msg_ptr->msg_header.link 	= (BYTE *)evtp;				/*# 2次リンクあり #*/

	if(irq==ON)
	{
		com_isndmsg( DOEP_ECB, ( BYTE * )msg_ptr );				/*#	メッセージ送出 #*/
	}
	else
	{
		com_sndmsg( DOEP_ECB, ( BYTE * )msg_ptr );				/*#	メッセージ送出 #*/
	}
}	

	