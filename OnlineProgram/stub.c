/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  デバッグ用スタブのソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#include <stdint.h>
#include <unistd.h>

#include "def.h"
#define TEMP_HEADER
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

BYTE *com_poolget(BYTE id) {return &pool_area[id][0];}
void com_poolput(BYTE id, BYTE *p) {}
void com_sndmsg(BYTE id, BYTE *p) {}
BYTE *com_rcvmsg(BYTE id, WORD tm) {return &msg_area[id][0];}

void com_threadstart(BYTE id, void *p) {}
void dbg_print_set(BYTE i,BYTE v,BYTE z) {}
void dbg_print(BYTE i,BYTE v,char *p,...) {}

BYTE tmr_stop(BYTE id) {return OK;}
BYTE tmr_start(BYTE id, WORD cnt) {return OK;}
BYTE tmr_entry(WORD sndid, BYTE tmid, BYTE exeid) {return OK;}
BYTE tmr_delete(BYTE timno) {return OK;}

BYTE mnt_config_dat(char *path, BYTE write) {return OK;}

BYTE com_FpgaRegRead( off_t phy_addr, uint16_t len, uint8_t *buff_p ) {return OK;}
BYTE com_FpgaRegWrite( off_t phy_addr, uint8_t val ) {return OK;}
BYTE com_FpgaLED( uint8_t mask, uint8_t val, uint8_t *prev_p ) {return OK;}

BYTE com_GpioRegRead( off_t phy_addr, uint32_t mask, uint32_t *buff_p ) {return OK;}
BYTE com_GpioRegUpdate( off_t phy_addr, uint32_t mask, uint32_t bit ) {return OK;}

BYTE com_IPLVerGet( char *ver ) {return OK;}

int com_SpiflashRead(WORD addr, WORD size, WORD *p) {return OK;}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  デバッグ用スタブのソースファイル								  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/