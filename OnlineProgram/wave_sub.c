/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  WAVE・保留音サブルーチンのソースファイル						  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>	/* stat() */
#include <sys/stat.h>		/* stat() */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#include "str_comdt.h"
#include "wave_sub.h"

#define SAVE_FOLDER		"/flsdata/"
extern	COM_DATA		com_data;

/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/* RIFFチャンクの定義 */
typedef struct RIFF_CHUNK_t {
    uint8_t		chunk_id[4];
    uint32_t	chunk_size;
    uint8_t		format[4];
} RIFF_CHUNK_t;

/* fmtチャンクの定義 */
typedef struct FMT_CHUNK_t {
    uint8_t		chunk_id[4];
    uint32_t	chunk_size;
    uint16_t	audio_format;
    uint16_t	channel_no;
    uint32_t	sample_rate;
    uint32_t	byte_rate;
    uint16_t	block_align;
    uint16_t	bits_per_sample;
} FMT_CHUNK_t;

/* dataチャンクの定義 */
typedef struct DATA_CHUNK_t {
    uint8_t		chunk_id[4];
    uint32_t	chunk_size;
	//これ以降がデータ
} DATA_CHUNK_t;

/** WAVEヘッダフォーマット */
typedef struct WAVE_HDR_t {
    RIFF_CHUNK_t	riff;
    FMT_CHUNK_t		fmt;
    DATA_CHUNK_t	data;
} WAVE_HDR_t;


/*** ファイル内で共有するstatic 変数宣言 ***/
/* errno文字列生成バッファ */
static char				errno_str[16];

/******************************************************************************/
/* 関数名	  errno文字列変換												  */
/* 機能概要	  引数のerrnoを文字化する										  */
/* パラメータ err : (in)	変換するerrno									  */
/* リターン	  変換した文字列												  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
#define CASE_STR(a)	case a:	 return #a
static const char *mkstr_errno(int err)
{
	switch(err)
	{
	  CASE_STR(EACCES);
	  CASE_STR(EDQUOT);
	  CASE_STR(EEXIST);
	  CASE_STR(ENAMETOOLONG);
	  CASE_STR(ENOENT);
	  CASE_STR(ENOSPC);
	  CASE_STR(ENOTDIR);
	  CASE_STR(EROFS);
	  CASE_STR(EFAULT);
	  CASE_STR(EINTR);
	  CASE_STR(EINVAL);
	  CASE_STR(ENFILE);
	  CASE_STR(ENOMEM);
	  default:
		sprintf(errno_str, "misc(%d)",err);
		return errno_str;
	}
}


/******************************************************************************/
/* 関数名	  保留音の書き込み												  */
/* 機能概要	  WAVEファイルからチャンクヘッダを取り除き保留音ファイル		  */
/*			  として保存する												  */
/* パラメータ fl_name : (in)	パス付WAVEファイル名						  */
/* リターン	  結果															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE wave_file_write( const char *fl_name )
{
	FILE		*fp					= NULL;
	FILE		*w_fp				= NULL;
	char		*p					= NULL;
	size_t		retv				= 0;
	size_t		wt_sz				= 0;
	BYTE		result				= NG;
	WAVE_HDR_t	wave_header		= {};
	char		savefl[128]			= {};
	char		buff[512];

	fp = fopen(fl_name, "rb");
	if (fp == NULL)
	{
		dbg_print(COM_ID, LOG_ERR, "wave_file_write fopen(rb) Error:%s", mkstr_errno(errno));
		return NG;
	}
	retv = fread(&wave_header, 1, sizeof(wave_header), fp);
	if (retv <= 0)
	{
		if (retv < 0)
		{
			dbg_print(COM_ID, LOG_ERR, "wave_file_write fread() Error:%s", mkstr_errno(errno));
		}
		return NG;
	}

	if (memcmp(wave_header.riff.chunk_id, "RIFF", 4) != 0)
	{
		dbg_print(COM_ID, LOG_ERR, "wave_file_write RIFF chunk Error" );
		return NG;
	}
	if (memcmp(wave_header.fmt.chunk_id,  "fmt ", 4) != 0)
	{
		dbg_print(COM_ID, LOG_ERR, "wave_file_write fmt chunk Error" );
		return NG;
	}
	if (memcmp(wave_header.data.chunk_id, "data", 4) != 0)
	{
		dbg_print(COM_ID, LOG_ERR, "wave_file_write data chunk Error" );
		return NG;
	}

	p = strrchr(fl_name, '/');
	strcpy(savefl, SAVE_FOLDER);
	strcat(savefl, p +1);
	w_fp = fopen(savefl, "wb");
	if (w_fp == NULL)
	{
		dbg_print(COM_ID, LOG_ERR, "wave_file_write fopen(wb) Error:%s", mkstr_errno(errno));
		return NG;
	}

	while(feof(fp) == 0)
	{
		result = OK;
		wt_sz = fread(buff, 1, 512, fp);
		if (wt_sz < 0)
		{
			dbg_print(COM_ID, LOG_ERR, "wave_file_write fread(fp) Error:%s", mkstr_errno(errno));
			result = NG;
			break;
		}
		else if (wt_sz > 0)
		{
			retv = fwrite(buff, 1, wt_sz, w_fp);
			if (retv != wt_sz)
			{
				dbg_print(COM_ID, LOG_ERR, "wave_file_write fwrite() Error:%s", mkstr_errno(errno));
				result = NG;
				break;
			}
		}
	}

	/* ファイルの最後に無音をセットする */
	if (result == OK)
	{
		if(com_data.silent_ptn == VO_SLTPTN_U)
		{
			memset(buff, (BYTE)VO_SLT_U, 40);	/*# 無音がμ－ｌａｗの場合 #*/
		}
		else
		{
			memset(buff, (BYTE)VO_SLT_A, 40);	/*# 無音がＡ－ｌａｗの場合 #*/
		}
		retv = fwrite(buff, 1, 40, w_fp);
		if (retv != 40)
		{
			dbg_print(COM_ID, LOG_ERR, "wave_file_write fwrite(40) Error:%s", mkstr_errno(errno));
			result = NG;
		}
	}
	fclose(w_fp);
	fclose(fp);
	return result;
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要		  WAVE・保留音サブルーチンのソースファイル						  */
/*								Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
