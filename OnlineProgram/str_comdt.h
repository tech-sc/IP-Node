typedef struct  T_SLOT_INF{
    BYTE        slot_status;                        /*# スロット状態 #*/
    BYTE        pkg_inf;                            /*# 挿入PKG情報 #*/
    BYTE        dpram_adr;                          /*# 先頭アドレス    #*/
    BYTE        dpram_size;                         /*# DPRAMサイズ     #*/
    BYTE        q_id;                               /*# メッセージキューID #*/
    BYTE        door_scn;                           /*# ドアホンスキャン要求 #*/
    BYTE        helilg_flg;                         /*# ヘルスチェックイリーガルフラグ #*/
    BYTE        pkg_id;                             /*# パッケージ種別 #*/  // 99.11.01
    BYTE        pkg_ver;                            /*# パッケージバージョン #*/    // 99.11.01
    BYTE        slot_pre_status;                    /*# 前回スロット状態 #*/            //LU-99.7.29
    WORD        door_pre_data;                      /*# 前回ドアホンスキャン情報 #*/    //LU-99.7.29
    }SLOT_INF;

typedef struct  T_DOWN_LOAD{
    BYTE        crint;                              /*# リンガ同期周期  #*/
    BYTE        law;                                /*# A－Low/μ－Low指定  #*/
    BYTE        iptype;                             /*# IPv4/IPv6指定   #*/
    BYTE        mult_ttl;                           /*# 同期パケットTTL値 #*/
    DWORD       multi_addr[4];                      /*# マルチキャストアドレス  #*/
    WORD        port1;                              /*# 同期パケット用ポート    #*/
    WORD        port2;                              /*# CR-INTパケット用ポート  #*/
    WORD        u_port1;                            /*# ユニキャスト同期CLKD用ポート番号 #*/    // 2000.07  V2.1 追加
    WORD        u_port2;                            /*# ユニキャスト同期CRINT用ポート番号 #*/   // 2000.07  V2.1 追加
    DWORD       clk_addr[4];                        /*# 同期パケット送信元アドレス #*/
    BYTE        yuragi;                             /*# ゆらぎ設定値(１～５)(V1) #*/
    BYTE        buffer;                             /*# バッファ面数指定(１～５)(V1) #*/
    BYTE        clkdsync;                           /*# 同期方式指定 #*/    // 2000.05.11  V1.2 追加
    BYTE        tftp;                               /*# 保留音／トーキーダウンロード許可 #*/    //2001.02.28 V2.2追加
    WORD        ccuver;                             /*# ＣＣＵバージョン #*/
    WORD        dmy1;                               /*#  #*/
    }LU_DWNLOAD;

typedef struct  T_DPDWNLOAD{
    BYTE        pkg_code;                           /*# PKG種別     #*/
    BYTE        dpram_adr;                          /*# 先頭アドレス    #*/
    BYTE        dpram_size;                         /*# DPRAMサイズ     #*/
    BYTE        helilg_flg;                         /*# ヘルスチェックイリーガルフラグ #*/
    }DP_DWNLOAD;

typedef struct  T_LU_NETWORK{                       /*# ネットワークデータ #*/
    BYTE        macaddr[6];                         /*# LU MACアドレス #*/
    DWORD       yiaddr;                             /*# LU IPアドレス #*/
    DWORD       siaddr;                             /*# CCU IPアドレス#*/
    DWORD       fiaddr;                             /*# FILEサーバ IPアドレス #*/   //2000.07 V2.1 追加
    DWORD       giaddr;                             /*# ゲートウエイIPアドレス #*/
    DWORD       subnet_msk;                         /*# サブネットマスク #*/
    DWORD       tag_a;                              /*# ＶＬＡＮタグＡ #*/
    DWORD       tag_b;                              /*# ＶＬＡＮタグＢ #*/
    DWORD       tag_c;                              /*# ＶＬＡＮタグＣ #*/
    DWORD       tag_d;                              /*# ＶＬＡＮタグＤ #*/
    DWORD       ccu1_ip;                            /*# ＣＣＵ１アドレス #*/                    // 2001.08 V2.3 追加
    DWORD       ccu2_ip;                            /*# ＣＣＵ２アドレス #*/                    // 2001.08 V2.3 追加
    DWORD       ccu3_ip;                            /*# ＣＣＵ３ドレス #*/  //2004.08 SS9100対応追加
    DWORD       act_ccu;                            /*# ＡＣＴ ＣＣＵアドレス #*/               // 2001.08 V2.3 追加
}LU_NETWORK;

typedef struct  T_COM_DATA{
    /*# マスタ/スレーブ共通 #*/
    DWORD       lu_cabinet;                         /*# ＬＵキャビネットタイプ 0/1:BS/EX #*/
    DWORD       dpram_num;                          /*# DPRAM実装数  #*/
    BYTE        tag_ctl;                            /*# TAG設定(あり：1、無し：０) #*/
    BYTE        clkd_ins;                           /*# CLKD-INS情報(INS：1、OUS：０) #*/
    BYTE        lu_type;                            /*# LUタイプ (KP4.3：マスタ=0／スレーブ=1 #*/
//  BYTE        lu_number;                          /*# ＬＵ装置番号 #*/
    WORD        lu_number;                          /*# ＬＵ装置番号 #*///SS9100対応拡張 2004.08
    BYTE        clkd_type;                          /*# CLKDタイプ (ｸﾛｯｸﾏｽﾀ=0／ｸﾛｯｸｽﾚｰﾌﾞ=1) #*/
    BYTE        slv_number;                         /*# スレーブ番号 (KP1.0～3：マスタ=0(?)／スレーブ=1(0)～16(15)) #*/
    BYTE        ins_pkgnum;                         /*# 実装PKG数 (PORTスロット数) #*/
    BYTE        ins_chnum;                          /*# 実装ch数 (KP1.4：4(0)／8(1)) 但しV1は4ch固定 #*/
    BYTE        chsta_num;                          /*# 使用先頭CH番号 (KP1.0～3：スレーブ=0(0)～15(15)) #*/
    BYTE        country;                            /*# 国種別(V1)(KP4.2:JAPAN(0)／USA(1)) #*/
    BYTE        syn_ptn;                            /*# 同期制御パターン #*/                // 2000.05.12 V1.2 追加
    BYTE        hard_ver;                           /*# ハードウェアバージョン情報 #*/      // 2000.05.12 V1.2 追加
    BYTE        crint_trg;                          /*# ＣＲＩＮＴ頭だし要求 #*/            // 2000.05.12 V1.2 追加
    BYTE        clkd_ins_chg;                       /*# CLKD実装変化情報 #*/                // 2000.05.20 V1.2 追加
    BYTE        subnet_master;                      /*# サブネットマスタ指定 #*/            // 2000.07 V2.1変更
    BYTE        remote;                             /*# 遠隔設置情報 #*/                    // 2000.12.26 V2.2追加
    BYTE        mnt;                                /*# 保守モード情報 #*/                  // 2000.12.26 V2.2追加
    BYTE        slv_mnt;                            /*# スレーブκ保守モード #*/            // LU32B 
    BYTE        ccu_id;                             /*# CCU識別子（MST:DIPSW、SLV:SIO） #*/ // 2000.12.26 V2.2追加
    BYTE        tos_a;                              /*# TOS A #*/                           // 2000.12.27 V2.2追加
    BYTE        tos_b;                              /*# TOS B #*/                           // 2000.12.27 V2.2追加
    BYTE        tos_c;                              /*# TOS C #*/                           // 2000.12.27 V2.2追加
    BYTE        tos_d;                              /*# TOS D #*/                           // 2000.12.27 V2.2追加
    BYTE        dmy[1];                             /*#  #*/
    WORD        kappa_ver;                          /*# κＬＳＩバージョン #*/              // 2000.09 V2.1追加
    WORD        hel_tim;                            /*# ヘルスチェックタイマカウント #*/    // 2000.12.27 V2.2追加
    LU_DWNLOAD  lu_download;                        /*# ダウンロードデータ #*/          // 2000.07 V2.1変更
    LU_NETWORK  lu_network;                         /*# ネットワークデータ #*/          // 2000.07 V2.1変更
//  DWORD       voice_start[4];                     /*# 音声デコード開始バッファ面数 #*/// 2000.01.16   //2000.07 V2.1変更
    DWORD       voice_start[CNMAX];                     /*# 音声デコード開始バッファ面数 #*/// 2000.01.16   //2000.07 V2.1変更
//  DWORD       voice_stop[4];                      /*# 音声データ破棄バッファ面数 #*/  // 2000.01.16   //2000.07 V2.1変更
    DWORD       voice_stop[CNMAX];                      /*# 音声データ破棄バッファ面数 #*/  // 2000.01.16   //2000.07 V2.1変更
    /*# LUマスタ情報 #*/
    SLOT_INF    slot_inf[DPRAM_MAX];                /*# スロット情報(V1) #*/
    DP_DWNLOAD  dpram_data[256];                    /*# DPRAMダウンロードデータ #*/
    BYTE        clkd_noinsevt;                      /*# CLKD未実装イベント通知状態 #*/                  // 2000.09 V2.1追加
    BYTE        dl_ord_bc;                          /*# ダウンロードオーダサイズ（暫定） #*/            // 2000.09 V2.1追加
    BYTE        flash_write;                        /*# ＦＬＡＳＨ書き換え中 #*/                        // 2001.01.19 V2.2追加
    BYTE        obs_notify;                         /*# 障害通知要求 #*/ //LU32B
    DWORD       lu_slv_restart;                     /*# スレーブリスタート要求0/1なし／あり #*/         // 2001.1.18 追加
    /*#LUスレーブ情報 #*/
//  BYTE        *horyud_ptr;                        /*# 再生中音声ファイル保留音データ有効面ポインタ #*/
//  BYTE        *horyud_ptr[4];                     /*# 再生中音声ファイル保留音データ有効面ポインタ #*///2001.01.17 V2.2変更
    BYTE        *horyud_ptr[CNMAX];                 /*# 再生中音声ファイル保留音データ有効面ポインタ #*///2004.01.19 変更
//  WORD        horyu_rdcnt;                        /*# 保留音データリードカウンタ #*/
//  WORD        horyu_rdcnt[4];                     /*# 保留音データリードカウンタ #*/                  //2001.01.17 V2.2変更
    WORD        horyu_rdcnt[CNMAX];                 /*# 保留音データリードカウンタ #*/              //2004.01.19 変更
//  WORD        horyu_rdmax;                        /*# 保留音データリードＭＡＸ#*/
//  WORD        horyu_rdmax[4];                     /*# 保留音データリードＭＡＸ#*/                     //2001.01.17 V2.2変更
    WORD        horyu_rdmax[CNMAX];                 /*# 保留音データリードＭＡＸ#*/                     //2004.01.19 変更
//  DWORD       horyu_act;                          /*# 保留音データアクト面 #*/                        //2001.01.17 V2.2削除
//  DWORD       chunk_size;                         /*# 保留音データサイズ #*/                          //2001.01.17 V2.2削除
    DWORD       horyu_chunk_size;                   /*# 保留音データサイズ #*/                          //2001.01.17 V2.2追加
    DWORD       talky_chunk_size;                   /*# トーキーデータサイズ #*/                        //2001.01.17 V2.2追加
    DWORD       silent_data[VO_1FRM_DWORD];         /*# 無音データ #*/
    BYTE        *horyu_data_top;                    /*# 保留音ファイルデータ再生開始位置 #*/            //2001.01.31 V2.2追加
    BYTE        *talky_data_top;                    /*# トーキーファイルデータ再生開始位置 #*/          //2001.01.31 V2.2追加
    BYTE        silent_ptn;                         /*# 無音パターン #*/
    BYTE        dsp_ch;                             /*# DSP制御PPチャンネル数 #*/                       // 2000.08 V2.1追加
    WORD        voice_acm_cnt[CNMAX];               /*# 音声データ蓄積カウンタ #*/
    DWORD       timer_cnt;                          /*# ＭＩＫ用タイマカウント #*/
    WORD        sndok_flg;                          /*# 送信OKフラグ #*/
    WORD        snd_cnt;                            /*# DSP音声送信数 #*/
    WORD        sndstop_flg;                        /*# DSP音声フロー制御中フラグ 0/1:送信／停止中 #*/
    WORD        io_bz_ctrl;                         /*# ﾌﾞｻﾞｰｺﾝﾄﾛｰﾙﾚｼﾞｽﾀｾｰﾌﾞｴﾘｱ(V1) #*/
    WORD        io_port0;                           /*# ポート０出力イメージ #*/
//  WORD        dsp_rdng_cnt;                       // LU 99.8.9    2001.03.16ログ管理に移動
//  WORD        dsp_sndng_cnt;                      // LU 99.9.9    2001.03.16ログ管理に移動
    WORD        online;                             // LU 99.8.9
    WORD        rtp_sndseq[CNMAX];
    WORD        rtp_rcvseq[CNMAX];
    DWORD       rtp_time[CNMAX];
//  DWORD       rtp_rcvng1[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvng2[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvng3[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvng4[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvng5[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvng6[CNMAX];                  //  2001.03.16ログ管理に移動
//  DWORD       rtp_rcvok[CNMAX];                   //  2001.03.16ログ管理に移動
    volatile DWORD  time_log_cnt;                   /*# ログ収集用５ｍｓ時間情報 #*/
//圧縮LU対応 2003.12.12
    WORD        silent_set;                         /*# 無音パターンセットフラグ #*/
//IP-CS対応 2004.04
    BYTE        led_flg;                        /*# 障害時LED点滅フラグ #*/
    BYTE        tftp_ing;                           /*# TFTPH動作中フラグ #*/
    BYTE        reset;                              /*# TFTP動作中のリセット指示保持エリア #*/
//SS9100(PH1再開)対応 2006.06
    BYTE        ph1_flg;                    /*PH1再開中フラグ*/
    BYTE		ph1_dpram_err;				/*PH1設定レジスタ書き込み異常フラグ*/

}COM_DATA;

/*# FLASH情報管理構造体 #*/
typedef struct {    //2000.09 V2.1追加
    BYTE        lu_num;             /*# ＬＵ番号 #*/
    BYTE        lsi_num;            /*# ＬＳＩ番号 #*/
    BYTE        dummy[2];           /*# ダミー #*/
    DWORD       ip;                 /*# ＩＰアドレス #*/
}LU_SIPINF; //8byte

/*# FLASH情報：管理データ構造体 #*/
typedef struct {    //2000.09 V2.1追加
    BYTE        vlan_id;                /*# ＶＬＡＮ－ＩＤ #*/
    BYTE        class_a;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ａ #*/
    BYTE        class_b;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｂ #*/
    BYTE        class_c;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｃ #*/
    BYTE        class_d;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｄ #*/
    BYTE        ccu_id;                 /*# ＣＣＵ識別子 #*/
    BYTE        ccu_ver[2];             /*# ＣＣＵバージョン #*///ここまで28バイト
    DWORD       yiaddr;                 /*# 自ＩＰアドレス #*/
    DWORD       ipmask;                 /*# サブネットマスク #*/
    DWORD       giaddr;                 /*# ゲートウェイアドレス #*/
    DWORD       siaddr;                 /*# ＣＣＵＩＰアドレス #*/
    DWORD       siaddr2;                /*# ＣＣＵＩＰアドレス２ #*/
    LU_SIPINF   sip_inf[SLV_MAX];       /*# スレーブκＩＰ情報 #*/  //8*16=128byte
    WORD        lu_number;          /*# LU番号 #*/  //2004.01
    BYTE        dipsw;                  /*# DIPSW情報 #*/   //IP-CS対応 2004.04
    BYTE        extension;              /*# 拡張情報 #*/    //IP-CS対応 2004.04
    WORD        vlan_id_W;              /*# (WORD)ＶＬＡＮ－ＩＤ #*///SS9100対応 2004.08
//  WORD        lu_number_W;            /*# (WORD)LU番号 #*///SS9100対応 2004.08
    WORD        reserve;            /*#  lu_number_W廃止 #*///SS9100対応 2004.11.09
    DWORD       siaddr3;                /*# ＣＣＵＩＰアドレス3 #*///SS9100対応 2004.08
    BYTE        dummy[88];              /*# ダミー #*/
}COM_FLS_DATA;  //28+128+100=256

/*# FLASH情報：管理データ設定状態構造体 #*/
typedef struct {    //2000.09 V2.1追加
    BYTE        vlan_id;                /*# ＶＬＡＮ－ＩＤ #*/
    BYTE        class_a;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ａ #*/
    BYTE        class_b;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｂ #*/
    BYTE        class_c;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｃ #*/
    BYTE        class_d;                /*# ＶＬＡＮプライオリティＣＬＡＳＳ－Ｄ #*/
    BYTE        ccu_id;                 /*# ＣＣＵ識別子 #*/
    BYTE        ccu_ver;                /*# ＣＣＵバージョン #*/
    BYTE        yiaddr;                 /*# 自ＩＰアドレス #*/
    BYTE        ipmask;                 /*# サブネットマスク #*/
    BYTE        giaddr;                 /*# ゲートウェイアドレス #*/
    BYTE        siaddr;                 /*# ＣＣＵＩＰアドレス #*/
    BYTE        siaddr2;                /*# ＣＣＵＩＰアドレス２ #*/
    BYTE        sip_inf[SLV_MAX];       /*# スレーブκＩＰ情報 #*/  //16byte
    BYTE        lu_number;          /*# LU番号 #*/      //2004.01
    BYTE        dipsw;                  /*# DIPSW情報 #*/   //IP-CS対応 2004.04
    BYTE        extension;              /*# 拡張情報 #*/    //IP-CS対応 2004.04
    BYTE        vlan_id_W;              /*# (WORD)ＶＬＡＮ－ＩＤ #*///SS9100対応 2004.08
//  BYTE        lu_number_W;            /*# (WORD)LU番号 #*///SS9100対応 2004.08
    BYTE        reserve;            /*#  lu_number_W廃止 #*///SS9100対応 2004.11.09
    BYTE        siaddr3;                /*# ＣＣＵＩＰアドレス3 #*///SS9100対応 2004.08
    BYTE        dummy[30];              /*# ダミー #*/
}COM_FLS_INFO;  //12+16+36=64byte

/*# FLASHファイル管理構造体 #*/
typedef struct {    //2000.09 V2.1追加
    DWORD       lu_size;                /*# ＴＦＴＰ受信ＬＵファイルサイズ #*/
    BYTE        lu_fn[256];             /*# ＬＵオンラインファイル名 #*/                //2001.01.17 V2.2追加
    BYTE        hold_fn[256];           /*# 保留音ファイル名 #*/                        //2001.01.17 V2.2追加
    BYTE        talky_fn[256];          /*# トーキーファイル名 #*/                      //2001.01.17 V2.2追加
}COM_FLS_FMNG;

/* FLASHダウンロードファイル管理構造体V3用 #*/
typedef struct {    //2002.10 V3追加
    BYTE        fpga_fn[256];           /*# FPGAファイル名 #*/
    BYTE        tone_fn[256];           /*# トーンROMファイル名 #*/
    BYTE        inter_fn[256];          /*# インタROMファイル名 #*/
    BYTE        fpga_ver[4];            /*# FPGAファイルバージョン情報 #*/
    BYTE        tone_ver[4];            /*# トーンROMファイルバージョン情報 #*/
    BYTE        inter_ver[4];           /*# インタROMファイルバージョン情報 #*/
    DWORD       fpga_sum;               /*# FPGAファイルSUM値 #*/
    DWORD       tone_sum;               /*# トーンROMファイルSUM値 #*/
    DWORD       inter_sum;              /*# インタROMファイルSUM値 #*/
    
}COM_FLS_V3FMNG;

/*# FLASH情報構造体 #*/
typedef struct {    //2000.09 V2.1追加
    COM_FLS_DATA    data;               /*# データ部 #*/
    COM_FLS_INFO    info;               /*# 情報部 #*/
    COM_FLS_FMNG    fmng;               /*# ファイル管理 #*/
    BYTE            comment[128];       /*# コメント #*/
    COM_FLS_V3FMNG  v3fmng;             /*# V3ファイル管理 #*/  //2002.10 V3追加
}COM_FLSDATA;

/*# FLASH情報状態構造体 #*/
typedef struct {    //2000.09 V2.1追加
    BYTE        data;                   /*# 管理データ状態 #*/
    BYTE        sys;                    /*# システムファイル状態（ＤＳＰプログラム含む） #*/
    BYTE        hory;                   /*# 保留音状態 #*/
//  BYTE        dmy;                    /*#  #*/
    BYTE        talky;                  /*# トーキー #*/
    BYTE        cadata;                 /*# CA切り替え情報状態：IP-CS(SS9100_PH1再開) 追加 #*/
}COM_FLSDATA_STS;

/*# TOS設定用ソケットディスクリプタ構造体 #*/
typedef struct {    //2001.01.16 V2.2追加
    DWORD       tftpsrv;                /*# ＴＦＴＰサーバ #*/
    DWORD       bootpsrv;               /*# ＢＯＯＴＰサーバ #*/
    DWORD       clkd_mlt;               /*# ＣＬＫＤマルチキャスト #*/
    DWORD       clkd_uni;               /*# ＣＬＫＤユニキャスト #*/
    DWORD       clkd_phs;               /*# ＣＬＫＤ ＰＨＳ #*/
    DWORD       doep;                   /*# ＤＯＥＰ #*/
    DWORD       crint_mlt;              /*# ＣＲＩＮＴマルチキャスト #*/
    DWORD       crint_uni;              /*# ＣＲＩＮＴユニキャスト #*/
    DWORD       lulog;                  /*# ＬＵログ #*/
}COM_TOS_SD;

/*# FLASH情報構造体：CA切り替え情報部 #*/
typedef struct {  // IP-CS(SS9100_PH1再開) 追加
    BYTE        serv_id;                /*# 接続先識別子 #*/
    BYTE        surv_chg_time;          /*# サバイバル切り替えタイマ #*/
    BYTE        surv_ret_time;          /*# サバイバル切り戻しタイマ #*/
    BYTE        connect_time;           /*# コネクション再接続タイマ #*/
    BYTE        reserve[12];            /*# 予備 #*/
}FLS_CA_DATA;
