/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/*# MODULE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : 保守ツールタスク                                         #*/
/*#                                                                         #*/
/*#     日本語名 : 保守ツールタスク                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 01/02/-- V2.1追加 ＳＡ                                   #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#                                                                         #*/
/*###########################################################################*/
#ifndef _MNT_DEF_H_
#define _MNT_DEF_H_

#define		ORD_DATA_READ		0xf0		/*# アドレス読み出し要求 #*/
#define		ORD_DATA_WRITE		0xf1		/*# アドレス書き込み要求 #*/
#define		ORD_FILE_WRITE		0xf2		/*# ファイル変更 #*/
#define		ORD_MAC_WRITE		0xf3		/*# ＭＡＣアドレス設定 #*/
#define		ORD_MKTR_DOWNLOAD	0xf4		/*# MKT-Rダウンロード要求 #*/	// 2006/09/20 追加 MKT-Rダウンロード

#define		EVT_DATA_READ		0xf0		/*# アドレス読み出し要求 #*/
#define		EVT_DATA_WRITE		0xf1		/*# アドレス書き込み要求 #*/
#define		EVT_FILE_WRITE		0xf2		/*# ファイル変更 #*/
#define		EVT_MAC_WRITE		0xf3		/*# ＭＡＣアドレス設定 #*/

#define		EVT_DATA_READ_BC	0xcc		/*# アドレス読み出し要求ＢＣ #*/
#define		EVT_DATA_WRITE_BC	0xcc		/*# アドレス書き込み要求ＢＣ #*/
#define		EVT_FILE_WRITE_BC	0x08		/*# ファイル変更ＢＣ #*/
#define		EVT_MAC_WRITE_BC	0x07		/*# ＭＡＣアドレス設定ＢＣ #*/

//#define		FILE_TYPE_ONLINE	0x00		/*##*/
#define		FILE_TYPE_HOLD		0x01		/*##*/
//#define		FILE_TYPE_TALKY		0x02		/*##*/
//#define		FILE_TYPE_IPL		0x03		/*##*/
//#define		FILE_TYPE_FPGA		0x04		/*##*/	//2005.03 NCLUCTL再利用
//#define		FILE_TYPE_TONE		0x05		/*##*/	//2005.03 NCLUCTL再利用
//#define		FILE_TYPE_INTER		0x06		/*##*/	//2005.03 NCLUCTL再利用
//#define		FILE_TYPE_TONEINTER	0x07		/*##*/	//2005.03 NCLUCTL追加
//#define		FILE_TYPE_FROM		0x08		/*##*/

//#define		WRITE_WORK_ADDR		0xc0350000
//#define		WRITE_WORK2_ADDR	0xc0360000	//2002.10 V3追加
//#define		WRITE_WORK32B_ADDR	0xc0a00000	//LU32B
//#define		IPL_SIZE			0x20000
//#define		IPL_LU32B_SIZE		0x40000		//LU32B
//#define		MAC_ADDR_ADDR		0x1fffc
//#define		MAC_ADDR_LU32B_ADDR	0x3fffc		//LU32B

/* ダウンロードファイルサイズ */		//2005.03 NCLUCTL再利用
//#define		FPGAFILESIZE		(64*1024)	
//#define		TONEFILESIZE		(32*1024)
//#define		INTERFILESIZE		(32*1024)

#define		DATA_WRITE			1
#define		NO_WRITE			0

#define		ELEMENT_MAX			22			/*# 設定ファイル内の要素数 #*/
#define		COMMAND_CHAR_MAX	12			/*# 設定ファイル要素キャラクタ数 #*/		

/* データ読み書きオーダ／イベント用構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;				/*# Ｂ．Ｃ #*/
	BYTE		code;			/*# オーダコード #*/
	BYTE		dmy1;			/*#  #*/
	BYTE		type;			/*#  #*/
	DWORD		lsi_ip[9];		/*#  #*/
	DWORD		subnet;			/*#  #*/
	DWORD		gateway;		/*#  #*/
	DWORD		ccu1_ip;		/*#  #*/
	DWORD		ccu2_ip;		/*#  #*/

	BYTE		vlan_ctl;		/*#  #*/
	BYTE		vlan_id;		/*#  #*/
	BYTE		pri_a;			/*#  #*/
	BYTE		pri_b;			/*#  #*/

	BYTE		pri_c;			/*#  #*/
	BYTE		pri_d;			/*#  #*/
	BYTE		ccu_ctl;		/*#  #*/
	BYTE		ccu_id;			/*#  #*/

	BYTE		comment[128];	/*#  #*/

	WORD		lu_number;		/*#  #*/
	BYTE		dipsw;			/*# IPCS #*/
	BYTE		extension;		/*# IPCS #*/

	BYTE		dmy2;			/*# SS9100 #*/
	BYTE		vlan_ctl_W;		/*# SS9100 #*/
	WORD		vlan_id_W;		/*# SS9100 #*/

	DWORD		ccu3_ip;		/*# SS9100 #*/
}DATA_RDWT;

/* ファイル書き込みオーダ用構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;				/*# Ｂ．Ｃ #*/
	BYTE		code;			/*# オーダコード #*/
	BYTE		dmy1;			/*#  #*/
	BYTE		type;			/*#  #*/
	BYTE		file_name[1];
}FILE_WRITE;

/* ＭＡＣアドレス読み書き用構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;				/*# Ｂ．Ｃ #*/
	BYTE		code;			/*# オーダコード #*/
	BYTE		dmy1;			/*#  #*/
	BYTE		mac4;			/*#  #*/
	BYTE		mac5;			/*#  #*/
	BYTE		mac6;			/*#  #*/
	BYTE		sum;			/*#  #*/
}MAC_RDWT;

/* ファイル変更結果報告用構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;				/*# Ｂ．Ｃ #*/
	BYTE		code;			/*# オーダコード #*/
	BYTE		dmy1;			/*#  #*/
	BYTE		type;			/*# ファイルタイプ #*/
	BYTE		ver10_;			/*# バージョン１０位 #*/
	BYTE		ver01_;			/*# バージョン１位 #*/
	BYTE		ver_10;			/*# バージョン０．１位 #*/
	BYTE		ver_01;			/*# バージョン０．０１位 #*/
}FILE_VERSION;

/* 結果報告用構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;				/*# Ｂ．Ｃ #*/
	BYTE		code;			/*# オーダコード #*/
	BYTE		dmy1;			/*#  #*/
}END_RESULT;


// 2006/09/20 追加 start MKT-Rダウンロード
/* MKT-Rダウンロード要求構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;						/*# Ｂ．Ｃ #*/
	BYTE		code;					/*# オーダコード #*/
	BYTE		dmy1;					/*#  #*/
	BYTE		pkg_num;				/*# 対象PKG番号 #*/
	BYTE		pkg_class;				/*# PKGクラス #*/
	BYTE		mktr_time;				/*# MKT-R立ち上がり時間 #*/
	BYTE		term_class;				/*# ターミナルクラス #*/
	BYTE		term_time;				/*# ターミナルクラス待ち時間 #*/
	WORD		data_snd_time;			/*# データ転送間隔 #*/
	BYTE		pg_res_time;			/*# PGダウンロード応答待ち時間 #*/
	BYTE		mktr_file_name[256];	/*# ファイル名称(フルパス) #*/
}MKTR_DOWNLOAD_ORDER;

/* MKT-Rダウンロード結果構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;						/*# Ｂ．Ｃ #*/
	BYTE		code;					/*# イベントコード #*/
	BYTE		dmy1;					/*#  #*/
	BYTE		pkg_no;					/*# PKG番号 #*/
	BYTE		ch_result[8];			/*# MKT-Rダウンロード結果(ch0～7) #*/
}MKTR_DOWNLOAD_RESULT;

/* MKT-Rダウンロード状況構造体 */
typedef	struct	{
	LU_HEADER	lu_header;
	BYTE		bc;						/*# Ｂ．Ｃ #*/
	BYTE		code;					/*# イベントコード #*/
	BYTE		dmy1;					/*#  #*/
	DWORD		total_cnt;				/*# KLCへのデータ転送ブロック書込み総回数 #*/
	DWORD		snd_cnt;				/*# KLCへのデータ転送ブロック書込み現回数 #*/
}MKTR_DOWNLOAD_STATE;
// 2006/09/20 追加 end MKT-Rダウンロード


extern	int		mnt_config_dat(char *path, BYTE write_f);
extern	void	mnt_config_txt();

#endif /* _MNT_DEF_H_ */

/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
