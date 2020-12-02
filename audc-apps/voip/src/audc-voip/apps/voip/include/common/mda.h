/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       LUメインスレッドヘッダ                                          */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/* IPCS_V4 <PD><ADD-STA> */
#ifndef _MDA_H
#define _MDA_H

/*** システムヘッダの取り込み ***/

/*** ユーザ作成ヘッダの取り込み ***/

/*** 自ファイル内でのみ使用する#define マクロ ***/
#define DSP_MAX				0x01	//κ搭載DSP数//2004.04 IP-CS対応
#define	DSP_CHANNEL_MAX		0x04	//DSP管理チャネル数(4ch)
#define	IPCS_CH_MAX		0x03	//DSP管理チャネル数(3ch)//2004.04 IP-CS
    /*# 共通 #*/
#define    IPV4             0           /*# IPアドレスＶ４タイプ #*/
#define    IPADDRMAX        16          /*# IPアドレス最大数(BYTE) #*/
#define    IPV4NUM          4           /*# IPV4アドレス数(BYTE) #*/
#define    IPV6NUM          16          /*# IPV6アドレス数(BYTE) #*/

#define    TONE_PARAERR     4           /*# トーン制御要求パラメータエラー値 #*/
//#define    PASS_CTLERR      2           /*# パス制御要求制御エラー値 #*/
#define    PASS_CTLERR      4           /*# パス制御要求制御エラー値 #*/
#define    PASS_PASSERR     3           /*# パス制御要求方向エラー値 #*/

#define    HORY_CTLERR      2           /*# 保留音制御要求制御エラー値 #*/

#define    PBDATAMAX        48          /*# ＰＢデータ最大数 #*/

#define    DSPORD_MAX       0x20       /*# ＤＳＰ管理へのオーダコード最大値 #*/


/*# エラーコード #*/
#define    DSPMNG_MAIN      0
#define    DSPMNG_PASSSUB   1
#define    DSPMNG_ORDERR    2
#define    DSPMNG_PARAERR   3


/*# タイマ値算出 #*/
#define    T5MS             5           /*# タイマ５ｍｓ単位 #*/
#define    T10MS            10          /*# タイマ１０ｍｓ単位 #*/
#define    T20MS            20          /*# タイマ２０ｍｓ単位 #*/
#define    T50MS            50          /*# タイマ５０ｍｓ単位 #*/
#define    T100MS           100         /*# タイマ１００ｍｓ単位 #*/




    /*# 通話パス状態 #*/
#define    DISCON           0x00       /*#  #*/
#define    PASS             0x01        /*# 通常パス接続 #*/
#define    MULTI            0x02        /*# マルチパス接続 #*/


    /*# マルチキャスト指定 #*/
#define    MULT_OFF         0x00        /*# 通常パス #*/
#define    MULT_ON          0x01        /*# マルチパス #*/


    /*# IPチェック識別子 #*/
#define    SNDIP            0x01        /*# 送話IPアドレス #*/
#define    RCVIP            0x02        /*# 受話IPアドレス #*/
#define    SNDMULTIP        0x03        /*# 送話マルチIPアドレス #*/
#define    RCVMULTIP        0x04        /*# 受話マルチIPアドレス #*/


    /*# エコーキャンセラ制御指示 #*/
#define    SNDPASS          0x01        /*# 送話パスのみ接続 #*/
#define    RCVPASS          0x02        /*# 受話パスのみ接続 #*/
#define    BOTHPASS         0x03        /*# 双方向パス接続 #*/
#define    ECHO_ON          0x04        /*# エコーキャンセラＯＮ #*/
//#define    ECHO_OFF         0x05        /*# エコーキャンセラＯＦＦ #*/
#define    PB_POSE          0x06        /*# ＰＢ（ポーズ） #*/
#define	   TONEPBSND		0x07

    /*# トーン監視方法変更指示 #*/
#define    SCN_CHECK        0x01        /*# 監視方法変更チェック #*/
#define    SCN_NEXT         0x02        /*# 次ＨＷｃｈチェック #*/
#define    SCN_STOP         0x03        /*# 監視停止チェック #*/



    /*# H323メッセージパラメータ：パス方向 #*/
#define    SNDDIR           0x00        /*# 送話パス接続指示 #*/
#define    RCVDIR           0x01        /*# 受話パス接続指示 #*/
#define    BOTHDIR          0x02        /*# 双方向パス接続指示 #*/


    /*# 送話データ送信周期 #*/
//#define    SEND_PERIOD_MIN  1           /*# 最小周期＝１（５ｍｓ） #*/
#define    SEND_PERIOD_MIN  2           /*# 最小周期＝１０ｍｓ #*/
#define    SEND_PERIOD_MAX  8           /*# 最大周期＝８（４０ｍｓ） #*/

#define		G729_SEND_PERIOD_MIN		1	/*# 最小周期＝１０ｍｓ #*/
#define		G729_SEND_PERIOD_MAX		16	/*# 最大周期＝１６０ｍｓ #*/


    /*# 送話ＰＡＤ最小判定値 #*/
//#define    PAD_MIN          0x0041      /*# 送話ＰＡＤ：－３０ｄＢ #*/
//圧縮LU対応 PAD値は即値に
#define    PAD_MIN          0xffffffe2      /*# 送話ＰＡＤ：－３０ｄＢ #*/


    /*# トーン送出要求内容 #*/
#define   TONE_SEND         0x00        /*# トーン送出 #*/
#define   PULS_SEND         0x01        /*# パルス送出 #*/
#define   TONE_CUT          0x02        /*# トーン停止 #*/
#define   TONE_CRINT        0x03        /*# 同期トーン送出 #*/


    /*# トーン送出状態 #*/
#define	 NOSND            0x00        /*# 非送信 #*/
#define    SNDTONE          0x01        /*# トーン送出 #*/
#define    SNDPULS          0x02        /*# パルス送出 #*/
#define    SNDHRY           0x03        /*# 保留音送出 #*/
#define    SNDPB            0x04        /*# ＰＢ送出 #*/


    /*# トーン送出パターン #*/
#define    TONE_PTN0        0x00        /*# パターン０ #*/
#define    TONE_PTN1        0x01        /*# パターン１ #*/
#define    TONE_PTN2        0x02        /*# パターン２ #*/
#define    TONE_PTN3        0x03        /*# パターン３ #*/


    /*# トーンＯＮ／ＯＦＦ時間種別 #*/
#define    SET_ON1          0x00        /*# タイマＯＮ１ #*/
#define    SET_ON2          0x01        /*# タイマＯＮ２ #*/
#define    SET_ON3          0x02        /*# タイマＯＮ３ #*/
#define    SET_OFF1         0x04        /*# タイマＯＦＦ１ #*/
#define    SET_OFF2         0x05        /*# タイマＯＦＦ２ #*/
#define    SET_OFF3         0x06        /*# タイマＯＦＦ３ #*/

    /*# トーン送出種別 #*/
#define    SINGLE           0x00        /*# 単周波数 #*/
#define    DUAL             0x01        /*# ２周波数 #*/
#define    FAM              0x02        /*# ＡＭ変調 #*/
#define    FK               0x03        /*# 交番 #*/


    /*# ＰＢ送出タイマ状態 #*/
#define    PBNUM_STS        0x01        /*# ＰＢ数字送出 #*/
#define    POSE_STS         0x02        /*# ポーズ送出 #*/

   /*# ＰＢデータ抽出マスク #*/
#define    PB_CHECK         0x80        /*# 数字／ポーズ判定用 #*/
#define    PB_MASK          0x0F        /*# 数字データ抽出用 #*/
#define    POSE_MASK        0x7F        /*# ポーズデータ抽出用 #*/


   /*# ＰＢ送出時の送話ＰＡＤミュート値 #*/
//#define    SND_MUTE         0x0000      /*#ＰＡＤ－ｂ：－８０ｄＢ #*/
//#define    SND_MUTE         0x0014      /*#ＰＡＤ－ｂ：－４０ｄＢ #*/	// 2000.01.22 変更
#define    SND_MUTE         0x0000      /*#ＰＡＤ－ｂ：－８０ｄＢ #*/	// 2000.01.25 変更（-40dBだと聞こえる！）


    /*# 使用中タイマID種別 #*/
#define    ID_1             0x01        /*# ＩＤ＿１ #*/
#define    ID_2             0x02        /*# ＩＤ＿２ #*/

    /*# 交番トーンフェーズ #*/
#define    TOME_PH_1             0x01        /*# フェーズ１ #*/
#define    TOME_PH_2             0x02        /*# フェーズ #*/


    /*# タイマ種別 #*/
#define    TONE_TIME        0x00        /*# トーンタイマ #*/
#define    DSPHEL_TIME		0x01        /*# DSPヘルス起動タイマ #*/
#define    DSPHELOUT_TIME	0x02        /*# DSPヘルス起動タイムアウトタイマ #*/
//IPCS IPstage対応(start) BTDT検出監視タイマ追加 2004.9.1
#define    BTDT_DET_TIME	0x04		//BTDT検出監視タイマ
//IPCS IPstage対応(end) 2004.9.1


//#define    PB_TIME          0x01        /*# ＰＢタイマ #*/
//#define    POSE_TIME        0x02        /*# ポーズタイマ #*/
//#define    SHFT_TIME        0x03        /*# シフトタイマ #*/
//#define    WAIT_TIME        0x04        /*# 監視有効待ちタイマ #*/


    /*# タイマ制御 #*/
#define    TONECTL          0x00        /*# トーンタイマ #*/
#define    PBCTL            0x01        /*# ＰＢタイマ #*/


/*# ＤＴ，ＢＴ，ＰＢ監視要求 #*/

#define    REQ_OFF          0x00        /*# 要求ＯＦＦ #*/
#define    REQ_ON           0x01        /*# 要求ＯＮ #*/

/*# ＤＴ＿ＯＮ／ＯＦＦ検出状態 #*/
#define    DET_ON           0x01        /*# ＯＮ検出 #*/
#define    DET_OFF          0x02        /*# ＯＦＦ検出 #*/


/*# ＢＴ／ＤＴ監視方法 #*/
#define    CONT_SCN         0x01         /*# 連続監視 #*/
#define    SHFT_SCN         0x02         /*# シフト監視 #*/
#define    EXT_SCN          0x03         /*# 外部監視（ＤＴ：ＵＳＡ） #*/


/*# ＰＢ監視時間（固定値）#*/
#define    PB_ONTIM        10           /*# ＰＢ＿ＯＮ固定監視時間：１００ｍｓ #*/
#define    PB_OFFTIM        5           /*# ＰＢ＿ＯＦＦ固定監視時間：５０ｍｓ #*/


/*# ＯＡＫ有効待ち時間値 #*/
#define    OAK_WAIT         0x01         /*# ５０ｍｓ #*/

/*# エコーキャンセラー制御種別 #*/
#define		ECHO_OFF		0x00
#define		ECHO_IN_ON		0x01
#define		ECHO_OUT_ON		0x02

   /*# パスエラー種別 #*/
#define    UDP_ERR          0x01        /*# ＵＤＰオープンエラー #*/
#define    T30S_TO          0x02        /*# ３０ｓ監視タイムアウト #*/


//圧縮LU対応。OAKへのオーダ無し。削除。2003.09.07
   /*# ＯＡＫオーダパターン #*/

#define    HORYPTN_HOLD     0x00        /*# 保留音（Ｖ１からのデフォルト値） #*/    //2001.01.17 V2.2追加
#define    HORYPTN_TALKY    0x01        /*# トーキー #*/                            //2001.01.17 V2.2追加

//圧縮LU対応。2003.09.07

#define		DIGIT_PAUSE		0x10

#define		VOICEtoFAXCHANGE	0

#define		FAXtoVOICECHANGE	1

#define		THRU_ON				1

#define		PASS_OVER_WRITE		0x00
#define		PASS_NEW_WRITE		0x01

//交番出力用トーン送出状態
#define		FK_TONE_ON1	0x01
#define		FK_TONE_ON2	0x02
#define		FK_TONE_ON3	0x03
#define		FK_TONE_OFF1	0x04
#define		FK_TONE_OFF2	0x05
#define		FK_TONE_OFF3	0x06

/* MDAゲイン基数 */
#define 	MDA_GAIN_RADIX	32
/* ペイロードタイプ */
#define		PT_G711u		0 /* G711μLaw */
#define		PT_G711a		8 /* G711ALaw */
#define		PT_G729		   18 /* G711μLaw */

/* ソケットインタフェース種別 */
#define		S_RTP_SND	1	/* RTP送信用 */
#define		S_RTP_RCV	2	/* RTP受信用 */
#define		S_RTCP_SND	17	/* RTCP送信用 */
#define		S_RTCP_RCV	18	/* RTCP受信用 */
#define		S_RTP_MASK	0x0001	/* RTP用マスク値 */
#define		S_RTCP_MASK	0x0002	/* RTCP用マスク値 */
#define		S_RTP_CLR_MASK	0xFFFE	/* RTP用マスク値 */
#define		S_RTCP_CLR_MASK	0xFFFD	/* RTCP用マスク値 */

#define		MDA_TIMEOUT_MAX		32    /* タイムアウトメッセージ分析テール最大要素数 */
#define		KIND_OF_STOPPER		0xff /* メッセージ分析テールストッパー */

//DSPエラー関連(初期化時の戻り含む)
#define	DSP_EVTRCV_ERROR			17
#define	DSP_SYSTEM_ERROR			18
#define	ASYNCHRONOUS_ERROR_LEVEL9	15
#define	ASYNCHRONOUS_ERROR_LEVEL7	14
#define	ASYNCHRONOUS_ERROR_LEVEL4	13
#define	DSP_READY_SILICON_ERROR		12
#define	DSP_READY_TDM_ERROR			11
#define	DSP_READY_MEMORY_ERROR		10
#define	DSP_READY_INNER_ERROR		9
#define	DSP_HRDY_TIMOUT_ERROR		8
#define	DSP_INIT_ERROR				7
#define	DSP_INIT_RPLY_ERROR			6
#define	DSP_INIT_CMD_ERROR			5
#define	DSP_INIT_RDY_TIMOUT			4
#define	DSP_INIT_MEMORY_ERROR		3
#define	HPI_HELTH_ERROR				2
#define	HPI_HELTH_TIMOUT_ERROR		1

#define	XC_TSC_TONEPLT_CNG		9

//DSCの状態
#define XC_FAULT 1
#define XC_NORMAL 0

//DSPのID
#define XC_DSP_ID 0

#define XC_ECHOREPLY_MAX 128
#define XC_AGC_GAIN_DEFAULT -15

#define XC_DSP_TYPE_VOCODER_OFF 0
#define XC_DSP_TYPE_G723_1_53   11//BV1.0-dsc-009
#define XC_DSP_TYPE_G723_1_63   12//BV1.0-dsc-009
#define XC_DSP_TYPE_G729A       2 //BV1.0-dsc-004
#define XC_DSP_TYPE_G711_U      21/* MV1.0=I(2) *//* PV1.0=I */
#define XC_DSP_TYPE_G711_A      22/* PV1.0=I */

//モード設定用
#define XC_DSP_MODE_DISABLE     0
#define XC_DSP_MODE_VOICE       1
#define XC_DSP_MODE_FAX         2

#define	BITOFF		0x00000000
#define BIT00		0x00000001
#define BIT01		0x00000002
#define BIT02		0x00000004
#define BIT03		0x00000008
#define BIT04		0x00000010
#define BIT05		0x00000020
#define BIT06		0x00000040
#define BIT07		0x00000080
#define BIT08		0x00000100
#define BIT09		0x00000200
#define BIT10		0x00000400
#define BIT11		0x00000800
#define BIT12		0x00001000
#define BIT13		0x00002000
#define BIT14		0x00004000
#define BIT15		0x00008000
#define BIT16		0x00010000
#define BIT17		0x00020000
#define BIT18		0x00040000
#define BIT19		0x00080000
#define BIT20		0x00100000
#define BIT21		0x00200000
#define BIT22		0x00400000
#define BIT23		0x00800000
#define BIT24		0x01000000
#define BIT25		0x02000000
#define BIT26		0x04000000
#define BIT27		0x08000000
#define BIT28		0x10000000
#define BIT29		0x20000000
#define BIT30		0x40000000
#define BIT31		0x80000000


/*** 自ファイル内でのみ使用する#define 関数マクロ ***/


/*** 自ファイル内でのみ使用するtypedef 定義 ***/
typedef void (*MdaFuncAddr)( INNER_MSG * );
/*# DSP管理用ストラクチャ #*/
typedef struct T_RXBUF{
	short max;
	short start;
	short stop;
} RXBUF;
typedef struct T_XMIT_DTMF_PARA
{
	short upper_level;
	short lower_level;
	short continuous_length;
	short space_length;
	short pause_length;
} XMIT_DTMF_PARA;
typedef struct T_DSPCHANNEL
{
	short mode;					//チャネルモード
	short type;					//パス状態
	short option;				//オプション設定フラグ
	short frame1_size;
	short dtmf_detect_length;	//DTMF検出判定用信号継続時間、追加
	XMIT_DTMF_PARA dtmf_para;	//送出DTMFパターンセーブエリア
	short calltone_band;		//検出器
	short calltone_on_time;		//呼制御トーンon時間
	short calltone_off_time;	//呼制御トーンoff時間
	short calltone_detect_time;	//呼制御トーン検出時間
	short PAD_Tx;				//PAD
	short PAD_Rx;				//PAD
	short faxtone_mode;			//FAXTONE検出種別
	short faxtone_ced_length;	//FAXトーン(CED)検出時間、追加
	short faxtone_ced_detect_status;
	short faxtone_pre_length;	//FAXトーン(プリアンブル)検出時間、追加
	short faxtone_pre_detect_status;
	short faxtone_cng_detect_status;
	short AGC_target_gain;		//AGCターゲットゲイン
	short xmit_DTMF;			//送出DTMFdigit
	short xmit_tone;			//送出トーンID
	short xmit_status;			//トーン状態
	RXBUF rxbuf;				//受信バッファ制御
	short tone_suppress;		//送信バッファ制御
} DSPCHANNEL;
typedef struct T_DSP_DATA
{
	char modulo;							//シリアル番号用
	char status;							//DSP状態
	short law;				//TDMポートのμ/A-law設定
	short loglevel;			//DSPのログレベル
	short password;			//アクセス番号
	DSPCHANNEL channel[DSP_CHANNEL_MAX];	//DSP_CHANNEL_MAX=4(0～3)
	int	helouttim_id;
	int debug;			//DSPデバッグフラグ
	short mod;			//ヘルスチェック用シリアル
	short change_palette_id;
	short tone_status[10];
	int init_error;
	char error_count;
	char cmd_snd;
	char errstat;
	char helseqng;
} DSP_DATA;

typedef struct {
	WORD	create_flag[2];		/* 生成フラグ(インデックス 0:送信用/1：受信用) */
	WORD	rtp_port[2];		/* RTPポート番号(インデックス 0:送信用/1：受信用) */
	WORD	rtcp_port[2];		/* RTCPポート番号(インデックス 0:送信用/1：受信用) */
	int		rtp_sd[2];			/* RTP用ソケットディスクリプタ(インデックス 0:送信用/1：受信用) */
	int		rtcp_sd[2];			/* RTCP用ソケットディスクリプタ(インデックス 0:送信用/1：受信用) */
} MDA_SOCKET_CTL_T;

/* メッセージ属性テーブル */
typedef struct {
	BYTE	kind;		/* 種別 */
	BYTE	dummy[3];
	MdaFuncAddr func_adr; /* 関数アドレス */
} MDAMSG_ATTR;

/*################################################*/
/*#          DSPコーデック管理エリア             #*/
/*################################################*/
typedef struct T_CODEC_INFO{
	short	len;
	short	msec;
}CODEC_INFO;

/*** 自ファイル内でのみ使用するenum
		タグ定義 ***/
/* MDA エラーコード */
typedef enum {
	MDA_ABORT_01= 0,		/* lan2dsp select異常時 */
	MDA_ABORT_02,			/* mda_fd_init socket#1異常時 */
	MDA_ABORT_03,			/* mda_fd_init socket#2異常時 */
	MDA_ABORT_04,			/* mda_fd_init bind#1異常時 */
	MDA_ABORT_05,			/* mda_fd_init bind#2異常時 */
	MDA_ABORT_06,			/* mda_fd_init bindポート番号オーバ#1 */
	MDA_ABORT_07,			/* mda_fd_init bindポート番号オーバ#2 */
	H323_ABORT_01 = 128,	/* h323_udpopen S_RTP_SND ソケット二重オープン */
	H323_ABORT_02,			/* h323_udpopen S_RTP_RCV ソケット二重オープン */
	H323_ABORT_03,			/* h323_udpopen S_RTCP_SND ソケット二重オープン */
	H323_ABORT_04,			/* h323_udpopen S_RTCP_RCV ソケット二重オープン */
	H323_ABORT_05,			/* h323_udpclose S_RTP_SND ソケット二重クローズ */
	H323_ABORT_06,			/* h323_udpclose S_RTP_RCV ソケット二重クローズ */
	H323_ABORT_07,			/* h323_udpclose S_RTCP_SND ソケット二重クローズ */
	H323_ABORT_08,			/* h323_udpclose S_RTCP_RCV ソケット二重クローズ */
	H323_ABORT_09,			/* h323_udpclose S_RTP_SND ソケットディスクリプタ不一致 */
	H323_ABORT_10,			/* h323_udpclose S_RTP_RCV ソケットディスクリプタ不一致 */
	H323_ABORT_11,			/* h323_udpclose S_RTCP_SND ソケットディスクリプタ不一致 */
	H323_ABORT_12,			/* h323_udpclose S_RTCP_RCV ソケットディスクリプタ不一致 */
	H323_ABORT_13,			/* h323 h323_evtal 戻り値異常*/
} MDA_ERROR_CODE;

/* IPCSメッセージ・キー */

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/

/*** ファイル内で共有するstatic 変数宣言 ***/

/*** static 関数宣言 ***/

/*** 変数定義 ***/
extern MDA_SOCKET_CTL_T mda_socket_ctl_table[CNMAX];  /* MDAソケット管理テーブル */
/*** プロトタイプ定義 ***/

#endif	/* _MDA_H */
/* IPCS_V4 <PD><ADD-END> */

