/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : LU                                                       #*/
/*#                                                                         #*/
/*#     日本語名 : LU共通デファイン定義ファイルREL                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/28 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
#include <arpa/inet.h> /* IPCS_V4 <PD><ADD> */
/*---------------------------------------------------------------------------*/
/*       作成ファイル種別用                                                  */
/*---------------------------------------------------------------------------*/

/*--- デバック／正式で切替必要な定義 ---*/

#ifndef V4_DEBUG                                                                                //makefileへ
//#define   V4_DEBUG                                                                                //makefileへ
#endif

#ifdef      V1_DEBUG            /* デバック時定義（正式版は未定義） */
#define         NON_HELCHK_TO       /*# 定義時ＣＣＵからのヘルスチェックＴ．Ｏ監視無効（試験時は常にＴ．Ｏ．無効） #*///2004.04 IP-CS debug
#define FPGA_CHK_DBG    //オンラインDBG用 fpgaコンフィグルート有効 IP-CS対応 2004.04
#else       //V4_DEBUG          /* デバック時未定義（正式版は定義） */
#define         CCU_HELCHK_START    // 2000.03.28 初期状態からヘルス監視実施（ソフト笹岡さん確認用）

#ifdef  MENTE
#define             NON_HELCHK_TO   /*# 定義時ＣＣＵからのヘルスチェックＴ．Ｏ監視無効（試験時は常にＴ．Ｏ．無効） #*/
#endif  //#ifdef    MENTE

#endif      //V4_DEBUG


/*--- システム更新用固定定義 ---*/
//#define   TEST_TEST_TEST      // 工場試験オーダ有効（工場試験用の場合定義する）   99.10.26
#ifdef      TEST_TEST_TEST
#ifndef         NON_HELCHK_TO
#define             NON_HELCHK_TO           /*# 定義時ＣＣＵからのヘルスチェックＴ．Ｏ監視無効（正式時はデファインしない） #*/
#endif          //NON_HELCHK_TO
#endif      //TEST_TEST_TEST

#define         V1_2_HARD           /*# Ｖ１．２ハード動作用 #*/    // 2000.05.11 追加
#define         REMAKE_PKG          /*# 再作ハード環境実行用 #*/    // 99.10.15
#define         EMA_PORT_CHG        /*# 直通切替(EMA)ポート３に変更ハード実行用 #*/ // 99.12.17
#define         CCU_HELCHK_START    // 2000.03.28 初期状態からヘルス監視実施（ソフト笹岡さん確認用）

#define         LU_RTP_SEQ          /*# RTPｼｰｹﾝｽﾁｪｯｸ有効フラグ #*/

#define         ALM_LMP     0x80    /*# 時間測定LEDＡＬＭランプ #*/
#define         OP_LMP      0x40    /*# 時間測定LEDＯＰランプ #*/

//初期設定専用ＰＯＲＴ０データ（マスタ／スレーブ共通）
#define         ALM_LMP_ON  0x004f  /*# アラームランプＯＮ、ＰＰリセット、ＥＣ機能ＯＦＦ #*/    //ＩＰＬにはＥＣ関係ないが紛らわしくないように
#define         ALM_LMP_OFF 0x00cf  /*# アラームランプＯＦＦ、ＰＰリセット、ＥＣ機能ＯＦＦ #*/  //ＩＰＬにはＥＣ関係ないが紛らわしくないように


#define         VO_FLOW             /*# 音声フロー制御有効 #*/
//#define       VO_FLOW_CNT     5   /*# 音声フロー制御有効時の閾値 #*/
#define         VO_FLOW_STOP    5   /*# 音声停止時の閾値 #*/
#define         VO_FLOW_START   2   /*# 音声開始時の閾値 #*/
//#define           VO_SEQ_NG       16  /*# シーケンスＮＧの範囲値 #*/
#define         VO_SEQ_NG       10  /*# シーケンスＮＧの範囲値 #*/

/*# NON_DOEP_SEQCHKとNON_DOEP_SEQCHK_RETRYはどちらか１つを有効にすること #*/
////#define     NON_DOEP_SEQCHK     /*# 定義時ＣＣＵシーケンスチェック無効 #*/
#define     NON_DOEP_SEQCHK_RETRY   /*# 定義時ＣＣＵイベント無限再送 #*/ // 正式時もデファイン

//#define       DSP_SEARCH_ORD

#ifndef     OK
#define         OK              0x00
#endif      //OK

#define     CLR                 0x00
/*
********  追加：V3-IPCS-001 2008.12.04  --- Minimam Delay Time 入替え条件変更に伴い  *********
*/
#define		DELAY_TIME_FULL		0xFFFFFFFF      /* V03.03 遅延時間最大値               */

#define     NG                  0xFF
//#define     NULL                0x00 /* IPCS_V4 <PD><DEL> */
//#define       NULLP           (void*)(0)
#define     ON                  0x01
#define     OFF                 0x00
#define     CONT                0x02
#define     SYS_NG              0xFFFFFFFF

//圧縮LU対応。2003.09.16
#define XC_ON   0x01
#define XC_OFF  0x00

#define XC_OK   0x00
#define XC_NG   0x01

/* ＮＵＬＬ定義 */
#define         XC_NULLP        0          /* ＮＵＬＬポインタ */
#define         XC_NULL         0          /* ＮＵＬＬ */

#define     NON                 0xFF

typedef     unsigned char       BYTE;   /*# BYTE  = 8bit #*/
typedef     unsigned short      WORD;   /*# WORD  = 16bit #*/
typedef     unsigned long       DWORD;  /*# DWORD = 32bit #*/
/* IPCS_V4 <PD><ADD-STA> */
typedef     unsigned char 	uint_8;
typedef     unsigned short	uint_16;
typedef     unsigned long       uint_32;
typedef     char       int_8;
typedef     short      int_16;
typedef     long       int_32;

/* IPCS_V4 <PD><ADD-END> */
/* 圧縮LU */
#ifndef LONG
#define LONG       long            /* 32ビット符号有り整数
                                                     -2147483648～2147483647 */
#endif

#ifndef  ULONG
#define  ULONG     unsigned long   /* 32ビット符号無し整数 0～4294967295 */
#endif
#ifndef  USHORT
#define  USHORT    unsigned short  /* 16ビット符号無し整数 0～65535 */
#endif
#ifndef  UCHAR
#define  UCHAR     unsigned char   /* 8ビット符号無し整数 0～255 */
#endif
#ifndef  CHAR
#define  CHAR      char            /* 8ビット符号有り整数 -128～127 */
#endif

#ifndef  SHORT
#define  SHORT    short  /* 16ビット符号無し整数 0～65535 */
#endif

#ifndef  INT
#define  INT       int             /* 32ビット符号有り整数
                                                     -2147483648～2147483647 */
#endif
#ifndef  UINT
#define  UINT      unsigned int    /* 32ビット符号無し整数 0～4294967295 */
#endif
#ifndef  BOOL
#define  BOOL      int             /* 32ビット符号無し整数 0～4294967295 */
#endif
/* 圧縮LU */


// ＳＬＣバグ対策のためＳＬＣパッケージ認識用 2000.01.18 追加
#define     SLC_PKG_ID      0x12

/*---  V2.1LUファーム追加（START）  ---*/

//#define       V2_EC_FORCE_CUT                 /*# エコーキャンセル設定強制ＯＦＦ #*/                              //2001.02.12 V2.2追加

#ifdef      MAKE_IPL
#define     V2_1_IPL_EXCLUSIVE              /*# IPL処理V2.1専用化 #*/                                           //2000.09 V2.1追加
#define     IPL_FLASH_NOWRITE               /*# IPL(FROM)処理でのFLASH書き込み(コマンド含む)抑止 #*/            //2000.12.05 V2.1追加
#endif

#define     V2_1_IPL_SLVNODELAY             /*# IPL処理スレーブκ一斉ＢＯＯＴ #*/                                           //2000.09 V2.1追加

#define     V2_1_PHY_LINKUPSCAN             /*# PHYの割り込み発生不良対応（リンクアップスキャン方式盛り込み） #*/

//#define       BOOT_SAVE_ONLINE            /*# オンラインファイル側でＢＯＯＴ情報をセーブする #*/              //2000.07 V2.1追加
#define         BOOT_MAKE_ONLINE            /*# オンラインファイル側でＢＯＯＴ情報を作成する #*/                //2000.11.28 V2.1DEBUG
//#define       V2_1_IPL_BEFORE             /*# Ｖ２．１以前のＩＰＬ使用（Ｖ２．１ＩＰＬ開発以降は削除） #*/    //2000.07 V2.1追加
//#define       V2_1_DSP2CH_INI4CH          /*# ２ＣＨＤＳＰで４ＣＨ分初期化 #*/

//#define       V2_1_DBG_2CHDSPSTART            /*# 評価試験ROM用：２ｃｈＤＳＰスタート #*/
//#define       V2_1_DBG_4CHDSPFIX              /*# 評価試験ROM用：４ｃｈＤＳＰ固定 #*/

//#define           RFCD_TRACE

//音声受信エラー確認用  2001.03.12
#define RTP_SEQ_ERR_LOG
#define RTP_BUF_ERR_LOG

//#define   RTP_SEQ_ERR_TIME                    // デバッグ用   2001.04.26

//#define   SYSERR_PKT_SND                      /*# シスエラー時、マルチキャストパケットで情報送信（デバッグ用） #*/    // 2001.03.23 追加
//#define   MAC_RESET_LOG                       /*# ＭＡＣハングリセットログ（通常未定義） #*/                          // 2001.04.16 追加
//#define   MAC_FIFO_LOG                        /*# ＭＡＣＦＩＦＯログ（通常未定義） #*/                                // 2001.04.16 追加
//#define   KMAC_DBG_LOG                        /*# ＭＡＣデバッグログ（通常未定義） #*/                                // 2001.06.13 追加


#ifdef      V2_1_DBG
#define     V2_DBG_POOL0_DET                /*# POOL0解析用構造体定義モード #*/
//#define       V2_1_DBG_TFTPOVER
//#define       V2_1_DBG_IPADDR             /*# デバック用：マシンＩＰアドレス #*/
#define     V2_1_DBG_FANALM             /*# デバック用：ファンアラーム制御 #*/
#define     V2_1_DBG_MACADDR            /*# デバック用：ＭＡＣアドレス #*/
//#define       V2_1_DBG_BUFFERCNT          /*# デバック用：バッファ面指定時間数 #*/
//#define       V2_1_DBG_PGEINNFO           /*# デバック用：プール取得エラー詳細情報収集 #*/
//#define       V2_1_DBG_TIMEEXE            /*# デバック用：タイマ実行時間測定 #*/
//#define       V2_1_DBG_TFTPSENDEXE            /*# デバック用：TFTPパケット送信実行時間測定 #*/
//#define       V2_1_DBG_DOEPEXE            /*# デバック用：実行時間測定 #*/
//#define       V2_1_DBG_CRINTEXE           /*# デバック用：実行時間測定 #*/
//#define       V2_1_DBG_FUSIONHEAPWAIT     /*# デバック用：FUSIONヒープ取得待ち(LEDもONすること) #*/
//#define       V2_1_DBG_CRINTCNT           /*# デバック用：CRINT実行回数 #*/
//#define       V2_1_DBG_CLKDCNT            /*# デバック用：CRINT実行回数 #*/

//#define       V2_1_DBG_INTPHY             /*# デバック用：ＰＨＹ割り込み強制コール #*/
//#define       V2_1_DBG_INTMAC             /*# デバック用：ＭＡＣ割り込み強制コール #*/
#ifdef  MAKE_IPL
//#define       V2_1_DBG_INTDSP             /*# デバック用：ＤＳＰ割り込み強制コール #*/
#else
//#define           V2_1_DBG_INTDSP             /*# デバック用：ＤＳＰ割り込み強制コール #*/
//#define       LSI_DOWN_CHECK                  /*#  #*/    //2001.02.26 V2.2追加
#endif
//#define           V2_1_DBG_SIOINTCHK          /*# デバック用：ＳＩＯ割り込みチェック #*/
//#define       V2_1_DBG_LUNUM              /*# デバック用：ＬＵ番号強制設定 #*/

//#define       V2_1_DBG_ECHOEX             /*# デバック用：ＥＣＨＯキャンセルボード実装 #*/
//#define       V2_1_DBG_DBGLED             /*# デバック用：ＤＢＧ－ＬＥＤ #*/
//#define       V2_1_DBG_IPL                /*# デバック用：ＩＰＬ #*/
//#define       V2_1_DBG_SIONG              /*# デバック用：ＳＩＯ通信ＮＧ #*/
//#define       V2_1_DBG_UNISELFOK          /*# UNI CAST 自分への送信抑止停止 #*/
//#define       V2_1_DBG_LUMNGSYNCSND       /*# デバック用：LUMNG SYNC SEND制御 #*/
//#define       V2_1_DBG_UNIINTFULL         /*# デバック用：LUMNG SYNC SEND制御 #*/
//#define       V2_1_DBG_UNISELFSTOP        /*# デバック用：UNI CAST 自分への送信抑止 #*/
//#define       V2_1_DBG_CRINTBZOFF         /*# デバック用：ＣＲＩＮＴスタート時のブザーＯＦＦ #*/
//#define       V2_1_DBG_UNIFULL            /*# デバック用：ユニキャスト相手ＦＵＬＬ #*/
//#define       V2_1_DBG_TFTPSRVRTY         /*# デバック用：TFTPSRV送信リトライ #*/
//#define       V2_1_DBG_TFTPSRV1PKTJ           /*# デバック用：TFTPSRV１パケット512bytejust送信 #*/
//#define       V2_1_DBG_TFTPSRV1PKTU           /*# デバック用：TFTPSRV１パケット512byteunder送信 #*/
//#define       V2_1_DBG_CRINTADDR          /*# デバック用：ＣＲＩＮＴとＣＬＫＤアドレスの切り分け情報 #*/
//#define       V2_1_DBG_TFTPEX             /*# デバック用：ＴＦＴＰパケット外出 #*/
#endif

/*---  V2.1LUファーム追加（END）  ---*/



/*###########################################################################*/
/*#                                                                         #*/
/*#                               システム定数                              #*/
/*#                                                                         #*/
/*###########################################################################*/
#define     VO_1FRM_BYTE    40              /*# 音声１フレームサイズ(Byte) #*/
#define     VO_1FRM_WORD    20              /*# 音声１フレームサイズ(WORD) #*/
#define     VO_1FRM_DWORD   10              /*# 音声１フレームサイズ(DWORD) #*/
#define     VO_RTP_FRM_MAX  38              /*# ＲＴＰ音声フレーム数MAX(5ms単位) #*/
//#define       VO_BUF_FRM_MAX  150             /*# 受信バッファ音声フレーム数MAX(5ms単位) #*/
//#define       VO_BUF_FRM_MAX  200             /*# 受信バッファ音声フレーム数MAX(5ms単位) 10ms100面 #*/    // 2000.01.16 変更
#define     VO_BUF_FRM_MAX  400             /*# 受信バッファ音声フレーム数MAX(5ms単位) 10ms200面 #*/    // 2000.01.16 変更      // 2000.09 V2.1変更

//圧縮LU対応。音声1フレームバイト数追加。2003.09.07
#define G723L_1FRM_BYTE     20
#define G723H_1FRM_BYTE     24
#define G729_1FRM_BYTE      10
#define G711U_1FRM_BYTE     80
#define G711A_1FRM_BYTE     80



#define     VO_SLT_U        0xffffffff      /*# 無音パターンデータ：ｕ－ｌａｗ #*/
#define     VO_SLT_A        0x55555555      /*# 無音パターンデータ：Ａ－ｌａｗ #*/
#define     VO_SLTPTN_U     0               /*# 無音パターン指定：ｕ－ｌａｗ #*/
#define     VO_SLTPTN_A     1               /*# 無音パターン指定：Ａ－ｌａｗ #*/

#define     CODEC_U         0               /*# ＣＯＤＥＣ：μ－ｌａｗ指定 #*/
#define     CODEC_A         1               /*# ＣＯＤＥＣ：Ａ－ｌａｗ指定 #*/

#define     CNMAX           4               /*# 最大CN数 #*///2004.04 IP-CS変更
#define     CNMAX_IPCS          3               /*# IPCS実最大CN数 #*///2004.10 SS9100 


#define     COM_NOMAL       0           /*# 無変換転送（WORD転送時に使用）#*/
#define     COM_CHANGE      1           /*# 変換転送（WORD転送時に使用）#*/

#ifdef  NO_BOOTP_USE
#define     LU_IP                   0xc0a80164      // 192.168.1.100
#define     CCU_IP                  0xc0a801e0      // 192.168.1.222
#define     DEFAULT_MASK            0xffffff00
#define     CCU_IP1                 0xe001a8c0      // 192.168.1.222
#define     CCU_IP2                 0xe101a8c0      // 192.168.1.222
#endif  // NO_BOOTP_USE

#define         DOEP_CCU_PORT       0x30c0          // 49200
#define         DOEP_MY_PORT        0x30c0          // 49200

#define         LU_LOG_PORT     0x32c0          // 49202

/*# マジッククッキー#*/
#define BOOTP_MAGIC     0x63825363
#define BOOTP_MAGIC_LE  0x63538263

/*# 国種別（Ｖ１） #*/
#define         LU_JAPAN        0x00
#define         LU_USA          0x01

//#define           LU_NUM_MAX      96              /*# 有効ＬＵ番号ＭＡＸ #*/
#define         LU_NUM_MAX      255             /*# 有効ＬＵ番号ＭＡＸ #*///2004.01 LU32B

#define         LU_NUM_DIAL_MAX 98              //2004.01 LU32B

#define     FAN_MSK 0x04                                    /*# 障害情報ﾚｼﾞｽﾀ bit2ﾏｽｸ用  #*/
#define     AC_MSK      0x18                                /*# 障害情報レジスタ bit3,4マスク用  #*/
#define     AC_OK       0x18                                /*# ＡＣ正常 #*/
#define     AC_OFF      0x00                                /*# 停電ＡＣオフ #*/
#define     AC_FAIL0    0x10                                /*# ０系異常 #*/
#define     AC_FAIL1    0x08                                /*# １系異常 #*/

#define     EMA_OK      0x0001                              /*# 通常（運用中） #*/
#define     EMA_NG      0x0000                              /*# 障害（直通切替） #*/

// ハードウェアバージョン
#define     E_V1_1_HARD 0x11                                /*# V1.1値（V1.1ハードは0xffに見えるが処理で強制変換する） #*/      // 2000.09 V2.1追加
#define     E_V1_2_HARD 0x12                                /*# V1.2値 #*/                                                      // 2000.09 V1.2変更
#define     E_V2_1_HARD 0x21                                /*# V2.1値 #*/                                                      // 2000.09 V2.1変更

#define     E_V1_0_KAPPA    0x0000                          /*# κＶ１ #*/  //2000.09 V2.1追加
#define     E_V2_0_KAPPA    0x0100                          /*# κＶ２ #*/  //2000.09 V2.1追加

// ＣＣＵバージョン
#define     E_V2_1_CCU  0x3231                              /*# CCUバージョン番号 #*/   // 2000.07 V2.1追加     2001.08 変更
#define     E_V2_2_CCU  0x3232                              /*# CCUバージョン番号 #*/
#define     E_V2_3_CCU  0x3233                              /*# CCUバージョン番号 #*/

//  同期制御方式定義 V1.2追加
#define     E_SYN_STOP  0                                   /*# 同期制御未確定 #*/  // 2000.05.22 V1.2追加
#define     E_MST_V1_1  1                                   /*# マスタＬＵ V1.1 #*/ // 2000.05.11 V1.2追加
#define     E_MST_V1_2  2                                   /*# マスタＬＵ V1.2 #*/ // 2000.05.11 V1.2追加
#define     E_BETUSEN1  3                                   /*# 別線同期１ #*/      // 2000.05.11 V1.2追加
#define     E_DOUKI1    4                                   /*# 同期制御１ #*/      // 2000.05.11 V1.2追加
#define     E_PACKET1   5                                   /*# パケット同期１ #*/  // 2000.05.11 V1.2追加
#define     E_BETUSEN2  6                                   /*# 別線同期２ #*/      // 2000.05.11 V1.2追加
#define     E_DOUKI2    7                                   /*# 同期制御２ #*/      // 2000.05.11 V1.2追加
#define     E_PACKET2   8                                   /*# パケット同期２ #*/  // 2000.10.19 V2.1追加

#define SOCKET_NG 0xffffffff /* IPCS_V4 <PD><ADD> */

/*# ＳＩＯ通信内部データフォーマット #*/
enum{
    SIOCTL_IN_VER,                  /*# ＳＩＯ通知バージョン（RESERVE） #*/     //LU32B
    SIOCTL_IN_LUNUM_H,              /*# ＬＵ番号上位 #*/    //LU32B
    SIOCTL_IN_LUNUM,                /*# ＬＵ番号 #*/
    SIOCTL_IN_VLAN,                 /*# ＶＬＡＮ－ＩＤ#*/
    SIOCTL_IN_CCUID,                /*# ＣＣＵ－ＩＤ #*/
    SIOCTL_IN_DIPSW,                /*# ＤＩＰＳＷ情報 #*/  //LU32B
    SIOCTL_IN_EXT,                  /*# 拡張情報 #*/    //LU32B
    SIOCTL_IN_SUV,                  /*# サバイバル情報 #*/  //LU32B 
    SIOCTL_IN_DATAMAX               /*# データ数最大値 #*/
};
#define SIOCTL_IN_BUFMAX    1024

/*# ＳＩＯ内部通信データフォーマット #*/
enum{
    SIOCTL_OUT_STX,                 /*# ＳＴＸ #*/
    SIOCTL_OUT_VER_10,              /*# ＳＩＯ通知バージョン（１０の位）（RESERVE） #*/     //LU32B
    SIOCTL_OUT_VER_1,               /*# ＳＩＯ通知バージョン（１の位）（RESERVE） #*/       //LU32B
    SIOCTL_OUT_LUNUM_1000,          /*# ＬＵ番号（１０００の位） #*/        //LU32B
    SIOCTL_OUT_LUNUM_100,           /*# ＬＵ番号（１００の位） #*/          //LU32B
    SIOCTL_OUT_LUNUM_10,            /*# ＬＵ番号（１０の位） #*/
    SIOCTL_OUT_LUNUM_1,             /*# ＬＵ番号（１の位） #*/
    SIOCTL_OUT_VLAN_10,             /*# ＶＬＡＮ－ＩＤ（１０の位）#*/
    SIOCTL_OUT_VLAN_1,              /*# ＶＬＡＮ－ＩＤ（１の位） #*/
    SIOCTL_OUT_CCUID_10,            /*# ＣＣＵ－ＩＤ（１０の位） #*/
    SIOCTL_OUT_CCUID_1,             /*# ＣＣＵ－ＩＤ（１の位） #*/
    SIOCTL_OUT_DIPSW_10,            /*# ＤＩＰＳＷ情報（１０の位） #*/  //LU32B
    SIOCTL_OUT_DIPSW_1,             /*# ＤＩＰＳＷ情報（１の位） #*/    //LU32B
    
    SIOCTL_OUT_EXT_10,              /*# 拡張情報（１０の位） #*///LU32B 
    SIOCTL_OUT_EXT_1,               /*# 拡張情報（１の位） #*///LU32B
    SIOCTL_OUT_SUV_10,              /*# サバイバル動作（１０の位） #*///LU32B   
    SIOCTL_OUT_SUV_1,               /*# サバイバル動作（１の位） #*///LU32B 
    
    SIOCTL_OUT_SUM0,                /*# サム値：０バイト目 #*/
    SIOCTL_OUT_SUM1,                /*# サム値：１バイト目 #*/
    SIOCTL_OUT_ETX,                 /*# ＥＴＸ #*/
    SIOCTL_OUT_DATAMAX              /*# データ数最大値 #*/
};
#define SIOCTL_OUT_BUFMAX   (SIOCTL_IN_BUFMAX*2)

#define     EXT_D01     0x0001  //2004.01 LU32B
#define     EXT_SS9100      0x0002      //0x0004    //2004.08 SS9100
#define     EXT_OTHERS      0x0004      //0x0008    //2004.08 SS9100

// CA切り替え情報デフォルト値：IP-CS(SS9100_PH1再開) 追加
#define     CA_DATA_SERV_ID         0x00            // 接続先識別子(プライマリ:0)
#define     CA_DATA_SURV_CHG_TIME   0x02            // サバイバル切り替えタイマ(120秒:2分)
#define     CA_DATA_SURV_RET_TIME   0x00            // サバイバル切り戻しタイマ(0秒)
#define     CA_DATA_CONNECT_TIME    0x00            // コネクション再接続タイマ(0秒)


/*###########################################################################*/
/*#                                                                         #*/
/*#                           メッセージ関連                                #*/
/*#                                                                         #*/
/*###########################################################################*/
/*# タスクID(タスクアタッチ用,その他タスク識別用) #*/
typedef enum {
		MON_ID = 0,				/*# WDT管理スレッド #*/
		TMR_ID,					/*# タイマ管理スレッド  #*/
		CSCTL_ID,				/*# CS管理スレッド #*/
		MDA_ID,					/*# 音声管理スレッド #*/
		LUMNG_ID,				/*# 装置管理スレッド #*/
		DOEPR_ID,				/*# オーダ受信スレッド #*/
		DOEPS_ID,				/*# イベント送信スレッド #*/
		DWL_ID,					/*# tftpサーバ管理スレッド #*/
		MNT_ID,					/*# 保守管理スレッド #*/
		LULOG_ID,				/*# ログ管理スレッド #*/
		SUP_ID,					/*# サポート機能スレッド #*/
		DSPEVT_ID,				/*# dspevt #*/
		DSP2LAN_ID,				/*# dsp2lan #*/
		LAN2DSP_ID,				/*# lan2dsp #*/
		CYCCTL_ID,				/*# sys_ctl #*/
		SPROCTL_ID,				/*# spro_ctl #*/
		LPBCTL_ID,				/*# lpb_ctl #*/
		FPGAR_ID,				/*# fpga_read #*/
		WRT_ID,					/*# writer #*/
		FPGAW_ID,				/*# fpga_write #*/
		COM_TASK_MAX_N,			/*# 最大タスクID数 #*/
		COM_ID = 0x80,			/*# 共通関数(タスクではない！！) #*/
		MAIN_ID,				/*# MAINスレッド(タスクではない！！) #*/
		SYS_ER_NORMAL = 0xFF	/*# 通常再開時システムエラーコード #*/
} COM_TASK_ID;

/* タスク状態 */
typedef enum {
	TASK_INACTIVE= 0,	/* 非起動中 */
	TASK_ACTIVE			/* 起動中 */
} COM_TASK_STATUS;
#define COM_TASK_MAX_N   20  /* 最大タスク数 */


/*# タスクECB(SND_MSG用) #*/
typedef enum {
	SUP_ECB = 0,   /* モニタタスク用 */
	LUMNG_ECB ,   /* LU管理タスク用 */
	DOEP_ECB ,    /* DOEP制御用 */
	MDA_ECB ,     /* メディア制御用 */
	TFTP_ECB ,    /* ＴＦＴＰ制御用 */
	TMR_ECB ,     /* タイマタスク用 */
	WRITER_ECB ,  /* TFTP-writerスレッド用 */
	DSPEVT_ECB ,  /* DSPエベント受信スレッド用 */
	DSP2LAN_ECB , /* DSP2LANスレッド用 */
	LAN2DSP_ECB , /* LAB2DSPスレッド用 */
	CSCTL_ECB ,   /* cs_ctlスレッド用 */
	SPROCTL_ECB , /* spro_ctlスレッド用 */
	FPGAWRI_ECB , /* fpga_writeスレッド用 */
	MNT_ECB ,     /* 保守用インタフェース用 */
	LULOG_ECB ,   /* ＬＵログ用 */
	COM_ECB_MAX   /* ECB数 */
} COM_ECB_NO;

    /*# メッセージ区分 #*/
#define     IN_MSG          0x01        /*# 内部メッセージ #*/
#define     LU_ODR          0x02        /*# ＬＵオーダ #*/
#define     LU_EVT          0x03        /*# ＬＵイベント #*/
#define     TIM_OUT         0x04        /*# タイムアウトメッセージ #*/

    /*# メッセージ種別：内部メッセージ #*/

#define     I_VOSTART       0x01        /*# 音声通信スタート #*/
#define     I_VOSTOP        0x02        /*# 音声通信ストップ #*/
#define     I_VOPAUSE       0x03        /*# 音声通信一時停止 #*/
#define     I_VORESTART     0x04        /*# 音声通信再開 #*/
#define     I_VODISKREP     0x05        /*# 音声通信切断通知 #*/
#define     I_VOSTOPRSP     0x06        /*# 音声通信ストップレスポンス #*/
#define     I_CRINT         0x07        /*# CRINT通知 #*/
#define     I_PLSEND        0x08        /*# パルス送出完了 #*/
#define     I_TONESTART     0x09        /*# トーン送出開始 #*/
#define     I_TONESTOP      0x0A        /*# トーン送出停止 #*/
#define     I_PBSNDSTART    0x0B        /*# PB送出開始 #*/
#define     I_PBSNDSTOP     0x0C        /*# PB送出停止 #*/
#define     I_BTDETSTART    0x0D        /*# BT監視開始 #*/
#define     I_BTDETSTOP     0x0E        /*# BT監視停止 #*/
#define     I_DTDETSTART    0x0F        /*# DT監視開始 #*/
#define     I_DTDETSTOP     0x10        /*# DT監視停止 #*/
#define     I_PBDETSTART    0x11        /*# PB監視開始 #*/
#define     I_PBDETSTOP     0x12        /*# PB監視停止 #*/
#define     I_CHSWCHG       0x13        /*# CHSW切り替え #*/
#define     I_DSPCTL        0x14        /*# DSP制御 #*/
#define     I_MUONPTNSET    0x15        /*# 無音パターン指定 #*/
#define     I_CODECCHG      0x16        /*# CODEC切り替え #*/
#define     I_PADCTL        0x17        /*# PAD/EC制御 #*/

//圧縮LU対応。DSP系MSG。2003.09.08
#define     I_DSPCMDSND             0x18    
#define     I_DEVICE_STATUS        0x19    /* DSPデバイス状態受信 */
#define     I_DSP_REALTIME_COMMAND  0x1a
#define     I_DSP_MODECHG           0x1b    /* DSP⇔FAX切替通知 */
#define     I_DSPSTART              0x1c    /* DSP開始通知 */
#define     I_PASSCHGEND            0x1d    /* パス切替完了 */
#define     I_DSPERROR              0x1e    /* パス切替完了 */
//2003.10.02追加
#define     I_DTMFCOMPLETE          0x1f    /* DTMF送出完了 */


#define     I_HELCHKREQ     0x20        /*# ヘルスチェック要求 #*/
#define     I_HELCHKRSP     0x21        /*# ヘルスチェック結果 #*/
#define     I_LINKUP        0x22        /*# DOEPリンク制御 #*/
#define     I_DOEPRCV       0x23        /*# DOEP受信 #*/

#define     I_LINKUPCMP     0x40        /*# DOEPリンク制御結果 #*/
#define     I_CLKDOBS       0x41        /*# CLKD障害通知要求 #*/
#define     I_FANOBS        0x42        /*# FAN障害通知要求 #*/
#define     I_SYNCMP        0x43        /*# CLKD同期合わせ完了通知要求 #*/  // 2000.05.13 追加
#define     I_DSPCMP        0x44        /*# DSP起動完了通知 #*//* IPCS_V4 <PD><ADD> */

//#define       I_DPRAMRD       0x50        /*# DPRAMデータ読み出し要求 #*/
#define     I_PGDLCMP       0x51        /*# プログラムダウンロード完了 #*///2004.04 IP-CS 追加

#define     I_CRINTSRT      0x60        /*# CRINT起動要求 #*/

#define     I_CLKDSRT       0x70        /*# CLKD起動要求 #*/

#define     I_VOICESND      0x80        /*# 音声送信要求 #*/
#define     I_SDESRCV       0x81        /*# SDES受信 #*/
#define     I_BYERCV        0x82        /*# BYE受信 #*/
#define     I_HORYCOMPLETE  0x83        /*# 保留音送出完了 #*//* IPCS_V4 <PD><ADD> */
#define     I_DTMFABORT     0x84        /*# DTMF送出アボート #*//* IPCS_V4 <PD><ADD> */

#define     I_BOOTPREQRCV   0x90        /*# BOOTPreq受信 #*/    //2000.07 V2.1追加

#define     I_TFTPDATRCV    0xa0        /*# TFTPデータ受信 #*/  //2000.07 V2.1追加

#define     I_SIODATRCV     0xb0        /*# SIOデータ受信 #*/   //2000.07 V2.1追加

/* FAX管理タスクにて受信するメッセージの定義    */
#define     I_FAXTONE       0xc0        /*# FAXトーン検出 #*/
#define     I_FAXEND        0xc1        /*# FAX手順終了 #*/
#define     I_T38STATUS     0xc2        /*# T38状態通知 #*/
#define     I_FAXKILL       0xc3        /*# FAX強制停止指示 #*/
#define     I_FAXTOVOICE        0xc4    /*# FAXtoVOICE終了 #*/
#define     I_FAXPASS       0xc5        /*# FAXパス切断指示 #*/ //2003.11.10追加

/* T30タスクにて受信するメッセージの定義    */
#define     I_T30REQ        0xc8        /*# T30処理要求 #*/

/* MDMタスクにて受信するメッセージの定義    */
#define     I_FAXMDMREQ     0xcc        /*# faxモデム処理要求 #*/

/* T38タスクにて受信するメッセージの定義    */
#define     I_T38REQ        0xd0        /*# T38処理要求 #*/
#define     I_ACCEPTRECV        0xd1        /*# T38データ受信(アクセプト) #*/
#define     I_CONNECTRECV       0xd2        /*# T38データ受信(コネクト) #*/
#define     I_UDPRECV       0xd3        /*# T38データ受信(UDP) #*/
#define     I_CONNECTRES        0xd4        /*# コネクト結果通知 #*/
#define     I_ACCEPTRES     0xd5        /*# アクセプト通知 #*/
#define     I_SENDERR       0xd6        /*# 送信エラー通知 #*/

/* アクセプトタスクにて受信するメッセージの定義 */
#define     I_ACCEPTREQ     0xd8        /*# アクセプト指示 #*/

/* コネクトタスクにて受信するメッセージの定義   */
#define     I_CONNECTREQ        0xd9        /*# コネクト指示 #*/
#define     I_T38SENDREQ        0xda        /*# T38送信指示 #*/

//SS9100(PH1再開)対応 2006.06
#define     I_PH1ORDEND     0xe0        /*# PH1再開中オーダ終了 #*/

#define     I_LOGSTART      0xf0        /*# ログオープン #*/    // 2001.08 V2.3追加
#define     I_LOGCMD        0xf1        /*# ログコマンド #*/    // 2001.08 V2.3追加
#define     I_LOGTRACE      0xf2        /*# ログトレース #*/    // 2003.09 追加

// UF7200追加メッセージ
#define I_SERVER_REQ    0xf0
#define I_SERVER_RESP   0xf1
#define I_CLIENT_REQ    0xf2
#define I_CLIENT_RESP   0xf3
#define I_WRITE_REQ     0xf4
#define I_WRITE_RESP    0xf5


    /*# メッセージ種別：ＬＵオーダ #*/
#define     O_DOWNLOAD      0x01        /*# ダウンロード #*/
#define     O_DPRAMID       0x02        /*# DPRAM種別ダウンロード #*/
#define     O_HORYDOWN      0x03        /*# 保留音データダウンロード要求 #*/
#define     O_PKGINITREQ    0x04        /*# 個別PKG初期設定要求 #*/
#define     O_LUHELREQ      0x05        /*# LUヘルスチェック要求 #*/
#define     O_PKGHELREQ     0x06        /*# PKGヘルスチェック要求 #*/
#define     O_PASSCTLREQ    0x07        /*# パス制御要求 #*/
#define     O_TONECTLREQ    0x08        /*# TONE制御要求 #*/
#define     O_HORYCTLREQ    0x09        /*# 保留音制御要求 #*/
#define     O_PBDETSTART    0x0A        /*# PB信号監視開始要求 #*/
#define     O_PBDETSTOP     0x0B        /*# PB信号監視停止要求 #*/
#define     O_BTDETSTART    0x0C        /*# BT監視要求 #*/
#define     O_DTDETSTART    0x0D        /*# DT監視要求 #*/
#define     O_BTDETSTOP     0x0E        /*# BT監視停止要求 #*/
#define     O_DTDETSTOP     0x0F        /*# DT監視停止要求 #*/
#define     O_PBSNDSTART    0x10        /*# PB送信要求 #*/
#define     O_PBSNDSTOP     0x11        /*# PB信号送信停止要求 #*/
#define     O_DOORHACT      0x12        /*# ドアホン活性 #*/
#define     O_GHORYCTL      0x13        /*# 外部保留音制御 #*/
#define     O_BROADCTL      0x14        /*# 放送装置制御 #*/
#define     O_MUONPTNSET    0x15        /*# 無音パターン指定 #*/
#define     O_DOORHSCNREQ   0x16        /*# ドアホンスキャン要求 #*/
#define     O_PKGHELILG     0x17        /*# PKGヘルスチェックイリーガルPKG種別ダウンロード #*/
#define     O_MEMDMPREQ     0x18        /*# メモリDUMP要求#*/ // 99-5-18
#define     O_SYNCSNDCTL    0x19        /*# 同期/CRINTパケット送信制御 #*/  //2000.07 V2.1追加
#define     O_SYNCRCVCTL    0x1a        /*# 同期/CRINTパケット受信制御 #*/  //2000.07 V2.1追加
#define     O_DSPCHCTL      0x1b        /*# DSPch制御 #*/                   //2000.08 V2.1追加

#define     O_FAXPASSCTLREQ     0x1C        /*# FAXパス制御要求 #*//* 圧縮LU */
#define     O_T38LISTENREQ      0x1E        /*# T38Listen要求 #*/
#define     O_FAXDOWNLOAD       0x1D        /*# FAXダウンロード #*/

#define     O_TEST_ORD_TYPE 0xf8        /*# 工場試験用識別 #*/
#define     O_TST_DRAM_CHK  0xf8        /*# 工場試験用 ＤＲＡＭチェック #*/
#define     O_TST_MEM_CHK   0xf9        /*# 工場試験用 メモリチェック #*/
#define     O_TST_LED_CTL   0xfa        /*# 工場試験用 ＬＥＤ表示 #*/
#define     O_TST_MEM_RD    0xfb        /*# 工場試験用 メモリリード #*/
#define     O_TST_MEM_WT    0xfc        /*# 工場試験用 メモリライト #*/
#define     O_TST_DSP_CK    0xfd        /*# 工場試験用 ＤＳＰメモリチェック #*/ // 2001.05.22 追加
#define     O_TEST_ORD_TEND 0xff        /*# 工場試験用識別終了 #*/          //2000.07 V2.1追加

#define     O_FLASHTOOL     0x2F        /*# フラッシュツール要求 #*/        //2000.12.04 V2.1FLASH_TOOL

#define     O_BUFFVALUE     0x30        /*# バッファ面指定 #*/              //2000.07 V2.1追加
#define     O_LUCHGBAK      0x31        /*# 切り戻し #*/                //2004.02 LU32B追加
#define     O_OBSNTFYREQ    0x32        /*# 障害情報通知要求 #*/        //2004.02 LU32B
#define     O_PGDOWNLOAD    0x33        /*# プログラムダウンロード要求 #*/      //2004.04 IP-CS
#define     O_AQACT         0x35        /*# 音声情報通知起動/停止要求 #*/       //2004.10 SS9100

//SS9100(PH1再開)対応 2006.06
#define     O_PH1ENDREQ    0x37         /*PH1終了要求*/
#define     O_RESTARTREQ   0x38         /*再開処理要求*/

#define     O_PKGORDER     0x80         /*# 既存PKGオーダ #*/
//SS9100(PH1再開)対応 2006.06
#define     O_PH1ORDER     0x81         /*PH1再開オーダ*/

#define     O_MNT_ORD_START     0xf0    /*# メンテナンス用オーダ開始値 #*/  //2001.01.11 V2.2 追加
#define     O_MNT_ORD_END       0xff    /*# メンテナンス用オーダ終了値 #*/  //2001.01.11 V2.2 追加

//圧縮LU対応。DSP調査用オーダ追加。2003.11.13
//DSP調査用オーダコード start and end
#define O_DSP_ORD_TYPE      0xe0
#define O_DSP_ORD_TEND      0xef

//DSP調査用オーダコード
#define     O_DSP_GET_SYSTEMSTATUS  0xe0
#define     O_DSP_GET_CHANNELSTATUS 0xe1
#define     O_DSP_REQUEST_PARAM     0xe2

    /*# メッセージ種別：ＬＵイベント #*/
#define     E_LUONLINE      0x01        /*# LUオンライン通知 #*/
#define     E_PKGINITRSP    0x02        /*# 個別PKG初期設定応答 #*/
#define     E_LUHELRSP      0x03        /*# LUヘルスチェック結果 #*/
#define     E_PKGHELRSP     0x04        /*# PKGヘルスチェック結果 #*/
#define     E_PKGSTATE      0x05        /*# PKG状態通知 #*/
#define     E_PBDET         0x06        /*# PB信号受信 #*/
#define     E_BTDET         0x07        /*# BT受信 #*/
#define     E_DTDET         0x08        /*# DT受信 #*/
#define     E_PBSNDEND      0x09        /*# PB送信完了 #*/
#define     E_DOORHSCNRSP   0x0A        /*# ドアホンスキャン #*/
#define     E_PKGTROUBLE    0x0B        /*# PKG障害検出 #*/
#define     E_CLKDTROUBLE   0x0C        /*# CLKD障害検出 #*/
#define     E_CLKDRESTORE   0x0D        /*# CLKD障害復旧検出 #*/
#define     E_FANTROUBLE    0x0E        /*# FAN障害検出 #*/
#define     E_MEMDMPRSP     0x0F        /*# メモリDUMP結果 #*/ // 99-5-18
#define     E_SYNC_COMP     0x10        /*# ＣＬＫＤ同期合わせ完了通知（安定補正時に通知） #*/  // 2000.05.11 V1.2 追加
#define     E_HORYDLEND     0x11        /*# 保留音ダウンロード完了通知 #*/                      //2000.07 V2.1追加
#define     E_RELDET        0x12        /*# チャネル解放通知 #*/                                //2001.08 V2.3追加

#define     E_FAXERROR  0x16        /*# FAN障害検出 #*//* 圧縮LU */
#define     E_FAXPASSCHG    0x17        /*# 呼種別切り替え #*/

/*圧縮LU対応*/
#define     E_DSP_ERROR_DETECT  0x18
#define     E_DSP_REPAIR_DETECT 0x19


#define     E_PGDLEND           0x1A//2004.04 IP-CS
#define     E_AQNTC         0x1B//2004.10 SS9100


#define     E_DSP_SYSTEMSTATUS  0x1E//2004.04 IP-CS 
#define     E_DSP_CHANNELSTATUS 0x1F//2004.04 IP-CS 

//#define       E_DSP_SYSTEMSTATUS  0x1b//2004.04 IP-CS 
//#define       E_DSP_CHANNELSTATUS 0x1c//2004.04 IP-CS 

//#define       E_DSP_SYSTEMSTATUS  0x1a
//#define       E_DSP_CHANNELSTATUS 0x1b


// 工場試験用
#define     E_TST_DRAM_CHK  0xf8        /*# 工場試験用 ＤＲＡＭチェック #*/
#define     E_TST_MEM_CHK   0xf9        /*# 工場試験用 メモリチェック #*/
#define     E_TST_LED_CTL   0xfa        /*# 工場試験用 ＬＥＤ表示 #*/
#define     E_TST_MEM_RD    0xfb        /*# 工場試験用 メモリリード #*/
#define     E_TST_MEM_WT    0xfc        /*# 工場試験用 メモリライト #*/
#define     E_TST_DSP_CK    0xfd        /*# 工場試験用 ＤＳＰメモリチェック #*/ // 2001.05.22 追加
// 工場試験用

#define     E_PKGEVENT      0x80        /*# 既存PKGイベント #*/


/*# オーダ/イベントデータ #*/
#define     NUM_OF_MSG      0x0FF       /*# メッセージ種別MAX値 #*/
#define     ORD_MIN         2           /*# ミニマムサイズ #*/

/*# LUキャビネットタイプ #*/
#define     LU_BS_LU        0           /*# BS-LU #*/
#define     LU_EX_LU        1           /*# EX-LU #*/

/*# LUタイプ #*/
#define     LU_MASTER       0           /*# LUマスタ #*/
#define     LU_SLAVE        1           /*# LUスレーブ #*/

/*# CLKDタイプ #*/
#define     CLKD_MASTER     0           /*# クロックマスタ #*/
#define     CLKD_SLAVE      1           /*# クロックスレーブ #*/

/*# 実装MAX値 #*/
//#define       SLOT_MAX        4           /*# 最大実装可能スロット数 #*/
#define     DPRAM_MAX       8           /*# 最大DPRAM数 #*/

//#define       LU_MAX          96          /*# 最大LU数(V1) #*/
#define     LU_MAX          255         /*# 最大LU数(V4) #*///2004.01 LU32B
#define     SLV_MAX         16          /*# 最大SLV数(V1) #*/

/*# 割り込み #*/
#define     IRQ_ENABLE      1           /*# IRQ割り込みのみ許可 #*/

/*# スロット実装状態 #*/
#define     INS_SLOT    0               /*# PKG実装中 #*/
#define     OUS_SLOT    1               /*# PKG未実装 #*/

/*# 送信元／送信先識別子 #*/
#define     SNDID_LU    0               /*# LUオーダイベント #*/
#define     SNDID_PKG   1               /*# PKGオーダイベント #*/

/*# 送信元PKG値 #*///IP-CS対応
#define     FROM_SLAVE  1               /*# 旧スレーブ送信元イベントPKG番号エリア値 #*/


/*# 書き込み種別 #*/
#define PKG_ORDER           1           /*# 一般オーダ #*/
#define HELTH_CHECK         2           /*# ヘルスチェックオーダ #*/
#define DOOR_CTL            3           /*# ドアホン制御要求 #*/
//SS9100(PH1再開)対応 2006.06
#define PH1_ORDER           4           /*# PH1再開オーダ #*/

/*# BOOTP #*/ 
#define VEND_LU_TAG         128         /*# ベンダデータタイプ #*/
#define VEND_LU_SIZE        7           /*# ＬＵベンダ情報サイズ #*/ // 99.05.21 変更
#define VEND_VLAN_TAG       131         /*# ＶＬＡＮタグ #*/ // 99.08.18
#define VEND_VER_TAG        132         /*# IPLバージョンタグ #*/               // 2000.06.20 V1.2追加
#define VEND_VER_SIZE       4           /*# ＬＵバージョンサイズ #*/            // 2000.06.20 V1.2追加
#define VEND_CCUID_TAG      133         /*# CCU IDタグ #*/                      // 2000.07 V2.1追加
#define VEND_CCUID_SIZE     1           /*# CCU IDサイズ #*/                    // 2000.07 V2.1追加
#define VEND_CCUIP_TAG      134         /*# CCU IPアドレスタグ #*/              // 2000.07 V2.1追加
#define VEND_CCUIP_SIZE     4           /*# CCU IPアドレスアドレス #*/          // 2000.07 V2.1追加
#define VEND_CCUVER_TAG     135         /*# CCUバージョンタグ #*/               // 2000.07 V2.1追加
#define VEND_CCUVER_SIZE    2           /*# CCUバージョンサイズ #*/             // 2000.07 V2.1追加
#define VEND_EOL_TAG        255         /*# エンドオブリスト #*/ // 99.05.21 追加
#define VEND_SUBNET_SIZE    4           /*# サブネットマスク #*/                // 2000.09 V2.1追加
#define VEND_GATEWAY_SIZE   4           /*# ゲートウェイIPアドレス #*/          // 2000.09 V2.1追加
#define VEND_VLAN_SIZE      5           /*# サブネットマスク #*/                // 2000.09 V2.1追加
#define VEND_LUTYPE_TAG     136         /*# ＬＵタイプタグ #*/                  // 2001.07.05 追加
#define VEND_LUTYPE_SIZE    1           /*# ＬＵタイプサイズ #*/                // 2001.07.05 追加
#define VEND_LUTYPE_TYPE    9           /*# ＬＵタイプ（IP-CS） #*/             // 2004.04 追加
#define VEND_CCU2_TAG       137         /*# CCU2 IPアドレスタグ #*/             // 2001.08 V2.3 追加
#define VEND_CCU2_SIZE      4           /*# CCU2 IPアドレスアドレス #*/         // 2001.08 V2.3 追加


/*# ダウンロードオーダ #*/
/*# ダウンロードオーダサイズ #*/
#define     V1_1_DL_SIZE    45                  /*# V1.1CCUのダウンロードオーダサイズ #*/       // 2000.05.12 V1.2追加
#define     V2_1_DL_SIZE    51                  /*# V2.1CCUのダウンロードオーダサイズ #*/       // 2000.07 V2.1追加
#define     V2_1_DL_SIZEV2  91                  /*# V2.1CCUのダウンロードオーダサイズVer.2# */  // 2000.09 V2.1追加
//#define       V2_2_DL_SIZE    97                  /*# V2.2CCUのダウンロードオーダサイズ# */       // 2000.12.27 V2.2追加
#define     V2_2_DL_SIZE    98                  /*# V2.2CCUのダウンロードオーダサイズ# */       // 2001.03.08 変更
#define     V2_3_DL_SIZE    100                 /*# V2.3CCUのダウンロードオーダサイズ# */       // 2001.08 追加

//  ゆらぎ最大値定義 2000.09 V2.1追加
#define     V2_1_CCU_YURAGIMAX  11                          /*# V2.1 CCU指定ゆらぎ最大値 #*/
#define     V1_1_CCU_YURAGIMAX  5                           /*# V1.1 CCU指定ゆらぎ最大値 #*/

//  バッファ面指定最大値定義 2000.09 V2.1追加
#define     V2_1_CCU_BUFFNUMMAX 10                          /*# V2.1 CCU指定バッファ面指定最大値 #*/
#define     V1_1_CCU_BUFFNUMMAX 5                           /*# V1.1 CCU指定バッファ面指定最大値 #*/

//  D01サバイバル対応 2004.02 LU32B追加
#define     SURV_SWITCH_TIM     1800                    /*# PowerOn時のデフォルトCCU切換タイマ値(秒) #*/

//SS9100対応追加 2004.11.01
#define     SURV_SWITCH_SS9100_TIM  120                 /*# SS9100時のデフォルトCCU切換タイマ値(秒) #*/

/****************************************************************************/
/* FAXトーン検出種別                                                        */
/****************************************************************************/
enum
{
    FAXTONE_CNG         ,       /* CNG      */
    FAXTONE_CED         ,       /* CED          */
    FAXTONE_PRI             /* プリアンブル */
} ;


/***********************************************************************/
/* DSP種別                                 */
/***********************************************************************/
enum
{
    DSP_0         ,       /* ch0-ch3      */
    DSP_1                 /* ch4-ch7      */
} ;

/***********************************************************************/
/* 強制停止種別                                */
/***********************************************************************/
enum
{
    STOP_DSP         ,      /* DSP    */
    STOP_CH                 /* CH         */
} ;



/*###########################################################################*/
/*#                                                                         #*/
/*#                               BOOTP関連                                 #*/
/*#                                                                         #*/
/*###########################################################################*/
#define     BOOTP_RESP_AERA 0x60000         /*# BOOTP RESP 書き込み先頭アドレス(仮） #*/

/*###########################################################################*/
/*#                                                                         #*/
/*#                               保留音関連                                #*/
/*#                                                                         #*/
/*###########################################################################*/
//#define       HORYSIZE        (256*1024+40)               /*# 保留音サイズ #*/
#define     HORYSIZE        (256*1024)                  /*# 保留音サイズ #*/
#define     HORYSAVESIZE    (256*1024)                  /*# 保留音フラッシュ保存サイズ #*/  //2001.01.16 V2.2追加
//#define       TALKYSIZE       (128*1024+40)               /*# トーキーサイズ #*/
#define     TALKYSIZE       (128*1024)                  /*# トーキーサイズ #*/
#define     TALKYSAVESIZE   (128*1024)                  /*# トーキーフラッシュ保存サイズ #*/    //2001.01.16 V2.2追加
//#define       HORY1_ADR       0x2130000               /*# 保留音１先頭アドレス(仮） #*/
//#define       HORY2_ADR       HORY1_ADR+HORYSIZE      /*# 保留音２先頭アドレス #*/
//#define       HORY_RDMAX      0x1770      /*# 保留音データ読み込み最大値(40BYTE単位) #*/

/*###########################################################################*/
/*#                                                                         #*/
/*#                           共通関数用デファイン                          #*/
/*#                                                                         #*/
/*###########################################################################*/
typedef enum {
		POOL0 = 0x00,		/*# 一般メッセージ用        #*//*# DOEPにてUSER-QUE使用 #*//* 旧CDLC部では、CD_MPID_TSK_MSGと定義 */
		POOL1,				/*# 二次メッセージ用        #*//*# LU管理にてUSER-QUE使用 #*/
		POOL2,				 /*# DSP制御データ用     #*//*# DSPにてUSER-QUE使用 #*/
		POOL3,				/*# 音声受信用 #*/
		POOL4,				/*# 音声送信用 #*/
		POOL5,				/*# UDP送信用 #*/
		POOL6,				/*# FUSION用 #*/
		POOL7,				/*# TAG用 #*/
		POOL8,				/*# DMY #*//*# ダミーではない、DOEPで使用している #*/
		CD_MPID_EVT_DATA,	/*# 旧CDLC部　イベント通知(データ受信通知)用 #*/
		CD_MPID_EVT,		/*# 旧CDLC部　イベント通知(データ受信通知以外)用 #*/
		CD_MPID_ORD,		/*# 旧CDLC部　オーダ制御タスク用 #*/
		CD_MPID_SND_IDATA,	/*# 旧CDLC部　送信Iデータ用 #*/
		CD_MPID_RCV_IDATA,	/*# 旧CDLC部　受信Iデータ用 #*/
		CD_MPID_WAIT_CFRM,	/*# 旧CDLC部　送信待ち制御フレーム用 #*/
		CD_MPID_RCV_SIO_MSG,/*# 旧CDLC部　SIO受信通知メッセージ用 #*/
		COM_POOL_MAX	/*# プールID数 #*/
} COM_POOL_ID;
#define CD_MPID_TSK_MSG		POOL0		/*# 旧CDLC部メッセージ送信用プール名 #*/

/*# プールサイズ #*/
#define     POOL0_SIZE      		56          /*# 一般メッセージ用 [12(os)+44(内部msg)] #*/ //2003.10.23
#define     POOL1_SIZE      		280         /*# 二次メッセージ用 [12(os)+12(LUﾍｯﾀﾞ)+256(ｵｰﾀﾞ/ｲﾍﾞﾝﾄ情報)] #*/
#define     POOL2_SIZE      		80          /*# DSP制御データ用 [12(os)+4(SystemSTS)+64] #*/
#define     POOL3_SIZE      		1540        /*# 音声受信メッセージ用 [1528(Max LAN Rcv Size)] #*/
#define     POOL4_SIZE      		512         /*# 音声送信メッセージ用 [12(rtp)+320(音声G711-40ms)) #*//* IPCS_V4 <PD><CHG> */
#define     POOL5_SIZE      		140         /*# UDP送信用 [12(os)+128(RTCP、CLKD、CRINT)]#*/
#define     POOL6_SIZE      		20          /*# FUSION用 [12(os)+4(SystemSTS)+4(Dmy)] #*/
#define     POOL7_SIZE      		28          /*# TAG用 [12(os)+16(TAG)] #*/
#define     POOL8_SIZE      		1048        /*# EXメッセージ用 [12(os)+12(LUﾍｯﾀﾞ)+1024(ｵｰﾀﾞ/ｲﾍﾞﾝﾄ情報)]  #*/
#define 	CD_BLKSZ_EVT_DATA   	144       	/* イベント通知(データ受信通知)用プール */
#define 	CD_BLKSZ_EVT        	48    		/* イベント通知(データ受信通知以外)用プール */
#define 	CD_BLKSZ_ORD        	144         /* オーダ制御用プール */
#define 	CD_BLKSZ_SND_IDATA  	288         /* 送信Ｉデータ用プール */
#define 	CD_BLKSZ_RCV_IDATA  	288         /* 受信Ｉデータ用プール */
#define 	CD_BLKSZ_WAIT_CFRM  	16          /* 送信待ち制御フレーム用プール */
#define 	CD_BLKSZ_RCV_SIO_MSG    32        	/* ＳＩＯ受信通知メッセージ用プール */

    /*# プール面数 #*/
#define     POOL0_CNT       		512         /*# 一般メッセージ用 #*/
#define     POOL1_CNT       		256         /*# 二次メッセージ用 #*/
#define     POOL2_CNT       		32          /*# DSP制御データ用 #*/
#define     POOL3_CNT       		8           /*# 音声受信用 #*//* IPCS_V4 <PD><CHG> */
#define     POOL4_CNT       		512         /*# 音声送信用 #*/
#define     POOL5_CNT       		32          /*# UDP送信用 #*/
#define     POOL6_CNT       		256         /*# FUSION用 #*/
#define     POOL7_CNT       		224         /*# TAG用[96(uni_CLKD)+96(uni_CRINT)+32(元の数)] #*/ // 2002.03.28変更
#define     POOL8_CNT       		10          /*# EXメッセージ用 #*/
#define 	CD_BLKCNT_EVT_DATA  	3         	/* イベント通知(データ受信通知)用プール */
#define 	CD_BLKCNT_EVT       	12    		/* イベント通知(データ受信通知以外)用プール */
#define 	CD_BLKCNT_ORD       	8           /* オーダ制御用プール */
#define 	CD_BLKCNT_SND_IDATA 	32          /* 送信Ｉデータ用プール */
#define 	CD_BLKCNT_RCV_IDATA 	52          /* 受信Ｉデータ用プール */
#define 	CD_BLKCNT_WAIT_CFRM 	36          /* 送信待ち制御フレーム用プール */
#define 	CD_BLKCNT_RCV_SIO_MSG   48        	/* ＳＩＯ受信通知メッセージ用プール */

    /*# ユーザキューID #*/
#if 1 /* IPCS V4 Change */
typedef enum {
	DOEP_QUEID = 0,		/*# DOEP用キューID #*/
	LUMNG_DPQID,		/*# LU管理用キューID #*/
	DSPMNG_QUEID,		/*# DSP制御データ用 #*/
	H323_CN0QUEID,		/*# 受信音声処理CN0にてUSER-QUE使用 #*/
	H323_CN1QUEID,		/*# 受信音声処理CN1にてUSER-QUE使用 #*/
	H323_CN2QUEID,		/*# 受信音声処理CN2にてUSER-QUE使用 #*/
	H323_CN3QUEID,		/*# 受信音声処理CN3にてUSER-QUE使用 #*/
	DPISR_SL0QUEID,		/*# DPRAM制御CN0にてUSER-QUE使用 #*/
	TFTPSRV_QUEID,		/*# TFTPサーバ用キューID #*/
	CYCCTLORD_QUEID,	/*# 周期制御用(オーダ受信) #*/
	CYCCTLEVT_QUEID,	/*# 周期制御用(イベント送出) "*/
	CBX_BS_DT_QUEID,	/*# ＢＳデータ蓄積バッファキュー #*/
	CBX_BS_DT_TMP_QUEID,/*# ＢＳデータ蓄積バッファキュー(クリア処理時のテンポラリ) #*/
	CBX_SIG_QUEID,		/*# ＢＳ管理情報バッファキュー #*/
	CBX_CFRM_QUEID,		/*# 送信待ち制御フレームバッファキュー #*/
	COM_QUE_MAX
} COM_QUE_ID;
#else /* IPCS V4 Change */
#define     DOEP_QUEID      POOL0       /*# DOEP用キューID #*/
#define     LUMNG_DPQID     POOL1       /*# LU管理用キューID #*/
#define     DSPMNG_QUEID    POOL2       /*# DSP制御データ用 #*/
#define     H323_CN0QUEID   POOL4       /*# 受信音声処理CN0にてUSER-QUE使用 #*/
#define     H323_CN1QUEID   POOL4+1     /*# 受信音声処理CN1にてUSER-QUE使用 #*/
#define     H323_CN2QUEID   POOL4+2     /*# 受信音声処理CN2にてUSER-QUE使用 #*/
#define     H323_CN3QUEID   POOL4+3     /*# 受信音声処理CN3にてUSER-QUE使用 #*/
#define     DPISR_SL0QUEID  POOL8       /*# DPRAM制御CN0にてUSER-QUE使用 #*/
#define     DPISR_SL1QUEID  POOL8+1     /*# DPRAM制御CN1にてUSER-QUE使用 #*/
#define     DPISR_SL2QUEID  POOL8+2     /*# DPRAM制御CN2にてUSER-QUE使用 #*/
#define     DPISR_SL3QUEID  POOL8+3     /*# DPRAM制御CN3にてUSER-QUE使用 #*/
#define     DPISR_SL4QUEID  POOL8+4     /*# DPRAM制御CN4にてUSER-QUE使用 #*/
#define     DPISR_SL5QUEID  POOL8+5     /*# DPRAM制御CN5にてUSER-QUE使用 #*/
#define     DPISR_SL6QUEID  POOL8+6     /*# DPRAM制御CN6にてUSER-QUE使用 #*/
#define     DPISR_SL7QUEID  POOL8+7     /*# DPRAM制御CN7にてUSER-QUE使用 #*/
#define     DPISR_SL8QUEID  POOL8+8     /*# DPRAM制御CN8にてUSER-QUE使用 #*/    //2000.07 V2.1追加
#define     DPISR_SL9QUEID  POOL8+9     /*# DPRAM制御CN9にてUSER-QUE使用 #*/    //2000.07 V2.1追加
#define     DPISR_SL10QUEID POOL8+10    /*# DPRAM制御CN10にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     DPISR_SL11QUEID POOL8+11    /*# DPRAM制御CN11にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     DPISR_SL12QUEID POOL8+12    /*# DPRAM制御CN12にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     DPISR_SL13QUEID POOL8+13    /*# DPRAM制御CN13にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     DPISR_SL14QUEID POOL8+14    /*# DPRAM制御CN14にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     DPISR_SL15QUEID POOL8+15    /*# DPRAM制御CN15にてUSER-QUE使用 #*/   //2000.07 V2.1追加
#define     TFTPSRV_QUEID   POOL18      /*# TFTPサーバ用キューID #*/            //2000.07 V2.1追加

#define     T38_QUEID   POOL19      /*# T38PKT受信用キューID #*//* 圧縮LU */
#define     FAX_DATQUEID    POOL1A      /*# FAXデータ用キューID #*//* 圧縮LU */
#define     FAX_LINKQUEID   POOL1B      /*# FAXデータリンク用キューID #*//* 圧縮LU */
//圧縮LU対応。DSPリアルタイムコマンド用。2003.09.03
#define     DSP_REALQUEID   POOL1C          /*# DSPリアルタイムコマンド用　#*/
#define     LOG_TRACEQUEID  POOL1D          /*# トレース情報用　#*/         //2003.09
#endif /* IPCS V4 Change */
    /*# ユーザキュー接続種別 #*/
#define     QUE_TOP             0
#define     QUE_TAIL            1

    /*# ログ収集面数 #*/
#define     MSG_LOG_SIZE        1024

//2000.10 V2.1追加(start)
#define     SYNC_1S_5MSBASE     200                     /* 5msベースでの1s分のカウント */
#define     SYNC_CNT_MAX        (15*SYNC_1S_5MSBASE)    /*# 補正カウンタカウント最大値（5ms：15秒） #*/
//2000.10 V2.1追加(end)

#define     DOEP_HELCHK_TO      30                      /*# ヘルスチェックタイマカウントデフォルト値（秒） #*/  //2000.12.27 V2.2追加（名称変更して共通化）

//SS9100対応追加 2004.11.01
#define     DOEP_HELCHK_SS9100_TO   60                      /*# SS9100ヘルスチェックタイマカウントデフォルト値（秒） #*/

/*#########################################*/
/*#            マクロ定義                 #*/
/*#########################################*/
//#define     com_chgword(C)          (WORD)((((C)>>8)&0xFF)|(((C)<<8)&0xFF00))
#define	com_chgword(C)	htons(C) /* IPCS_V4 <PD><CHG> */
//#define     com_chgdword(C)         ((DWORD)(((((DWORD)C)>>24)&0xFF)|((((DWORD)C)>>8)&0xFF00)|((((DWORD)C)<<8)&0xFF0000)|((((DWORD)C)<<24)&0xFF000000)))
#define	com_chgdword(C)	htonl(C) /* IPCS_V4 <PD><CHG> */
#define com_10dwordcopy(a,b)        *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a++ = *b++;\
                                    *a = *b;
#if 0 /* IPCS V4 Del */
//圧縮LU対応。セマフォ用マクロ追加。2003.08.27
#define     com_getsemp(a,b,c)      XGETSEM(a,b,c)
#define     com_putsemp(a)          XPUTSEM(a)
#define     com_initsemp(a,b)       XINITSEM(a,b)
#define     com_sempsns(a)          XSEMSNS(a)
//#endif //LU_MACRO
#endif /* IPCS V4 Del */

#ifndef TMRIPT_START                                        // LU 99.7.14
    #define     TMRIPT_START    (fusion_ipt_flg |= 0x02)
#endif
#ifndef TMRIPT_END
    #define     TMRIPT_END      (fusion_ipt_flg &= ~0x02)
#endif
#ifndef LANCIPT_START
    #define     LANCIPT_START   (fusion_ipt_flg |= 0x01)
#endif
#ifndef LANCIPT_END
    #define     LANCIPT_END     (fusion_ipt_flg &= ~0x01)
#endif


/*# モードレジスタビットアサイン #*/
#define     IO_MODE_VLAN        0x0001              /*# VLAN設定 #*/    //値変更不可＝ハード仕様変更不可
#define     IO_MODE_REMOTE      0x0002              /*# 遠隔設定 #*/
#define     IO_MODE_AUTONEGO    0x0004              /*# AUTO-NEGO設定 #*/
#define     IO_MODE_NEGOMODE    0x0018              /*# 強制ネゴシエーションモード設定 #*/
#define     IO_MODE_CCUID       0x0060              /*# CCU識別子設定 #*/
#define     IO_MODE_MNT         0x0080              /*# 保守モード設定 #*/

/*# マスクデータ #*/
//#define       LU_IAM_BIT          0x0002              /*# ＩＡＭ搭載情報 PORT4.1 #*/      //2001.02.13 V2.2DEBUG
#ifdef      V2_1_DBG_ECHOEX
#define     LU_ECHO_HWSEL       0x0000              /*# ＲＣ－ＰＫＧ実装(0)/未実装(1)情報 スレーブPORT4.1#*/    //デバック用
#else   //V2_1_DBG_ECHOEX
#define     LU_ECHO_HWSEL       0x0002              /*# ＲＣ－ＰＫＧ実装(0)/未実装(1)情報 スレーブPORT4.1#*/    //2000.12.08 V2.2追加
#endif  //V2_1_DBG_ECHOEX
//#define       LU_JPUS_BIT         0x0040              /*# ＬＵ国種別 PORT4.2 #*/
#define     LU_JPUS_BIT         0x0004              /*# ＬＵ国種別 PORT4.2 #*/              //2000.09.06 修正
#define     LU_TYPE_MSK         0x0008              /*# ＬＵマスタ・スレーブ PORT4.3 #*/

#define     LU_CHD_BIT          0x0020              /*# チャネル制御数 PORT1.5 #*/
#define     LU_PBREC_BIT        0x0010              /*# ＰＢ監視時間指定 PORT1.4 #*/
//#define       LU_PKG_MSK          0x000f              /*# スロット数 PORT1.0_3 #*/
#define     LU_PKG_MSK          0x0007              /*# スロット数 PORT1.0_3 #*///IP-CS 2004.04
#define     LU_SLAVE_MASK       0x000f              /*# スレーブ番号 PORT1.0_3 #*/
#define     LU_CHD_MSK          0x0010              /*# チャネル制御数 PORT1.4 #*/

#define     LU_UD_MSK           0x000f              /*# ＬＵ番号（１０位） #*/
#define     LU_LD_MSK           0x000f              /*# ＬＵ番号（１位） #*/

//圧縮LU対応。I/Oマップ変更。以下削除。2003.08.25
#define     IO_LSIVER_MASK      0x00000fff          /*# κバージョンマスク #*/              //2000.09 V1.2追加

/* COMMON エラーコード */
typedef enum {
	COM_ABORT_01= 0,	/* com_poolget メモリブロックID異常時 */
	COM_ABORT_02,		/* com_poolput メモリブロックID異常時 */
	COM_ABORT_03,		/* com_poolput メモリブロックアドレス異常時 */
	COM_ABORT_04,		/* com_ecbinit mq_open異常時 */
	COM_ABORT_05,		/* com_sndmsg ECB番号異常時 */
	COM_ABORT_06,		/* com_sndmsg mq_open異常時 */
	COM_ABORT_07,		/* com_sndmsg mq_send異常時 */
	COM_ABORT_08,		/* com_rcvmsg ECB番号異常時 */
	COM_ABORT_09,		/* com_rcvmsg mq_open異常時 */
	COM_ABORT_10,		/* com_rcvmsg mq_receive異常時 */
	COM_ABORT_11,		/* com_rcvmsg mq_receiveタイムアウト以外の異常時 */
	COM_ABORT_12,		/* com_queput キューID異常時 */
	COM_ABORT_13,		/* com_queget キューID異常時 */
	COM_ABORT_14,		/* com_quesns キューID異常時 */
	COM_ABORT_15,		/* com_threadstart スレッドID異常時 */
	COM_ABORT_16,		/* com_threadstart スレッド状態異常時 */
	COM_ABORT_17,		/* com_threadstart pthread_attr_init異常時 */
	COM_ABORT_18,		/* com_threadstart pthread_attr_setdetachstate異常時 */
	COM_ABORT_19,		/* com_threadstart pthread_attr_setinheritsched異常時 */
	COM_ABORT_20,		/* com_threadstart pthread_attr_setschedpolicy異常時 */
	COM_ABORT_21,		/* com_threadstart pthread_attr_setschedparam異常時 */
	COM_ABORT_22,		/* com_threadstart pthread_create異常時 */
	COM_ABORT_23,		/* com_threadstop スレッドID異常時 */
	COM_ABORT_24,		/* com_threadstop スレッド状態異常時 */
	COM_ABORT_25,		/* com_threadstop pthread_cancel異常時 */
	COM_ABORT_26,		/* dbg_print_set スレッドID異常時 */
	COM_ABORT_27,		/* dbg_print スレッドID異常時 */
	COM_ABORT_28		/* com_get_pool_status メモリブロックID異常時 */
} COM_ERROR_CODE;

/* ECB関連 */
#define LUMQ_MAXMSG     64  /* Maximum number of messages in queue */
#define LUMQ_MSGSIZE    80  /* Maximum message size (in bytes) */

/* デバッグプリント関連 */
#define LOGDST_CNS    1 /* Output to console */
#define LOGDST_SYSLOG 2 /* Output to syslog */

/* LU動作モード */
#define LUMODE_NORMAL 1 /* 通常モード */
#define LUMODE_MAINTE 0 /* 保守モード */


/* TAG FIELDS in the vendor information area (v_fields) */

/* FIXED  LENGTH DATA: one tag byte, followed by optional data as described 
 * below 
*/
#define PAD_TAG         (BYTE)0   /* padding fields */
#define END_TAG         (BYTE)255 /* end of usable data */
#define SUBNET_TAG      (BYTE)1   /* followed by 4 bytes subnet mask */
/*time offset of the local subnet in seconds from Coordinated Universal Time 
 * (UTC) (signed 32 bit integer)
*/
#define TIMEOFFSET_TAG  (BYTE)2   /* followed by 4 bytes of time offset */
#define BOOTFILESIZE_TAG (BYTE)13 /* Two byte value in network order specifiyng
                  the number of 512 bytes blocks in the default
                  boot file.
                */

/* VARIABLE LENGTH DATA: one tag byte, one length byte, and N bytes of 
 * data 
*/
#define GATEWAY_TAG     (BYTE)3   /*IP addresses of N/4 gateways for
                       this subnet. Preferred one is first*/ 
#define TIMESERVER_TAG  (BYTE)4   /*IP addresses of N/4 time servers (RFC-868) */

#define NAMESERVER_TAG  (BYTE)5   /*IP addresses of N/4 name servers (IEN_116) */
#define DOMAINNAMESERVER_TAG (BYTE)6  /*IP addresses of N/4 domain name servers
                                (RFC1034) */
#define LOGSERVER_TAG   (BYTE)7   /* IP addresses of N/4 MIT-LCS UDP log server */
#define QUOTESERVER_TAG (BYTE)8   /* IP addresses of N/4 Quote of the Day servers
                                (RFC865) */
#define LPRSERVER_TAG   (BYTE)9   /* IP addresses of N/4 Berkeley 4BSD printer 
                    servers (LPD) */
#define IMPRESSSERVER_TAG (BYTE)10 /*IP addresses of N/4 Impress network image 
                    servers [IMAGEN] */ 
#define RLPSERVER_TAG   (BYTE)11  /* IP addresses of N/4 Resource Location 
                   Protocol servers (RFC-887) */

#define HOSTNAME_TAG    (BYTE)12  /* Name of the client. Site specific issue */

