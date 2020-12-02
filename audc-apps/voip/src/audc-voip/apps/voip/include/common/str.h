/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : LU                                                       #*/
/*#                                                                         #*/
/*#     日本語名 : LU共通ストラクチャー定義ファイル                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/28 - 毛塚  照明                                    #*/
//*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
#if 0 /* IPCS V4 De */
#include "../../mik/mik_armc.h"
#include "str_boot.h"
#include "str_comdt.h"
#include "str_start.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/syslog.h>
#include <mqueue.h>

/*################################################*/
/*#         音声ＲＴＰ用ストラクチャ             #*/
/*################################################*/
typedef struct T_RTPDAT{
    BYTE    head;                                   /*# 0x00:V(2)/P(1)/X(1)/CC(4) #*/
    BYTE    mpt;                                    /*# 0x01:Marker(1)/PT(7) #*/
    WORD    seq;                                    /*# 0x02:sequence number #*/
    DWORD   ts;                                     /*# 0x04:time stamp #*/
    DWORD   ssrc;                                   /*# 0x08:SSRC (IP(low3byte)+ch) #*/
    //圧縮LU対応。音声エリア変更。2003.09.13
//  WORD    vce_data[VO_RTP_FRM_MAX][VO_1FRM_WORD]; /*# 0x0c:data area #*/
    WORD    vce_data[760];                          /*# 0x0c:data area #*/
}RTPDAT;

/*################################################*/
/*#         メッセージ用ストラクチャ             #*/
/*################################################*/

typedef struct  T_MSG_HEADER{           /*# 内部メッセージヘッダ #*/
    DWORD       sys;                    /*# システムデータ（ＯＳ用） #*/
    BYTE        id;                     /*# 送信元タスクＩＤ #*/
    BYTE        div;                    /*# メッセージ区分 #*/
    BYTE        kind;                   /*# メッセージ種別 #*/
    BYTE        no;                     /*# ＰＫＧ／ＣＮ＿ＮＯ #*/
    BYTE        *link;                  /*# ２次リンクアドレス #*/
}MSG_HEADER;

/*########  ＬＵオーダヘッダ  ##############################*/

typedef struct  T_LU_HEADER{        /*# インタフェーサ・LUヘッダ #*/
    DWORD       size;               /*# メッセージサイズ #*/
    WORD        seq_no;             /*# シーケンス番号 #*/
    WORD        massage_id;         /*# メッセージ種別 #*/
    BYTE        lu_no;              /*# ＬＵ番号 #*/
    BYTE        slv_no;             /*# スレーブ番号 #*/
    BYTE        type;               /*# 送信元／先種別 #*/
    BYTE        pkg_no;             /*# ＰＫＧ番号 #*/
}LU_HEADER;
                                    // LU-99.8.17
typedef struct  T_LU_EX_OE{         /*# 拡張インタフェーサ・LUヘッダ #*/
    WORD        size;
    BYTE        type;               /*# 送信元／先種別 #*/
    BYTE        pkg_no;             /*# ＰＫＧ番号 #*/
    BYTE        data;
}LU_EX_OE;
                                    // LU-99.8.17
typedef struct  T_LU_EX_HEADER{     /*# 拡張インタフェーサ・LUヘッダ #*/
    DWORD       size;               /*# メッセージサイズ #*/
    WORD        seq_no;             /*# シーケンス番号 #*/
    WORD        massage_id;         /*# メッセージ種別 #*/
    BYTE        lu_no;              /*# ＬＵ番号 #*/
    BYTE        slv_no;             /*# スレーブ番号 #*/
    WORD        oe_num;             /*# オーダイベント数 #*/
    BYTE        oe_data[1012];
}LU_EX_HEADER;

typedef struct  T_CCU_HEADER{           /*# インタフェーサヘッダ #*/
    DWORD       size;               /*# メッセージサイズ #*/
    WORD        seq_no;             /*# シーケンス番号 #*/
    WORD        massage_id;         /*# メッセージ種別 #*/
    LU_HEADER   lu_header;          /*#  #*/
}CCU_HEADER;

typedef struct  T_LU_ODEV{      /*# LUオーダ共通フォーマット #*/
    LU_HEADER   lu_header;          /*#  #*/
    BYTE        info[256];          /*# オーダ／イベント情報 #*/
}LU_ODEV;

/*########  ユーザデータ  ##############################*/

typedef struct  T_VOICE_CTL1{       /*# 音声ストップレスポンス用 #*/
    WORD        port;                   /*# ポート番号 #*/
}VOICE_CTL1;

typedef struct  T_VOICE_CTL2{       /*# 音声ストップ用 #*/
    WORD        direct;                 /*# パス方向 #*/
    WORD        port;                   /*# ポート番号 #*/
    BYTE        ip_mult;                /*# マルチキャスト指定 #*/
    BYTE        dmy;                    /*#  #*/
}VOICE_CTL2;

typedef struct  T_VOICE_CTL3{       /*# 音声切断通知用 #*/
    WORD        result;                 /*# 切断理由 #*/
    WORD        port;                   /*# ポート番号 #*/
}VOICE_CTL3;

typedef struct T_CLIENT_INFO{       /*# クライアントアクセス情報 #*///2000.07 V2.1追加
    DWORD       len;                /*# 受信データサイズ #*/
    DWORD       sd;                 /*# データ通信ソケットディスクリプタ #*/
    DWORD       ip;                 /*# 相手ＩＰアドレス #*/
    WORD        port;               /*# 相手ＰＯＲＴ番号 #*/
    WORD        reserve;            /*# ＲＥＳＥＲＶＥ #*/
}CLIENT_INFO;

typedef struct  T_LUEVT_LINK{       /*# ＤＯＥＰ使用 #*/
    BYTE        dmy[12];                /*#  #*/
    DWORD       *link;                  /*# キューリンクアドレス #*/
}LUEVT_LINK;

typedef struct  T_DPRAM_WRITE{      /*# LUオーダ共通フォーマット #*/
    BYTE        wt_cnt;             /*# 書き込みバイト数 #*/
    BYTE        wt_kind;            /*# 書き込み種別 #*/
    WORD        *wt_adr;            /*# 書き込みアドレス #*/
    BYTE        info[256];          /*# 書き込みデータ #*/
}DPRAM_WRITE;

typedef struct  T_PKG_OREV{     /*# 既存オーダ/イベント #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダ／イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        user[252];          /*# オーダ／イベント情報 #*/
}PKG_OREV;




/* FAX管理タスクMSGパラメータ   */
typedef union T_FAXMNG_PARA1    /* FAX管理タスクmsgパラメータ1 */
{
    ULONG   tone    ;   /* トーン   */
    ULONG   kind    ;   /* kind     */
} FAXMNG_PARA1 ;

typedef union T_FAXMNG_PARA2    /* FAX管理タスクmsgパラメータ2 */
{
    ULONG   dsp_no  ;   /* DSP番号  */
    ULONG   status  ;   /*  */
} FAXMNG_PARA2 ;

typedef union T_FAXMNG_PARA3    /* FAX管理タスクmsgパラメータ3 */
{
    ULONG   error   ;   /*  */
} FAXMNG_PARA3 ;

typedef struct T_FAXMNG_MSGPARA
{
    FAXMNG_PARA1    para1   ;   /* 第１パラメータ */
    FAXMNG_PARA2    para2   ;   /* 第２パラメータ */
    FAXMNG_PARA3    para3   ;   /* 第３パラメータ */
} FAXMNG_MSGPARA ;

/* T30タスクMSGパラメータ   */
typedef struct T_T30_MSGPARA
{
    ULONG   kind    ;       /* 種別 */
    short   speed   ;       /* 速度*/
    short   type    ;       /* データタイプ */
    int     size    ;       /* データサイズ */
} T30_MSGPARA ;



typedef union T_T38_PARA1   /* T38タスクmsgパラメータ1 */
{
    ULONG   command ;   /* 要求種別 */
    ULONG   result  ;   /* 結果     */
    ULONG   sd_no   ;   /* SD番号   */
} T38_PARA1 ;

typedef union T_T38_PARA2   /* T38タスクmsgパラメータ2 */
{
    ULONG   size    ;   /* サイズ　*/
    ULONG   error   ;   /* エラー情報 */
} T38_PARA2 ;

typedef union T_T38_PARA3   /* T38タスクmsgパラメータ3 */
{
    ULONG   type    ;   /* データ種別 */
} T38_PARA3 ;

/* 2003.10.23 パラメータ4,5追加 */
typedef union T_T38_PARA4   /* T38タスクmsgパラメータ4 */
{
    DWORD   ip  ;   /* IPアドレス */
} T38_PARA4 ;

typedef union T_T38_PARA5   /* T38タスクmsgパラメータ5 */
{
    WORD    port    ;   /* ポート */
} T38_PARA5 ;

/* T38タスクMSG */
typedef struct T_T38_MSGPARA{
    T38_PARA1   para1;
    T38_PARA2   para2;
    T38_PARA3   para3;
    T38_PARA4   para4;  /* 2003.10.23 追加 */
    T38_PARA5   para5;  /* 2003.10.23 追加 */
}T38_MSGPARA;


/* FAXモデムタスクMSGパラメータ */
typedef struct T_MDM_MSGPARA
{
    ULONG   command ;       /* 要求種別 */
    int speed   ;       /* 速度     */
    int     option  ;       /* オプション指定 */
} MDM_MSGPARA ;

/* アクセプトタスクMSGパラメータ    */
typedef struct T_ACC_MSGPARA
{
    int sd_no   ;       /* SD番号 */
    void*   peer    ;       /* 相手情報格納アドレス   */
    int     size    ;       /* サイズ */
} ACC_MSGPARA ;

/* コネクトタスクMSGパラメータ  */

typedef union T_CON_PARA1   /* conタスクmsgパラメータ1 */
{
    ULONG   command ;   /* 要求種別 */
    int sd_no   ;   /* SD番号 */
} CON_PARA1 ;

typedef union T_CON_PARA2   /* conタスクmsgパラメータ2 */
{
    ULONG   size    ;   /* サイズ　*/
} CON_PARA2 ;

typedef union T_CON_PARA3   /* conタスクmsgパラメータ3 */
{
    ULONG   type    ;   /* データ種別 */
    void*   peer    ;   /* 相手情報格納アドレス   */
} CON_PARA3 ;


typedef struct T_CON_MSGPARA
{
    CON_PARA1   para1;
    CON_PARA2   para2;
    CON_PARA3   para3;
} CON_MSGPARA ;

//圧縮LU対応。2003.09.13
typedef struct  T_DSP_PASSCHG{
    short   channel;
    short   result;
}DSP_PASSCHG;

typedef struct  T_DSP_ERROR_INFO{
    short   dsp_id;
    short   error;
}DSP_ERROR_INFO;

typedef struct  T_HPI_REPLY_INFO{
    short   dsp_id;
    short   mod;
}HPI_REPLY_INFO;


typedef struct  T_INNER_MSG{        /*# 内部メッセージフォーマット #*/
    MSG_HEADER  msg_header;
    union   {
/*          BYTE    user[16];   # ＵＳＥＲ領域 #*/
            BYTE    user[32];   /*# ＵＳＥＲ領域 #*//* 2003.10.23 tamu kari */
            VOICE_CTL1  voice_ctl1;
            VOICE_CTL2  voice_ctl2;
            VOICE_CTL3  voice_ctl3;
            LUEVT_LINK  luevt_link;
            CLIENT_INFO client_info;    /*# クライアントアクセス情報 #*///2000.07 V2.1追加
//          RTPDAT      *link[LU_CHANNEL_MAX];  /*# 音声送信要求 #*/
            RTPDAT      *link[CNMAX];   /*# 音声送信要求 #*/
            FAXMNG_MSGPARA  faxmng_para;    /* FAX管理タスクmsgパラメータ */
            T30_MSGPARA t30_para;   /* T30タスクmsgパラメータ */
            T38_MSGPARA t38_para;   /* T38タスクmsgパラメータ */
            MDM_MSGPARA mdm_para;   /* FAXモデムタスクmsgパラメータ */
            CON_MSGPARA con_para;   /* コネクトタスクmsgパラメータ */
            ACC_MSGPARA acc_para;   /* アクセプトタスクmsgパラメータ */
            //圧縮LU対応。2003.09.13
            DSP_PASSCHG pass_chg;   /* パス切替通知msgパラメータ */
            DSP_ERROR_INFO  errorinfo;
            HPI_REPLY_INFO  reply_info;
        }user_data;
}INNER_MSG;

/*########  ＬＵオーダ  ##############################*/

typedef struct  T_LUORD_DOWNLOAD{       /*# ダウンロード #*/
    LU_HEADER   lu_header;          /*#  #*/
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        crint;              /*# リンガ周期 #*/
    BYTE        law;                /*# ｌａｗ指定 #*/
    BYTE        iptype;             /*# CLKD/CRINT用IPタイプ #*/
    BYTE        multadd[16];            /*# CLKD/CRINT用マルチアドレス #*/
    WORD        port1;              /*# CLKD用ポート番号 #*/
    WORD        port2;              /*# CRINT用ポート番号 #*/
    BYTE        mult_ttl;           /*# 同期パケットTTL値 #*/
    BYTE        clkaddr[16];        /*# 同期パケット送信元アドレス #*/
    BYTE        yuragi;             /*# 揺らぎ設定値 #*/
    BYTE        buffer;             /*# バッファ面数指定 #*/
    BYTE        clkdsync;           /*# 同期方式指定 #*/    // 2000.05.11  V1.2 追加
    WORD        u_port1;            /*# ユニキャスト同期CLKD用ポート番号 #*/    // 2000.07  V2.1 追加
    WORD        u_port2;            /*# ユニキャスト同期CRINT用ポート番号 #*/   // 2000.07  V2.1 追加
    BYTE        clkmst;             /*# クロックマスタ／スレーブ指定 #*/        // 2000.07  V2.1 追加
    BYTE        buf1_st[2];         /*# バッファ面パターン番号１開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf1_max[2];        /*# バッファ面パターン番号１最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf2_st[2];         /*# バッファ面パターン番号２開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf2_max[2];        /*# バッファ面パターン番号２最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf3_st[2];         /*# バッファ面パターン番号３開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf3_max[2];        /*# バッファ面パターン番号３最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf4_st[2];         /*# バッファ面パターン番号４開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf4_max[2];        /*# バッファ面パターン番号４最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf5_st[2];         /*# バッファ面パターン番号５開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf5_max[2];        /*# バッファ面パターン番号５最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf6_st[2];         /*# バッファ面パターン番号６開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf6_max[2];        /*# バッファ面パターン番号６最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf7_st[2];         /*# バッファ面パターン番号７開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf7_max[2];        /*# バッファ面パターン番号７最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf8_st[2];         /*# バッファ面パターン番号８開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf8_max[2];        /*# バッファ面パターン番号８最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf9_st[2];         /*# バッファ面パターン番号９開始時間 #*/    //2000.09 V2.1追加
    BYTE        buf9_max[2];        /*# バッファ面パターン番号９最大時間 #*/    //2000.09 V2.1追加
    BYTE        buf10_st[2];        /*# バッファ面パターン番号１０開始時間 #*/  //2000.09 V2.1追加
    BYTE        buf10_max[2];       /*# バッファ面パターン番号１０最大時間 #*/  //2000.09 V2.1追加
    BYTE        tos_a;              /*# TOS A #*/                               //2000.12.27 V2.2追加
    BYTE        tos_b;              /*# TOS B #*/                               //2000.12.27 V2.2追加
    BYTE        tos_c;              /*# TOS C #*/                               //2000.12.27 V2.2追加
    BYTE        tos_d;              /*# TOS D #*/                               //2000.12.27 V2.2追加
    BYTE        hel_tim[2];         /*# ヘルスチェックタイマカウント #*/        //2000.12.27 V2.2追加
    BYTE        tftp;               /*# 保留音／トーキーダウンロード許可 #*/    //2001.02.28 V2.2追加
    WORD        ccuver;             /*# ＣＣＵバージョン #*/
    BYTE        ext_hld;            /*# EXT_HLD：LU-CTL外部保留音指定（CTIOX専用）※RESERVE #*/
    BYTE        survtim;            /*# D01サバイバル対応 CCU切換タイマ値 #*///2004.02 LU32B追加
    BYTE        surv_rtim;          /*# サバイバル切り戻しタイマ：IP-CS(SS9100_PH1再開) 追加 #*/
    BYTE        cwit_tim;           /*# コネクション再接続タイマ：IP-CS(SS9100_PH1再開) 追加 #*/
}LUORD_DOWNLOAD;

typedef struct  T_LUORD_DPRAMID{        /*# DPRAM種別ダウンロード #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        flg;                /*# 継続フラグ #*/
    BYTE        pkgcode[252];   /*# PKG_DPRAM情報 #*/
}LUORD_DPRAMID;

typedef struct  T_LUORD_HELPKGILG{  /*# ヘルスチェックイリーガルダウンロード #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgcode[253];   /*# PKG_DPRAM情報 #*/
}LUORD_HELPKGILG;

typedef struct  T_LUORD_HORYDOWN{       /*# 保留音ダウンロード #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        data[256];          /*# 保留音ファイルデータ #*/
}LUORD_HORYDOWN;

typedef struct  T_LUORD_PKGINITREQ{     /*# 個別PKG初期設定要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        rsttim;             /*# リセットタイミング #*/
}LUORD_PKGINITREQ;

typedef struct  T_LUORD_LUHELREQ{       /*# LUヘルスチェック要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
}LUORD_LUHELREQ;

typedef struct  T_LUORD_PKGHELREQ{      /*# PKGヘルスチェック要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        status;             /*# リード／ライト指定 #*/
}LUORD_PKGHELREQ;

typedef struct  T_LUORD_PASSCTLREQ{     /*# パス制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        pass;               /*# パス方向指定 #*/
    BYTE        myport[2];              /*# 自ポート番号 #*/
    BYTE        iptype;             /*# IPアドレスタイプ #*/
    BYTE        ipadd[16];          /*# IPアドレス #*/
    BYTE        port[2];                /*# 相手ポート番号 #*/
    BYTE        ipmult;             /*# マルチキャスト指定 #*/
    BYTE        multadd[16];            /*# マルチアドレス #*/
    BYTE        mult_ttl;           /*# マルチキャストＴＴＬ値 #*/
    BYTE        pad[2];             /*# ＰＡＤ値 #*/
    BYTE        codec;              /*# CODEC種別 #*/
    BYTE        echo;               /*# エコーキャンセラ指定 #*/
    BYTE        thru;               /*# スルーモード指定 #*/
    BYTE        send_period;        /*# パケット送信周期 #*/
    BYTE        rtcp;               /*# RTCP制御 #*/
}LUORD_PASSCTLREQ;

typedef struct  T_LUORD_TONECTLREQ{         /*# トーン制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        pad[2];             /*# 送出ＰＡＤ値 #*/
    BYTE        ptn;                /*# 送出パターン #*/
    BYTE        on1[2];             /*# ＯＮ１時間 #*/
    BYTE        off1[2];                /*# ＯＦＦ１時間 #*/
    BYTE        on2[2];             /*# ＯＮ２時間 #*/
    BYTE        off2[2];                /*# ＯＦＦ２時間 #*/
    BYTE        on3[2];             /*# ＯＮ３時間 #*/
    BYTE        off3[2];                /*# ＯＦＦ３時間 #*/
    BYTE        toneid;             /*# トーン＿ＩＤ #*/
    BYTE        f1[2];              /*# Ｆ１周波数 #*/
    BYTE        f2[2];              /*# Ｆ２周波数 #*/
    BYTE        fam[2];             /*# ＦＡＭ周波数 #*/
    BYTE        fk[2];              /*# ＦＫ周波数 #*/
}LUORD_TONECTLREQ;

typedef struct  T_LUORD_HORYCTLREQ{         /*# 保留音制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        pad[2];             /*# 送出ＰＡＤ値 #*/
    BYTE        ptn;                /*# 保留音パターン #*/
}LUORD_HORYCTLREQ;

typedef struct  T_LUORD_PBDETSTART{         /*# PB監視開始 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ontim;              /*# ＯＮ監視時間 #*/
    BYTE        offtim;             /*# ＯＦＦ監視時間 #*/
}LUORD_PBDETSTART;

typedef struct  T_LUORD_PBDETSTOP{          /*# PB監視停止 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUORD_PBDETSTOP;

typedef struct  T_LUORD_BTDETSTART{         /*# BT監視開始 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ontim;              /*# ＯＮ監視時間 #*/
    BYTE        offtim;             /*# ＯＦＦ監視時間 #*/
    BYTE        puls;               /*# 周期回数 #*/
    BYTE        toneid;             /*# トーン＿ＩＤ #*/
}LUORD_BTDETSTART;

typedef struct  T_LUORD_DTDETSTART{         /*# DT監視開始 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ontim;              /*# ＯＮ監視時間 #*/
    BYTE        offtim;             /*# ＯＦＦ監視時間 #*/
    BYTE        puls;               /*# 周期回数 #*/
    BYTE        toneid;             /*# トーン＿ＩＤ #*/
}LUORD_DTDETSTART;

typedef struct  T_LUORD_BTDETSTOP{          /*# BT監視停止 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUORD_BTDETSTOP;

typedef struct  T_LUORD_DTDETSTOP{          /*# DT監視停止 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUORD_DTDETSTOP;

typedef struct  T_LUORD_PBSNDSTART{         /*# PB送出開始 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        pad_l[2];           /*# 送出ＰＡＤ値LOW #*/ /*# DAVIS-LU-CT 99-6-10 #*/
    BYTE        pad_h[2];           /*# 送出ＰＡＤ値HI #*/  /*# DAVIS-LU-CT 99-6-10 #*/
    BYTE        sndtim;             /*# メーク時間 #*/
    BYTE        sndmin;             /*# ブレイク時間 #*/
    BYTE        num;                /*# データ数 #*/
    BYTE        digit[48];          /*# 数字／ポーズデータ #*/
}LUORD_PBSNDSTART;

typedef struct  T_LUORD_PBSNDSTOP{          /*# PB送出停止 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUORD_PBSNDSTOP;

typedef struct  T_LUORD_DOORCOM{            /*# ドアホン共通 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    WORD        data;               /*# データ #*/
}LUORD_DOORCOM;

typedef struct  T_LUORD_GHORYCTL{           /*# 外部保留音制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    WORD        data;               /*# データ #*/
}LUORD_GHORYCTL;

typedef struct  T_LUORD_BROADCTL{           /*# 放送装置制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    WORD        data;               /*# データ #*/
}LUORD_BROADCTL;

typedef struct  T_LUORD_MUONPTNSET{     /*# 無音パターン指定 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        law;                /*# 無音パターン指定 #*/
}LUORD_MUONPTNSET;

typedef struct  T_LUORD_DOORHSCNREQ{        /*# ドアホンスキャン要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        ctl;                /*# 制御種別 #*/
}LUORD_DOORHSCNREQ;

typedef struct  T_LUORD_BUFFVALUE{          /*# バッファ面指定要求 #*///2000.07 V2.1追加
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cn;                 /*# 制御対象ＣＨ番号 #*/
    BYTE        buffer;             /*# バッファ面数指定（パターン番号） #*/
}LUORD_BUFFVALUE;

typedef struct  T_LUORD_SYNCSNDCTL{         /*# 同期/CRINTパケット送信制御 #*///2000.07 V2.1追加
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        iptype;             /*# IPタイプ #*/
    BYTE        ip[16];             /*# クロックマスタＩＰアドレス #*/
}LUORD_SYNCSNDCTL;

typedef struct  T_LUORD_SYNCRCVCTL{         /*# 同期/CRINTパケット受信制御 #*///2000.07 V2.1追加
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        iptype;             /*# IPタイプ #*/
    BYTE        ip[16];             /*# クロックマスタＩＰアドレス #*/
}LUORD_SYNCRCVCTL;

typedef struct  T_LUORD_DSPCHCTL{           /*# DSPch制御 #*///2000.08 V2.1追加
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        ch_num;             /*# DSP制御ch数 #*/
}LUORD_DSPCHCTL;

                                                            /*# DAVIS-LU-CT 99-5-24 メモリダンプ処理追加 #*/
typedef struct      T_LUORD_MEMDMPREQ{                      /*# メモリダンプ要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;                                         /*# Ｂ．Ｃ #*/
    BYTE        code;                                       /*# オーダコード #*/
    BYTE        dmy1;                                       /*#  #*/
    BYTE        addr[4];                                    /*# アドレス #*/
    BYTE        size;                                       /*# サイズ #*/
    BYTE        id;                                         /*# 読み込みBit幅 32bit=0,16bit,8bit=2 #*/
}LUORD_MEMDMPREQ;

                                                            /*# 工場試験用 #*/
typedef struct      T_LUORD_MEMWTREQ{                       /*# メモリライト要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;                                         /*# Ｂ．Ｃ #*/
    BYTE        code;                                       /*# オーダコード #*/
    BYTE        dmy1;                                       /*#  #*/
    BYTE        addr[4];                                    /*# アドレス #*/
    BYTE        size;                                       /*# サイズ #*/
    BYTE        id;                                         /*# 読み込みBit幅 32bit=0,16bit,8bit=2 #*/
    BYTE        data[128];                                  /*# ダンプデータ #*/
}LUORD_MEMWTREQ;

/* 圧縮LU */
typedef struct  T_LUORD_FAXPASSCTLREQ{      /*# T38パス制御 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        ctl;                /*# 制御種別 #*/
    BYTE        pass;               /*# パス方向指定 #*/
    BYTE        codec;              /*# コーデック種別 #*/
    BYTE        myport[2];          /*# 自ポート番号 #*/
    BYTE        ipadd[4];           /*# IPアドレス #*/
    BYTE        port[2];            /*# 相手ポート番号 #*/
    BYTE        capmax[2];          /*# 相手最大受信能力 #*/
    BYTE        udptl[2];           /*# 相手最大受信パケット数 #*/
    BYTE        pad[2];             /*# FAX受信パス側PAD値 #*//* 2004.01.22 */
}LUORD_FAXPASSCTLREQ;

typedef struct  T_LUORD_T38LISTENREQ{       /*# T38Listen要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        myport[2];          /*# 自ポート番号 #*/
}LUORD_T38LISTENREQ;

typedef struct  T_LUORD_FAXDOWNLOAD{        /*# FAX情報ダウンロード #*/
    LU_HEADER   lu_header;          /*#  #*/
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        udp_start[2];           /*# UDP先頭ポート番号 #*/
    BYTE        tcp_rstart[2];          /*# TCP受信先頭ポート番号 #*/
    BYTE        tcp_sstart[2];          /*# TCP送信先頭ポート番号 #*/
    BYTE        fax_spd;            /*# 最大画像転送速度 #*/
    BYTE        fax_att;            /*# FAXモデム減衰レベル #*/
    BYTE        fax_detectstop[2];      /*# FAX信号監視時間 #*/
    BYTE        fax_cngstptim;          /*# CNG送出停止タイマ #*/
    BYTE        fax_conntime;           /*# T38FAXコネクトタイマ #*/
    BYTE        fax_holdtime;           /*# パケット保持時間 #*/
    BYTE        fax_nonecmdly;          /*# Non-ecmのDelay時間 #*/
    BYTE        fax_uloadlen;           /*# FAXモデムUpload長 #*/
    BYTE        fax_dloadlen;           /*# FAXモデムDwnload長 #*/
    BYTE        fax_nsftrs;         /*# NSF転送可否 #*/
    BYTE        fax_csitrs;         /*# CSI転送可否 #*/
    BYTE        fax_tsitrs;         /*# TSI転送可否 #*/
    BYTE        fax_tcflen[2];          /*# TCF生成時間 #*/
    BYTE        fax_ecm;            /*# ECM通信可否 #*/
    BYTE        fax_tcfctl;         /*# TCF制御方式 #*/
    BYTE        fax_tcfcheck;           /*# TCFエラー閾値 #*/
    BYTE        fax_dettime[2];         /*# FAX検出時間 #*/
    BYTE        fax_predettime[2];      /*# プリアンブル検出時間 #*/
    BYTE        fax_redun;          /*# FAXリダンダント数 #*/
    BYTE        fax_bitorder;           /*# Bit送出方向 #*/
    BYTE        fax_ifpmaxlen[2];       /*# IFPパケット最大長 #*/
//圧縮LU対応。オーダ変更により追加。2003.09.08
    BYTE        buf1_max[2];        /*# バッファ面パターン番号１最大受信バッファサイズ #*/
    BYTE        buf1_sta[2];        /*# バッファ面パターン番号１遅延回復処理開始バッファサイズ #*/
    BYTE        buf1_stp[2];        /*# バッファ面パターン番号１遅延回復処理停止バッファサイズ #*/

    BYTE        buf2_max[2];        /*# バッファ面パターン番号２最大受信バッファサイズ #*/
    BYTE        buf2_sta[2];        /*# バッファ面パターン番号２遅延回復処理開始バッファサイズ #*/
    BYTE        buf2_stp[2];        /*# バッファ面パターン番号２遅延回復処理停止バッファサイズ #*/

    BYTE        buf3_max[2];        /*# バッファ面パターン番号３最大受信バッファサイズ #*/
    BYTE        buf3_sta[2];        /*# バッファ面パターン番号３遅延回復処理開始バッファサイズ #*/
    BYTE        buf3_stp[2];        /*# バッファ面パターン番号３遅延回復処理停止バッファサイズ #*/

    BYTE        buf4_max[2];        /*# バッファ面パターン番号４最大受信バッファサイズ #*/
    BYTE        buf4_sta[2];        /*# バッファ面パターン番号４遅延回復処理開始バッファサイズ #*/
    BYTE        buf4_stp[2];        /*# バッファ面パターン番号４遅延回復処理停止バッファサイズ #*/

    BYTE        buf5_max[2];        /*# バッファ面パターン番号５最大受信バッファサイズ #*/
    BYTE        buf5_sta[2];        /*# バッファ面パターン番号５遅延回復処理開始バッファサイズ #*/
    BYTE        buf5_stp[2];        /*# バッファ面パターン番号５遅延回復処理停止バッファサイズ #*/

    BYTE        buf6_max[2];        /*# バッファ面パターン番号６最大受信バッファサイズ #*/
    BYTE        buf6_sta[2];        /*# バッファ面パターン番号６遅延回復処理開始バッファサイズ #*/
    BYTE        buf6_stp[2];        /*# バッファ面パターン番号６遅延回復処理停止バッファサイズ #*/

    BYTE        buf7_max[2];        /*# バッファ面パターン番号７最大受信バッファサイズ #*/
    BYTE        buf7_sta[2];        /*# バッファ面パターン番号７遅延回復処理開始バッファサイズ #*/
    BYTE        buf7_stp[2];        /*# バッファ面パターン番号７遅延回復処理停止バッファサイズ #*/

    BYTE        buf8_max[2];        /*# バッファ面パターン番号８最大受信バッファサイズ #*/
    BYTE        buf8_sta[2];        /*# バッファ面パターン番号８遅延回復処理開始バッファサイズ #*/
    BYTE        buf8_stp[2];        /*# バッファ面パターン番号８遅延回復処理停止バッファサイズ #*/

    BYTE        buf9_max[2];        /*# バッファ面パターン番号９最大受信バッファサイズ #*/
    BYTE        buf9_sta[2];        /*# バッファ面パターン番号９遅延回復処理開始バッファサイズ #*/
    BYTE        buf9_stp[2];        /*# バッファ面パターン番号９遅延回復処理停止バッファサイズ #*/

    BYTE        buf10_max[2];       /*# バッファ面パターン番号１０最大受信バッファサイズ #*/
    BYTE        buf10_sta[2];       /*# バッファ面パターン番号１０遅延回復処理開始バッファサイズ #*/
    BYTE        buf10_stp[2];       /*# バッファ面パターン番号１０遅延回復処理停止バッファサイズ #*/

}LUORD_FAXDOWNLOAD;


typedef struct T_LUORD_DSP_REQUEST_PARAM{
    LU_HEADER   lu_header;          /*#  #*/
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dsp_id;             
    BYTE        kind;
    BYTE        cnno;
}LUORD_DSP_REQUEST_PARAM;



typedef struct  T_LUORD_AQACT{      /*# 音声品質情報通知起動/停止要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cn;             /*# CN #*/
    BYTE        ctl;                /*# 起動：0　停止：1 #*/
}LUORD_AQACT;//SS9100 2004.10

//SS9100(PH1再開)対応 2006.06  -start-
typedef struct  T_LUORD_RESTARTREQ{     /*再開処理要求*/
    LU_HEADER   lu_header;
    BYTE        bc;
    BYTE        code;
    BYTE        dumy;
    BYTE        ch;
}LUORD_RESTARTREQ;

typedef struct  T_LUORD_PH1ENDREQ{      /*PH1終了要求*/
    LU_HEADER   lu_header;
    BYTE        bc;
    BYTE        code;
    BYTE        dumy;
}LUORD_PH1ENDREQ;
//SS9100(PH1再開)対応 2006.06  -end-


/*########  ＬＵイベント############################*/

typedef struct  T_LUEVT_LUONLINE{       /*# LUオンライン通知 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        master;             /*# マスタ表示 #*/
    BYTE        pkgnum;             /*# 実装PKG数 #*/
    BYTE        chnum;              /*# 実装ch数 #*/
    BYTE        iplver[4];          /*# ＩＰＬバージョン #*/        //2000.07 V2.1追加
    BYTE        olver[4];           /*# オンラインバージョン #*/    //2000.07 V2.1追加
    BYTE        hdver[4];           /*# ハードバージョン #*/        //2000.09 V2.1追加
    BYTE        rmt;                /*# 遠隔 #*/                    //2000.07 V2.1追加
    BYTE        hold_dl;            /*# 保留音ＤＬ要求 #*/          //2001.08 V2.3追加
    BYTE        macaddr[6];         /*# MACアドレス #*/         //2004.08 SS9100追加
}LUEVT_LUONLINE;

typedef struct  T_LUEVT_PKGINITRSP{     /*# 個別PKG初期設定応答 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        result;             /*# 初期設定結果 #*/
    BYTE        pkgcode;            /*# ＰＫＧ種別 #*/
    BYTE        pkgver;             /*# ＰＫＧバージョン番号 #*/
    BYTE        misc;               /*# ＭＩＳＣ＿ＰＫＧ回路実装 #*/
}LUEVT_PKGINITRSP;

typedef struct  T_LUEVT_LUHELRSP{       /*# LUヘルスチェック応答 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
}LUEVT_LUHELRSP;

typedef struct  T_LUEVT_PKGHELRSP{      /*# PKGヘルスチェック応答 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        result;             /*# 読み出し結果 #*/
}LUEVT_PKGHELRSP;

typedef struct  T_LUEVT_PKGSTATE{           /*# PKG状態通知 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
//  BYTE        ins[SLOT_MAX];      /*# スロット情報 #*/
    BYTE        ins[8];             /*# スロット情報 #*/
    BYTE        opt_ins;            /*# オプションボード実装状態 #*/    //2001.01.11 V2.2 追加
}LUEVT_PKGSTATE;

typedef struct  T_LUEVT_PBDET{          /*# PB受信 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        digit;              /*# 受信数字 #*/
}LUEVT_PBDET;

typedef struct  T_LUEVT_BTDET{          /*# BT受信 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUEVT_BTDET;

typedef struct  T_LUEVT_DTDET{          /*# DT受信 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUEVT_DTDET;

typedef struct  T_LUEVT_PBSNDEND{           /*# PB送信完了 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUEVT_PBSNDEND;

typedef struct  T_LUEVT_DOORHSCNRSP{        /*# ドアホンスキャン #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;          /*# PKG番号 #*/
    WORD        scn ;           /*# スキャン情報 #*/
}LUEVT_DOORHSCNRSP;

typedef struct  T_LUEVT_PKGTROUBLE{         /*# PKG障害検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        pkgno;              /*# ＰＫＧ＿ＮＯ #*/
    BYTE        result;             /*# 障害要因 #*/
}LUEVT_PKGTROUBLE;

typedef struct  T_LUEVT_CLKDTROUBLE{            /*# CLKD障害検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        result;             /*# 障害要因 #*/
}LUEVT_CLKDTROUBLE;

typedef struct  T_LUEVT_CLKDRESTORE{        /*# CLKD障害復旧検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        result;             /*# 障害復旧要因 #*/
}LUEVT_CLKDRESTORE;

typedef struct  {                   /*# CLKD同期合わせ完了通知 #*/  // 2000.05.11  V1.2 追加
    LU_HEADER   lu_header;
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        sts;                /*# 同期合わせ状態 #*/
    BYTE        seq_n;              /*# ｎ #*/
}LUEVT_SYNCCOMP;

typedef struct  T_LUEVT_HORYDLEND{  /*# 保留音ダウンロード完了通知 #*/  //2000.07 V2.1追加
    LU_HEADER   lu_header;
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
}LUEVT_HORYDLEND;

typedef struct  {   /*# チャネル解放通知 #*/
    LU_HEADER   lu_header;
    BYTE        bc;                 /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
}LUEVT_REL_DET;

//IP-CS 2004.04追加start
typedef struct  T_LUORD_PGDOWNLOAD{     /*# プログラムダウンロード #*///2004.04 IP-CS 追加
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        ctl;            /*# 0=LU 1=CS #*/
    BYTE        ci[4];          /*#  #*/
    BYTE        data[152];          /*# プログラムファイルデータ 128+1+5+1+1+16=152 #*/
}LUORD_PGDOWNLOAD;

typedef struct  T_LUEVT_PGDLEND{    /*# プログラムダウンロード完了通知 #*///2004.04 IP-CS追加
    LU_HEADER   lu_header;
    BYTE        bc;                 /*# Ｂ．Ｃ ＝5#*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        ctl;                /*# ダウンロード種別 #*/
    BYTE        result;             /*#  TFTP/FLASH書き込み結果 #*/
}LUEVT_PGDLEND;

//IP-CS 2004.04追加end

                                                            /*# DAVIS-LU-CT 99-5-24 メモリダンプ処理追加 #*/
typedef struct  T_LUEVT_MEMDMPRSP{                          /*# メモリダンプ要求 #*/
    LU_HEADER   lu_header;
    BYTE        bc;                                         /*# Ｂ．Ｃ #*/
    BYTE        code;                                       /*# イベントコード #*/
    BYTE        dmy1;                                       /*#  #*/
    BYTE        addr[4];                                    /*# アドレス #*/
    BYTE        size;                                       /*# サイズ #*/
    BYTE        id;                                         /*# 読み込みBit幅 32bit=0,16bit,8bit=2 #*/
    BYTE        data[128];                                  /*# ダンプデータ #*/
    
}LUEVT_MEMDMPRSP;
                                                            /*# 工場試験用 #*/
typedef struct  T_LUEVT_MEMWTRSP{                           /*# メモリライト結果 #*/
    LU_HEADER   lu_header;
    BYTE        bc;                                         /*# Ｂ．Ｃ #*/
    BYTE        code;                                       /*# イベントコード #*/
    BYTE        dmy1;                                       /*#  #*/
    BYTE        addr[4];                                    /*# アドレス #*/
    BYTE        size;                                       /*# サイズ #*/
    BYTE        id;                                         /*# 読み込みBit幅 32bit=0,16bit,8bit=2 #*/
    BYTE        data[128];                                  /*# ダンプデータ #*/
    
}LUEVT_MEMWTRSP;

/* 圧縮LU */
typedef struct  T_LUEVT_FAXERROR{           /*# FAX障害検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        result[4];              /*# 障害種別 #*/
}LUEVT_FAXERROR;

typedef struct  T_LUEVT_FAXPASSCHG{         /*# 呼種別切り替え #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        kind;               /*# 切り替え種別 #*/
}LUEVT_FAXPASSCHG;

//圧縮LU対応。DSP障害通知/復旧通知追加。2003.09.08

typedef struct  T_LUEVT_DSPERROR{           /*# DSP障害検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        cnno;               /*# ＣＮ番号 #*/
    BYTE        result[4];              /*# 障害種別 #*/
}LUEVT_DSPERROR;

typedef struct  T_LUEVT_DSPREPAIR{          /*# DSP障害検出 #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dmy1;               /*#  #*/
    BYTE        dspno;              /*# DSP番号 #*/
}LUEVT_DSPREPAIR;

typedef struct  T_LUEVT_DSP_SYSTEM_STATUS{  /*# システムステータス #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dspno;              /*# DSP番号 #*/
    BYTE        dmy;
    WORD        major_ver;          /* DSPメジャーバージョン*/
    WORD        minor_ver;          /* DSPマイナーバージョン*/
    WORD        effect_cnno;        /* TDMポートに割り当てられるチャネル数*/
    
}LUEVT_DSP_SYSTEM_STATUS;

typedef struct  T_LUEVT_DSP_CHANNEL_STATUS{ /*# システムステータス #*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# イベントコード #*/
    BYTE        dspno;              /*# DSP番号 #*/
    BYTE        dmy;
    WORD        tone_xmit_status;   /* トーン送出状態 */
    WORD        present_tone;       /* 送出中トーンID*/
    WORD        rx_buffer_space;    /* TDMポートに割り当てられるチャネル数*/
}LUEVT_DSP_CHANNEL_STATUS;

typedef struct  T_LUEVT_AQNTC{      /*# 音声品質情報通知#*/
    LU_HEADER   lu_header;
    BYTE        bc;             /*# Ｂ．Ｃ #*/
    BYTE        code;               /*# オーダコード #*/
    BYTE        cn;             /*# CN #*/
    BYTE        rtppktsnd[4];               /*# RTPパケット送信数 #*/
    BYTE        rtppktrcv[4];               /*# RTPパケット受信数 #*/
    BYTE        delaymin[4];                /*# 遅延時間最小値 #*/
    BYTE        delaymax[4];                /*# 遅延時間最大値 #*/
    BYTE        delayave[4];                /*# 遅延時間平均値 #*/
    BYTE        seqgapcnt[4];               /*# シーケンス番号ずれ個数 #*/
    BYTE        ipaddr[4];              /*# 送信元IPアドレス #*/
    BYTE        myport[2];              /*# 自受信ポート #*/
    BYTE        youport[2];             /*# 相手送信ポート #*/
}LUEVT_AQNTC;//SS9100 2004.10



/*################################################*/
/*#         LU共通参照用ストラクチャ             #*/
/*################################################*/

typedef struct  {
    BYTE    led_status;
    WORD    timer_save;
    BYTE    int_tim;
    BYTE    base_tim;
    BYTE    ctl[8];
    BYTE    tim[8];
}LED_DAT;

/*# ユニキャスト先ホスト情報構造体 #*/
typedef struct {    //2000.07 V2.1追加
    DWORD       use;                    /*# 使用状態 #*/
    DWORD       sd;                     /*# ソケットディスクリプタ #*/
    DWORD       ip;                     /*# 相手ＩＰアドレス #*/
    WORD        port1;                  /*# 相手CLKD同期ＰＯＲＴ番号 #*/
    WORD        port2;                  /*# 相手CRINT同期ＰＯＲＴ番号 #*/
    WORD        d_sync_cnt;             /*# CLKD同期シーケンス番号 #*/      //RESERVE
    WORD        c_cync_cnt;             /*# CRINT同期シーケンス番号 #*/     //RESERVE
}COM_UNIHOST;

/*# ユニキャスト管理構造体 #*/
typedef struct {    //2000.07 V2.1追加
    DWORD       sd;                     /*# 自ソケットディスクリプタ #*/    //RESERVE
    DWORD       cm_ip;                  /*# クロックマスタＩＰアドレス #*/
    WORD        cm_port;                /*# クロックマスタＰＯＲＴ番号 #*/  //RESERVE
    WORD        sm_port;                /*# サブネットマスタ自ポート番号 #*///RESERVE
    WORD        sync_cnt;               /*# パケット送出補正カウンタ #*/
    BYTE        sm_st;                  /*# サブネットマスタ動作開始 #*/
    BYTE        host_cnt;               /*# 登録ホスト数 #*/                //2000.10.13 V2.1DEBUG
    COM_UNIHOST uni_host[LU_NUM_MAX];   /*# ユニキャスト先ホスト情報 #*/
}COM_UNICAST;

/*######################################################*/
/*#      ＤＳＰ割り込み処理用ストラクチャ              #*/
/*######################################################*/

                                                              /*# 音声制御用 #*/
typedef struct  T_DSPINT_VOICECTL{
                DWORD    hory_sts;                        /*# 保留音送出状態 #*/

                DWORD    snd_num;                     /*# 送話データ数指定値 #*/
                DWORD    snd_cnt;                       /*# 送話データ処理数 #*/
                BYTE     *snd_link;       /*# 送話データセーブリンクアドレス #*/

                DWORD    rcv_start_flg;           /*# 受話デコード開始フラグ #*/
                DWORD    rcv_frm_rc;          /*# 受信フレームリードカウンタ #*/
                DWORD    rcv_frm_wc;          /*# 受信フレームライトカウンタ #*/
//                BYTE     *rcv_wt_bp;          /*# 受信データセーブアドレス #*/
                DWORD    *rcv_wt_bp;            /*# 受信データセーブアドレス #*/
                DWORD    rcv_wt_cnt;                  /*# 受信データカウンタ #*/
             }DSPINT_VOICECTL;

typedef struct{
    DWORD   frm_data[VO_BUF_FRM_MAX][VO_1FRM_DWORD];    /*# [受信フレーム数][40バイト(5ms)] #*/
}RCV_RTP_FRM;

                                                             /*# 監視制御用 #*/
typedef struct  T_DSPINT_SCANCTL{
                DWORD    btdet_sts;                         /*# ＢＴ監視状態 #*/
               // DWORD    bt_rdflg;          /*# ＯＡＫイベント読み込みフラグ #*/
                DWORD    bton_time;      /*# BT_ON監視時間（ONフレーム回数） #*/
                DWORD    btoff_time;   /*# BT_OFF監視時間（OFFフレーム回数） #*/
                DWORD    bt_puls;           /*# BT監視周期回数（ON/OFF回数） #*/
                DWORD    bt_toneid;                      /*# BT_検出TONE種別 #*/
                //DWORD    btdet_way;                     /*# ＢＴ監視方法 #*/
                //WORD     bt_shfttime;                  /*# BT_シフト監視時間 #*/
                //IPCS IPstage対応(start) BT検出回数、BT検出時間、BT検出タイマカウントを追加 2004.9.1
                WORD    btdet_cnt;                          /*# BT検出回数 #*/
                //WORD  btdet_tim;                          /*# BT検出時間 #*/
                WORD    bttim_cnt;                          /*# BT検出タイマカウント #*/
                //IPCS IPstage対応(end) 2004.9.1

                DWORD    dtdet_sts;                         /*# ＤＴ監視状態 #*/
                DWORD    dt_rdflg;          /*# ＯＡＫイベント読み込みフラグ #*/
                DWORD    dton_time;      /*# DT_ON監視時間（ONフレーム回数） #*/
                DWORD    dtoff_time;   /*# DT_OFF監視時間（OFFフレーム回数） #*/
                DWORD    dt_puls;           /*# DT監視周期回数（ON/OFF回数） #*/
                DWORD    dt_toneid;                      /*# DT_検出TONE種別 #*/
                //DWORD    dtdet_way;                      /*# ＤＴ監視方法 #*/
                //WORD     dt_shfttime;                  /*# DT_シフト監視時間 #*/
                //DWORD    dt_det;             /*# DT_ON/OFFフレーム検出中状態 #*/
                //DWORD    dton_cnt;                 /*# DT_ONフレーム検出回数 #*/
                //DWORD    dtoff_cnt;               /*# DT_OFFフレーム検出回数 #*/
                //DWORD    dtflame_cnt;              /*# DT_フレーム内監視回数 #*/
                //DWORD    dtflmon_cnt;            /*# DT_フレーム内ON検出回数 #*/
                //DWORD    dtflmoff_cnt;          /*# DT_フレーム内OFF検出回数 #*/
                //DWORD    dt_flmdet;                  /*# DT_フレーム検出結果 #*/
                //DWORD    dt_flmdet_back;         /*# DT_前回フレーム検出結果 #*/
                //DWORD    dtpuls_cnt;                   /*# DT_ON/OFF検出回数 #*/
                //IPCS IPstage対応(start) DT検出回数、DT検出時間、DT検出タイマカウントを追加 2004.9.1
                WORD    dtdet_cnt;                          /*# DT検出回数 #*/
                //WORD  dtdet_tim;                          /*# DT検出時間 #*/
                WORD    dttim_cnt;                          /*# DT検出タイマカウント #*/
                //IPCS IPstage対応(end) 2004.9.1

                DWORD    pbdet_sts;                         /*# ＰＢ監視状態 #*/
                DWORD    pb_det;                     /*# PB_ON/OFF検出中状態 #*/
                DWORD    pbon_time;             /*# PB_ON監視時間（5ms単位） #*/
                DWORD    pboff_time;           /*# PB_OFF監視時間（5ms単位） #*/
                //DWORD    pbon_cnt;                         /*# PB_ON検出回数 #*/
                //DWORD    pboff_cnt;                       /*# PB_OFF検出回数 #*/
                BYTE     pb_digit;                           /*# PB_検出数字 #*/

                //DWORD    waittim_id1;        /*# ＢＴ／ＤＴ監視有効待ちタイマID_1 #*/
                //DWORD    waittim_id2;        /*# ＢＴ／ＤＴ監視有効待ちタイマID_2 #*/
                //DWORD    waittim_idptn;      /*# 使用中監視有効待ちタイマID種別 #*/
//                BYTE    pb_swdet;                       /*# PB_DBSW検出状態 #*/
                //IPCS IPstage対応(start) BTDT検出タイマID追加 2004.9.1
                DWORD btdtdettim_id;
                //IPCS IPstage対応(end) 2004.9.1

}DSPINT_SCANCTL;

                                                       /*# ＤＳＰ制御データ #*/
typedef struct  T_DSPINT_CTLQUE{
                DWORD   oakcnt;
                DWORD   data[14];
}DSPINT_CTLQUE;

/*######################################################*/
/*#     ＤＳＰ状態管理用ストラクチャ                   #*/
/*######################################################*/
                                            /*# パス制御用 #*/
typedef struct  T_DSPMNG_PASSCTL{
                DWORD    snd_sts;            /*# 送話パス状態 #*/
                WORD     snd_myport;         /*# 送話自ポート番号 #*/
                WORD     snd_port;           /*# 送話先ポート番号 #*/
                DWORD    snd_iptype;         /*# 送話IPタイプ #*/
                BYTE     snd_ipaddr[16];     /*# 送話先IP #*/
                BYTE     snd_multaddr[16];   /*# 送話先マルチアドレス #*/
//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
//                DWORD    snd_pad;            /*# 送話PAD値 #*/
                LONG    snd_pad;            /*# 送話PAD値 #*/
                //圧縮LUD01対応 RTCP制御フラグ 2003.12.12 
                BYTE    snd_rtcp;


                DWORD    rcv_sts;            /*# 受話パス状態 #*/
                WORD     rcv_myport;         /*# 受話自ポート番号 #*/
                WORD     rcv_port;           /*# 受話元ポート番号 #*/
                DWORD    rcv_iptype;         /*# 受話IPタイプ #*/
                BYTE     rcv_ipaddr[16];     /*# 受話元IPアドレス #*/
                BYTE     rcv_multaddr[16];   /*# 受話元マルチアドレス #*/
//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
//                DWORD    rcv_pad;            /*# 受話PAD値 #*/
                LONG    rcv_pad;            /*# 受話PAD値 #*/
                //圧縮LUD01対応 RTCP制御フラグ 2003.12.12 
                BYTE    rcv_rtcp;

//                DWORD    codec_sel;          /*# CODEC種別（未使用） #*/
                DWORD    codec;             /*# CODEC種別 #*/
                DWORD    echo;               /*# エコーキャンセラ #*/
                DWORD    thru_mode;          /*# スルーモード #*/
                DWORD    echo_sts;           /*# エコーキャンセラ状態 #*/
//圧縮LU対応。2003.09.13
                BYTE    send_period;        /*# 送信周期 #*/
                BYTE    fax_sts;            /*# fax状態 #*/
                BYTE    rcvcut_wait;        /*# 送話パス切断待ち　*/
                BYTE    sndcut_wait;        /*# 受話パス切断待ち #*/
                short   sendcount;
                short   send_framecount;
                short   horyu_codec;
                short   password;
                BYTE    buf_num;

}DSPMNG_PASSCTL;


                                            /*# トーン制御用 #*/
typedef struct  T_DSPMNG_TONECTL{
                DWORD   send_sts;           /*# トーン送出状態 #*/
                DWORD   tonetim_id1;        /*# トーンタイマID_1 #*/
                DWORD   tonetim_id2;        /*# トーンタイマID_2 #*/
                DWORD   tonetim_idptn;      /*# 使用中トーンタイマID種別 #*/
                BYTE    *crint_tone;        /*# CRINT待ちセーブアドレス #*/
//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
//                DWORD   tone_pad;           /*# 送出トーンＰＡＤ値 #*/
                LONG    tone_pad;           /*# 送出トーンＰＡＤ値 #*/
                DWORD   tone_ptn;           /*# トーンパターン #*/
                DWORD   on1_tim;            /*# トーンON1時間値 #*/
                DWORD   off1_tim;           /*# トーンOFF1時間値 #*/
                DWORD   on2_tim;            /*# トーンON2時間値 #*/
                DWORD   off2_tim;           /*# トーンOFF2時間値 #*/
                DWORD   on3_tim;            /*# トーンON3時間値 #*/
                DWORD   off3_tim;           /*# トーンOFF3時間値 #*/
                DWORD   tone_tim;           /*# パルス時間値 #*/
                DWORD   tone_id;            /*# 送出トーンＩＤ #*/
                DWORD   tone_f1;            /*# Ｆ１周波数 #*/
                DWORD   tone_f2;            /*# Ｆ２周波数 #*/
                DWORD   tone_fam;           /*# ＦＡＭ周波数 #*/
                DWORD   tone_fk;            /*# ＦＫ周波数 #*/

                DWORD    pb_tim;             /*# PBメーク時間値 #*/
                DWORD    pb_min;             /*# PBブレイク時間値 #*/
 //圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
//                DWORD   pb_pad_l;           /*# 低群ＰＡＤ値 #*/
//                DWORD   pb_pad_h;           /*# 高群ＰＡＤ値 #*/
                LONG    pb_pad_l;           /*# 低群ＰＡＤ値 #*/
                LONG    pb_pad_h;           /*# 高群ＰＡＤ値 #*/
               //DWORD   pbtim_id1;          /*# PBタイマID_1 #*/
                //DWORD   pbtim_id2;          /*# PBタイマID_2 #*/
                //DWORD   posetim_id1;        /*# ポーズタイマID_1 #*/
                //DWORD   posetim_id2;        /*# ポーズタイマID_2 #*/
                DWORD   pbtim_idptn;        /*# 使用中ＰＢタイマID種別 #*/
                DWORD   pbtim_sts;          /*# PBタイマ状態 #*/
                DWORD   pb_num;             /*# 送出PB数 #*/
                DWORD   pb_cnt;             /*# PBデータ読み込み位置 #*/
                BYTE    pb_data[48];        /*# 送出PBデータ #*/
                BYTE    pbread;
                BYTE    pbwrite;
                BYTE    pbcount;
                BYTE    oikosi;
                BYTE    pb_acm_flg;

//圧縮LU対応。オーダPAD値が符号付に変更。2003.09.09
//                DWORD   hory_pad;           /*# 保留音ＰＡＤ値 #*/
                LONG    hory_pad;           /*# 保留音ＰＡＤ値 #*/
                short   fk_sts;
                short   fk_toneid;
                DWORD   fk_tim;             /* 交番フェース切替タイミング */
                BYTE    fk_phase;           /* 交番トーンフェーズ */
                BYTE    dumy[3];            /* ダミーエリア */
}DSPMNG_TONECTL;


                                                                                                                                /*# シフト制御用(V1) #*/
typedef struct  T_DSPMNG_SHFTCTL{
                                DWORD       tim_id1;                                /*# トーンタイマID_1 #*/
                                DWORD       tim_id2;                                /*# トーンタイマID_2 #*/
                                DWORD       tim_idptn;                          /*# 使用中トーンタイマID種別 #*/
                                DWORD       cnno;                                       /*# シフト監視中ＣＮ番号 #*/
}DSPMNG_SHFTCTL;


/*################################################*/
/*#         ＭＩＫ用ストラクチャ                 #*/
/*################################################*/
typedef struct  {                               /*# ＭＩＫタスクテーブル構造 #*/
    void    (*tskaddr)();                         /*# タスクスタートアドレス #*/
    BYTE    *stackptr;                            /*# タスクスタックポインタ #*/
    BYTE    pri;                                          /*# タスク優先順位 #*/
    BYTE    zero1;                                                /*# ’０’ #*/
    BYTE    zero2;                                                /*# ’０’ #*/
    BYTE    attach;                                         /*# アタッチ指定 #*/
    DWORD   rsv;                                                /*# リザーブ #*/
} S_MIKTSK;

typedef struct  {                               /*# ＭＩＫプールブロック構造 #*/
    BYTE    *addr;                                    /*# プール先頭アドレス #*/
    DWORD   len;                                        /*# プールブロック長 #*/
    DWORD   count;                                      /*# プールブロック数 #*/
    DWORD   rsv;                                                /*# リザーブ #*/
} S_MIKPOOL;

typedef struct  {                               /*# ＭＩＫログエリア管理構造 #*/
    BYTE    *top_addr;                                  /*# ログ開始アドレス #*/
    BYTE    *end_addr;                                  /*# ログ終了アドレス #*/
    BYTE    *cur_ptr;                                       /*# ログアドレス #*/
    DWORD   rsv;                                                /*# リザーブ #*/
} S_MIKLOG;

/*################################################*/
/*#         その他用ストラクチャ                 #*/
/*################################################*/
#define ASI_SBUF_MAX        1024
#define ASI_RBUF_MAX        1024
typedef struct  {                                         /*# ＡＳＩ管理構造 #*/
    BYTE    *snd_r_ptr;                               /*# 送信リードポインタ #*/
    BYTE    *snd_w_ptr;                               /*# 送信ライトポインタ #*/
    BYTE    asi_sndbuf[ASI_SBUF_MAX];                       /*# 送信バッファ #*/
    BYTE    *rcv_r_ptr;                               /*# 受信リードポインタ #*/
    BYTE    *rcv_w_ptr;                               /*# 受信ライトポインタ #*/
    BYTE    asi_rcvbuf[ASI_RBUF_MAX];                       /*# 送信バッファ #*/
} S_ASIDAT;

#define LUN_BIT_MAX         38                  /*# 出力ＬＵ番号ビットＭＡＸ #*/
#define LUN_SCN_MAX         100                     /*# スキャンデータＭＡＸ #*/
typedef struct  {                                       /*# ＬＵ番号通知構造 #*/
    DWORD   lun_out_ctrl;                               /*# ＬＵ番号出力制御 #*/
    DWORD   lun_out_cnt;                      /*# ＬＵ番号ビット出力カウンタ #*/
    DWORD   lun_out_bit[LUN_BIT_MAX];           /*# ＬＵ番号ビット出力データ #*/
    DWORD   lun_in_rcnt;                  /*# ＬＵ番号スキャンリードカウンタ #*/
    DWORD   lun_in_wcnt;                  /*# ＬＵ番号スキャンライトカウンタ #*/
    DWORD   lun_scn_num;                          /*# ＬＵ番号スキャンデータ #*/
    DWORD   lun_scn_sts;                            /*# ＬＵ番号スキャン状態 #*/
    DWORD   lun_scn_dat[LUN_SCN_MAX];             /*# ＬＵ番号スキャンデータ #*/
} S_LUNUM_DAT;

typedef struct  {                                     /*# ＬＥＤ制御情報構造 #*/
    DWORD   red;
    DWORD   green;
    DWORD   last_red;
    DWORD   last_green;
    DWORD   int_exec;
} TEST_LED_INF;

typedef struct  {                           /*# ＤＲＡＭチェック結果情報構造 #*/
    DWORD   exec;
    DWORD   addr;
    DWORD   ok_data;
    DWORD   rd_data;
} TEST_DARAM_RESULT;

typedef struct  {                                   /*# ＣＰＵ使用率情報構造 #*/
    DWORD   exec_flg;
    DWORD   tim_5ms;
    DWORD   tim_100ms;
    DWORD   tim_1s;
    DWORD   cnt_5ms_cnt[20];
    DWORD   cnt_5ms_max;
    DWORD   cnt_5ms_min;
    DWORD   cnt_100ms_cnt[10];
    DWORD   cnt_100ms_max;
    DWORD   cnt_100ms_min;
    DWORD   cnt_1s_cnt[10];
    DWORD   cnt_1s_max;
    DWORD   cnt_1s_min;
} RATE_CPU_COUNT;

typedef struct{
    DWORD   ready_link_top;
    DWORD   ready_link_tail;
    DWORD   user_link_top;
    DWORD   user_link_tail;
    DWORD   ready_link_cnt;
    DWORD   user_link_cnt;
    DWORD   mik_rsv1;
    DWORD   mik_rsv2;
} MIK_PLCB;

                                                                /*# １次元ＪＵＭＰテーブル構造 #*/
typedef struct  {void (*mtx_adr)();}MTX_ADR;
typedef struct  {void (*mtx_adr2)(BYTE,BYTE,BYTE);}MTX_ADR2;
typedef struct  {BYTE (*mtx_adr3)();}MTX_ADR3;
typedef struct  {void (*mtx_adr4)(WORD);}MTX_ADR4;
typedef struct  {void (*mtx_adr5)(DWORD,DWORD,DWORD);}MTX_ADR5;
typedef struct  {void (*mtx_adr6)(WORD,signed int);}MTX_ADR6;

//typedef   void (BYTE*)()  MTX_ADR;
typedef struct  {
    BYTE    status;
    BYTE    exeid;
    BYTE    tmid;
    WORD    stmcnt;
    WORD    tmcnt;
    BYTE    mscnt;
    WORD    sndid;
}TMR_TMCB;




#ifdef LU_DBG
/*# DBG #*/
typedef struct  {
    DWORD   ordacscnt[4];
    DWORD   ordwtcnt;
    DWORD   helwtcnt;
    DWORD   doorwtcnt;
    DWORD   pool0cnt;
    DWORD   pool1cnt;
    DWORD   pool2cnt;
    DWORD   pool3cnt;
    DWORD   q_acm_flg;
    DWORD   msg_stock[4];
    DWORD   vo_dump_cnt;                                    /*# DAVIS-LU-ST 99-5-17 #*/
    DPRAM_WRITE *msg_pool[4][10];
    WORD    dpram[100];
    }DBG;
#endif
 
// ＰＢ検出ＮＧ調査用

typedef struct{
    DWORD   dspint_ldial[4];
    DWORD   dspmng_ldial[4];
    DWORD   doep_ldial[4];
} PB_LL_DBG;


typedef struct  T_DOEP_DATA{                                /*# DOEP内部データ #*/
    BYTE    sts;
    BYTE    t1id;
    //BYTE  t1cnt;
    WORD    t1cnt;      //D01サバイバル対応 2004.02 LU32B追加
    BYTE    t2id;
    BYTE    t2cnt;
    BYTE    t3id;
    BYTE    *sndmsg;
    WORD    sndseq;
    WORD    rcvseq;
    DWORD   sd;
    DWORD   sndng_cnt;                                      /*# LAN送信NGｶｳﾝﾄ(V1) #*/
    BYTE    survflg;    //D01サバイバル対応 2004.02 LU32B追加
    BYTE    t4id;       // サバイバル切り戻しタイマ：IP-CS(SS9100_PH1再開) 追加
    BYTE    t5id;       // コネクション再接続タイマ：IP-CS(SS9100_PH1再開) 追加
    //SS9100(PH1再開)対応 2006.06
    BYTE                snd_rsv_online_flg;         /*LUオンライン通知送信予約フラグ*/
    LUEVT_LUONLINE      snd_rsv_online_msg;         /*LUオンライン通知確保領域*/
    BYTE                snd_rsv_pkgini_flg;         /*個別PKG初期設定応答送信予約フラグ*/
    LUEVT_PKGINITRSP    snd_rsv_pkgini_msg;         /*個別PKG初期設定応答確保領域*/
    BYTE				snd_rsv_helchk_flg;			/*LUヘルスチェック応答送信予約フラグ*/
    LUEVT_LUHELRSP		snd_rsv_helchk_msg;			/*LUヘルスチェック応答確保領域*/
	BYTE				ph1_end_flg;				/*PH1終了処理用フラグ*/

}DOEP_DATA;


typedef struct{
    DWORD       fsq_busy[CNMAX];
    DWORD       rtp_sndng1[CNMAX];
    DWORD       rtp_err[CNMAX];
    DWORD       rtp_sndcnt[CNMAX];              /*# ＲＴＰ送信数 #*/
    DWORD       rtp_rcvcnt[CNMAX];              /*# ＲＴＰ受信数 #*/
    DWORD       rtcp_rcvng[CNMAX];
    DWORD       rtcp_sndng[CNMAX];
    DWORD       rtp_rcvng1[CNMAX];

    DWORD       rtp_rcvng2[CNMAX];
    DWORD       rtp_rcvng3[CNMAX];
    DWORD       rtp_rcvng4[CNMAX];
    DWORD       rtp_rcvng5[CNMAX];

    DWORD       rtp_rcvng6[CNMAX];
    DWORD       rtp_rcvok[CNMAX];
    DWORD       dsp_rdng_cnt;
    DWORD       dsp_sndng_cnt;

    DWORD       dsp_slng_cnt;
    DWORD       h323_sdes_to;
    DWORD       h323_bye_rcv;
    DWORD       doep_ack_seqng;

    DWORD       doep_ord_seqng;
    DWORD       doep_seq_retry;
    DWORD       doep_evt_retry;
    DWORD       doep_pkt_rcv;

    DWORD       doep_hd_ok;
    DWORD       doep_time;

    DWORD       dsp_mute[CNMAX];

    DWORD       hel_seq_err[8];

    BYTE        hd_inf[16];
} LU_LOG;

#define RTP_SEQ_LOGMAX  32
typedef struct  {
    WORD        seq_ng_cnt[CNMAX];
    WORD        seq_ng_idx[CNMAX];
    WORD        seq_okng[CNMAX];
    WORD        last_okseq[CNMAX];
    WORD        seq_log[CNMAX][RTP_SEQ_LOGMAX][4];
}RTP_SEQ_LOG;

#define RTP_BUF_LOGMAX  16
typedef struct  {
    DWORD       max_ng_idx[CNMAX];
    DWORD       max_log[CNMAX][RTP_BUF_LOGMAX][2];
}RTP_BUF_LOG;

#define SEQ_NG_LOGMAX   2048
typedef struct  {
    DWORD       log_idx;
    DWORD       seq_log[SEQ_NG_LOGMAX];
}SEQ_NG_TIME_LOG;

#define MAC_RST_LOGMAX  256
struct  RSTINF{
//  WORD        reset_cnt;
//  WORD        reset_port;
    DWORD       reset_time;
};
typedef struct  {
    DWORD       reset_cnt;
    DWORD       reset_idx;
    struct  RSTINF  rst_inf[MAC_RST_LOGMAX];
}MAC_RST_LOG;


/* MAC8110K error information TAG */
typedef struct{
    /* packet inf */
    unsigned long   rcv_good;               /* 正常受信 */
    unsigned long   rcv_bad;                /* 受信異常 */
    unsigned long   snd_req;                /* 送信要求 */
    unsigned long   snd_bad;                /* 送信異常 */

    unsigned long   jab_ipt;                /* JAB割込み発生 */
    unsigned long   jab_lost;               /* JABロスト */
    unsigned long   link_lost;              /* リンクNGロスト */
    unsigned long   link_ipt;               /* リンク状態変化割り込み発生 */

    unsigned long   tag_lost;               /* VLANロスト */
    unsigned long   full_lost;              /* 受信フルロスト */
    unsigned long   geterr_lost;            /* バッファなしロスト */
    unsigned long   linkup_chat;            /* リンクアップチャタリング回数 */

    unsigned long   linkdown_chat;          /* リンクダウンチャタリング回数 */
    /* transmit */
    unsigned long   tx_se;                  /* SQE試験エラー */
    unsigned long   tx_lo;                  /* ロスオブキャリアセンス */
    unsigned long   tx_td;                  /* 送信完了 */

    unsigned long   tx_lca;                 /* レイトコリジョンアボート */
    unsigned long   tx_xd;                  /* XDEFアボート */
    unsigned long   tx_xc;                  /* 過大コリジョンアボート */
    unsigned long   tx_ur;                  /* アンダーランアボート */

    unsigned long   tx_hu;                  /* 過大パケットアボート */
    unsigned long   tx_pd;                  /* 送信待ち後の送信 */
    unsigned long   tx_bc;                  /* ブロードキャスト */
    unsigned long   tx_mc;                  /* マルチキャスト */

    unsigned long   tx_ce;                  /* CRCエラー */
    unsigned long   tx_lc;                  /* レイトコリジョン */
    /* receive */
/*  リセットまでクリアされないため参照せず*/
    unsigned long   rx_drop;                /* 受信パケットドロップ */
    unsigned long   rx_oftr;                /* オーバーフロートランケイト */

/*  リセットまでクリアされないため参照せず */
    unsigned long   rx_td;                  /* TRDR */
    unsigned long   rx_oa;                  /* OVDA */
    unsigned long   rx_od;                  /* OVDR */
    unsigned long   rx_ot;                  /* OVTR */

    unsigned long   rx_fl;                  /* フィルタリング */
    unsigned long   rx_sp;                  /* ショートパケット */
    unsigned long   rx_sf;                  /* スタックフル */
    unsigned long   rx_ca;                  /* キャリアアサート */

    unsigned long   rx_rx;                  /* RXDVNT */
    unsigned long   rx_rc;                  /* RSVOK */
    unsigned long   rx_bc;                  /* ブロードキャスト */
    unsigned long   rx_mc;                  /* マルチキャスト */

    unsigned long   rx_ce;                  /* CRCエラー */
    unsigned long   rx_dr;                  /* ドリブルエラー */
    unsigned long   rx_co;                  /* CODERR */
    unsigned long   rx_lo;                  /* ロング */

    unsigned long   rx_sh;                  /* ショート */
    unsigned long   rx_dfram;               /* 受信フレームドロップカウント */
    unsigned long   sf_reset;               /* 受信ステータススタックフルリセット */    // 2001.03.12追加
    unsigned long   rcvlen_lost;            /* 受信レングス異常ロスト */                // 2001.03.16 追加

    unsigned long   start_ng;               /* スタートワードＮＧ */                    // 2001.04.23 追加
    unsigned long   end_find;               /* エンドワード */                          // 2001.04.23 追加
    unsigned long   end_ng;                 /* エンドワードＮＧ */                      // 2001.04.23 追加
    unsigned long   mac_rst_ng;             /* ＭＡＣリセットＮＧ */                    // 2001.04.23 追加 2001.05.28変更

#ifdef  MAC_FIFO_LOG
    unsigned long   fifo_ovlf1;             /* オーバーフロー */    // 2001.04.16 追加
    unsigned long   fifo_lc1;               /* レイトコリジョン */  // 2001.04.16 追加
    unsigned long   fifo_xd1;               /* xdef */              // 2001.04.16 追加
    unsigned long   fifo_xc1;               /* 過大コリジョン */    // 2001.04.16 追加

    unsigned long   fifo_ur1;               /* アンダーラン */      // 2001.04.16 追加
    unsigned long   fifo_hu1;               /* 過大パケット */      // 2001.04.16 追加
    unsigned long   fifo_ovlf2;             /* オーバーフロー */    // 2001.04.16 追加
    unsigned long   fifo_lc2;               /* レイトコリジョン */  // 2001.04.16 追加

    unsigned long   fifo_xd2;               /* xdef */              // 2001.04.16 追加
    unsigned long   fifo_xc2;               /* 過大コリジョン */    // 2001.04.16 追加
    unsigned long   fifo_ur2;               /* アンダーラン */      // 2001.04.16 追加
    unsigned long   fifo_hu2;               /* 過大パケット */      // 2001.04.16 追加
#endif  //MAC_FIFO_LOG

}KM_INF;

typedef struct  {
    DWORD   rcv_time;
    DWORD   rcv_len;
    DWORD   dmy1;
    DWORD   dmy2;
    BYTE    rcv_dat[80];
}KMAC_RINF;
typedef struct  {
    DWORD   mac_isr_time;
    DWORD   nw_jab_time;
    DWORD   pc_jab_time;
    DWORD   nw_rx_time;

    DWORD   pc_rx_time;
    DWORD   tx_ipt_time;
    DWORD   sts_ipt_time;
    DWORD   rx_ng_time;

    DWORD   rx_ng_cnt;
    BYTE    rcvsts_nw;
    BYTE    rcvport_nw;
    BYTE    rcvdel_nw;
    BYTE    dmy1;
    BYTE    *write_p_nw;
    BYTE    *rx_mp_nw;

    BYTE    rcvsts_pc;
    BYTE    rcvport_pc;
    BYTE    rcvdel_pc;
    BYTE    dmy2;
    BYTE    *write_p_pc;
    BYTE    *rx_mp_pc;
    DWORD   port;

    DWORD   rcv_sts;
    DWORD   sts_time;

    DWORD   rxov_cnt;
    DWORD   rxbal_cnt;
    DWORD   rxinf_cnt;
    KMAC_RINF   rx_inf[4];
}KMAC_LOG;

typedef struct {
    BYTE    name[16];
    void    *addr;
    DWORD   size;
} S_ADDR;
/*************** 構造体定義 ***************/
/* 注意：変更の際は、以下のコメントに注意             */
/*       この構造体は必ず４の倍数になること           */
/*       ４の倍数にならない場合は利用先で問題となる。 */
typedef struct {
  BYTE *AllocAddr;
  WORD Index;
  WORD Dmmy;
  void *pool_next_adr;
  void   *link_next; /* Queueuリンク用 */
  void   *link_pre; /* Queueuリンク用 */
} XS_POOL_INFO;
/* メモリプールデータ */
typedef struct {
  DWORD  PoolSize;
  WORD PoolNum;
  WORD Dmy2;
} XS_POOL_DAT;
/* メモリプール属性 */
typedef struct {
  DWORD PoolSize;
  WORD PoolNum;
  WORD get_count;
  void *pool_adr;
} XS_POOL_ATR;

/* メッセージキュー関連 */
#define LUMQ_INACTIVE   0   /* Inactive */
#define LUMQ_ACTIVE      1  /* Active */
#define LUMQ_MAXMSG     64  /* Maximum number of messages in queue */
#define LUMQ_MSGSIZE    80  /* Maximum message size (in bytes) */

typedef void (*ComFuncAddr)( void * );

/* ECB属性 */
typedef struct {
	mqd_t   mq_desc;
	int     status;
	struct mq_attr attr;    /* Attribute */
} XS_ECB_ATTR;

/* タスク属性 */
typedef struct {
	BYTE	create_flg;  /* 初期生成フラグ(ON:初期生成/OFF:初期生成しない) */
	BYTE	op_mode;	 /* 動作種別(1:通常モード時のみ/2:保守モード時のみ/3:両モード時) */
	BYTE	task_pri;    /* 優先度 */
	BYTE	dumy;		 /* ダミーデータ */
	ComFuncAddr	start_routine; /* Thread start routine */
} XS_TASK_ATTR;

/* タスク状態 */
typedef struct {
	int				status;      /* 状態(0:TASK_INACTIVE/1:TASK_ACTIVE) */
	pthread_t		thread_id;   /* Thread id */
    pthread_attr_t	thread_attr; /* Thread attribute */
} XS_TASK_STATUS;

/* ECB属性 */
typedef struct {
	char    *name;          /* Name of message queue */
	int     oflag;          /* oflag */
	mode_t  mode;           /* Mode */
	struct mq_attr *attr; /* Attribute */
} XS_ECB_STTR;

/* キュー関連 */
/* キュー管理テーブル */
typedef struct {
	DWORD        Que_num;	/* 登録キュー数 */
	XS_POOL_INFO *Que_Head;	/* 先頭メモリプールアドレス */
	XS_POOL_INFO *Que_Tail;	/* 末尾メモリプールアドレス */ 
} XS_QUE_CTL;

/* デバッグプリント関連 */
/* デバッグプリント管理テーブル */
typedef struct {
	int        log_level;	/* ログレベル */
	int        output_dst;/* 出力先 */
} XS_DBGPRINT_CTL;

