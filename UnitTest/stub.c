/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  デバッグ用スタブのソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#include <stdint.h>
#include <unistd.h>

#include "def.h"
#ifdef TEMP_HEADER
#include "temp_header.h"
#include "temp_tmr_def.h"
#else
#include "str.h"
#include "tmr_def.h"
#endif
#include "com_mem.h"
#include "str_comdt.h"


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
void com_sndmsg(BYTE id, BYTE *p) {sndmsg_ecb=id; sndmsg_ptr=p;}
BYTE *com_rcvmsg(BYTE id, WORD tm) {rcvmsg_ecb=id; rcvmsg_tim=tm; return &msg_area[0][0];}

void com_threadstart(BYTE id, void *p) {threadstart_id=id; threadstart_ptr=p;}
void dbg_print_set(BYTE id,BYTE lv,BYTE out) {print_set_id=id; print_set_lv=lv; print_set_out=out;}
void dbg_print(BYTE i,BYTE v,char *p,...) {printf("ID=%d, ErrLV=%d\n",i,v); printf(p,...);}

BYTE tmr_stop(BYTE id) {return OK;}
BYTE tmr_start(BYTE id, WORD cnt) {return OK;}
BYTE tmr_entry(WORD sndid, BYTE tmid, BYTE exeid) {return OK;}
BYTE tmr_delete(BYTE timno) {return OK;}

BYTE mnt_config_dat(char *path, BYTE write) {return OK;}

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
