/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       共通関数のソースファイル                                        */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
#include <stdio.h>

#include "def.h"
#include "com_mem.h"

#ifdef DEBUG
#define _ATTR_SYM
#else  // !DEBUG
#define _ATTR_SYM		static
#endif // DEBUG

#define	IPL_MTD			"/dev/mtd3"
#define IPL_VER			"IPL_VERSION"
#define IPL_VER_LEN		11
#define BUFF_SZ			1024

/******************************************************************************/
/* 関数名	  BootLoaderプログラムバージョン情報取得						  */
/* 機能概要	  バージョン情報を文字列(例：1.00)＋null終端付きで返す			  */
/* パラメータ ver : (out)	取得したバージョン情報を格納する領域			  */
/* リターン	  取得したバージョン情報の文字数（null終端含むまない）			  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_IPLVerGet( char *ver )
{
	BYTE	result	= 0;
	int		rd_sz	= 0;
	FILE	*fp		= NULL;
	char	*str	= NULL;
	char	buff[BUFF_SZ];

	fp = fopen(IPL_MTD, "rb");
	if (fp != NULL)
	{
		rd_sz = fread(buff, sizeof(char), BUFF_SZ, fp);
		if (rd_sz < 0)
		{
			break;
		}
		str = (char*)seq_search((BYTE*)buff, BUFF_SZ, IPL_VER, IPL_VER_LEN);
		if (str == NULL)
		{
			break;
		}
		while ((*str == ' ') && (str < &buff[BUFF_SZ]))
		{
			str++;
		}
		/* バージョン番号を返す */
		while ((*str != ' ') && (*str != '\n') && (*str != '\r') && (str < &buff[BUFF_SZ]))
		{
			*ver++ = *str++;
			result++;
		}
		*ver = '\0';
		fclose(fp);
	}
	return result;
}

/******************************************************************************/
/* 関数名	  シーケンシャル検索アルゴリズム								  */
/* 機能概要	  buff内からtargetに合致する部分検索を行う						  */
/* パラメータ buff   : (in)	検索領域のバッファ								  */
/*			  siz    : (in)	検索領域のバイト数								  */
/*			  target : (in)	検索するデータのポインタ						  */
/*			  len    : (in)	検索するデータのバイト長						  */
/* リターン	  targetに一致した検索領域内のポインタ							  */
/*			  NULLの場合、一致箇所なし										  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
// com_mem.cへ移動する
_ATTR_SYM BYTE *seq_search(BYTE *buff, size_t siz, const char *target, size_t len)
{
	size_t		i    = 0;
	size_t		same = 0;
	const BYTE		*p   = NULL;

	while (i < siz)
	{
		p = (const BYTE*)target;
		while ((i < siz)&&(*buff != *p))
		{
			i++;
			buff++;
		}
		if (i >= siz)
		{
			return NULL;
		}
		/* 1文字目一致 */
		i++;
		buff++;
		p++;
		same = 1;
		while ((i < siz)&&(*buff == *p)&&(same < len))
		{
			i++;
			buff++;
			p++;
			same++;
		}
		if (same == len)
		{
			return buff - len;
		}
	}
	return NULL;
}

/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       共通関数のソースファイル                                        */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/
