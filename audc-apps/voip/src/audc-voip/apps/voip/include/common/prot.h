/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : LU                                                       #*/
/*#                                                                         #*/
/*#     日本語名 : LU共通プロトタイプ定義ファイル                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/28 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
//#include "../../mik/mik_armc.h"/* IPCS_V4 <PD><DEL> */

/*# 共通関数 #*/
void	com_syserr(BYTE,BYTE);
void	com_poolinit(void);
void	com_strcopy(BYTE*,BYTE*,WORD);
void	com_wordcopy(WORD*,WORD*,WORD,BYTE);
void	com_dwordcopy(DWORD * , DWORD * , WORD ,BYTE);
void	com_queinit(void);
void	lu_boot_save_olposs(void);	//2000.07 V2.1追加

void	ZWDTCLR(void);
void	lu_boot_save(void);
void	lu_boot_make(void);
BYTE	lu_D01(void);//2004.01 LU32B追加
BYTE	lu_SS9100(void);//2004.08 SS9100追加

BYTE	*com_poolget(BYTE);
BYTE	*com_ipoolget(BYTE);
void	com_poolput(BYTE,BYTE*);
void	com_ipoolput(BYTE,BYTE*);
void	com_get_pool_status(BYTE poolid, WORD *PoolNum, WORD *get_count);
void	com_iptdis(void);
void	com_iptena(BYTE);
void	com_sndmsg(BYTE,BYTE *);
void	com_isndmsg(BYTE,BYTE *);
BYTE	*com_rcvmsg(BYTE,BYTE);
void	com_queinit(void);
DWORD	com_queput(WORD,BYTE*,BYTE);
BYTE	*com_queget(WORD,BYTE);
DWORD	com_quesns(WORD);
DWORD	com_iqueput(WORD,BYTE*,BYTE);
BYTE	*com_iqueget(WORD,BYTE);
DWORD	com_iquesns(WORD);
//WORD	com_chgword(WORD);/* IPCS_V4 <PD><DEL> */
//DWORD	com_chgdword(DWORD);/* IPCS_V4 <PD><DEL> */
void	com_pthreadinit(void);
void	com_threadstart(BYTE thread_id, void *arg);
void	com_threadstop(BYTE thread_id);
void	dbg_print_set(BYTE thread_id, DWORD log_level, DWORD output_dst);
void   dbg_print(BYTE thread_id, int log_level, const char *format, ...);

/*# タイマタスク #*/
BYTE	tmr_entry(WORD,BYTE,BYTE);
BYTE	tmr_delete(BYTE);				//2000.07 V2.1追加
BYTE	tmr_start(BYTE,WORD);
BYTE	tmr_stop(BYTE);
BYTE	tmr_sense(BYTE,WORD*,BYTE*);
void	tmr_initial(void);
void in_usnet_mac_rev(BYTE	*macadr);
#if 0 /* IPCS_V4 <PD><DEL-STA> */
void	tmr(void);						/*# タイマタスクメイン #*/

/*# ＬＵ管理タスク #*/
void	lu_manage( void );				/*# Ｌｕ管理メイン #*/

/*# DOEPタスク #*/
void	doep(void);						/*# DOEPタスクメイン #*/
void	doep_init(void);

/*# DPRAM制御 #*/
void	dpisr_main( void );				/*# DPRAM制御メイン #*/
void	dpisr_init( void );

/*# CRINT制御 #*/
void	crintctl( void );				/*# CRINT制御メイン #*/
void	crintctl_init( void );

/*# CLKD #*/
void	clkd( void );					/*# CLKDメイン #*/
void	clkd_init( void );

/*# ドアホンタスク #*/
void	door_init( void );
void	door_main( void );				/*# ドアホンタスクメイン #*/

/*# サポートタスク #*/
void	sup( void );
void	sup_init( void );

/*# Ｈ３２３制御 #*/
void	h323( void );					/*# Ｈ３２３制御 #*/
void	h323_init( void );

/*# ＴＦＴＰ制御 #*/
void	tftp( void );					/*# ＴＦＴＰ制御 #*/
void	tftp_init( void );

/*# TFTPサーバ #*/
void	tftpsrv_init( void );
void	tftpsrv( void );

/*# ＬＵログ #*/
void	lu_log(void);
void	lulog_init(void);

/*# アクセプト管理１ #*/
void	fcl_acc1( void );	/* タスクエントリー*/
void    fcl_acc1_init( void ); /* 初期化関数 */

/*# コネクト管理１ #*/
void	fcl_con1( void );	/* タスクエントリー*/
void    fcl_con1_init( void ); /* 初期化関数 */
#endif /* IPCS_V4 <PD><DEL-END> */
