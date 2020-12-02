/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : LU                                                       #*/
/*#                                                                         #*/
/*#     日本語名 : LU共通データ定義ファイル                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/28 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
#include "def.h"
#include "str.h"
#include "str_comdt.h"
#include "prot.h"
#include "str_boot.h"
#include "str_start.h"
// IPCD V4 Del #include "../../mik/mik_armc.h"

/* IPCS_V4 <PD><ADD-STA> */
/* thread prototype */
void lu_mon_thread(void *arg);
void tmr_thread(void *arg);
void cs_ctl(void *arg);
void mda_thread(void *arg);
void lumng_thread(void *arg);
void doep_r_thread(void *arg);
void doep_s_thread(void *arg);
void downld(void *arg);
void mnt_thread(void *arg);
void lu_log(void *arg);
void support(void *arg);
void dspevt(void *arg);
void dsp2lan(void *arg);
void lan2dsp(void *arg);
void cyc_ctl(void *arg);
void spro_ctl(void *arg);
void lpb_ctl(void *arg);
void fpga_read(void *arg);
void writer(void *arg);
void fpga_write(void *arg);
/* IPCS_V4 <PD><ADD-END> */
char ipcs_lu_ver[8] = {"040000"};/* IPCS_V4 <PD><ADD> */
/*# LU共通参照エリア #*/
COM_DATA			com_data;					/*# LU共通参照エリア #*/
BYTE				led_time;					/*# ＬＥＤ制御データ #*/
#ifndef V2_1_IPL_EXCLUSIVE	//IPLはV2専用化する
COM_UNICAST			com_unicast;				/*# ユニキャスト管理情報 #*///2000.07 V2.1追加
#endif //ifndef V2_1_IPL_EXCLUSIVE	//IPLはV2専用化する
COM_FLSDATA			com_flsdata;				/*# ＦＬＡＳＨ格納データ #*/					//2000.09 V2.1追加
COM_FLSDATA_STS		com_flsdata_sts;			/*# ＦＬＡＳＨステータス #*/					//2000.09 V2.1追加
COM_TOS_SD			com_tos_sd;					/*# ＴＯＳ設定ソケットディスクリプタ構造体 #*/	//2001.01.16 V2.2追加

FLS_CA_DATA			fls_ca_data;				/*# CA切り替え情報：IP-CS(SS9100_PH1再開) 追加 #*/

/*# 工場試験用 #*/
TEST_LED_INF	test_led_ctl;
DWORD	tp_exe_flg;								/*# 工場試験実施フラグ #*/
DWORD	test_lunum_ctl;							/*# ＬＵ番号出力制御状態 #*/

#if 0 /* IPCS_V4 <PD><DEL-STA> */
/*# ＭＩＫ管理エリア #*/
BYTE		ztcbtbl[32*TCB_MAX];
BYTE		zecbtbl[16*ECB_MAX];
BYTE		zrdytbl[8*PRI_MAX+4];
#endif /* IPCS_V4 <PD><DEL-END> */
#ifdef	V2_DBG_POOL0_DET

typedef	struct{
	WORD		pbsts;
	WORD		plid;
	void		*next_link;
	void		*prev_link;
	INNER_MSG	data;
}MSG_STRUCT;

typedef	struct{
	void		*ready_top;
	void		*ready_tail;
	void		*user_top;
	void		*user_tail;
	DWORD		ready_cnt;
	DWORD		user_cnt;
	DWORD		rsv1;
	DWORD		rsv2;
	MSG_STRUCT	msg[POOL0_CNT];
}POOL_STRUCT;
#endif


#if 0 /* IPCS_V4 <PD><DEL-STA> */
/*# メモリプール #*/
//#ifdef	V2_DBG_POOL0_DET
POOL_STRUCT		RAM_POOL0;	/*# プール０エリア #*/
//#else	//#ifdef	V2_DBG_POOL_DET
BYTE		RAM_POOL0[32+POOL0_SIZE*POOL0_CNT];	/*# プール０エリア #*/
//#endif	//#ifdef	V2_DBG_POOL_DET
BYTE		RAM_POOL1[32+POOL1_SIZE*POOL1_CNT];	/*# プール１エリア #*/
BYTE		RAM_POOL2[32+POOL2_SIZE*POOL2_CNT];	/*# プール２エリア #*/
BYTE		RAM_POOL3[32+POOL3_SIZE*POOL3_CNT];	/*# プール３エリア #*/
BYTE		RAM_POOL4[32+POOL4_SIZE*POOL4_CNT];	/*# プール４エリア #*/
BYTE		RAM_POOL5[32+POOL5_SIZE*POOL5_CNT];	/*# プール５エリア #*/
BYTE		RAM_POOL6[32+POOL6_SIZE*POOL6_CNT];	/*# プール６エリア #*/
BYTE		RAM_POOL7[32+POOL7_SIZE*POOL7_CNT];	/*# プール７エリア #*/
BYTE		RAM_POOL8[32+POOL8_SIZE*POOL8_CNT];	/*# プール８エリア #*/
BYTE		RAM_POOL9[32+POOL9_SIZE*POOL9_CNT];	/*# プール９エリア #*/
BYTE		RAM_POOLA[32+POOLA_SIZE*POOLA_CNT];	/*# プールＡエリア #*/
BYTE		RAM_POOLB[32+POOLB_SIZE*POOLB_CNT];	/*# プールＢエリア #*/
BYTE		RAM_POOLC[32+POOLC_SIZE*POOLC_CNT];	/*# プールＣエリア #*/
BYTE		RAM_POOLD[32+POOLD_SIZE*POOLD_CNT];	/*# プールＤエリア #*/
BYTE		RAM_POOLE[32+POOLE_SIZE*POOLE_CNT];	/*# プールＥエリア #*/
BYTE		RAM_POOLF[32+POOLF_SIZE*POOLF_CNT];	/*# プールＦエリア #*/
BYTE		RAM_POOL10[32+POOL10_SIZE*POOL10_CNT];	/*# プール１０エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL11[32+POOL11_SIZE*POOL11_CNT];	/*# プール１１エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL12[32+POOL12_SIZE*POOL12_CNT];	/*# プール１２エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL13[32+POOL13_SIZE*POOL13_CNT];	/*# プール１３エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL14[32+POOL14_SIZE*POOL14_CNT];	/*# プール１４エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL15[32+POOL15_SIZE*POOL15_CNT];	/*# プール１５エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL16[32+POOL16_SIZE*POOL16_CNT];	/*# プール１６エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL17[32+POOL17_SIZE*POOL17_CNT];	/*# プール１７エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL18[32+POOL18_SIZE*POOL18_CNT];	/*# プール１８エリア #*/	//2000.07 V2.1追加
BYTE		RAM_POOL19[32+POOL19_SIZE*POOL19_CNT];	/*# プール１９エリア #*//* 圧縮LU */
BYTE		RAM_POOL1A[32+POOL1A_SIZE*POOL1A_CNT];	/*# プール１Aエリア  #*//* 圧縮LU */
BYTE		RAM_POOL1B[32+POOL1B_SIZE*POOL1B_CNT];	/*# プール１Bエリア  #*//* 圧縮LU */
//圧縮LU対応。DSPリアルタイムコマンド用。2003.09.03
BYTE		RAM_POOL1C[32+POOL1C_SIZE*POOL1C_CNT];	/*# プール１Cエリア  #*/
BYTE		RAM_POOL1D[32+POOL1D_SIZE*POOL1D_CNT];	/*# プール１Dエリア  #*/
BYTE		RAM_POOL1E[32+POOL1E_SIZE*POOL1E_CNT];	/*# プール１Eエリア  #*/
BYTE		RAM_POOL1F[32+POOL1F_SIZE*POOL1F_CNT];	/*# プール１Fエリア  #*/



/*# タスク用スタック #*/
//#ifdef	MAKE_IPL
BYTE		TASK_STACK[	sup_Stack_size
					   +fusion_rcv_Stack_size
					   +fusion_tmr_Stack_size
					   +ipl_Stack_size
//RESERVE
//					   +sio_Stack_size
					  ];

//#else
//#ifdef	MENTE
BYTE		TASK_STACK[	sup_Stack_size
						+lumng_Stack_size
						+doep_Stack_size
						+tmr_Stack_size
						+fusion_rcv_Stack_size
						+fusion_tmr_Stack_size
						+mnt_Stack_size
					  ];
//#else	//#ifdef	MENTE
BYTE		TASK_STACK[	sup_Stack_size
						+dpisr_main_Stack_size
						+lumng_Stack_size
						+doep_Stack_size
						+h323_Stack_size
						+clkd_Stack_size
						+crintctl_Stack_size
						+tftp_Stack_size
						+tmr_Stack_size
//						+door_main_Stack_size//2004.04 IP-CS
						+dspmng_Stack_size
						+fusion_rcv_Stack_size
						+fusion_tmr_Stack_size
//						+almmng_Stack_size//2004.04 IP-CS
//						+bootpsrv_Stack_size//2004.04 IP-CS
//						+tftpsrv_Stack_size//2004.04 IP-CS
//						+sioctl_Stack_size//2004.04 IP-CS
					/* 圧縮LU */
						+lu_log_Stack_size /* */
//						+fcl_mng_Stack_size//2004.04 FAX削除IP-CS
//						+fcl_t30_Stack_size
//						+fcl_t38_Stack_size
//						+fcl_mdm_Stack_size
//						+fcl_acc1_Stack_size
//						+fcl_acc2_Stack_size
//						+fcl_acc3_Stack_size
//						+fcl_acc4_Stack_size
//						+fcl_acc5_Stack_size
//						+fcl_acc6_Stack_size
//						+fcl_acc7_Stack_size
//						+fcl_acc8_Stack_size
//						+fcl_con1_Stack_size
//						+fcl_con2_Stack_size
//						+fcl_con3_Stack_size
//						+fcl_con4_Stack_size
//						+fcl_con5_Stack_size
//						+fcl_con6_Stack_size
//						+fcl_con7_Stack_size
//						+fcl_con8_Stack_size
						//圧縮LU対応。DSPコマンドタスクスタック追加。2003.08.25
						+dspcommand0_Stack_size

//						+dspcommand1_Stack_size//2004.04 IP-CS
					  ];
//#endif	//#ifdef	MENTE
//#endif	//MAKE_IPL

/*# ＭＩＫログエリア #*/
S_MIKLOG	miklog;
BYTE		MIK_LOG_BUF[16*MIK_LOG_MAX];	  /*# ＭＩＫログエリア #*/
#endif /* IPCS_V4 <PD><DEL-END> */


#ifndef V2_1_IPL_EXCLUSIVE	//IPLはV2専用化する
/*# ＤＳＰ状態管理使用エリア #*/
DSPMNG_PASSCTL	dspmng_passctl[CNMAX];			/*# パス制御用メモリ #*/
DSPMNG_TONECTL	dspmng_tonectl[CNMAX];			/*# トーン制御用メモリ #*/
//DSPMNG_PASSCTL	dspmng_passctl[LU_CHANNEL_MAX];			/*# パス制御用メモリ #*/
//DSPMNG_TONECTL	dspmng_tonectl[LU_CHANNEL_MAX];			/*# トーン制御用メモリ #*/
DSPMNG_SHFTCTL	dspmng_shftctl;					/*# シフト監視用メモリ #*/

/*# ＤＳＰ割り込み処理使用エリア #*/
//DSPINT_VOICECTL	dspint_voicectl[LU_CHANNEL_MAX];			/*# 音声制御用メモリ #*/
//DSPINT_SCANCTL	dspint_scanctl[LU_CHANNEL_MAX];			/*# 監視制御用メモリ #*/
DSPINT_VOICECTL	dspint_voicectl[CNMAX];			/*# 音声制御用メモリ #*/
DSPINT_SCANCTL	dspint_scanctl[CNMAX];			/*# 監視制御用メモリ #*/

//RCV_RTP_FRM		rcv_rtp_buf[LU_CHANNEL_MAX];				/*# RTP受信音声バッファ #*/	// 99.10.21
RCV_RTP_FRM		rcv_rtp_buf[CNMAX];				/*# RTP受信音声バッファ #*/	// 99.10.21

//BYTE	hory0[HORYSIZE];						/*# ダウンロードエリア #*/		//2001.01.16 V2.2追加	// 2001.08 V2.3 変更
//BYTE	hory1[HORYSIZE];						/*# 保留音 #*/					//2001.01.16 V2.2変更	// 2001.08 V2.3 変更
//BYTE	hory2[HORYSIZE];						/*# デッドナンバートーキ #*/	//2001.01.16 V2.2変更	// 2001.08 V2.3 変更


#endif //ifndef V2_1_IPL_EXCLUSIVE	//IPLはV2専用化する



S_ASIDAT	asi_data;
S_LUNUM_DAT	lun_data;							/*# ＬＵ番号通知データ #*/

/*# タイマ関連 #*/
BYTE	tmr_entry_5ms;
BYTE	tmr_entry_50ms;
BYTE	tmr_entry_1s;

BYTE	tmr_cnt_5ms;							//2000.10.12 V2.1DEBUG
BYTE	tmr_cnt_50ms;							//2000.10.12 V2.1DEBUG
BYTE	tmr_cnt_1s;								//2000.10.12 V2.1DEBUG

TMR_TMCB	tmr_tmcb[255];

#ifdef LU_DBG
/*# デバッグ用ワークエリア #*/
DBG			dpdbg_work;							/*# デバッグ用ワーク #*/
#endif

DWORD	pool_get_err[COM_POOL_MAX];				/*# プール取得ＮＧログ #*/


KM_INF			km_inf[2];						/* LANC部ログ情報 */
unsigned long	km_txerr_sts_no[2];				/* 送信ステータス最新No. */	
unsigned long	km_rxerr_sts_no[2];				/* 受信ステータス最新No. */	
unsigned long	km_txerr_sts_area[2][256];		/* 送信ステータスログ領域 */
unsigned long	km_rxerr_sts_area[2][256][2];	/* 受信ステータスログ領域 */


#ifdef RATE_CPU_EXE
	RATE_CPU_COUNT	rate_cpu;
#endif


#ifndef	MAKE_IPL
//音声受信エラー確認用	2001.03.12
#ifdef RTP_SEQ_ERR_LOG
RTP_SEQ_LOG	rtp_seq_errlog;
#endif //RTP_SEQ_ERR_LOG

#ifdef RTP_BUF_ERR_LOG
RTP_BUF_LOG	rtp_buf_errlog;
#endif //RTP_BUF_ERR_LOG

#ifdef RTP_SEQ_ERR_TIME
SEQ_NG_TIME_LOG	seq_ng_log;
#endif //RTP_SEQ_ERR_TIME

LU_LOG	lu_log_inf;								/*# ＬＵログ情報 #*/	// 2001.03.15追加

#ifdef		PB_NG_DEBUG	
PB_LL_DBG	pb_dgt_chk;
#endif

#ifdef MAC_RESET_LOG
MAC_RST_LOG	mac_reset_log;
#endif //MAC_RESET_LOG

#endif	//MAKE_IPL

// アドレス情報（メモリダンプ等のため）---------------
#ifndef	MAKE_IPL
//extern KM_INF	km_inf[2];
//extern		CLKD_DATA	clkd_data;/* IPCS_V4 <PD><DEL> */
extern	LU_BOOT_DAT	lu_boot_data;
#ifdef		OAK_ORD_DEBUG
extern	ord_cnt;
extern	ord_dat;
#endif		//OAK_ORD_DEBUG

extern	BYTE	hory0[HORYSIZE];						/*# ダウンロードエリア #*/
extern	BYTE	hory1[HORYSIZE];						/*# 保留音 #*/
extern	BYTE	hory2[HORYSIZE];						/*# デッドナンバートーキ #*/

#if 0 /* IPCS_V4 <PD><DEL-STA> */
const S_ADDR	addr_tbl[]={
//       0123456789abcdef
		"com_data"        , &com_data         , sizeof(com_data),
		"__lu_download"   , &com_data.lu_download , sizeof(com_data.lu_download),
		"__io_bz_ctrl"    , &com_data.io_bz_ctrl , sizeof(com_data.io_bz_ctrl),
		"com_flsdata"     , &com_flsdata      , sizeof(com_flsdata),
		"com_flsdata_sts" , &com_flsdata_sts  , sizeof(com_flsdata_sts),
		"com_tos_sd"      , &com_tos_sd       , sizeof(com_tos_sd),
		"ztcbtbl"         , &ztcbtbl          , sizeof(ztcbtbl),
		"zecbtbl"         , &zecbtbl          , sizeof(zecbtbl),
		"zrdytbl"         , &zrdytbl          , sizeof(zrdytbl),
		"RAM_POOL0"       , &RAM_POOL0        , sizeof(RAM_POOL0),
		"RAM_POOL1"       , &RAM_POOL1        , sizeof(RAM_POOL1),
		"RAM_POOL2"       , &RAM_POOL2        , sizeof(RAM_POOL2),
		"RAM_POOL3"       , &RAM_POOL3        , sizeof(RAM_POOL3),
		"RAM_POOL4"       , &RAM_POOL4        , sizeof(RAM_POOL4),
		"RAM_POOL5"       , &RAM_POOL5        , sizeof(RAM_POOL5),
		"RAM_POOL6"       , &RAM_POOL6        , sizeof(RAM_POOL6),
		"RAM_POOL7"       , &RAM_POOL7        , sizeof(RAM_POOL7),
		"RAM_POOL8"       , &RAM_POOL8        , sizeof(RAM_POOL8),
		"RAM_POOL18"      , &RAM_POOL18       , sizeof(RAM_POOL18),
		"TASK_STACK"      , &TASK_STACK       , sizeof(TASK_STACK),
		"dspmng_passctl"  , &dspmng_passctl   , sizeof(dspmng_passctl),
		"dspmng_tonectl"  , &dspmng_tonectl   , sizeof(dspmng_tonectl),
		"dspmng_shftctl"  , &dspmng_shftctl   , sizeof(dspmng_shftctl),
		"dspint_voicectl" , &dspint_voicectl  , sizeof(dspint_voicectl),
		"dspint_scanctl"  , &dspint_scanctl   , sizeof(dspint_scanctl),
		"hory0"           , &hory0            , sizeof(hory0),
		"hory1"           , &hory1            , sizeof(hory1),
		"hory2"           , &hory2            , sizeof(hory2),
		"lu_log_inf"      , &lu_log_inf       , sizeof(lu_log_inf),
		"km_inf[NW]"      , &km_inf[0]        , sizeof(km_inf[0]),
		"km_inf[PC]"      , &km_inf[1]        , sizeof(km_inf[1]),
		"clkd_data"       , &clkd_data        , 0,
		"lu_boot_data"    , &lu_boot_data     , sizeof(lu_boot_data)
#ifdef	RTP_SEQ_ERR_LOG
		,
		"rtp_seq_errlog"  , &rtp_seq_errlog   , sizeof(rtp_seq_errlog)
#endif	//	RTP_SEQ_ERR_LOG
#ifdef	RTP_BUF_ERR_LOG
		,
		"rtp_buf_errlog"  , &rtp_buf_errlog   , sizeof(rtp_buf_errlog)
#endif	//	RTP_BUF_ERR_LOG
#ifdef		OAK_ORD_DEBUG
		,
		"ord_cnt"         , &ord_cnt          , 0,
		"ord_dat"         , &ord_dat          , 0
#endif	//	OAK_ORD_DEBUG
#ifdef MAC_RESET_LOG
		,
		"mac_reset_log"   , &mac_reset_log    , sizeof(mac_reset_log)
#endif //MAC_RESET_LOG
#ifdef RTP_SEQ_ERR_TIME
		,
		"seq_ng_log"      , &seq_ng_log       , sizeof(seq_ng_log)
#endif //RTP_SEQ_ERR_TIME
		,		
		"fls_ca_data"     , &fls_ca_data      , sizeof(fls_ca_data)			// IP-CS(SS9100_PH1再開) 追加
};
#endif /* IPCS_V4 <PD><DEL-END> */
#endif	//MAKE_IPL

/* メモリ属性 */
XS_POOL_DAT pool_dat[COM_POOL_MAX] = {{POOL0_SIZE,POOL0_CNT,0}, {POOL1_SIZE,POOL1_CNT,0}, {POOL2_SIZE,POOL2_CNT,0}, {POOL3_SIZE,POOL3_CNT,0}, {POOL4_SIZE,POOL4_CNT,0},
                                      {POOL5_SIZE,POOL5_CNT,0}, {POOL6_SIZE,POOL6_CNT,0}, {POOL7_SIZE,POOL7_CNT,0}, {POOL8_SIZE,POOL8_CNT,0}, {CD_BLKSZ_EVT_DATA,CD_BLKCNT_EVT_DATA,0},
                                      {CD_BLKSZ_EVT,CD_BLKCNT_EVT,0}, {CD_BLKSZ_ORD,CD_BLKCNT_ORD,0}, {CD_BLKSZ_SND_IDATA,CD_BLKCNT_SND_IDATA,0}, {CD_BLKSZ_RCV_IDATA,CD_BLKCNT_RCV_IDATA,0},
	                                  {CD_BLKSZ_WAIT_CFRM,CD_BLKCNT_WAIT_CFRM,0}, {CD_BLKSZ_RCV_SIO_MSG,CD_BLKCNT_RCV_SIO_MSG,0}};
XS_POOL_ATR pool_atr_tbl[COM_POOL_MAX];

/* LU再開情報 */
LU_START lu_start;

/* タスク属性 */
XS_TASK_ATTR task_attr[COM_TASK_MAX_N] = 
   {{ON, 3, 95, 0, lu_mon_thread}, {ON, 3, 95, 0, tmr_thread},   {ON, 1, 95, 0, &cs_ctl},          {ON, 1, 95, 0, mda_thread},
   {ON, 3, 95, 0, lumng_thread},   {ON, 3, 95, 0, doep_r_thread},{ON, 3, 95, 0, doep_s_thread},   {ON, 3, 95, 0, downld},
   {ON, 2, 95, 0, mnt_thread},     {ON, 3, 95, 0, lu_log},       {ON, 3, 95, 0, support},         {OFF, 1, 95, 0, dspevt},
   {OFF, 1, 95, 0, dsp2lan},       {OFF, 1, 95, 0, dsp2lan},     {OFF, 1, 95, 0, cyc_ctl},        {OFF, 1, 95, 0, spro_ctl},
   {OFF, 1, 95, 0, lpb_ctl},       {OFF, 1, 95, 0, fpga_read},   {OFF, 1, 95, 0, writer}, {OFF, 1, 95, 0, fpga_write}};
/* タスク状態 */
XS_TASK_STATUS task_status[COM_TASK_MAX_N];

/* メッセージキュー属性 */
struct mq_attr lu_mq_attr_v = {0, LUMQ_MAXMSG, LUMQ_MSGSIZE, 0, {0,0,0,0}};
/* ECB属性 */
XS_ECB_STTR ecb_attr[COM_ECB_MAX] = {{"SUP_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"LUMNG_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"DOEP_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"MDA_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"TFTP_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"TMR_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"WRITER_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"DSPEVT_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"DSP2LAN_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"LSN2DSP_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"CSCTL_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"SPROCTL_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"FPGAWRI_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"MNT_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v},
									 {"LULOG_ECB", (O_RDWR | O_CREAT), (S_IRWXU | S_IRWXG | S_IRWXO), &lu_mq_attr_v}};
/* ECBディスクリプタ */
mqd_t ecb_mq_desc[COM_ECB_MAX];

/* キュー関連 */
/* キュー管理テーブル */
XS_QUE_CTL com_que_ctl[COM_QUE_MAX];

/* デバッグプリント関連 */
/* デバッグプリント管理テーブル */
XS_DBGPRINT_CTL dbgprint_ctl[COM_TASK_MAX_N];
XS_DBGPRINT_CTL com_dbgprint_ctl;

