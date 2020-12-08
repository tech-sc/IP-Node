/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/* IPCS_V4 <PD><ADD-STA> */
/******************************************************************************/
/* 概要       LUメインスレッドプログラム                                      */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdlib.h>

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#include "str.h"
#include "str_comdt.h"
#include "prot.h"
#include "str_boot.h"
#include "str_start.h"
#include "com_mem.h"
#include "lu_main.h"
#include "mnt_def.h"

/*** 自ファイル内でのみ使用する#define マクロ ***/

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/

/*** 自ファイル内でのみ使用するtypedef 定義 ***/

/*** 自ファイル内でのみ使用するenum タグ定義 ***/

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/

/*** ファイル内で共有するstatic 変数宣言 ***/

/*** static 関数宣言 ***/

/*** 変数定義 ***/
extern LU_BOOT_DAT	lu_boot_data;					/*# ＩＰＬブートデータ #*///2004.01 LU32B追加
extern XS_POOL_DAT pool_dat[COM_POOL_MAX];
extern XS_POOL_ATR pool_atr_tbl[COM_POOL_MAX];
extern XS_TASK_ATTR task_attr[COM_TASK_MAX_N];
extern XS_TASK_STATUS task_status[COM_TASK_MAX_N];
extern struct mq_attr lu_mq_attr_v;
extern XS_ECB_STTR ecb_attr[COM_ECB_MAX];
extern mqd_t ecb_mq_desc[COM_ECB_MAX];
extern XS_QUE_CTL com_que_ctl[COM_QUE_MAX];
extern XS_DBGPRINT_CTL dbgprint_ctl[COM_TASK_MAX_N];
extern XS_DBGPRINT_CTL com_dbgprint_ctl;
extern LU_START lu_start;
extern FLS_CA_DATA			fls_ca_data;				/*# CA?؂????? #*/
extern	COM_FLSDATA_STS	com_flsdata_sts;
extern	S_LUNUM_DAT lun_data;						/*# ＬＵ番号通知データ管理 #*/
extern	COM_DATA	com_data;						/*# 共通参照データ #*/
extern  COM_FLSDATA     com_flsdata;
extern char ipcs_lu_ver[8];

/*** 関数定義 ***/
/* For compile start */
void tmr_initial(void);                                         /*# タイマタスク初期設定 #*/
void doep_init(void);                                           /*# DOEPタスク初期設定 #*/
void dpisr_init(void);                                          /*# DPRAM制御初期設定 #*/
void dpisr_init(void);                                          /*# DPRAM制御初期設定 #*/
/* For compile end */
/******************************************************************************/
/* 関数名     LUメインスレッド                                                */
/* 機能概要   LUプロセスのメイン処理を行う。                                  */
/* パラメータ argc : (in)     引数の数(未使用)                                */
/*            arg  : (in)     引数のポインタ配列(未使用)                      */
/* リターン   int型(リターンすることはないので、Don’t Care)                  */
/******************************************************************************/

int main(int argc, char **argv)
{
	uint_8  prev_p;
	int		lu_main_mag_key;

	/* 各シグナルのハンドラを定義し、登録する */
	lu_sigaction();
	
	/* 起動し、フラッシュ共通参照データの初期化、共通データの初期化をおこない、さらに、NWインタフェースの初期化を行う */
	lu_data_init();
	
	/* メモリプールの初期化を行う */
	com_poolinit();
	
	/* キューの初期化を行う */
	com_queinit();
	
	/* スレッド初期化を行う */
	com_pthreadinit();
	
	/* com_memInit関数を起動する */
	com_memInit();
	
	/* モジュール個別初期化を実施する */
    tmr_initial( );                                         /*# タイマタスク初期設定 #*/
    doep_init( );                                           /*# DOEPタスク初期設定 #*/
    dpisr_init( );                                          /*# DPRAM制御初期設定 #*/
    dpisr_init( );                                          /*# DPRAM制御初期設定 #*/
	
	/* 立ち上がり時のスレッドの生成&起動を行う */
	lu_thread_initialstart();
	
	/* LED表示を変更する */
	if (com_data.operation_mode == LUMODE_NORMAL) {
		/* LED青点滅 （遅い点滅）[1sINT（500ms点灯/500ms消灯）] */
		com_FpgaLED(LED_120INT | LED_LU | LED_BLUE, ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, &prev_p);
	}
	else {
		/* LED＜赤⇔青＞点滅[2sINT（1s点灯/1s消灯）] */
		com_FpgaLED(LED_60INT | LED_LU | LED_BLUE | LED_RED, ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK, &prev_p);
	}
	
	/* プロセス外からの処理要求受付けメッセージキューを取得する */
	lu_main_mag_key = msgget(MSG_KEY_LUPROC, IPC_CREAT );
	if (lu_main_mag_key == -1) {
		dbg_print(MAIN_ID, LOG_ERR, "%s mdgget key=%d error %s\n", __func__, MSG_KEY_LUPROC, strerror(errno));
		com_syserr(MAIN_ID, MAIN_ABORT_05);
	}
	/* ループ処理 */
	while(1) {
		struct {
				  long mtype;
				  char mtext[64];
		} ipcmsg_buf;
		ipcmsg_buf.mtype = 0;
		
		/* プロセス外からの処理要求を受付け処理する */
		if (msgrcv(lu_main_mag_key, (void *)&ipcmsg_buf, sizeof(ipcmsg_buf.mtext), 0, 0) == -1) {
			dbg_print(MAIN_ID, LOG_ERR, "%s msgrcv id=%d error %s\n", __func__, strerror(errno));
			com_syserr(MAIN_ID, MAIN_ABORT_06);
		}
		dbg_print(MAIN_ID, LOG_DEBUG, "%s msg received msg=%p\n", __func__, ipcmsg_buf.mtext);
	}
}

static void print_siginfo(unsigned int log_level, siginfo_t *siginfo)
{
	dbg_print(MAIN_ID, log_level, "lu_proc signo=%d errno=%d code=%d\n", 
		siginfo->si_signo, siginfo->si_errno, siginfo->si_code);
	dbg_print(MAIN_ID, log_level, " pid=%d uid=%d status=%d utime=%d stime=%d\n", 
		siginfo->si_pid, siginfo->si_uid, siginfo->si_status, siginfo->si_utime, siginfo->si_stime);
	dbg_print(MAIN_ID, log_level, " value=%d int=%d ptr=%p addr=%p band=%d\n", 
		siginfo->si_value, siginfo->si_int, siginfo->si_ptr, siginfo->si_addr, siginfo->si_band);
	dbg_print(MAIN_ID, log_level, " fd=%d\n", 
		siginfo->si_fd);
}
static void do_reboot(void)
{
	/* システムエラーを起動 */
	com_syserr(MAIN_ID, MAIN_ABORT_01);
	while(1) {
		;
	}
}

static void signal_abort(int signal, siginfo_t *siginfo, void *data)
{
	print_siginfo(LOG_ERR, siginfo);
	do_reboot();
}

static void signal_warning(int signal, siginfo_t *siginfo, void *data)
{
	print_siginfo(LOG_WARNING, siginfo);
}

static void signal_dummy(int signal, siginfo_t *siginfo, void *data)
{
	print_siginfo(LOG_NOTICE, siginfo);
}
struct sigaction sa_abort = {
	.sa_sigaction = signal_abort,
	.sa_flags = SA_SIGINFO
};
struct sigaction sa_warning = {
	.sa_sigaction = signal_warning,
	.sa_flags = SA_SIGINFO
};
struct sigaction sa_dummy = {
	.sa_sigaction = signal_dummy,
	.sa_flags = SA_SIGINFO
};

/******************************************************************************/
/* 関数名     LUプロセスシグナルハンドラ登録                                  */
/* 機能概要   LUプロセスのシグナルのハンドラを定義し、登録する。              */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/******************************************************************************/
void lu_sigaction(void)
{
#if _DBGMODE ==0
	signal(SIGTRAP, SIG_IGN);
	signal(SIGABRT, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGUSR2, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGCONT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGVTALRM, SIG_IGN);
	signal(SIGPROF, SIG_IGN);
	signal(SIGWINCH, SIG_IGN);
	signal(SIGPOLL, SIG_IGN);
#else
	sigaction(SIGTRAP, &sa_dummy, NULL);
	sigaction(SIGABRT, &sa_dummy, NULL);
	sigaction(SIGUSR2, &sa_dummy, NULL);
	sigaction(SIGPIPE, &sa_dummy, NULL);
	sigaction(SIGALRM, &sa_dummy, NULL);
	sigaction(SIGTERM, &sa_dummy, NULL);
	sigaction(SIGCHLD, &sa_dummy, NULL);
	sigaction(SIGCONT, &sa_dummy, NULL);
	sigaction(SIGSTOP, &sa_dummy, NULL);
	sigaction(SIGTSTP, SIG_IGN);
	sigaction(SIGTTIN, &sa_dummy, NULL);
	sigaction(SIGTTOU, &sa_dummy, NULL);
	sigaction(SIGVTALRM, &sa_dummy, NULL);
	sigaction(SIGPROF, &sa_dummy, NULL);
	sigaction(SIGWINCH, &sa_dummy, NULL);
	sigaction(SIGPOLL, &sa_dummy, NULL);
#endif
	sigaction(SIGHUP, &sa_dummy, NULL);
	sigaction(SIGINT, &sa_dummy, NULL);
	sigaction(SIGQUIT, &sa_dummy, NULL);
	sigaction(SIGILL, &sa_abort, NULL);
	sigaction(SIGEMT, &sa_abort, NULL);
	sigaction(SIGBUS, &sa_abort, NULL);
	sigaction(SIGFPE, &sa_abort, NULL);
	sigaction(SIGKILL, &sa_abort, NULL);
	sigaction(SIGUSR1, &sa_dummy, NULL);
	sigaction(SIGSEGV, &sa_abort, NULL);
	sigaction(SIGURG, &sa_warning, NULL);
	sigaction(SIGXCPU, &sa_warning, NULL);
	sigaction(SIGXFSZ, &sa_warning, NULL);
	sigaction(SIGPWR, &sa_abort, NULL);
	sigaction(SIGSYS, &sa_abort, NULL);
}

/******************************************************************************/
/* 関数名     LUプロセスシグナルハンドラ登録                                  */
/* 機能概要   LUプロセスのシグナルのハンドラを定義し、登録する。              */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/******************************************************************************/
void lu_data_init(void)
{
	uint_8 prev_p;
	BYTE   macdata[6];
	int	   normal_end_flag;
	int	   return_code;
	FILE   *fpout;
	int    fd_w;
	BYTE   op_mode;
	char   cmd_buf[64];
	int  mask_val,loop_cnt;
	char ipaddr_buf[32];
	char mask_buf[4];
	
	/* 共通データエリア(com_data)を0初期化する */
	memset((void *)&com_data, 0, sizeof(com_data));
	/* 共通データのled_flg(com_data.led_flg)を「青点灯」に更新する */
	com_data.led_flg = LED_ON | LED_LU | LED_BLUE;
	/* com_flsdataを初期化する */
	if (mnt_config_dat("/flsdata/CONFIG.DAT", 1) == NG) {
		/* 実行結果NG時は、青色LED点滅を65ms消灯/20ms点灯に設定し、無限ループする */
		com_FpgaLED(LED_BLINK | LED_LU | LED_BLUE, ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK,&prev_p);
		while(1) {
			;
		}
	}
	/* com_flsdata_sts、fls_ca_data、lu_startを初期化する */
	lu_flsdata_init();
	/* BOOTPデータを作成する */
	lu_boot_make();
	/* BOOTPデータをセーブする */
	lu_boot_save();
	
	/* 共通データ(com_data)をデータ仕様_共通データの初期値に従って初期化する */
	com_data.lu_cabinet = LU_EX_LU; /* ＬＵキャビネットタイプ */
	com_data.dpram_num = 0x00000004; /* DPRAM実装数(未使用) */
	if (com_flsdata.data.vlan_id_W == 0) {
		com_data.tag_ctl = 0; /* TAG設定 */
	}
	else {
		com_data.tag_ctl = 1; /* TAG設定 */
	}
	com_data.lu_type = LU_MASTER; /* LUタイプ */
	com_data.lu_number = com_chgword(com_flsdata.data.lu_number);
    if( com_data.lu_number == 0x00 )                        /*# LU番号０ #*/
    {
        com_data.clkd_type = CLKD_MASTER;                   /*# クロックマスタセット #*/
    }
    else
    {
        com_data.clkd_type = CLKD_SLAVE;                    /*# クロックスレーブセット #*/
    }
	com_data.ins_pkgnum = 1; /* 実装PKG数(PORTスロット数) */
	com_data.ins_chnum = 3; /* 実装ch数 */
	com_data.country= LU_JAPAN; /* ＬＵ国種別＝日本 */
	/* MACアドレスをバイナリモードで読み出し、macdataに設定する */
	/* 読み出しフラグをOFFに設定する */
	normal_end_flag = OFF;
	/* デバイスオープン */
	fpout = fopen( "/dev/mtd4", "rb" );
	if (fpout != 0) {
		/* 正常時はデータを読み出す */
		if (fread( (void *)macdata, 1, sizeof(macdata), fpout) == sizeof(macdata)) {
			/* 読み出し成功時は、読み出しフラグをONに設定する */
			normal_end_flag = ON;
		}
		/* ファイルをクローズする */
		fclose( fpout );
	}
	/* ファイル読み出し異常時は、青色LED点滅を65ms消灯/20ms点灯に設定し、無限ループする */
	if (normal_end_flag == OFF) {
		com_FpgaLED(LED_BLINK | LED_LU | LED_BLUE, ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK,&prev_p);
		while(1) {
			;
		}
	}
    memcpy((char *)com_data.lu_network.macaddr, (char *)macdata, 6);
    in_usnet_mac_rev(com_data.lu_network.macaddr);                      /*  MAC ADDRESS bit-reverse */
    com_data.lu_network.yiaddr = com_chgdword(lu_boot_data.yiaddr);     /*# LU IPアドレス設定 #*/
    com_data.lu_network.siaddr = com_chgdword(lu_boot_data.siaddr);     /*# CCU IPアドレス設定 #*/
    com_data.lu_network.giaddr = com_chgdword(lu_boot_data.giaddr);     /*# ゲートウエイIPアドレス設定 #*/
    com_data.lu_network.fiaddr = com_chgdword(lu_boot_data.fiaddr);     /*# ファイルサーバIPアドレス設定 #*///2000.07 V2.1追加
    com_data.lu_network.subnet_msk = com_chgdword(lu_boot_data.ipmask); /*# サブネットマスク設定 #*/
    com_data.lu_network.ccu1_ip = lu_boot_data.ccu1_ip;                 /*# CCU1 IPアドレス設定 #*/ // 2001.08 V2.3 追加
    com_data.lu_network.tag_a=com_chgdword( 0x81000000+ ((lu_boot_data.class_a<<13)&0xe000) +
                                            (com_chgword(com_flsdata.data.vlan_id_W)&0x0fff) ); /*# Ａプライオリティタグ #*/
    com_data.lu_network.tag_b=com_chgdword( 0x81000000+ ((lu_boot_data.class_b<<13)&0xe000) +
                                            (com_chgword(com_flsdata.data.vlan_id_W)&0x0fff) ); /*# Ｂプライオリティタグ #*/
    com_data.lu_network.tag_c=com_chgdword( 0x81000000+ ((lu_boot_data.class_c<<13)&0xe000) +
                                            (com_chgword(com_flsdata.data.vlan_id_W)&0x0fff) ); /*# Ｃプライオリティタグ #*/
    com_data.lu_network.tag_d=com_chgdword( 0x81000000+ ((lu_boot_data.class_d<<13)&0xe000) +
                                            (com_chgword(com_flsdata.data.vlan_id_W)&0x0fff) ); /*# Ｄプライオリティタグ #*/
    com_data.lu_network.ccu2_ip = lu_boot_data.ccu2_ip;                 /*# CCU1 IPアドレス設定 #*/ // 2001.08 V2.3 追加
    com_data.lu_network.ccu3_ip = com_flsdata.data.siaddr3;             /*# CCU3 IPアドレス設定 #*/ // 2004.08 SS9100対応追加
	com_data.io_bz_ctrl = 0x13; /* ﾌﾞｻﾞｰｺﾝﾄﾛｰﾙﾚｼﾞｽﾀｾｰﾌﾞｴﾘｱ(V1)(未使用) */
	com_data.io_port0 = 0x0048; /* ポート０出力イメージ(未使用) */
	return_code = com_IPLVerGet(com_data.ipl_ver, sizeof(com_data.ipl_ver));
	dbg_print(MAIN_ID, LOG_DEBUG, " com_IPLVerGet ret %d Ver %s\n", return_code, com_data.ipl_ver);
	strncpy((char *)com_data.onl_ver, (const char *)ipcs_lu_ver, 8);
	dbg_print(MAIN_ID, LOG_DEBUG, " ipcs_lu_ver Ver %s\n", com_data.onl_ver);
	return_code = (int)com_FpgaRegRead( FPGA_HWVER3, 1, (BYTE *)&com_data.hard_ver_w[0]);
	return_code = (int)com_FpgaRegRead( FPGA_HWVER2, 1, (BYTE *)&com_data.hard_ver_w[1]);
	return_code = (int)com_FpgaRegRead( FPGA_HWVER1, 1, (BYTE *)&com_data.fpga_ver[0]);
	return_code = (int)com_FpgaRegRead( FPGA_HWVER0, 1, (BYTE *)&com_data.fpga_ver[1]);
	com_data.wdt_mode = 1; /* 0：起動/1：未起動 */
	
	/* 動作モードを求め、共通データに設定する */
	/* 動作モードを通常モードに初期化 */
	com_data.operation_mode = LUMODE_NORMAL;
	/* fls_ca_dataの保守モード起動フラグを読み出し、ONの場合は、共通データの動作モードを保守モードに設定する */
	if (fls_ca_data.mnt_reset == ON) {
		/* 動作モードを保守モードに設定 */
		com_data.operation_mode = LUMODE_MAINTE;
		dbg_print(MAIN_ID, LOG_DEBUG, " fls_ca_data.mnt_reset is ON \n");
		/* fls_ca_dataの保守モード起動フラグをOFFにし、flashにバックアップする */
		fls_ca_data.mnt_reset = OFF;
		fd_w = open("/flsdata/ca_data.dat", O_WRONLY | O_SYNC );
		if (fd_w != -1) {
			fpout = fdopen(fd_w, "wb");
			if (fpout == NULL) {
				dbg_print(MAIN_ID, LOG_ERR, "%s ca_data fopen error %s\n", __func__, strerror(errno));
				close(fd_w);
			}
			else {
				if (fwrite((const void *)&fls_ca_data, 1, (size_t)sizeof(fls_ca_data), fpout) < sizeof(fls_ca_data)) {
					dbg_print(COM_ID, LOG_ERR, "%s fls_ca_data fwrite error %s\n", __func__, strerror(errno));
				}
				fflush(fpout);
				fsync(fd_w);
				fclose(fpout);
				close(fd_w);
			}
		}
	}
	else  {
		/* FPGAドライバを介し、保守スイッチ検出ビットを読み出す */
		return_code = (int)com_FpgaRegRead( FPGA_HOSYU_SW, 1, &op_mode);
		/* 検出(1)の場合は、共通データの動作モードを保守モードに設定する */
		if (op_mode == 1) {
			/* 動作モードを保守モードに設定 */
			com_data.operation_mode = LUMODE_MAINTE;
			dbg_print(MAIN_ID, LOG_DEBUG, " FPGA_HOSYU_SW is ON \n");
		}
	}
	
	/* メイン実行回数(lu_start->main_cnt)を更新する */
	++lu_start.main_cnt;
	
	/* ネットワークインタフェースを初期化する */
	if (com_data.operation_mode == LUMODE_MAINTE) {
		/* 保守モード時、リンクアップのみ */
		sprintf(cmd_buf, "ip link set %s up", IPCS_NW_IF_NAME);
		system((const char *)cmd_buf);
	}
	else {
		/* 通常モード時、リンクアップダウンし、各種設定後UPする */
		/* リンクダウン */
		sprintf(cmd_buf, "ip link set %s down", IPCS_NW_IF_NAME);
		dbg_print(MAIN_ID, LOG_DEBUG, "!!!%s\n", cmd_buf);
		system((const char *)cmd_buf);
		/* IPアドレスを設定する */
		/* サブネットマスクビット数を求める */
		for(loop_cnt=0,mask_val=0;loop_cnt<32;++loop_cnt) {
			if (((com_data.lu_network.subnet_msk >> (31-loop_cnt)) & 0x00000001) == 0x00000001) {
				++mask_val;
			}
			else {
				break;
			}
		}
		sprintf((char *)mask_buf, "/%d", mask_val);
		/* IPアドレスを文字列に変換する */
		if (inet_ntop(AF_INET, (const void *)&com_data.lu_network.yiaddr, (char *)ipaddr_buf, sizeof(ipaddr_buf)) == NULL) {
			dbg_print(MAIN_ID, LOG_ERR, "%s inet_ntop error#1 ip=%08x\n", __func__, com_data.lu_network.yiaddr);
			com_syserr(MAIN_ID, MAIN_ABORT_02);
		}
		if (strncat((char *)ipaddr_buf, (const char *)mask_buf, sizeof(ipaddr_buf)) == NULL) {
			dbg_print(MAIN_ID, LOG_ERR, "%s strncat error %s %s\n", __func__, ipaddr_buf, mask_buf);
			com_syserr(MAIN_ID, MAIN_ABORT_03);
		}
		sprintf(cmd_buf, "ip addr add %s dev %s", ipaddr_buf, IPCS_NW_IF_NAME);
		/* IPアドレスを設定する */
		dbg_print(MAIN_ID, LOG_DEBUG, "!!!%s\n", cmd_buf);
		system((const char *)cmd_buf);
		/* ゲートウェイIPアドレスを文字列に変換する */
		if (inet_ntop(AF_INET, (const void *)&com_data.lu_network.giaddr, (char *)ipaddr_buf, sizeof(ipaddr_buf)) == NULL) {
			dbg_print(MAIN_ID, LOG_ERR, "%s inet_ntop#2 error ip=%08x\n", __func__, com_data.lu_network.giaddr);
			com_syserr(MAIN_ID, MAIN_ABORT_04);
		}
		/* デフォルトゲートウェイを設定する */
		sprintf(cmd_buf, "ip route add default via %s", ipaddr_buf);
		dbg_print(MAIN_ID, LOG_DEBUG, "!!!%s\n", cmd_buf);
		system((const char *)cmd_buf);
		/* リンクアップ */
		sprintf(cmd_buf, "ip link set %s up", IPCS_NW_IF_NAME);
		dbg_print(MAIN_ID, LOG_DEBUG, "!!!%s\n", cmd_buf);
		system((const char *)cmd_buf);
		/* VLAN有効時は、VAN-IFを作成する */
		if (com_data.tag_ctl == 1) {
			sprintf(cmd_buf, "ip link add kink %s name %s.%d type vlan id %d", IPCS_NW_IF_NAME, IPCS_NW_IF_NAME, (int)com_flsdata.data.vlan_id_W, (int)com_flsdata.data.vlan_id_W);
			dbg_print(MAIN_ID, LOG_DEBUG, "!!!%s\n", cmd_buf);
			system((const char *)cmd_buf);
		}
	}
	/* デバッグプリント管理テーブルを初期化する */
	for (loop_cnt=0;loop_cnt<COM_TASK_MAX_N;++loop_cnt) {
		dbgprint_ctl[loop_cnt].log_level = LOG_ERR;
		dbgprint_ctl[loop_cnt].output_dst = LOGDST_SYSLOG;
	}
	com_dbgprint_ctl.log_level = LOG_ERR;
	com_dbgprint_ctl.output_dst = LOGDST_SYSLOG;
}

/******************************************************************************/
/* 関数名     LUフラッシュデータ初期化                                        */
/* 機能概要   flashバックアップデータ(com_flsdata_sts、fls_ca_data、lu_start) */
/*            を読み出し、設定する。                                          */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/******************************************************************************/
void lu_flsdata_init(void)
{
	FILE				*fpout;
	int					normal_end_flag;

	/* /flsdata/flsdata_sts.datをバイナリモードで読み出し、com_flsdata_stsに設定する */
	/* 読み出しフラグをOFFに設定する */
	normal_end_flag = OFF;
	/* デバイスオープン */
	fpout = fopen( "/flsdata/flsdata_sts.dat", "rb" );
	if (fpout != 0) {
		/* 正常時はデータを読み出す */
		if (fread( (void *)&com_flsdata_sts, 1, sizeof(com_flsdata_sts), fpout) == sizeof(com_flsdata_sts)) {
			/* 読み出し成功時は、読み出しフラグをONに設定する */
			normal_end_flag = ON;
		}
		/* ファイルをクローズする */
		fclose( fpout );
	}
	/* ファイル読み出し異常時は、データ仕様書_共通データ編に指定されている初期値に設定する */
	if (normal_end_flag == OFF) {
		memset((void *)&com_flsdata_sts, 0, sizeof(com_flsdata_sts));
	}
	
	/* /flsdata/ca_data.datをバイナリモードで読み出し、fls_ca_dataに設定する */
	/* 読み出しフラグをOFFに設定する */
	normal_end_flag = OFF;
	/* デバイスオープン */
	fpout = fopen( "/flsdata/ca_data.dat", "rb" );
	if (fpout != 0) {
		/* 正常時はデータを読み出す */
		if (fread( (void *)&fls_ca_data, 1, sizeof(fls_ca_data), fpout) == sizeof(fls_ca_data)) {
			/* 読み出し成功時は、読み出しフラグをONに設定する */
			normal_end_flag = ON;
		}
		/* ファイルをクローズする */
		fclose( fpout );
	}
	/* ファイル読み出し異常時は、データ仕様書_共通データ編に指定されている初期値に設定する */
	if (normal_end_flag == OFF) {
		memset((void *)&fls_ca_data, 0, sizeof(fls_ca_data));
		fls_ca_data.surv_chg_time = 2;
	}

	/* /flsdata/lu_start.datをバイナリモードで読み出し、lu_startに設定する */
	/* 読み出しフラグをOFFに設定する */
	normal_end_flag = OFF;
	/* デバイスオープン */
	fpout = fopen( "/flsdata/lu_start.dat", "rb" );
	if (fpout != 0) {
		/* 正常時はデータを読み出す */
		if (fread( (void *)&lu_start, 1, sizeof(lu_start), fpout) == sizeof(lu_start)) {
			/* 読み出し成功時は、読み出しフラグをONに設定する */
			normal_end_flag = ON;
		}
		/* ファイルをクローズする */
		fclose( fpout );
	}
	/* ファイル読み出し異常時は、データ仕様書_共通データ編に指定されている初期値に設定する */
	if (normal_end_flag == OFF) {
		memset((void *)&lu_start, 0, sizeof(lu_start));
	}
}

/******************************************************************************/
/* 関数名     スレッド初期化処理                                              */
/* 機能概要   動作モードにあったスレッドを立ち上げる。                        */
/*            を読み出し、設定する。                                          */
/* パラメータ -                                                               */
/* リターン   -                                                               */
/******************************************************************************/
void lu_thread_initialstart(void)
{
	int	loop_cnt;
	/* 登録スレッド数分ループする */
	for (loop_cnt=0;loop_cnt<COM_TASK_MAX_N;++loop_cnt) {
		/* 初期生成の場合のみ生成する */
		if (task_attr[loop_cnt].create_flg == ON) {
			if ((com_data.operation_mode == LUMODE_NORMAL) &&
			((task_attr[loop_cnt].op_mode & 0x01) == 0x01)) {
				/* 通常モード時のスレッドを生成する */
				com_threadstart(loop_cnt, &com_data.operation_mode);
			}
			else if ((task_attr[loop_cnt].op_mode & 0x01) == 0x02) {
				/* 保守モード時のスレッドを生成する */
				com_threadstart(loop_cnt, &com_data.operation_mode);
			}
		}
	}
}
/* IPCS_V4 <PD><ADD-END> */

