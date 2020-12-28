/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  デバッグ用スタブのソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "def.h"
#include "str.h"
#include "tmr_def.h"
#include "com_mem.h"
#include "str_comdt.h"

struct mq_attr lu_mq_attr_v = {0, LUMQ_MAXMSG, LUMQ_MSGSIZE, 0, {0,0,0,0}};
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

COM_DATA		com_data;
uint8_t			pool_area[2][256];
uint8_t			msg_area[2][256];
/* パラメータ確認用変数 */
uint8_t			poolget_id;
uint8_t			poolput_id;
BYTE			*poolput_ptr;
uint8_t			sndmsg_ecb;
BYTE			*sndmsg_ptr;
uint8_t			rcvmsg_ecb;
WORD			rcvmsg_tim;

BYTE			threadstart_id;
void			*threadstart_ptr;
BYTE			print_set_id;
BYTE			print_set_lv;
BYTE			print_set_out;

BYTE *com_poolget(BYTE id) {poolget_id=id; return &pool_area[id][0];}
void com_poolput(BYTE id, BYTE *p) {poolput_id=id; poolput_ptr=p;}
void com_syserr(BYTE taskid,BYTE errcd) {}

void com_threadstart(BYTE id, void *p) {threadstart_id=id; threadstart_ptr=p;}
void dbg_print_set(BYTE id,BYTE lv,BYTE out) {print_set_id=id; print_set_lv=lv; print_set_out=out;}
void dbg_print(BYTE i,BYTE v, ...) {printf("ID=%d, ErrLV=%d\n", i,v);}

BYTE tmr_stop(BYTE id) {return OK;}
BYTE tmr_start(BYTE id, WORD cnt) {return OK;}
BYTE tmr_entry(WORD sndid, BYTE tmid, BYTE exeid) {return OK;}
BYTE tmr_delete(BYTE timno) {return OK;}

BYTE mnt_config_dat(char *path, BYTE write) {return OK;}
void com_ipsetaddr(DWORD subnet_msk, DWORD yiaddr, DWORD giaddr){}

//BYTE *com_rcvmsg(BYTE id, WORD tm) {rcvmsg_ecb=id; rcvmsg_tim=tm; return &msg_area[0][0];}
BYTE	*com_rcvmsg(BYTE ecbno,BYTE timcnt)
{
	BYTE	msg[LUMQ_MSGSIZE];
	unsigned int msg_prio=0;
	mqd_t mqdesc;
	struct timespec timeout;
	XS_POOL_INFO *pool_info;
	BYTE    *ret_msg;
	int cnt;
	int len;
	
	rcvmsg_ecb=ecbno; rcvmsg_tim=timcnt;

	if (ecbno >= COM_ECB_MAX) {
		dbg_print(COM_ID, LOG_ERR, "%s Illegal ECB No.[%d]\n", __func__, ecbno);
		/* ECB番号異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_08);
	}

	mqdesc = mq_open(ecb_attr[ecbno].name, O_RDWR);
	if (mqdesc == -1) {
		dbg_print(COM_ID, LOG_ERR, "%s mq_open NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
		/* mq_open異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_09);
	}
	
	/* 時限指定で分岐する */
	if (timcnt == 0) {
		/* 時限指定なし */
		len = mq_receive(mqdesc, (char *)msg, LUMQ_MSGSIZE, &msg_prio);
		if (len == -1) {
			dbg_print(COM_ID, LOG_ERR, "%s mq_receive NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
			/* mq_open異常終了時は、システムエラーを起動 */
			com_syserr(COM_ID, COM_ABORT_10);
		}
	}
	else {
		/* 時限指定あり */
		timeout.tv_sec = timcnt/200;
		timeout.tv_nsec = (timcnt%200)*5*1000;
		len = mq_timedreceive(mqdesc, (char *)msg, LUMQ_MSGSIZE, &msg_prio, &timeout);
		if (len == -1) {
			if (errno == ETIMEDOUT) {
				if (mq_close(mqdesc) == -1) {
					dbg_print(COM_ID, LOG_ERR, "%s mq_close #1 NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
					/* mq_close異常終了時は、システムエラーを起動 */
					com_syserr(COM_ID, COM_ABORT_06);
				}
				/* タイムアウト時 */
				return ((BYTE *)0xFFFFFFFF);
			}
			else {
				/* タイムアウト以外の異常修了 */
				dbg_print(COM_ID, LOG_ERR, "%s mq_receive NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
				/* mq_open異常終了時は、システムエラーを起動 */
				com_syserr(COM_ID, COM_ABORT_11);
			}
		}
	}
	if (mq_close(mqdesc) == -1) {
		dbg_print(COM_ID, LOG_ERR, "%s mq_close NG #2 ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
		/* mq_close異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_06);
	}
	pool_info = (XS_POOL_INFO *)msg;
	for (cnt=0;cnt<100;++cnt) {
		ret_msg = com_poolget(pool_info->Index);
		if (ret_msg != (BYTE *)0xFFFFFFF) {
			break;
		}
		usleep(10000);
	}
	if (cnt == 100) {
		dbg_print(COM_ID, LOG_ERR, "%s com_poolget pool_id=%d %s\n", __func__, pool_info->Index, strerror(errno));
		/* mq_open異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_11);
	}

	memcpy((void *)ret_msg, (void *)(msg + sizeof(XS_POOL_INFO)), len-sizeof(XS_POOL_INFO));
	return(ret_msg);
}

//void com_sndmsg(BYTE id, BYTE *p) {sndmsg_ecb=id; sndmsg_ptr=p;}
void	com_sndmsg(BYTE ecbno,BYTE *msg)
{
	unsigned int msg_prio=0;
	mqd_t mqdesc;
	XS_POOL_INFO *pool_info;

	sndmsg_ecb=ecbno; sndmsg_ptr=msg;

	if (ecbno >= COM_ECB_MAX) {
		dbg_print(COM_ID, LOG_ERR, "%s Illegal ECB No.[%d]\n", __func__, ecbno);
		/* ECB番号異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_05);
	}

	mqdesc = mq_open(ecb_attr[ecbno].name, O_RDWR);
	if (mqdesc == -1) {
		dbg_print(COM_ID, LOG_ERR, "%s mq_open NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
		/* mq_open異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_06);
	}
	
#ifdef LU_LOG
	log_trace((BYTE)ecbno,(BYTE*)msg);
#endif

	msg -= sizeof(XS_POOL_INFO);
	if (mq_send(mqdesc, (const char *)msg, LUMQ_MSGSIZE, msg_prio) == -1) {
		dbg_print(COM_ID, LOG_ERR, "%s mq_send NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
		/* mq_open異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_07);
	}
	
	if (mq_close(mqdesc) == -1) {
		dbg_print(COM_ID, LOG_ERR, "%s mq_close NG ECB_NO=%d %s\n", __func__, ecbno, strerror(errno));
		/* mq_close異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_06);
	}

	/* 入力メッセージを解放する */
	pool_info = (XS_POOL_INFO *)msg;
	com_poolput(pool_info->Index, (BYTE *)pool_info->AllocAddr);
}

//BYTE com_FpgaRegRead( off_t phy_addr, uint16_t len, uint8_t *buff_p ) {return OK;}
//BYTE com_FpgaRegWrite( off_t phy_addr, uint8_t val ) {return OK;}
//BYTE com_FpgaLED( uint8_t mask, uint8_t val, uint8_t *prev_p ) {return OK;}
//
//BYTE com_GpioRegRead( off_t phy_addr, uint32_t mask, uint32_t *buff_p ) {return OK;}
//BYTE com_GpioRegUpdate( off_t phy_addr, uint32_t mask, uint32_t bit ) {return OK;}

//BYTE com_SpiflashRead(DWORD addr, WORD size, WORD *p) {return OK;}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  デバッグ用スタブのソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
