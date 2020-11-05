/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : LU                                                       #*/
/*#                                                                         #*/
/*#     日本語名 : LU共通デファイン定義ファイルREL                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/28 - 毛塚  照明                                    #*/
/*#     更    新 : 08/12/05 - T.Kitazawa                                    #*/
/*#                                                                         #*/
/*###########################################################################*/
//#include "../../mik/mik_armc.h"

/*---------------------------------------------------------------------------*/
/*       作成ファイル種別用                                                  */
/*---------------------------------------------------------------------------*/

/*--- デバック／正式で切替必要な定義 ---*/

#ifndef V1_DEBUG                                                                                //makefileへ
//#define   V1_DEBUG                                                                                //makefileへ
#endif
//#define   V2_1_DBG                    /*# デバックdefine元スイッチ #*/    //V2.1LUファーム追加    //makefileへ

#ifdef      V1_DEBUG            /* デバック時定義（正式版は未定義） */

//#define       NO_BOOTP_USE        /*# IP=10.11.1.(2+(BYTE)MAC)にする #*///makefileにて操作

//#define           LED_INT_TEST        // 工場試験用ＬＥＤ有効     99.12.3
//#define           CLKD_SYNC_CHK       // ＣＬＫＤ同期合わせ状態をアラームランプで表示（LED_INT_TESTも定義する）   99.12.3
//#define           SYNC_PHS_CHK        // フェーズ変更通知パケット送出（ハードデバッグ用） 2000.06.26
//#define           SYNC_CMP_DBG        /*# 同期合わせ完了デバック（正式時未定義にする） #*/

#define         NON_HELCHK_TO       /*# 定義時ＣＣＵからのヘルスチェックＴ．Ｏ監視無効（試験時は常にＴ．Ｏ．無効） #*///2004.04 IP-CS debug

//#define       LU_TFTP_RETRY       //  ソフトデバッグ時にTFTPを無限リトライするときに定義する
//#define       NON_MAC_ROM_CHECK   /*# ＩＰＬプログラムチェック有効時、ＭＡＣエリアを対象としない時定義（出荷時に未定義） #*/
//#define       FAULT_LED_INTER     /*# 障害時ＬＥＤインター有効（正式版はに未定義にする） #*/
//#define       WDT_ON              // ウォッチドッグ有効（通常はメイクで定義するがデバッグ時も有効にする場合定義する）
//#define       WDT_STOP            // ウォッチドッグ停止（ＷＤＴ有効でもウォッチドッグｉ／ｏアクセスしない。出荷時未定義とする） //2000.2.1有効にした
//#define       DBG_CACHE_ON        /*# デバッグ時での命令キャッシュ有効 #*/
//#define       LU_DBG              /*# LUデバックフラグ #*/
//#define       LU_DBG2             /*# DBG_SW検出ガード #*/
//#define       LU_DBG3             /*# DBG_SW不具合解析 #*/
//#define       LU_DBG4             /*# DBG_SW無効 #*/

//#define           EXEC_TIME_LED       // 処理時間測定用（正式時未定義）
//#define           KM_RCV_TIME_LED     // 処理時間測定用（正式時未定義）
//#define           MAC_RST_TIME_LED        // 処理時間測定用（正式時未定義）
//#define           MAC_ISR_TIME_LED    // 処理時間測定用（正式時未定義）
//#define       LED_CHK_NO0         /*# 時間測定LED表示（５ｍｓ割込）有効 #*/
//#define       LED_CHK_NO1         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO2         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO3         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO4         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO5         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO6         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO7         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO8         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO9         /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO10        /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO11        /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO12        /*# 時間測定LED表示有効 #*/
//#define       LED_CHK_NO98        /*# 時間測定LED表示（アイドルインターバル）有効 #*/
//#define       LED_CHK_NO99        /*# 時間測定LED表示（ベース）有効 #*/

//#define       CLKD_DBG            // CLKD DBG
//#define       CLKD_LOG            // CLKD LOG収集モード詳細版（CLKD_LOG2と同時に設定しないこと）
//#define       CLKD_LOG2           // CLKD LOG収集モード簡略版（CLKD_LOGと同時に設定しないこと）
//#define       DPRAM_WT_TRC        /*# ＤＰＲＡＭ書込ログ有効 #*/
//#define       DPRAM_RW_LOG            /*# ＤＰＲＡＭ読書ログ有効 #*/

//#define       OAK_EVT_DEBUG       // ＯＡＫイベント読み取りログ
//#define       OAK_ORD_DEBUG       // ＯＡＫ設定オーダログ
//#define           TIME_LOG_DEBUG      //デバッグログ用５ｍｓ時間情報更新
//#define       VOICE_SET_DEBUG     // 音声設定ポインタログ
//#define       PB_NG_DEBUG         // ＰＢ受信ＮＧデバッグログ（ハードデバッグ用）
//#define       OAK_PB_DEBUG        // ＯＡＫＰＢ音声ログ（ハードデバッグ用）
//#define       PB_ON_TIME_DBG      // ＰＢオン監視時間変更（ハードデバッグ用）
//#define       PB_EC_ON_DBG        // ＰＢ監視中もエコキャンオンとする（ハードデバッグ用）

//#define       ECHO_CAN_STOP       // エコーキャンセラ常時停止（ハードデバッグ用）通常未定義とする
//#define LU_NUMSCN_DBG             // 2001.1.17 ＬＵ番号通知スキャン調査デバッグ用（正式版は未定義）
//#define       RATE_CPU_EXE        // ＣＰＵ使用率測定する場合に定義する） 99.11.17
#define FPGA_CHK_DBG    //オンラインDBG用 fpgaコンフィグルート有効 IP-CS対応 2004.04

#else       //V1_DEBUG          /* デバック時未定義（正式版は定義） */

//#define       RATE_CPU_EXE        // ＣＰＵ使用率測定する場合に定義する） 99.11.17
#define         PROG_CHECK_EXE      /*# プログラムチェックを有効にする（デバッグ時は未定義） #*/
#define         RAM_CHECK_EXE
#define         LU_PRG_CHECK        //  ダウンロードファイルチェックを行う場合に定義する（正式版の時は定義すること）
//#define       LSW_START_WAIT      /*# ＩＰＬでコアスイッチＬＳＷの動作待ちタイミング有効（正式版は定義する） #*/
#define         CCU_HELCHK_START    // 2000.03.28 初期状態からヘルス監視実施（ソフト笹岡さん確認用）

#ifdef  MENTE
#define             NON_HELCHK_TO   /*# 定義時ＣＣＵからのヘルスチェックＴ．Ｏ監視無効（試験時は常にＴ．Ｏ．無効） #*/
#endif  //#ifdef    MENTE

#endif      //V1_DEBUG

#define     FAULT_LED_INTER     /*# 障害時ＬＥＤインター有効（正式版はに未定義にする） #*/

//#define           MAC_RESET_DBG       // ＭＡＣリセット動作デバッグ用（正式時未定義）
//#define           RTP_SND_DBG         // ＲＴＰパケット抜け音声評価デバッグ用（正式時未定義）
//#define           RTP_SEQ_NG_DISP     // ＲＴＰパケット抜けＬＥＤ表示デバッグ用（正式時未定義）


/*--- システム更新用固定定義 ---*/

#ifdef      MAKE_IPL
//#ifdef            MAKE_IPL_ROM
#define             LU_TAG_BOOT         /*# LSWリレーエージェントでBOOTPreplyが中継されないことの対策（正式版は定義する） #*/
//#endif    //ifdef     MAKE_IPL_ROM
#endif  //ifdef     MAKE_IPL

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
//#define       OLD_IPL_ROM         /*# 旧版ＩＰＬでＭＡＣアドレスが0x1ff00の時定義する #*/ // 99.10.25

#define         LU_MACRO            /*# マクロ有効フラグ #*/    // 常に定義（POOL取得ＮＧとなるので）
#define         LU_RTP_SEQ          /*# RTPｼｰｹﾝｽﾁｪｯｸ有効フラグ #*/

#define         ALM_LMP     0x80    /*# 時間測定LEDＡＬＭランプ #*/
#define         OP_LMP      0x40    /*# 時間測定LEDＯＰランプ #*/

//初期設定専用ＰＯＲＴ０データ（マスタ／スレーブ共通）
//#define           ALM_LMP_ON  0x0042
//#define           ALM_LMP_OFF 0x00c2
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
//#define     NULL                0x00
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

//#define       CNMAX           8               /*# 最大CN数 （Ｖ１は、４ＨＷｃｈ）#*/
#define     CNMAX           4               /*# 最大CN数 #*///2004.04 IP-CS変更
#define     CNMAX_IPCS          3               /*# IPCS実最大CN数 #*///2004.10 SS9100 


#define     COM_NOMAL       0           /*# 無変換転送（WORD転送時に使用）#*/
#define     COM_CHANGE      1           /*# 変換転送（WORD転送時に使用）#*/

    /*# タスクＭＡＸ #*/
//#define       TCB_MAX         14          /*# タスク(TCB)数 #*/
//#define       TCB_MAX         17          /*# タスク(TCB)数 #*/   //2000.07 V2.1変更
//#define       TCB_MAX         18          /*# タスク数 0--17 #*/      // 2001.08 V2.3変更
//#define       TCB_MAX         38          /*# タスク数 0--37 #*/ /* 圧縮LU */
//#define       TCB_MAX         39          /*# タスク数 0--38 #*/ /* 圧縮LU */
//圧縮LU対応。DSPコマンドタスク2つにしたので変更。2003.09.08
//#define       TCB_MAX         40              /*# タスク数 0--39 #*/ /* 圧縮LU */
#define     TCB_MAX         14              /*# タスク数 0--14 #*/ /* IP-CS *///2004.04

    /*# プライオリティ ＭＡＸ #*/
//#define       PRI_MAX         1           /*# 優先度数 #*/
//#define       PRI_MAX         2           /*# 優先度数 #*/    // 99.11.10
#define     PRI_MAX         1           /*# 優先度数 #*/    // 99.11.29
    /*# ＥＣＢ ＭＡＸ #*/
//#define       ECB_MAX         44          /*# ＥＣＢ数 #*/
//#define       ECB_MAX         47          /*# ＥＣＢ数 #*/        //2000.07 V2.1変更
//#define       ECB_MAX         49              /*# ＥＣＢ数 #*/        //2001.02.08 V2.2変更forMENTE
//#define       ECB_MAX         50              /*# ＥＣＢ数  0--49 #*/     //2001.08.08 V2.3変更 forLULOG
//#define       ECB_MAX         70              /*# ＥＣＢ数  0--69 #*/ /* 圧縮LU */
//#define       ECB_MAX         72              /*# ＥＣＢ数  0--71 #*/ /* 圧縮LU */

//圧縮LU対応。DSPコマンドタスク2つにしたので変更。2003.09.08
//#define       ECB_MAX         73              /*# ＥＣＢ数  0--72 #*/ /* 圧縮LU */
#define     ECB_MAX         97              /*# ＥＣＢ数  0--96 #*/ /* 圧縮LU *///2003.10 V3.1


    /*# ＬＯＧ ＭＡＸ #*/
#define     MIK_LOG_MAX     256         /*# ＭＩＫログ面数 #*/
    /*# プールＭＡＸ #*/
//#define       POOL_MAX        16          /*# プールＩＤ数(V1) #*/
//#define       POOL_MAX        25          /*# プールＩＤ数(V1) #*///2000.07 V2.1変更
//#define       POOL_MAX        28          /*# プールＩＤ数 #*/ /* 圧縮LU */

//圧縮LU対応。DSPリアルタイムコマンド用追加したので変更。2003.09.08
#define     POOL_MAX        32          /*# プールＩＤ数 0--31 #*/ /* 圧縮LU */

    /*# タスク用スタックサイズ定義 #*/
#define     sup_Stack_size          2048        /*# supスタックサイズ #*/
#define     dpisr_main_Stack_size   2048
#define     lumng_Stack_size        2048
#define     doep_Stack_size         2048
#define     h323_Stack_size         2048
#define     clkd_Stack_size         2048
#define     crintctl_Stack_size     2048
#define     tftp_Stack_size         2048
#define     tmr_Stack_size          2048
//#define       door_main_Stack_size    2048//IP-CS対応削除 2004.04
#define     dspmng_Stack_size       2048
#define     fusion_rcv_Stack_size   2048        /*# fusion_rcvスタックサイズ #*/
#define     fusion_tmr_Stack_size   2048        /*# fusion_tmrスタックサイズ #*/
//#define       almmng_Stack_size       2048        /*# (V1) #*///IP-CS対応削除 2004.04
#define     ipl_Stack_size          2048        /*# IPL用(V1) #*/
//#define       bootpsrv_Stack_size     2048        /*# BOOTPサーバスタックサイズ #*/   //2000.07 V2.1追加//IP-CS対応削除 2004.04
//#define       tftpsrv_Stack_size      2048        /*# TFTPサーバスタックサイズ #*/    //2000.07 V2.1追加//IP-CS対応削除 2004.04
//#define       sioctl_Stack_size       2048        /*# SIO制御スタックサイズ #*/       //2000.07 V2.1追加//IP-CS対応削除 2004.04
#define     mnt_Stack_size          2048        /*# 保守設定スタックサイズ #*/      //2001.02.07 V2.2追加
#define     lu_log_Stack_size       2048        /*# ＬＵログスタックサイズ #*/      //2001.08 V2.3追加
/* 圧縮LU */
//#define       fcl_mng_Stack_size      2048        /*# FAX管理スタックサイズ #*///IP-CS FAX対応削除 2004.04
//#define       fcl_t30_Stack_size      2048        /*# T30 スタックサイズ #*/
//#define       fcl_t38_Stack_size      2048        /*# T38 スタックサイズ #*/
//#define       fcl_mdm_Stack_size      2048        /*# MDM スタックサイズ #*/
//#define       fcl_acc1_Stack_size     2048        /*# ACC1スタックサイズ #*/
//#define       fcl_acc2_Stack_size     2048        /*# ACC2スタックサイズ #*/
//#define       fcl_acc3_Stack_size     2048        /*# ACC3スタックサイズ #*/
//#define       fcl_acc4_Stack_size     2048        /*# ACC4スタックサイズ #*/
//#define       fcl_acc5_Stack_size     2048        /*# ACC5スタックサイズ #*/
//#define       fcl_acc6_Stack_size     2048        /*# ACC6スタックサイズ #*/
//#define       fcl_acc7_Stack_size     2048        /*# ACC7スタックサイズ #*/
//#define       fcl_acc8_Stack_size     2048        /*# ACC8スタックサイズ #*/
//#define       fcl_con1_Stack_size     2048        /*# CON1スタックサイズ #*/
//#define       fcl_con2_Stack_size     2048        /*# CON2スタックサイズ #*/
//#define       fcl_con3_Stack_size     2048        /*# CON3スタックサイズ #*/
//#define       fcl_con4_Stack_size     2048        /*# CON4スタックサイズ #*/
//#define       fcl_con5_Stack_size     2048        /*# CON5スタックサイズ #*/
//#define       fcl_con6_Stack_size     2048        /*# CON6スタックサイズ #*/
//#define       fcl_con7_Stack_size     2048        /*# CON7スタックサイズ #*/
//#define       fcl_con8_Stack_size     2048        /*# CON8スタックサイズ #*/

//圧縮LU対応 2003.08.22
//#define       dspcommand_Stack_size   2048        /*# DSPコマンドタスクスタックサイズ #*/
//圧縮LU対応 2003.09.03
#define     dspcommand0_Stack_size  2048        /*# DSPコマンドタスク0スタックサイズ #*/
//#define       dspcommand1_Stack_size  2048        /*# DSPコマンドタスク1スタックサイズ #*///IP-CS対応削除 2004.04


#ifdef  NO_BOOTP_USE
//#define   LU_IP                   0x02010b0a
//#define   CCU_IP                  0x01010b0a
//#define   DEFAULT_MASK            0x00ffffff
//#define   LU_IP                   0x0a0b0102      // 10.11.1.2
//#define   CCU_IP                  0x0a0b0101      // 10.11.1.1
#define     LU_IP                   0xc0a80164      // 192.168.1.100
//#define   LU_IP                   0xc0a8010a      // 192.168.1.10
//#define   CCU_IP                  0xc0a801dd      // 192.168.1.221
//#define   CCU_IP                  0xc0a801de      // 192.168.1.222
#define     CCU_IP                  0xc0a801e0      // 192.168.1.222
#define     DEFAULT_MASK            0xffffff00
//#define   CCU_IP1                 0xde01a8c0      // 192.168.1.222
//#define   CCU_IP2                 0xde01a8c0      // 192.168.1.222
#define     CCU_IP1                 0xe001a8c0      // 192.168.1.222
#define     CCU_IP2                 0xe101a8c0      // 192.168.1.222
#endif  // NO_BOOTP_USE

//#define           DOEP_CCU_PORT       49200
#define         DOEP_CCU_PORT       0x30c0          // 49200
//#define           DOEP_MY_PORT        49200
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
//#define       POW_MSK 0x18                                    /*# 障害情報ﾚｼﾞｽﾀ bit3,4ﾏｽｸ用  #*/
//#define       UPS_MSK 0x60                                    /*# 障害情報ﾚｼﾞｽﾀ bit5,6ﾏｽｸ用  #*/
#define     AC_MSK      0x18                                /*# 障害情報レジスタ bit3,4マスク用  #*/
#define     AC_OK       0x18                                /*# ＡＣ正常 #*/
#define     AC_OFF      0x00                                /*# 停電ＡＣオフ #*/
#define     AC_FAIL0    0x10                                /*# ０系異常 #*/
#define     AC_FAIL1    0x08                                /*# １系異常 #*/

#define     EMA_OK      0x0001                              /*# 通常（運用中） #*/
#define     EMA_NG      0x0000                              /*# 障害（直通切替） #*/

// ハードウェアバージョン
//#define       E_V1_2_HARD 0x13                                /*# V1.2値+1（V1.1ハードは0xffに見えるため+1して0とする） #*/   // 2000.05.12 V1.2追加
//#define       E_V2_1_HARD 0x22                                /*# V2.1値+1（V1.1ハードは0xffに見えるため+1して0とする） #*/   // 2000.07 V2.1追加
#define     E_V1_1_HARD 0x11                                /*# V1.1値（V1.1ハードは0xffに見えるが処理で強制変換する） #*/      // 2000.09 V2.1追加
#define     E_V1_2_HARD 0x12                                /*# V1.2値 #*/                                                      // 2000.09 V1.2変更
#define     E_V2_1_HARD 0x21                                /*# V2.1値 #*/                                                      // 2000.09 V2.1変更

#define     E_V1_0_KAPPA    0x0000                          /*# κＶ１ #*/  //2000.09 V2.1追加
#define     E_V2_0_KAPPA    0x0100                          /*# κＶ２ #*/  //2000.09 V2.1追加

// ＣＣＵバージョン
//#define       E_V1_1_CCU  0x0101                              /*# ダミー（未使用）#*/ // 2000.07 V2.1追加
//#define       E_V1_2_CCU  0x0102                              /*# ダミー（未使用）#*/ // 2000.07 V2.1追加
//#define       E_V2_1_CCU  0x0201                              /*# CCUバージョン番号 #*/   // 2000.07 V2.1追加
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

// ＦＬＡＳＨデータ種別 2000.09 V2.1追加
enum {
    FLS_DATID_IPL,                  /*# ＩＰＬ（使用不可） #*/
    FLS_DATID_CMD,                  /*# コマンド発行 #*/
    FLS_DATID_DAT,                  /*# 管理データ #*/
    FLS_DATID_HORY,                 /*# 保留音 #*/
    FLS_DATID_TALKY,                /*# トーキー #*/
    FLS_DATID_SYS,                  /*# オンラインシステムファイル #*/
    FLS_DATID_DSP,                  /*# ＤＳＰプログラム #*/
    FLS_DATID_V3DSP,                /*# 外付けＤＳＰプログラム #*///2003.8追加
    FLS_DATID_CADATA,               /*# CA切り替え情報：IP-CS(SS9100_PH1再開) 追加 #*/
    FLS_DATID_ETC,                  /*# その他 #*/
    FLS_DATID_MAX                   /*# データ種別最大個数 #*/
};

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

//SS9100 2004.11.12 EXT_IPstageは削除して以降つめる
//#define       EXT_IPstage     0x0002  //2004.04 IP-CS
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
#ifdef  MAKE_IPL
#define     SUP_ID          0x00        /*# サポートタスク #*/
#define     DRIVER_ID       0x01        /*# デバイスドライバー受信タスク #*/
#define     FUSION_ID       0x02        /*# FUSIONタイマタスク #*/
#define     IPL_ID          0x03        /*# IPLタスク #*/
//#define       SIOCTL_ID       0x04        /*# ＳＩＯ制御 #*/      // 2000.07 V2.1//IP-CS削除2004.04
#define     FLASH_ID        0x05        /*# ＦＬＡＳＨ制御 #*/  // 2000.09 V2.1追加
#else//オンライン
#ifdef  MENTE
#define     SUP_ID          0x00        /*# サポートタスク #*/
#define     LUMNG_ID        0x01        /*# ＬＵ管理タスク #*/
#define     DOEP_ID         0x02        /*# ＤＯＥＰ制御 #*/
#define     TMR_ID          0x03        /*# タイマタスク #*/
#define     DRIVER_ID       0x04        /*# デバイスドライバー受信タスク #*/
#define     FUSION_ID       0x05        /*# FUSIONタイマタスク #*/
#define     MNT_ID          0x06        /*# 保守設定制御 #*/    // 2001.02.07 V2.2追加
#define     DSPINT_ID       0x07        /*# ＤＳＰ割り込み #*/      //2000.07.13 V2.1DEBUG
#define     FLASH_ID        0x08        /*# ＦＬＡＳＨ制御 #*/  // 2000.09 V2.1追加

#define     COM_ID          0x80        /*# 共通関数(タスクではない！！) #*/
#else   //#ifdef    MENTE
    /*# タスクID(タスクアタッチ用,その他タスク識別用) #*/
#define     SUP_ID          0x00        /*# サポートタスク #*/
#define     DPISR_ID        0x01        /*# ＤＰＲＡＭ制御  #*/
#define     LUMNG_ID        0x02        /*# ＬＵ管理タスク #*/
#define     DOEP_ID         0x03        /*# ＤＯＥＰ制御 #*/
#define     H323_ID         0x04        /*# Ｈ３２３制御 #*/
#define     CLKD_ID         0x05        /*# ＣＬＫＤ制御 #*/
#define     CRINTCTL_ID     0x06        /*# ＣＲＩＮＴ制御 #*/
#define     TFTP_ID         0x07        /*# ＴＦＴＰ制御 #*/
#define     TMR_ID          0x08        /*# タイマタスク #*/
//#define       DOOR_ID         0x09        /*# ドアホン制御 #*///IP-CS削除2004.04
#define     DSPMNG_ID       0x09        /*# ＤＳＰ状態管理 #*/
#define     DRIVER_ID       0x0A        /*# デバイスドライバー受信タスク #*/
#define     FUSION_ID       0x0B        /*# FUSIONタイマタスク #*/
//#define       ALMMNG_ID       0x0D        /*# ＡＬＭＭＮＧ #*///IP-CS削除2004.04
//#define       BOOTPSRV_ID     0x0E        /*# ＢＯＯＴＰサーバ #*/// 2000.07 V2.1追加//IP-CS削除2004.04
//#define       TFTPSRV_ID      0x0F        /*# ＴＦＴＰサーバ #*/  // 2000.07 V2.1追加//IP-CS削除2004.04
//#define       SIOCTL_ID       0x10        /*# ＳＩＯ制御 #*/      // 2000.07 V2.1追加//IP-CS削除2004.04
#define     LULOG_ID        0x0C        /*# ＬＵログ #*/        // 2001.08 V2.3追加
//
//#define       FAXMNG_ID       0x12        /*# FAX管理タスク #*//* 圧縮LU *///IP-CS FAX削除2004.04
//#define       T30_ID          0x13        /*# T30タスク #*/
//#define       T38_ID          0x14        /*# T38タスク #*/
//#define       MDM_ID          0x15        /*# MDMタスク #*/
//#define       ACC1_ID         0x16        /*# accept(CH1)タスク #*/
//#define       ACC2_ID         0x17        /*# accept(CH2)タスク #*/
//#define       ACC3_ID         0x18        /*# accept(CH3)タスク #*/
//#define       ACC4_ID         0x19        /*# accept(CH4)タスク #*/
//#define       ACC5_ID         0x1A        /*# accept(CH5)タスク #*/
//#define       ACC6_ID         0x1B        /*# accept(CH6)タスク #*/
//#define       ACC7_ID         0x1C        /*# accept(CH7)タスク #*/
//#define       ACC8_ID         0x1D        /*# accept(CH8)タスク #*/
//#define       CON1_ID         0x1E        /*# connect(CH1)タスク #*/
//#define       CON2_ID         0x1F        /*# connect(CH2)タスク #*/
//#define       CON3_ID         0x20        /*# connect(CH3)タスク #*/
//#define       CON4_ID         0x21        /*# connect(CH4)タスク #*/
//#define       CON5_ID         0x22        /*# connect(CH5)タスク #*/
//#define       CON6_ID         0x23        /*# connect(CH6)タスク #*/
//#define       CON7_ID         0x24        /*# connect(CH7)タスク #*/
//#define       CON8_ID         0x25        /*# connect(CH8)タスク #*/
//圧縮LU対応。タスクID追加 2003.08.22
//圧縮LU対応。タスクID追加 2003.09.03

#define     DSPCOMMAND0_ID  0x0D        /*# DSP0コマンドタスク #*/
//#define       DSPCOMMAND1_ID  0x0E        /*# DSP1コマンドタスク #*/

#define     CRINT_ID        0x0E        /*# ＣＲＩＮＴ割り込み #*/  //2000.07.13 V2.1DEBUG
#define     DSPINT_ID       0x0F        /*# ＤＳＰ割り込み #*/      //2000.07.13 V2.1DEBUG
#define     DSPINT2_ID      0x10        /*# ＤＳＰ割り込み2 #*/ // 2003.08 V3.1追加
#define     FLASH_ID        0x11        /*# ＦＬＡＳＨ制御 #*/  // 2000.09 V2.1追加


#define     COM_ID          0x80        /*# 共通関数(タスクではない！！) #*/
#endif  //#ifdef    MENTE

#endif  // MAKE_IPL

    /*# タスクECB(SND_MSG用) #*/
#define     SUP_ECB         0x00        /*# サポートタスク #*/
#define     DPISR_ECB       0x01        /*# ＤＰＲＡＭ制御  #*/
#define     LUMNG_ECB       0x02        /*# ＬＵ管理タスク #*/
#define     DOEP_ECB        0x03        /*# ＤＯＥＰ制御 #*/
#define     H323_ECB        0x04        /*# Ｈ３２３制御 #*/
#define     CLKD_ECB        0x05        /*# ＣＬＫＤ制御 #*/
#define     CRINTCTL_ECB    0x06        /*# ＣＲＩＮＴ制御 #*/
#define     TFTP_ECB        0x07        /*# ＴＦＴＰ制御 #*/
#define     TMR_ECB         0x08        /*# タイマタスク #*/
#define     CRINT_ECB       0x09        /*# ＣＲＩＮＴ割り込み #*/
#define     DOOR_ECB        0x0A        /*# ドアホン制御 #*/
#define     DSPMNG_ECB      0x0B        /*# ＤＳＰ状態管理 #*/
#define     DSPINT_ECB      0x0C        /*# ＤＳＰ割り込み #*/
#define     ALMMNG_ECB      0x0D        /*# ＡＬＭＭＮＧ #*/
#define     BOOTPSRV_ECB    0x0E        /*# ＢＯＯＴＰサーバ #*/                // 2000.07 V2.1追加
#define     TFTPSRV_ECB     0x0F        /*# ＴＦＴＰサーバ #*/                  // 2000.07 V2.1追加
#define     SIOCTL_ECB      0x10        /*# ＳＩＯ制御 #*/                      // 2000.07 V2.1追加
//#define       DRIVER_ECB      0x0E        /*# デバイスドライバー受信タスク #*/
#define     DRIVER_ECB      0x11        /*# デバイスドライバー受信タスク #*/    // 2000.07 V2.1変更
//#define       FUSION_ECB1     0x0F        /*# FUSION用ECB1 #*/
#define     FUSION_ECB1     0x12        /*# FUSION用ECB1 #*/                    // 2000.07 V2.1変更
//#define       FUSION_ECB29    0x2F    /*# FUSION用ECB29（RESERVEレンジ） #*/  // 2000.07 V2.1変更
//#define       FUSION_ECB53    0x47    /*# FUSION用ECB53（RESERVEレンジ） #*/  // 2003.10 V3.1変更

#define     MNT_ECB         0x48        /*# 保守用インタフェース #*/            // 2001.02.07 V2.2追加
#define     LULOG_ECB       0x49        /*# ＬＵログ #*/                        // 2001.08.08 V2.3追加

#define     FAXMNG_ECB      0x4A        /*# FAX管理タスク #*//* 圧縮LU */
#define     T30_ECB         0x4B        /*# T30タスク #*/
#define     T38_ECB         0x4C        /*# T38タスク #*/
#define     MDM_ECB         0x4D        /*# MDMタスク #*/
#define     ACC1_ECB        0x4E        /*# accept(CH1)タスク #*/
#define     ACC2_ECB        0x4F        /*# accept(CH2)タスク #*/
#define     ACC3_ECB        0x50        /*# accept(CH3)タスク #*/
#define     ACC4_ECB        0x51        /*# accept(CH4)タスク #*/
#define     ACC5_ECB        0x52        /*# accept(CH5)タスク #*/
#define     ACC6_ECB        0x53        /*# accept(CH6)タスク #*/
#define     ACC7_ECB        0x54        /*# accept(CH7)タスク #*/
#define     ACC8_ECB        0x55        /*# accept(CH8)タスク #*/
#define     CON1_ECB        0x56        /*# connect(CH1)タスク #*/
#define     CON2_ECB        0x57        /*# connect(CH2)タスク #*/
#define     CON3_ECB        0x58        /*# connect(CH3)タスク #*/
#define     CON4_ECB        0x59        /*# connect(CH4)タスク #*/
#define     CON5_ECB        0x5A        /*# connect(CH5)タスク #*/
#define     CON6_ECB        0x5B        /*# connect(CH6)タスク #*/
#define     CON7_ECB        0x5C        /*# connect(CH7)タスク #*/
#define     CON8_ECB        0x5D        /*# connect(CH8)タスク #*/

//圧縮LU対応。ECB追加
#define     DSPCOMMAND0_ECB 0x5E        /*# DSPコマンドタスク0 #*/
#define     DSPCOMMAND1_ECB 0x5F        /*# DSPコマンドタスク1 #*/
#define     DSPINT2_ECB     0x60        /*# DSP割込み処理2 #*/



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
#define     I_HPI_ECHO_REPLY        0x19    /* HPIエコー応答受信 */
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

//#define       I_DPRAMRD       0x50        /*# DPRAMデータ読み出し要求 #*/
#define     I_PGDLCMP       0x51        /*# プログラムダウンロード完了 #*///2004.04 IP-CS 追加

#define     I_CRINTSRT      0x60        /*# CRINT起動要求 #*/

#define     I_CLKDSRT       0x70        /*# CLKD起動要求 #*/

#define     I_VOICESND      0x80        /*# 音声送信要求 #*/
#define     I_SDESRCV       0x81        /*# SDES受信 #*/
#define     I_BYERCV        0x82        /*# BYE受信 #*/

#define     I_BOOTPREQRCV   0x90        /*# BOOTPreq受信 #*/    //2000.07 V2.1追加

#define     I_TFTPDATRCV    0xa0        /*# TFTPデータ受信 #*/  //2000.07 V2.1追加

#define     I_SIODATRCV     0xb0        /*# SIOデータ受信 #*/   //2000.07 V2.1追加

/* FAX管理タスクにて受信するメッセージの定義    */
#define     I_FAXTONE       0xc0        /*# FAXトーン検出 #*/
#define     I_FAXEND        0xc1        /*# FAX手順終了 #*/
#define     I_T38STATUS     0xc2        /*# T38状態通知 #*/
#define     I_FAXKILL       0xc3        /*# FAX強制停止指示 #*/
#define     I_FAXTOVOICE        0xc4        /*# FAXtoVOICE終了 #*/
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
#define     POOL0           0x00        /*# 一般メッセージ用        #*//*# DOEPにてUSER-QUE使用 #*/
#define     POOL1           0x01        /*# 二次メッセージ用        #*//*# LU管理にてUSER-QUE使用 #*/
#define     POOL2           0x02        /*# DSP制御データ用     #*//*# DSPにてUSER-QUE使用 #*/
#define     POOL3           0x03        /*# 音声受信用 #*/
#define     POOL4           0x04        /*# 音声送信用 #*/
#define     POOL5           0x05        /*# UDP送信用 #*/
#define     POOL6           0x06        /*# FUSION用 #*/
#define     POOL7           0x07        /*# TAG用 #*/
#define     POOL8           0x08        /*# DMY #*//*# DPRAM処理CN0にてUSER-QUE使用 #*/
#define     POOL9           0x09        /*# DMY #*//*# DPRAM処理CN1にてUSER-QUE使用 #*/
#define     POOLA           0x0A        /*# DMY #*//*# DPRAM処理CN2にてUSER-QUE使用 #*/
#define     POOLB           0x0B        /*# DMY #*//*# DPRAM処理CN3にてUSER-QUE使用 #*/
#define     POOLC           0x0C        /*# DMY #*//*# DPRAM処理CN4にてUSER-QUE使用 #*/
#define     POOLD           0x0D        /*# DMY #*//*# DPRAM処理CN5にてUSER-QUE使用 #*/
#define     POOLE           0x0E        /*# DMY #*//*# DPRAM処理CN6にてUSER-QUE使用 #*/
#define     POOLF           0x0F        /*# DMY #*//*# DPRAM処理CN7にてUSER-QUE使用 #*/
#define     POOL10          0x10        /*# DMY #*//*# DPRAM処理CN8にてUSER-QUE使用 #*/ //2000.07 V2.1追加
#define     POOL11          0x11        /*# DMY #*//*# DPRAM処理CN9にてUSER-QUE使用 #*/ //2000.07 V2.1追加
#define     POOL12          0x12        /*# DMY #*//*# DPRAM処理CN10にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL13          0x13        /*# DMY #*//*# DPRAM処理CN11にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL14          0x14        /*# DMY #*//*# DPRAM処理CN12にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL15          0x15        /*# DMY #*//*# DPRAM処理CN13にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL16          0x16        /*# DMY #*//*# DPRAM処理CN14にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL17          0x17        /*# DMY #*//*# DPRAM処理CN15にてUSER-QUE使用 #*///2000.07 V2.1追加
#define     POOL18          0x18        /*# TFTPサーバ用 #*/                            //2000.07 V2.1追加
#define     POOL19          0x19        /*# T38パケット用  #*//* 圧縮LU */                          //2003.08 追加
#define     POOL1A          0x1A        /*# FAXデータ用    #*/                          //2003.08 追加
#define     POOL1B          0x1B        /*# FAXデータ管理用#*/                          //2003.08 追加
//圧縮LU対応。DSPリアルタイムコマンド用。2003.09.03
#define     POOL1C          0x1C        /*# DSPリアルタイムコマンド用#*/
#define     POOL1D          0x1D        /*# トレース情報用#*/                       //2003.09
#define     POOL1E          0x1E        /*# DSP初期化用#*/                      //2003.09
#define     POOL1F          0x1F        /*# DSP初期化用#*/                      //2003.09


    /*# プールサイズ #*/
//#define       POOL0_SIZE      40          /*# 一般メッセージ用 [12(os)+28(内部msg)] #*/
#define     POOL0_SIZE      56          /*# 一般メッセージ用 [12(os)+44(内部msg)] #*/ //2003.10.23
#define     POOL1_SIZE      280         /*# 二次メッセージ用 [12(os)+12(LUﾍｯﾀﾞ)+256(ｵｰﾀﾞ/ｲﾍﾞﾝﾄ情報)] #*/
#define     POOL2_SIZE      80          /*# DSP制御データ用 [12(os)+4(SystemSTS)+64] #*/
#define     POOL3_SIZE      1540        /*# 音声受信メッセージ用 [12(os)+1528(Max LAN Rcv Size)] #*/
#define     POOL4_SIZE      984         /*# 音声送信メッセージ用 [12(os)+12(rtp)+960(音声120ms)) #*/
#define     POOL5_SIZE      140         /*# UDP送信用 [12(os)+128(RTCP、CLKD、CRINT)]#*/
#define     POOL6_SIZE      20          /*# FUSION用 [12(os)+4(SystemSTS)+4(Dmy)] #*/
#define     POOL7_SIZE      28          /*# TAG用 [12(os)+16(TAG)] #*/
#define     POOL8_SIZE      1048        /*# EXメッセージ用 [12(os)+12(LUﾍｯﾀﾞ)+1024(ｵｰﾀﾞ/ｲﾍﾞﾝﾄ情報)]  #*/
#define     POOL9_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLA_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLB_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLC_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLD_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLE_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOLF_SIZE      16          /*# DMY USER-QUE使用 #*/
#define     POOL10_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL11_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL12_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL13_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL14_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL15_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL16_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL17_SIZE     16          /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL18_SIZE     528         /*# TFTPサーバ用[12(os)+2(op)+2(blk)+512(DUPパケット長最大)] #*///2000.07 V2.1追加
#define     POOL19_SIZE     (12+1500)       /*# T38パケット用  #*//* 圧縮LU */                          //2000.07 V2.1追加
#define     POOL1A_SIZE     (12+400)        /*# FAXデータ用    #*/                          //2000.07 V2.1追加
#define     POOL1B_SIZE     (12+16)     /*# FAXデータ管理用#*/                          //2000.07 V2.1追加
//圧縮LU対応。DSPリアルタイムコマンド用。2003.09.03
#define     POOL1C_SIZE     (12+2048)       /*# DSPリアルタイムコマンド用　#*/
#define     POOL1D_SIZE     (12+256)        /*# トレース情報用　#*/                         //2003.09
#define     POOL1E_SIZE     (12+4096+4)     /*# DSP用　#*/                          //2003.09
#define     POOL1F_SIZE     (12+4096+4)     /*# DSP用　#*/                          //2003.09


    /*# プール面数 #*/
#define     POOL0_CNT       512         /*# 一般メッセージ用 #*/
#define     POOL1_CNT       256         /*# 二次メッセージ用 #*/
#define     POOL2_CNT       32          /*# DSP制御データ用 #*/
#define     POOL3_CNT       1           /*# 音声受信用 #*/
#define     POOL4_CNT       512         /*# 音声送信用 #*/
#define     POOL5_CNT       32          /*# UDP送信用 #*/
#define     POOL6_CNT       256         /*# FUSION用 #*/

//#define       POOL7_CNT       32          /*# TAG用 #*/
#define     POOL7_CNT       224         /*# TAG用[96(uni_CLKD)+96(uni_CRINT)+32(元の数)] #*/ // 2002.03.28変更

#define     POOL8_CNT       10          /*# EXメッセージ用 #*/
#define     POOL9_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLA_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLB_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLC_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLD_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLE_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOLF_CNT       1           /*# DMY USER-QUE使用 #*/
#define     POOL10_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL11_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL12_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL13_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL14_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL15_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL16_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL17_CNT      1           /*# DMY USER-QUE使用 #*///2000.07 V2.1追加
#define     POOL18_CNT      (32*2)      /*# TFTP&BOOTPサーバ用 #*/  //2000.07 V2.1追加
#define     POOL19_CNT      (32)        /*# T38パケット用   #*//* 圧縮LU */
#define     POOL1A_CNT      (240)       /*# FAXデータ用     #*/
#define     POOL1B_CNT      (250)       /*# FAXデータ管理用 #*/
//圧縮LU対応。DSPリアルタイムコマンド用。2003.09.03
#define     POOL1C_CNT      16          /*# DSPリアルタイムコマンド用　#*/
#define     POOL1D_CNT      50          /*# トレース情報用　#*/             //2003.09
#define     POOL1E_CNT      8           /*# トレース情報用　#*/             //2003.09
#define     POOL1F_CNT      8           /*# トレース情報用　#*/             //2003.09



    /*# ユーザキューID #*/
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

#ifdef LU_MACRO
#if 0 /* Ozawa */
#define     com_poolget(a)          XGETPB_R(a);
#define     com_ipoolget(a)         XSGETPB_R(a);/* com_poolgetと同じにする */
#define     com_poolput(a,b)        XPUTPB_R(a,b)
#define     com_ipoolput(a,b)       XSPUTPB_R(a,b)
#define     com_iptdis(void)        pri = XIPTMR(void);\
                                    XIPTMW(3);
#define     com_iptena(a)           XIPTMW(a)
#define     com_sndmsg(a,b)         XPOSTM(a,b)
#define     com_isndmsg(a,b)        XSPOSTM(a,b)
#define     com_rcvmsg(a,b)         XWTMR1(a,b)
#define     com_getmsg(a)           XGETEV(a)
#define     com_igetmsg(a)          XSGETEV(a)
#define     com_queput(a,b,c)       XPUTUL_R(a,b,c)
#define     com_iqueput(a,b,c)      XSPUTUL_R(a,b,c)
#define     com_queget(a,b)         XGETUL_R(a,b)
#define     com_iqueget(a,b)        XSGETUL_R(a,b)
#define     com_quesns(a)           XULSNS_R(a)
#define     com_iquesns(a)          XSULSNS_R(a)
#endif /* Ozawa */
#define     com_chgword(C)          (WORD)((((C)>>8)&0xFF)|(((C)<<8)&0xFF00))
#define     com_chgdword(C)         ((DWORD)(((((DWORD)C)>>24)&0xFF)|((((DWORD)C)>>8)&0xFF00)|((((DWORD)C)<<8)&0xFF0000)|((((DWORD)C)<<24)&0xFF000000)))
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
//圧縮LU対応。セマフォ用マクロ追加。2003.08.27
#define     com_getsemp(a,b,c)      XGETSEM(a,b,c)
#define     com_putsemp(a)          XPUTSEM(a)
#define     com_initsemp(a,b)       XINITSEM(a,b)
#define     com_sempsns(a)          XSEMSNS(a)
#endif //LU_MACRO

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

/*###########################################################################*/
/*#                                                                         #*/
/*#                           ＣＰＵ周辺アドレス                            #*/
/*#                                                                         #*/
/*###########################################################################*/
/*##################################################################*/
/*#           κ－ＬＳＩ内部デバイス                               #*/
/*##################################################################*/
/*########################################################*/
/*#           プロセッサ内                               #*/
/*########################################################*/
#define     IO_RESET_VECT       0x00000000          /*# リセットベクタアドレス #*/
#define     IO_ROM_BASE         0x00000000          /*# ＲＯＭ先頭アドレス #*/
#define     IO_INRAM_BASE       0x10000000          /*# 内部ＲＡＭ先頭アドレス #*/
#define     IO_INRAM_LIMIT      0x10002000          /*# 内部ＲＡＭ終了アドレス #*/

/*########################################################*/
/*#           ＡＳＢデバイス／ＡＨＢデバイス             #*/
/*########################################################*/
#define     IO_ICE              0x3fffffa0          /*# 命令キャッシュ制御レジスタ #*/
#define     IO_WBE              0x3fffffc0          /*# ライトバッファ制御レジスタ #*/

/*#########################################*/
/*#           ＭＡＣ                      #*/
/*#########################################*/
// デバイスドライバ部で定義済み

/*#########################################*/
/*#           ＰＩＣ                      #*/
/*#########################################*/
//圧縮LU対応。V1用レジスタコメントアウト。2003.08.25
//削除9月以降
#define     IO_IRQ              0x78000090          /*# ＩＲＱ割込要因 #*/
#define     IO_IRQ_SOFT         0x78000010          /*# ＩＲＱソフトステータス #*/
#define     IO_FIQ              0x78000100          /*# ＦＩＱ割込要因 #*/
#define     IO_FIQRAW           0x78000104          /*# ＦＩＱ ＲＡＷステータス #*/
#define     IO_FIQEN            0x7800010c          /*# ＦＩＱイネーブル #*/
#define     IO_IRNR             0x78000080          /*# ＩＲＱ番号 #*/
#define     IO_CILR             0x78000084          /*# 現ＩＲＱ番号 #*/
#define     IO_IRL              0x78000088          /*# ＩＲＱレベル #*/
#define     IO_IREL             0x7800008c          /*# ＩＲＱエッジ／レベル制御 #*/
#define     IO_IRCL             0x78000090          /*# ＩＲＱステータスクリア #*/
#define     IO_ILCR0            0x780000a0          /*# ＩＲＱレベル制御０ #*/
#define     IO_ILCR1            0x780000a4          /*# ＩＲＱレベル制御１ #*/
//Ｖ２ハード用（ＩＮＴＲＣ）    //2000.09 V2.1追加
#define     IO_V2_IRQ           0x78000000          /*# ＩＲＱ割込要因 #*/
#define     IO_V2_IRQ_SOFT      0x78000004          /*# ＩＲＱソフトステータス #*/
#define     IO_V2_FIQ           0x78000008          /*# ＦＩＱ割込要因 #*/
#define     IO_V2_FIQRAW        0x7800000c          /*# ＦＩＱ ＲＡＷステータス #*/
#define     IO_V2_FIQEN         0x78000010          /*# ＦＩＱイネーブル #*/
#define     IO_V2_IRNR          0x78000014          /*# ＩＲＱ番号 #*/
#define     IO_V2_CILR          0x78000018          /*# 現ＩＲＱ番号 #*/
#define     IO_V2_IRL           0x7800001c          /*# ＩＲＱレベル #*/
//#define   IO_V2_IREL          0x7800008c          /*# ＩＲＱエッジ／レベル制御 #*/
//#define   IO_V2_IRCL          0x78000090          /*# ＩＲＱステータスクリア #*/
#define     IO_V2_ILCR0         0x78000020          /*# ＩＲＱレベル制御０ #*/
#define     IO_V2_ILCR1         0x78000024          /*# ＩＲＱレベル制御１ #*/

/*#########################################*/
/*#           ＸＭＣ                      #*/
/*#########################################*/
//圧縮LU対応。V1用レジスタコメントアウト。2003.08.25
#define     IO_BWCON            0x78100000          /*# バス幅コントロールレジスタ #*/
#define     IO_OTCON            0x78100004          /*# オフタイムコントロールレジスタ #*/
#define     IO_PWCON0           0x78100008          /*# プログラマブルウェイトコントロールレジスタ０ #*/
#define     IO_PWCON1           0x7810000c          /*# プログラマブルウェイトコントロールレジスタ１ #*/
#define     IO_PWCON2           0x78100010          /*# プログラマブルウェイトコントロールレジスタ２ #*/
#define     IO_DRCON            0x78100014          /*# ＤＲＡＭコントロールレジスタ #*/
#define     IO_RFTCN            0x78100018          /*# リフレッシュタイマカウンタレジスタ #*/
#define     IO_RCCON            0x7810001c          /*# リフレッシュサイクルコントロールレジスタ #*/
#define     IO_RFCON            0x78100024          /*# リフレッシュコントロールレジスタ #*/
#define     IO_ATCON0           0x78100028          /*# アクセスタイミングコントロールレジスタ０ #*/
#define     IO_ATCON1           0x7810002c          /*# アクセスタイミングコントロールレジスタ１ #*/
#define     IO_ATCON2           0x78100030          /*# アクセスタイミングコントロールレジスタ２ #*/
//Ｖ２ハード用ダミー（本定義はシステム起動処理）//2000.09 V2.1追加
#define     IO_V2_XMC_BWC       0x78100000          /*# バス幅コントロールレジスタ #*/
#define     IO_V2_XMC_BAC       0x78100004          /*# バスアクセスコントールレジスタ #*/
#define     IO_V2_XMC_DRMC      0x78100008          /*# ＤＲＡＭコントロールレジスタ #*/
#define     IO_V2_XMC_DRAC      0x7810000c          /*# ＥＤＯ－ＤＲＡＭアクセスコントロールレジスタ #*/
#define     IO_V2_XMC_SDACL1    0x78100010          /*# ＳＤＲＡＭアクセスコントロールレジスタ１ #*/
#define     IO_V2_XMC_SDACL2    0x78100014          /*# ＳＤＲＡＭアクセスコントロールレジスタ２ #*/
#define     IO_V2_XMC_SDMD1     0x78100018          /*# ＳＤＲＡＭモードレジスタ１ #*/
#define     IO_V2_XMC_SDMD2     0x7810001c          /*# ＳＤＲＡＭモードレジスタ２ #*/
#define     IO_V2_XMC_RFCNT     0x78100020          /*# ＤＲＡＭリフレッシュタイマカウンタレジスタ #*/
#define     IO_V2_XMC_RFSH      0x78100024          /*# ＤＲＡＭリフレッシュサイクルコントロールレジスタ #*/
#define     IO_V2_XMC_ROMAC     0x78100028          /*# ＲＯＭアクセスコントロールレジスタ #*/
#define     IO_V2_XMC_ROMPC     0x7810002c          /*# ＲＯＭページモードコントロールレジスタ #*/
#define     IO_V2_XMC_SRMAC     0x78100030          /*# ＳＲＡＭアクセスコントロールレジスタ #*/
#define     IO_V2_XMC_SRMPC     0x78100034          /*# ＳＲＡＭページモードコントロールレジスタ #*/
#define     IO_V2_XMC_IOAC1     0x78100038          /*# ＩＯアクセスコントロールレジスタ１ #*/
#define     IO_V2_XMC_IOAC2     0x7810003c          /*# ＩＯアクセスコントロールレジスタ２ #*/

/*#########################################*/
/*#           ＤＳＰ（ＯＡＫ）            #*/
/*#########################################*/
#define     IO_PRAM             0x7e800000          /*# ＯＡＫ ＰＲＡＭ先頭アドレス #*/

#define     IO_CNT              0x7e800004          /*# ハードウェアコントロールレジスタ #*/
#define     IO_ST0              0x7e800008          /*# ハードウェアステータスレジスタ０ #*/
#define     IO_ST1              0x7e80000c          /*# ハードウェアステータスレジスタ１ #*/
#define     IO_WORD             0x7e800010          /*# オーダ数レジスタ #*/
#define     IO_ORDER            0x7e800014          /*# オーダレジスタ #*/
#define     IO_EVENT            0x7e800018          /*# イベントレジスタ #*/
#define     IO_V2_INTENA        0x7e800024          /*# ＩＮＴイネーブルレジスタ #*/
#define     IO_V2_INTCLR        0x7e800028          /*# ＩＮＴイクリアレジスタ #*/
#define     IO_RTFIFO           0x7e80001c          /*# 音声データＦＩＦＯ #*/
#define     IO_OAK_RST          0x7e80fffc          /*# リセット #*/

/*#########################################*/
/*#           ＣＡＣＨＥ                  #*/       //2000.09 V2.1追加
/*#########################################*/
#define     IO_V2_CACHE_CON     0x78200004          /*# キャッシュコントロールレジスタ #*/
#define     IO_V2_CACHE_CACHE   0x78200008          /*# キャッシャブルレジスタ #*/
#define     IO_V2_CACHE_ABORT   0x78200010          /*# アボートアドレスレジスタ #*/
#define     IO_V2_CACHE_FLUSH   0x7820001c          /*# フラッシュレジスタ #*/

/*########################################################*/
/*#           ＡＰＢデバイス                             #*/
/*########################################################*/
/*#########################################*/
/*#           ＲＥＳＥＴ                  #*/
/*#########################################*/
#define     IO_WDT_CON          0xb0000000          /*# ウォッチドッグタイマコントロール #*/
#define     IO_RST_REG          0xb0000004          /*# リセット #*/
#define     IO_WDT_CDL          0xb0000008          /*# ウォッチドッグタイマカウンタ下位 #*/
#define     IO_WDT_CDU          0xb000000c          /*# ウォッチドッグタイマカウンタ上位 #*/

/*#########################################*/
/*#           同期制御                    #*/
/*#########################################*/
#define     IO_CST              0xb0001000          /*# count start reg #*/
#define     IO_COF              0xb0001004          /*# count of reg #*/
#define     IO_PHS              0xb0001008          /*# phase reg #*/
#define     IO_COU              0xb000100c          /*# count reg #*/
#define     IO_SFT              0xb0001010          /*# shift reg #*/
#define     IO_SEL1             0xb0001014          /*# select1 reg #*/
#define     IO_SEL2             0xb0001018          /*# select2 reg #*/

/*#########################################*/
/*#           電話機Ｉ／Ｏ                #*/
/*#########################################*/
// ＬＵでは汎用ポートのみ使用
#define     IO_PORT0            0xb0002004          /*# ポート０ #*/
#define     IO_PORT1            0xb0002008          /*# ポート１ #*/
#define     IO_PORT2            0xb000200c          /*# ポート２ #*/
#define     IO_PORT3            0xb0002010          /*# ポート３ #*/
#define     IO_PORT4            0xb0002014          /*# ポート４ #*/
#define     IO_P4CON            0xb0002018          /*# ポート４コントロールレジスタ #*/

/*#########################################*/
/*#           ＳＩＯ                      #*/
/*#########################################*/
#define     IO_SIO_LC           0xb000400c          /*# ラインコントロール #*/
#define     IO_SIO_MC           0xb0004010          /*# モデムコントロール #*/
#define     IO_SIO_LST          0xb0004014          /*# ラインステータス #*/
#define     IO_SIO_MST          0xb0004018          /*# モデムステータス #*/
//2000.09 V2.1追加（start）
#define     IO_V2_SIO_BUF       0xb8002000          /*# 送受信バッファレジスタ #*/
#define     IO_V2_SIO_STA       0xb8002004          /*# ステータスレジスタ #*/
#define     IO_V2_SIO_CON       0xb8002008          /*# コントロールレジスタ #*/
#define     IO_V2_SIO_BCN       0xb800200c          /*# ボーレートコントロールレジスタ #*/
#define     IO_V2_SIO_BTC       0xb8002010          /*# ボーレートタイマカウンタレジスタ #*/
#define     IO_V2_SIO_BT        0xb8002014          /*# ボーレートタイマレジスタ #*/
#define     IO_V2_SIO_TCN       0xb8002018          /*# テストコントロールレジスタ #*/
//2000.09 V2.1追加（end）

/*#########################################*/
/*#           ＴＩＭＥＲ１／２／３        #*/
/*#########################################*/
//圧縮LU対応。V1用レジスタコメントアウト。2003.08.25
#define     IO_TMCON1           0xb8001000          /*# タイマコントロールレジスタ１ #*/
#define     IO_TMEN1            0xb8001004          /*# タイマイネーブルレジスタ１ #*/
#define     IO_TMRLR1           0xb8001008          /*# タイマリロードレジスタ１ #*/
#define     IO_TMC1             0xb800100c          /*# タイマカウンタ１ #*/
#define     IO_TMOVFR1          0xb8001010          /*# オーバーフローレジスタ１ #*/
#define     IO_TMCON2           0xb0005000          /*# タイマコントロールレジスタ２ #*/
#define     IO_TMEN2            0xb0005004          /*# タイマイネーブルレジスタ２ #*/
#define     IO_TMRLR2           0xb0005008          /*# タイマリロードレジスタ２ #*/
#define     IO_TMC2             0xb000500c          /*# タイマカウンタ２ #*/
#define     IO_TMOVFR2          0xb0005010          /*# オーバーフローレジスタ２ #*/
//Ｖ２ハード用（ＴＩＭＥＲ１は共通、ＴＩＭＥＲ２／３は実際は未使用）//2000.09 V2.1追加
//#define       IO_V2_TMCON2        0xb8001020          /*# タイマコントロールレジスタ２ #*/
//#define       IO_V2_TMEN2         0xb8001024          /*# タイマイネーブルレジスタ２ #*/
//#define       IO_V2_TMRLR2        0xb8001028          /*# タイマリロードレジスタ２ #*/
//#define       IO_V2_TMC2          0xb800102c          /*# タイマカウンタ２ #*/
//#define       IO_V2_TMOVFR2       0xb8001030          /*# オーバーフローレジスタ２ #*/
//#define       IO_V2_TMCON3        0xb8001040          /*# タイマコントロールレジスタ３ #*/
//#define       IO_V2_TMEN3         0xb8001044          /*# タイマイネーブルレジスタ３ #*/
//#define       IO_V2_TMRLR3        0xb8001048          /*# タイマリロードレジスタ３ #*/
//#define       IO_V2_TMC3          0xb800104c          /*# タイマカウンタ３ #*/
//#define       IO_V2_TMOVFR3       0xb8001050          /*# オーバーフローレジスタ３ #*/

#define     IO_V2_TMCON1        0xb0005000          /*# タイマコントロールレジスタ１ #*/
#define     IO_V2_TMEN1         0xb0005004          /*# タイマイネーブルレジスタ１ #*/
#define     IO_V2_TMRLR1        0xb0005008          /*# タイマリロードレジスタ１ #*/
#define     IO_V2_TMC1          0xb000500c          /*# タイマカウンタ１ #*/
#define     IO_V2_TMOVFR1       0xb0005010          /*# オーバーフローレジスタ１ #*/
#define     IO_V2_TMCON2        0xb0005020          /*# タイマコントロールレジスタ２ #*/
#define     IO_V2_TMEN2         0xb0005024          /*# タイマイネーブルレジスタ２ #*/
#define     IO_V2_TMRLR2        0xb0005028          /*# タイマリロードレジスタ２ #*/
#define     IO_V2_TMC2          0xb000502c          /*# タイマカウンタ２ #*/
#define     IO_V2_TMOVFR2       0xb0005030          /*# オーバーフローレジスタ２ #*/
#define     IO_V2_TMCON3        0xb0005040          /*# タイマコントロールレジスタ３ #*/
#define     IO_V2_TMEN3         0xb0005044          /*# タイマイネーブルレジスタ３ #*/
#define     IO_V2_TMRLR3        0xb0005048          /*# タイマリロードレジスタ３ #*/
#define     IO_V2_TMC3          0xb000504c          /*# タイマカウンタ３ #*/
#define     IO_V2_TMOVFR3       0xb0005050          /*# オーバーフローレジスタ３ #*/

/*#########################################*/
/*#           その他                      #*/
/*#########################################*/
#define     IO_LSIVER           0xb0003000          /*# ＬＳＩバージョンレジスタ #*/    //2000.09 V2.1追加
#define     IO_PINCNT           0xb0003004          /*# ＬＳＩ端子設定レジスタ #*/
#define     IO_CLK_PINCNT       0xb000300c          /*# クロック端子設定レジスタ #*/
#define     IO_V2_INTRESET      0xb0003020          /*# 割り込み要因レジスタ #*/        //2000.10.31 V2.1DEBUG

/*#########################################*/
/*#           ＲＥＭＡＰ                  #*/
/*#########################################*/
#define     IO_REMAP            0xb8000020          /*# リマップ制御アドレス #*/

/*##################################################################*/
/*#           κ－ＬＳＩ外部デバイス                               #*/
/*##################################################################*/
/*########################################################*/
/*#           外部デバイス                               #*/
/*########################################################*/

#define     IO_DRAM_BASE        0xc0000000          /*# ＤＲＡＭ先頭アドレス #*/
//圧縮LU対応。DRAMエンド変更。2003.08.25
//#define       IO_DRAM_LIMIT       0xc0400000          /*# ＤＲＡＭ終了アドレス #*/
#define     IO_DRAM_LIMIT       0xc0a00000          /*# ＤＲＡＭ終了アドレス #*/
#define     IO_RE_DRAM_BASE     0x00000000          /*# リマップ後ＤＲＡＭ先頭アドレス #*/

//圧縮LU対応。DRAMエンド変更。2003.08.25
//#define       IO_RE_DRAM_LIMIT    0x00400000          /*# リマップ後ＤＲＡＭ終了アドレス #*/
#define     IO_RE_DRAM_LIMIT    0x00a00000          /*# リマップ後ＤＲＡＭ終了アドレス #*/
#define     IO_MACROM_ADR_V2    0xc803fffc          /*# ＭＡＣアドレス #*///2003.8変更

#define     IO_IPLROM_ADR       0xc8000000          /*# IPL-ROMアドレス #*/ //2000.07 V2.1追加

/*#########################################*/
/*#           ＦＬＡＳＨ                  #*/       //2000.09 V2.1追加
/*#########################################*/

#define IO_FLS_BASE     0xc8000000
#define IO_FLS_CMDBASE  (IO_FLS_BASE+0x00040000)//2003.8変更

#define IO_FLS_SR       (WORD *)(IO_FLS_CMDBASE+0x00000000)
#define IO_FLS_CR       (WORD *)(IO_FLS_CMDBASE+0x0000fffe)

#define IO_FLS_MAKER    (WORD *)(IO_FLS_CMDBASE+0x00000000)
#define IO_FLS_CMDADRS1 (WORD *)(IO_FLS_CMDBASE+0x0000aaaa)
#define IO_FLS_CMDADRS2 (WORD *)(IO_FLS_CMDBASE+0x00005554)

//圧縮LU対応。FLASHアドレス変更。2003.08.25
#define IO_FLS_HOLDDATA (WORD *)(IO_FLS_BASE+0x00050000)    /*# 保留音データ #*/
//#define   IO_FLS_FPGADATA (WORD *)(IO_FLS_BASE+0x00090000)    /*# FPGAコンフィグデータ #*/
#define IO_FLS_FPGADATA (WORD *)(IO_FLS_BASE+0x000b0000+0x00060000) /*# FPGAコンフィグデータ #*///2004.04 IP-CS



/*#########################################*/
/*#           ＤＰＲＡＭ（マスタＬＵ）    #*/
/*#########################################*/
#define     IO_PDRAM_PP0        0xf0000000          /*# イベント（リード） #*/
#define     IO_MDRAM_PP0        0xf0000000          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP0        0xf0000100          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP0       0xf0000102          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP0       0xf0000104          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP0       0xf0000106          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP0       0xf0000108          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP0       0xf000010C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP1        0xf0000200          /*# イベント（リード） #*/
#define     IO_MDRAM_PP1        0xf0000200          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP1        0xf0000300          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP1       0xf0000302          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP1       0xf0000304          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP1       0xf0000306          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP1       0xf0000308          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP1       0xf000030C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP2        0xf0000400          /*# イベント（リード） #*/
#define     IO_MDRAM_PP2        0xf0000400          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP2        0xf0000500          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP2       0xf0000502          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP2       0xf0000504          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP2       0xf0000506          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP2       0xf0000508          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP2       0xf000050C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP3        0xf0000600          /*# イベント（リード） #*/
#define     IO_MDRAM_PP3        0xf0000600          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP3        0xf0000700          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP3       0xf0000702          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP3       0xf0000704          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP3       0xf0000706          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP3       0xf0000708          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP3       0xf000070C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP4        0xf0000800          /*# イベント（リード） #*/
#define     IO_MDRAM_PP4        0xf0000800          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP4        0xf0000900          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP4       0xf0000902          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP4       0xf0000904          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP4       0xf0000906          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP4       0xf0000908          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP4       0xf000090C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP5        0xf0000a00          /*# イベント（リード） #*/
#define     IO_MDRAM_PP5        0xf0000a00          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP5        0xf0000b00          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP5       0xf0000b02          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP5       0xf0000b04          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP5       0xf0000b06          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP5       0xf0000b08          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP5       0xf0000b0C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP6        0xf0000c00          /*# イベント（リード） #*/
#define     IO_MDRAM_PP6        0xf0000c00          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP6        0xf0000d00          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP6       0xf0000d02          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP6       0xf0000d04          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP6       0xf0000d06          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP6       0xf0000d08          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP6       0xf0000d0C          /*# リセット（ライト） #*/

#define     IO_PDRAM_PP7        0xf0000e00          /*# イベント（リード） #*/
#define     IO_MDRAM_PP7        0xf0000e00          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP7        0xf0000f00          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP7       0xf0000f02          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP7       0xf0000f04          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP7       0xf0000f06          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP7       0xf0000f08          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP7       0xf0000f0C          /*# リセット（ライト） #*/

//2001.02.20 V2.2DEBUG（start）
#define     IO_PDRAM_PP8        0xf0001000          /*# イベント（リード） #*/
#define     IO_MDRAM_PP8        0xf0001000          /*# オーダ（ライト） #*/
#define     IO_PEREG_PP8        0xf0001100          /*# ＰＫＧ実装情報（リード） #*/
#define     IO_PCLREG_PP8       0xf0001102          /*# ＰＫＧクラス（リード） #*/
#define     IO_PPVREG_PP8       0xf0001104          /*# ＰＰバージョン（リード） #*/
#define     IO_MRQREG_PP8       0xf0001106          /*# オーダ読出要求（ライト） #*/
#define     IO_PRQREG_PP8       0xf0001108          /*# イベント書込許可（リード） #*/
#define     IO_RSTREG_PP8       0xf000110C          /*# リセット（ライト） #*/
//2001.02.20 V2.2DEBUG（end）

//#define       IO_DMY_WRITE        0xf7f00000          /*# DPRAMアクセスタイミング用ダミーアドレス #*/ // 99.10.4
//#define       IO_DMY_WRITE        0xc9000000          /*# DPRAMアクセスタイミング用ダミーアドレス #*/ // PRITヘルスNG対策 99.10.4
//#define       IO_DMY_WRITE        0xd0000000          /*# DPRAMアクセスタイミング用ダミーアドレス #*/ //2000.09 V2.1変更
#define     IO_DMY_WRITE        0xd00000a4          /*# DPRAMアクセスタイミング用ダミーアドレス #*/ //2004.04 IP-CS

/*#########################################*/
/*#           その他（マスタＬＵ）        #*/
/*#########################################*/
#define     IO_ALM_INF          0xf0001800          /*# 障害情報レジスタ #*/
#define     IO_CLK_SEL          0xf0001800          /*# ＣＬＫ切替レジスタ #*/
#define     IO_LUNUM01          0xf0002000          /*# ＬＵ番号（１位）レジスタ #*/
#define     IO_LUNUM10          0xf0002800          /*# ＬＵ番号（１０位）レジスタ #*/
#define     IO_SW_INF           0xf0003000          /*# ＳＷ情報レジスタ #*/
#define     IO_BZ_CTRL          0xf0003000          /*# ブザーコントロールレジスタ #*/
#define     IO_RST_SLV          0xf0003800          /*# リセットレジスタ out #*/            // 99.10.15
#define     IO_MODE             0xf0003800          /*# モードレジスタ in #*/               // 2000.06.14 V1.2 追加
#define     IO_WRT_PRO          0xf0004000          /*# ライトプロテクトレジスタ out #*/    // 99.10.15
#define     IO_VLAN_ID          0xf0004000          /*# ＶＬＡＮ－ＩＤレジスタ in #*/       // 2000.06.14 V1.2 追加
#define     IO_HARD_VER         0xf0004800          /*# ハードウェアバージョンレジスタ #*/  // 2000.05.11 V1.2 追加

//よくわからないアドレス
#define     IO_SIOSEL           0xf0005800          /*# ＳＩＯセレクタレジスタ #*/          // 2000.09 V2.1追加
//圧縮LU対応。I/Oマップ変更。以下削除。2003.08.25
//#define       IO_FROM_INI         0xf0006000          /*# FROMレジスタ #*/                    // 2000.09 V2.1追加


#define     IO_LEDSEL       0xd000009c          /*# LED表示切替レジスタ #*/ //IP-CS対応追加 2004.04
#define     IO_RSTHLD       0xd00000a0          /*# リセット保持レジスタ #*/    //IP-CS対応追加 2004.04

//ビットアサイン
#define     LU_LED_SEL          0x0001          /*# LU_LED表示(1)/CS_LED表示(0) #*/ //2004.04 IP-CS追加
#define     RST_HLD         0x0001              /*# 保守モード起動(1)/通常起動(0)(初期値) #*///2004.04 IP-CS追加
#define     IO_INITSW       0x0008          /*# 保守SW ON(1)/OFF(0)(通常モード) #*/     //2004.04 IP-CS追加



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



/*#########################################*/
/*#           ファイル関連                #*/
/*#########################################*/
#define     BOOT_DATA_SIZE      1024                /*# ＩＰＬブートデータ領域サイズ #*/
#define     LU_F_TYP_OFST       0x00000100          /*# ファイル識別情報オフセット #*/
#define     LU_F_END_OFST       0x00000110          /*# ファイル終了情報オフセット #*/
#define     LU_F_EXE_OFST       0x00000118          /*# ファイル実行番地オフセット（変更不可） #*/
#define     LU_FILE_SIZE        0x00100000          /*# 最大ダウンロードＬＵファイルサイズ（１Ｍまで） #*/
#define     LU_OAK_SIZE         0x00008000          /*# ＯＡＫファイルサイズ #*/                    //2000.08 V2.1追加
//#define       LU_V3OAK_SIZE       0x00080000          /*# 外付けＯＡＫファイルサイズ #*/              //2003.08追加
#define     LU_V3OAK_SIZE       0x00078000          /*# 外付けＯＡＫファイルサイズ #*/              //2004.05 IP-CS 変更
                                                    /*# ダウンロードＬＵファイルサイズ（LU+OAK） #*///2000.07 V2.1復活  //2000.09 V2.1変更
#define     LU_DL_SIZE          (0x00060000+0x00020000+LU_OAK_SIZE+LU_V3OAK_SIZE)//2004.05 IP-CS 変更

//圧縮LU対応。リンクファイル変更にあわせて保留音の先頭アドレス変更。2003.08.25
//#define       LU_CLR_LIMIT        0x00330000          /*# ０クリア対象メモリリミット（リンクファイルと一致させる！） #*/  // 2001.08.07
#define     LU_CLR_LIMIT        0x00830000          /*# ０クリア対象メモリリミット（リンクファイルと一致させる！） #*/  // 2001.08.07

#define     LU_V2_CPUSTACK_SIZE 0x00002000          /*# ＣＰＵ使用スタックサイズ #*/                //2001.02.19 V2.2DEBUG

#ifdef      MAKE_IPL

#ifdef      MAKE_IPL_ROM
#define     LU_FILE_TOP         IO_DRAM_BASE        /*# ファイルダウンロード先頭アドレス #*/
#else
#define     LU_FILE_TOP         0xc0350000          /*# ＩＣＥデバッグ時アドレス #*/
#endif      // MAKE_IPL

#else       // MAKE_IPL_ROM
#define     LU_FILE_TOP         IO_RE_DRAM_BASE     /*# ファイル先頭アドレス #*/
#endif      // MAKE_IPL_ROM

#define     LU_FILE_TYPE        (LU_FILE_TOP + LU_F_TYP_OFST)   /*# ファイル識別情報アドレス #*/
#define     LU_FILE_END         (LU_FILE_TOP + LU_F_END_OFST)   /*# ファイル終了情報アドレス #*/
#define     LU_FILE_EXE         (LU_FILE_TOP + LU_F_EXE_OFST)   /*# ファイル実行番地情報アドレス（変更不可） #*/

#define LU_VER_ADDR         0x180                               /*# ＩＰＬバージョン格納領域アドレス #*/    //2000.07 V2.1追加
#define LU_DATE_ADDR        0x190                               /*# 日付格納領域アドレス #*/                //2001.01.29 V2.2追加
#define IPL_VER_ADDR        (LU_VER_ADDR+IO_IPLROM_ADR)         /*# ＩＰＬバージョン格納実アドレス #*/      //2000.07 V2.1追加
#define ONL_VER_ADDR        (LU_VER_ADDR+IO_DRAM_BASE)          /*# オンラインバージョン格納実アドレス #*/  //2000.07 V2.1追加

// 内部ＲＡＭ
#ifdef  WDT_STOP
//#define       WDT_DMY_ADR         0x10001fa0          // ウォッチドッグダミーアクセス（デバッグ用）
//#define       WDT_DMY_ADR         0x10001f60          // ウォッチドッグダミーアクセス（デバッグ用）   //2000.09 V2.1変更
//#define       WDT_DMY_ADR_V2      0x00101f60          // ウォッチドッグダミーアクセス（デバッグ用）   //2000.09 V2.1追加
#define     WDT_DMY_ADR         0xd0000000          // ウォッチドッグダミーアクセス（デバッグ用）   //2000.09 V2.1変更
#define     WDT_DMY_ADR_V2      0xd0000000          // ウォッチドッグダミーアクセス（デバッグ用）   //2000.09 V2.1追加
#endif
//#define       LU_EXEC_DATA        0x10001fb0          /*# ＬＵ起動情報アドレス（64bytes） #*/
#define     LU_EXEC_DATA        0x10001f70          /*# ＬＵ起動情報アドレス（128bytes） #*/        //2000.09 V2.1変更
#define     LU_EXEC_DATA_V2     0x00101f70          /*# ＬＵ起動情報アドレス（128bytes） #*/        //2000.09 V2.1追加
// 工場試験用
#define     TP_DRAM_CHK         0x10001ff0          /*# 工場試験DRAMチェック結果アドレス #*/        //2000.09 V2.1変更
#define     TP_DRAM_CHK_V2      0x00101ff0          /*# 工場試験DRAMチェック結果アドレス #*/        //2000.09 V2.1追加


/*        Copyright (C) 1999 Oki Electric Industry Co.,Ltd.        */

