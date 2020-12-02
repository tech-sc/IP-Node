/*######## ブート情報 ############################*/

typedef	struct {	//2000.09 V2.1変更（切り出し）
	DWORD   v_magic;					/*# マジッククッキー #*/
	BYTE    v_fields[60];				/*# ベンダ領域 #*/
} BOOTP_VEND;

typedef	struct	{	//2000.09 V2.1変更（切り出し）
	BYTE		btp_op;						/*# オペレーション #*/
	BYTE		btp_htype;					/*# ハードウェアアドレスタイプ #*/
	BYTE		btp_hlen;					/*# ハードウェアアドレス長 #*/
	BYTE		btp_hops;					/*# ホップ数 #*/
	DWORD		btp_xid;					/*# トランザクションＩＤ #*/
	WORD		btp_secs;					/*# 秒数 #*/
	WORD		btp_unused;					/*# 未使用 #*/
	DWORD		btp_ciaddr;					/*# クライアントＩＰアドレス #*/
	DWORD		btp_yiaddr;					/*# YourＩＰアドレス #*/
	DWORD		btp_siaddr;					/*# サーバ ＩＰアドレス #*/
	DWORD		btp_giaddr;					/*# ゲートウェイＩＰアドレス #*/
	BYTE		btp_chaddr[16];				/*# クライアントハードアドレス #*/
	BYTE		btp_sname[64];				/*# サーバ名 #*/
	BYTE		btp_file[128];				/*# 起動ファイル名 #*/
	BOOTP_VEND	btp_vend;					/*# ベンダ領域 #*/
} BOOTP_PKT;

typedef	struct	{
	BOOTP_PKT	btp_rep;						/*# ＢＯＯＴＰｒｅｐ情報 #*/
	DWORD		sys_flag;						/*# システム実行済みフラグ #*/
	DWORD		lu_size;						/*# ＴＦＴＰ受信ＬＵファイルサイズ #*/
	DWORD		siaddr;							/*# ＣＣＵ アドレス #*/
	DWORD		yiaddr;							/*# ＬＵアドレス #*/
	DWORD		ipmask;							/*# サブネットマスク #*/
	DWORD		giaddr;							/*# ゲートウエイ（ルータ） #*/
	BYTE		vlan_id;						/*# ＶＬＡＮ用ＩＤ下位８ビット #*/
	BYTE		class_a;						/*# ＶＬＡＮプライオリティＡ #*/
	BYTE		class_b;						/*# ＶＬＡＮプライオリティＢ #*/
	BYTE		class_c;						/*# ＶＬＡＮプライオリティＣ #*/
	BYTE		class_d;						/*# ＶＬＡＮプライオリティＤ #*/
	BYTE		lu_type;						/*# マスタ／スレーブ #*/
	BYTE		lu_number;						/*# ＬＵ番号 #*/
	BYTE		slv_number;						/*# ＬＳＩ番号 #*/
	BYTE		macadr[6];						/*# ＭＡＣアドレス #*/
	BYTE		pad[2];							/*# LONG境界の明示的にパディング定義 #*/	// 2000.07 V2.1 修正
	DWORD		garp_vlan_id;					/*# GARP受信VLAN ID #*/
	BYTE		garp_sw_addr[6];				/*# GARP送信元SWアドレス #*/
	WORD		ccu_ver;						/*# CCUバージョン #*/						// 2000.07 V2.1 追加
	DWORD		fiaddr;							/*# FILEサーバアドレス #*/					// 2000.07 V2.1 追加
	DWORD		ccu1_ip;						/*# ＣＣＵ１アドレス #*/					// 2001.08 V2.3 追加
	DWORD		ccu2_ip;						/*# ＣＣＵ２アドレス #*/					// 2001.08 V2.3 追加
	DWORD		hold_dl;						/*# 保留音ＤＬ済フラグ #*/					// 2001.08 V2.3 追加
	DWORD		tone_dl;						/*# 保留音ＤＬ済フラグ #*/					// 2001.08 V2.3 追加
	DWORD		onl_restart;					/*# オンライン後リスタートフラグ #*/		// 2001.08 V2.3 追加
	BYTE		*horyu_data_top;				/*# 保留音先頭アドレス保存 #*/				// 2001.08 V2.3 追加
	DWORD		horyu_chunk_size;				/*# 保留音実データサイズ保存 #*/			// 2001.08 V2.3 追加
	BYTE		*talky_data_top;				/*# トーキー先頭アドレス保存 #*/			// 2001.08 V2.3 追加
	DWORD		talky_chunk_size;				/*# 保留音実データサイズ保存 #*/			// 2001.08 V2.3 追加
	DWORD		restart_cnt;					/*# リスタート回数 #*/						// 2001.08 V2.3 追加
	BYTE		extension;					/*# 拡張フラグ #*/	//2004.01 変更
	BYTE		padpad;						/*# パディング #*/
	WORD		survtim;					/*# D01サバイバル対応  CCU切換タイマ値 #*/	//2004.02 LU32B 
//	BYTE		rsv[631];						/*# リザーブ #*/							// 2000.07 V2.1 変更 	// 2001.08 V2.3 変更
	BYTE		rsv[624];						/*# リザーブ #*/							//2004.02 LU32B 変更	
// 2004.01 変更
} LU_BOOT_DAT;

