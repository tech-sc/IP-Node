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


/******************************************************************************/
/* 関数名	  保留音の書き込み												  */
/* 機能概要	  WAVEファイルからチャンクヘッダを取り除き保留音ファイル		  */
/*			  として保存する												  */
/* パラメータ fl_name : (in)	パス付WAVEファイル名ル						  */
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
    uint32_t	data_size			= 0;
	bool		loop				= true;
	BYTE		result				= NG;
	WAVE_HDR_t	wave_header		= {};
	char		savefl[128]			= {};
	char		buff[512];

	fp = fopen(fl_name, "rb");
	if (fp == NULL)
	{
		return NG;
	}
	retv = fread(&wave_header, sizeof(wave_header), 1, fp);
	if (retv <= 0)
	{
		return NG;
	}
	if ((memcmp(wave_header.riff.chunk_id, "RIFF", 4) != 0) ||
		(memcmp(wave_header.fmt.chunk_id,  "fmt ", 4) != 0) ||
		(memcmp(wave_header.data.chunk_id, "data", 4) != 0))
	{
		return NG;
	}

	p = strrchr(fl_name, '/');
	strcpy(savefl, SAVE_FOLDER);
	strcat(savefl, p +1);
	w_fp = fopen(savefl, "wb");
	if (fp == NULL)
	{
		return NG;
	}

	while((feof(fp) == 0)&&(loop != false))
	{
		result = OK;
		wt_sz = fread(buff, 512, 1, fp);
		if (wt_sz < 0)
		{
			result = NG;
			break;
		}
		else if (wt_sz > 0)
		{
			if (data_size + wt_sz >= wave_header.data.chunk_size)
			{
				wt_sz = wave_header.data.chunk_size - data_size;
				loop = true;
			}

			retv = fwrite(buff, wt_sz, 1, fp);
			if (retv != wt_sz)
			{
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
		retv = fwrite(buff, 40, 1, fp);
		if (retv != 40)
		{
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
