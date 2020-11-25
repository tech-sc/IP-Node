/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/******************************************************************************/
/* 概要       共通関数のソースファイル                                        */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <sys/types.h>	/* open() */
#include <sys/stat.h>		/* open() */
#include <fcntl.h>			/* open() */
#include <unistd.h>		/* close() */
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>	/* mmap() */

/*** ユーザ作成ヘッダの取り込み ***/
#include "def.h"
#include "com_mem.h"

/*** 自ファイル内でのみ使用する#define マクロ ***/

#define MAP_SIZE		4096UL
#define MAP_MASK		(MAP_SIZE - 1)

#define	IPL_MTD			"/dev/mtd3"
#define IPL_VER			"IPL_VERSION"
#define IPL_VER_LEN		11
#define BUFF_SZ			1024

/*** 自ファイル内でのみ使用する#define 関数マクロ ***/
/*** 自ファイル内でのみ使用するtypedef 定義 ***/
typedef union {
	uint32_t	addr;
	uint8_t		*byte_p;
	uint16_t	*word_p;
	uint32_t	*dword_p;
} ACC_PTR_t;
/*** 自ファイル内でのみ使用するenum タグ定義 ***/
/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/

/*** ファイル内で共有するstatic 変数宣言 ***/
/* API排他フラグ */
static pthread_mutex_t  mem_mutex;

/*** static 関数宣言 ***/


/******************************************************************************/
/* 関数名	  メモリアクセス初期設定										  */
/* 機能概要	  メモリアクセスAPIのための初期設定								  */
/* パラメータ なし															  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_memInit( void )
{
	pthread_mutex_init(&mem_mutex, NULL);
	return OK;
}

/******************************************************************************/
/* 関数名	  メモリリード API												  */
/* 機能概要	  メモリアドレスからデータをリードする							  */
/* パラメータ phy_addr : (in)	メモリアドレス								  */
/*            width : (in)		デバイスのバス幅							  */
/*            len : (in)		読み出すデータ数							  */
/*            buff_p : (out)	読み出したデータを格納する領域				  */
/* リターン	  OK/NG															  */
/* 注意事項	  phy_addrは、有効アドレスチェックしないのでコール側任せである	  */
/*            lenは、mmap()の制限内のデータ数である事						  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_memRead( off_t phy_addr, uint16_t width, uint16_t len, void *buff_p )
{
	BYTE	result = NG;
	int		fd = 0;
	void	*map_base = NULL;
	ACC_PTR_t	virt_addr = {};
	ACC_PTR_t	data_addr = {.byte_p = buff_p};

	if ((width == BYTE_WIDTH) || (width == WORD_WIDTH) || (width == DWORD_WIDTH))
	{
		if ((width * len) <= MAP_SIZE)
		{
			pthread_mutex_lock(&mem_mutex);
			fd = open("/dev/mem", O_RDONLY | O_SYNC);
			if (fd != -1)
			{
				map_base = mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, phy_addr & ~MAP_MASK);
				if (map_base != MAP_FAILED)
				{
					virt_addr.byte_p = map_base + (phy_addr & MAP_MASK);
					while (len > 0)
					{
						switch (width)
						{
						  case BYTE_WIDTH:
							*data_addr.byte_p++ = *virt_addr.byte_p++;
							break;
						  case WORD_WIDTH:
							*data_addr.word_p++ = *virt_addr.word_p++;
							break;
						  case DWORD_WIDTH:
							*data_addr.dword_p++ = *virt_addr.dword_p++;
							break;
						  default:	//ありえない
							break;
						}
						len--;
					}
					munmap(map_base, MAP_SIZE);
					result = OK;
				}
				close(fd);
			}
			pthread_mutex_unlock(&mem_mutex);
		}
	}
	return result;
}

/******************************************************************************/
/* 関数名	  メモリライト API												  */
/* 機能概要	  メモリアドレスにデータをライトする							  */
/* パラメータ phy_addr : (in)	メモリアドレス								  */
/*            width : (in)		デバイスのバス幅							  */
/*            len : (in)		書き込むデータ数							  */
/*            buff_p : (in)		書き込むデータが格納された領域				  */
/* リターン	  OK/NG															  */
/* 注意事項	  phy_addrは、有効アドレスチェックしないのでコール側任せである	  */
/*            lenは、mmap()の制限内のデータ数である事						  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_memWrite( off_t phy_addr, uint16_t width, uint16_t len, void *buff_p )
{
	BYTE	result = NG;
	int		fd = 0;
	void	*map_base = NULL;
	ACC_PTR_t	virt_addr = {};
	ACC_PTR_t	data_addr = {.byte_p = buff_p};

	if ((width == BYTE_WIDTH) || (width == WORD_WIDTH) || (width == DWORD_WIDTH))
	{
		if ((width * len) <= MAP_SIZE)
		{
			pthread_mutex_lock(&mem_mutex);
			fd = open("/dev/mem", O_RDWR | O_SYNC);
			if (fd != -1)
			{
				map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr & ~MAP_MASK);
				if (map_base != MAP_FAILED)
				{
					virt_addr.byte_p = map_base + (phy_addr & MAP_MASK);
					while (len > 0)
					{
						switch (width)
						{
						  case BYTE_WIDTH:
							*virt_addr.byte_p++ = *data_addr.byte_p++;
							break;
						  case WORD_WIDTH:
							*virt_addr.word_p++ = *data_addr.word_p++;
							break;
						  case DWORD_WIDTH:
							*virt_addr.dword_p++ = *data_addr.dword_p++;
							break;
						  default:	//ありえない
							break;
						}
						len--;
					}
					munmap(map_base, MAP_SIZE);
					result = OK;
				}
				close(fd);
			}
			pthread_mutex_unlock(&mem_mutex);
		}
	}
	return result;
}

/******************************************************************************/
/* 関数名	  アドレスデータ更新 API										  */
/* 機能概要	  引数maskとvalでアドレスのデータ値をbit演算して更新する		  */
/* パラメータ phy_addr : (in)	アドレス(主にレジスタアドレス)				  */
/*            width : (in)		デバイスのバス幅							  */
/*            mask : (in)		設定(変更)したいbitに1セット(マスク値)		  */
/*            val : (in)		書き込むデータ(bit値)						  */
/*            prev_p : (out)	書き込む直前のデータ値が格納される			  */
/* リターン	  OK/NG															  */
/* 注意事項	  phy_addrは、有効アドレスチェックしないのでコール側任せである	  */
/*            lenは、mmap()の制限内のデータ数である事						  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_memUpdate( off_t phy_addr, uint16_t width, uint32_t mask, uint32_t val, void *prev_p )
{
	BYTE	result = NG;
	int		fd = 0;
	void	*map_base = NULL;
	ACC_PTR_t	virt_addr = {};
	ACC_PTR_t	prev_addr = {.byte_p = prev_p};

	if ((width == BYTE_WIDTH) || (width == WORD_WIDTH) || (width == DWORD_WIDTH))
	{
		pthread_mutex_lock(&mem_mutex);
		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (fd != -1)
		{
			map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr & ~MAP_MASK);
			if (map_base != MAP_FAILED)
			{
				virt_addr.byte_p = map_base + (phy_addr & MAP_MASK);
				switch (width)
				{
				  case BYTE_WIDTH:
					*prev_addr.byte_p = *virt_addr.byte_p;
					*virt_addr.byte_p = (*virt_addr.byte_p & (uint8_t)mask) | (uint8_t)val;
					break;
				  case WORD_WIDTH:
					*prev_addr.word_p = *virt_addr.word_p;
					*virt_addr.word_p = (*virt_addr.word_p & (uint16_t)mask) | (uint16_t)val;
					break;
				  case DWORD_WIDTH:
					*prev_addr.dword_p = *virt_addr.dword_p;
					*virt_addr.dword_p = (*virt_addr.dword_p & mask) | val;
					break;
				  default:	//ありえない
					break;
				}
				munmap(map_base, MAP_SIZE);
				result = OK;
			}
			close(fd);
		}
		pthread_mutex_unlock(&mem_mutex);
	}
	return result;
}

/******************************************************************************/
/* 関数名	  FPGAレジスタのリード API										  */
/* 機能概要	  FPGAレジスタからレジスタ値をリードする						  */
/* パラメータ phy_addr : (in)	レジスタアドレス							  */
/*            len : (in)		読み出すデータ数							  */
/*            buff_p : (out)	読み出したデータを格納する領域				  */
/* リターン	  OK/NG															  */
/* 注意事項	  FPGAレジスタオフセットアドレス0x0000～0x0024をリードすると	  */
/*            FPGA-SIOドライバに支障が発生する可能性がある					  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_FpgaRegRead( off_t phy_addr, uint16_t len, uint8_t *buff_p )
{
	return com_memRead( phy_addr, BYTE_WIDTH, len, buff_p );
}

/******************************************************************************/
/* 関数名	  FPGAレジスタのライト API										  */
/* 機能概要	  FPGAレジスタにレジスタ値をライトする							  */
/* パラメータ phy_addr : (in)	レジスタアドレス							  */
/*            len : (in)		書き込むデータ								  */
/* リターン	  OK/NG															  */
/* 注意事項	  FPGAレジスタオフセットアドレス0x0000～0x0024をライトすると	  */
/*            FPGA-SIOドライバに支障が発生する可能性がある					  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_FpgaRegWrite( off_t phy_addr, uint8_t val )
{
	return com_memWrite( phy_addr, BYTE_WIDTH, 1, &val );
}

/******************************************************************************/
/* 関数名	  FPGAのLED制御 API												  */
/* 機能概要	  FPGAのLEDレジスタにレジスタ値をライトする						  */
/* パラメータ mask : (in)		設定(変更)したいbitに1セット(マスク値)		  */
/*            val : (in)		書き込むデータ								  */
/*            prev_p : (out)	直前のレジスタ値を格納する領域				  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_FpgaLED( uint8_t mask, uint8_t val, uint8_t *prev_p )
{
	return com_memUpdate( FPGA_LED, BYTE_WIDTH, (uint32_t)mask, (uint32_t)val, prev_p );
}

/******************************************************************************/
/* 関数名	  GPIOレジスタのリード API										  */
/* 機能概要	  GPIOレジスタからレジスタ値をリードする						  */
/* パラメータ phy_addr : (in)	レジスタアドレス							  */
/*            mask : (in)		読み出す対象bitに1セット(マスク値)			  */
/*            buff_p : (out)	読み出したデータを格納する領域				  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_GpioRegRead( off_t phy_addr, uint32_t mask, uint32_t *buff_p )
{
	BYTE	result = NG;

	result = com_memRead( phy_addr, DWORD_WIDTH, 1, buff_p );
	*buff_p &= mask;
	return result;
}

/******************************************************************************/
/* 関数名	  GPIOレジスタのデータ更新 API									  */
/* 機能概要	  引数maskとvalでGPIOレジスタのデータを更新する					  */
/* パラメータ phy_addr : (in)	レジスタアドレス							  */
/*            mask : (in)		更新対象bitに1セット(マスク値)				  */
/*            bit : (in)		書き込むデータ(bit値)						  */
/* リターン	  OK/NG															  */
/* 注意事項	  －															  */
/* その他	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_GpioRegUpdate( off_t phy_addr, uint32_t mask, uint32_t bit )
{
	return com_memUpdate( phy_addr, DWORD_WIDTH, mask, bit, NULL );
}


/******************************************************************************/
/* 関数名	  BootLoaderプログラムバージョン情報取得						  */
/* 機能概要	  バージョン情報を文字列(例：1.00)＋null終端付きで返す			  */
/* パラメータ ver : (out)	取得したバージョン情報を格納する領域			  */
/*			  ver_sz : (in) バージョン情報を格納する領域のサイズ			  */
/* リターン	  取得したバージョン情報の文字数（null終端含むまない）			  */
/* 注意事項	  －															  */
/* その他	  －															  */
/******************************************************************************/
BYTE com_IPLVerGet( char *ver, BYTE ver_sz )
{
	BYTE	result	= 0;
	int		rd_sz	= 0;
	FILE	*fp		= NULL;
	char	*str	= NULL;
	char	buff[BUFF_SZ];

	if (ver_sz < IPL_VER_LEN)
	{
		return NG;
	}

	fp = fopen(IPL_MTD, "rb");
	while (fp != NULL)
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
		break;
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
BYTE *seq_search(BYTE *buff, size_t siz, const char *target, size_t len)
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
