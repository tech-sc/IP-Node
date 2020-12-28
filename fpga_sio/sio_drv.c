/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */

/******************************************************************************/
/* 概要       CS部との通信(SIO)用ドライバ                                     */
/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
/******************************************************************************/

/*** システムヘッダの取り込み ***/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/sysfs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/interrupt.h>			/* IPCS_V4 PG 0007243 ADD */

/*** ユーザ作成ヘッダの取り込み ***/
#include "sio_drv.h"
#include "sio.h"


/*** 自ファイル内でのみ使用する#define マクロ ***/
#define DEV_MAJOR_DEFAULT	0			/** major number(auto numbering) */
#define DEV_MINOR_NO		0			/** minor number */
#define DEV_NAME			"/dev"		/** device name */
#define DEV_NUM				1			/** number of devices */
#define EXTINT1				2			/* SIOのIRQ番号 *//* IPCS_V4 PG 0007243 ADD */


/*** 自ファイル内でのみ使用するtypedef 定義 ***/
/** driver infomation */
typedef struct drv_info {
	int				status;				/** driver status */
	int				majorNo;			/** major number */
	struct class	*pClass;			/** driver class */
	NOTICE_REQ		notice;				/** notice request info */
} DRV_INFO;

/** device descriptor */
typedef struct dev_desc {
	// 制御に必要な情報はディスクリプタに持たせる
	int					value;
	// 排他制御子、cdevは基本情報
	struct semaphore	semDev; 		/** access lock */
	struct cdev			cdev;			/** character device's structure */
} DEV_DESC;


/*** static 関数宣言 ***/
static DRV_INFO		g_DrvInfo;
static DEV_DESC		g_DevDesc[DEV_NUM];

static int	SioDrv_probe(void);
static int SioDrv_open( struct inode *inode, struct file *p_file );
static int SioDrv_close( struct inode *inode, struct file *p_file );
static int SioDrv_read( struct file *file, char *buff ,size_t cnt, loff_t *pos);
static int SioDrv_R_chk(void);
static int SioDrv_write( struct file *file, const char *buff ,size_t cnt, loff_t *pos);
static long SioDrv_ioctl( struct file *file, unsigned int cmd , unsigned long arg);
void SioDrv_remove(void);

/*** 自ファイル内でのみ使用するstruct/union タグ定義 ***/
/** file operation */
struct file_operations g_FileOps = {
	owner:			THIS_MODULE,
	read:			SioDrv_read,
	write:			SioDrv_write,
	unlocked_ioctl:	SioDrv_ioctl,
	open:			SioDrv_open,
	release:		SioDrv_close
};

/*** 外部関数定義 ***/
extern	CD_RSIO_BUFF	r_sio_buf;
extern	CD_RSIO_ERR		g_rcv_err;
extern	void sio_drv_init(void);
extern	void sio_snd_sta( BYTE* pdata, BYTE size );
extern	irqreturn_t	sio_intr_handler(int irq, void *dev);			/* IPCS_V4 PG 0007243 ADD */

/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのprobeモジュール                                */
/* 機能概要   FPGA_SIOドライバの.probeに関連付けるモジュールである             */
/*            FPGA_SIOドライバの登録お作法処理と使用するリソース確保           */
/* パラメータ なし                                                             */
/* リターン   正常：0                                                          */
/*            異常：0未満                                                      */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int	SioDrv_probe()
{
	int			result;
	int			devno;
	dev_t		dev_t = 0;
	DEV_DESC	*p_dev_desc;

	/* デバイス番号及びメジャー番号を動的に割当てる */
	result = alloc_chrdev_region( &dev_t, DEV_MINOR_NO, DEV_NUM, DEV_NAME );
	g_DrvInfo.majorNo = MAJOR(dev_t);
	if (result < 0)
	{
		g_DrvInfo.status = USR_DRV_ERROR;
		return(result);
	}

	/* cdev構造体の初期化とシステムコールハンドラテーブルの登録 */
	p_dev_desc = &g_DevDesc[0];
	p_dev_desc->value = 0;

	devno = MKDEV(g_DrvInfo.majorNo, DEV_MINOR_NO);
	cdev_init( &p_dev_desc->cdev, &g_FileOps );
	
	p_dev_desc->cdev.owner = THIS_MODULE;
	p_dev_desc->cdev.ops   = &g_FileOps;

	/* このデバイスドライバ(cdev)をカーネルに登録する */
	result = cdev_add( &p_dev_desc->cdev, devno, 1 );
	
	if (result < 0)
	{
		g_DrvInfo.status = USR_DRV_ERROR;
		return(result);
	}

	/* SIO初期化 */
	sio_drv_init();

	/* このデバイスのクラス登録をする(/devを作る) */
	g_DrvInfo.pClass  = class_create(THIS_MODULE, DEV_NAME);
	if (IS_ERR(g_DrvInfo.pClass))
	{
		cdev_del(&p_dev_desc->cdev);
		unregister_chrdev_region(devno, 1);
		return -1;
	}

	/* /dev/fpga-spi を作る */
	device_create(g_DrvInfo.pClass, NULL, devno, NULL, DEV_NAME "fpga-sio");
//	device_create(g_DrvInfo.pClass, NULL, devno, NULL, DEV_NAME "fpga-spi", NULL);

	g_DrvInfo.status = USR_DRV_NORMAL;

	/* IPCS_V4 PG 0007243 STA */
	/* 割り込みハンドラ登録 */
	if (request_irq(EXTINT1, sio_intr_handler, IRQF_SHARED | IRQF_TRIGGER_LOW, "/dev/fpga-sio", NULL))
	{
		printk(KERN_ERR "SIO handler ERROR");
	}
	/* IPCS_V4 PG 0007243 END */

	return 0;
}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのremoveモジュール                               */
/* 機能概要   FPGA_SIOドライバの.removeに関連付けるモジュールである            */
/*            FPGA_SIOドライバの消去お作法処理と使用するリソース解放           */
/* パラメータ なし                                                             */
/* リターン   なし                                                             */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
void	SioDrv_remove()
{
	int		dev;

	/* dev/sio を削除する */
	device_destroy(g_DrvInfo.pClass, g_DrvInfo.majorNo);

	/* デバイスのクラス登録を取り除く */
	class_destroy(g_DrvInfo.pClass);

	dev = MKDEV(g_DrvInfo.majorNo, 0);

	/* デバイスドライバ(cdev)をカーネルから取り除く */
	cdev_del(&g_DevDesc->cdev);

	/* デバイスドライバで使用していたメジャー番号の登録を取り除く */
	unregister_chrdev_region(dev, 0);

	/* 割込みハンドラ解除 */
	free_irq(EXTINT1, NULL);		/* IPCS_V4 PG 0007243 ADD */

}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのopenモジュール                                 */
/* 機能概要   FPGA_SIOドライバの.openに関連付けるモジュールである              */
/*            ドライバのオープン処理                                           */
/* パラメータ arg1 : arg1(in) ｉノード                                         */
/*            arg2 : arg2(in) ファイルハンドル                                 */
/* リターン   正常：0                                                          */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int SioDrv_open( struct inode* inode, struct file* p_file )
{
	DEV_DESC* p_dev = NULL; /* デバイス情報 */
	
	p_dev = container_of( inode->i_cdev, DEV_DESC, cdev );
	p_file->private_data = p_dev;

	return 0;
}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのcloseモジュール                                */
/* 機能概要   FPGA_SIOドライバの.closeに関連付けるモジュールである             */
/*            ドライバのクローズ処理                                           */
/* パラメータ arg1 : arg1(in) ｉノード                                         */
/*            arg2 : arg2(in) ファイルハンドル                                 */
/* リターン   正常：0                                                          */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int SioDrv_close( struct inode* inode, struct file* p_file )
{
	return 0;
}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのreadモジュール                                 */
/* 機能概要   FPGA_SIOドライバの.readに関連付けるモジュールである              */
/*            SIOからの受診データを返す                                        */
/* パラメータ arg1 : arg1(in)  ファイルハンドル                                */
/*            arg2 : arg2(out) 受信データ格納領域                              */
/*            arg3 : arg3(in)  受信データ領域のサイズ                          */
/*            arg4 : arg4(in)  未使用                                          */
/* リターン   正常：0以上(受信バイト数)                                        */
/*            異常：0未満                                                      */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int SioDrv_read( struct file * file, char *buff ,size_t cnt, loff_t *pos)
{

	int	read_cnt;

	/* 引数の正常性チェック */
	if(( buff == NULL ) || (cnt == 0))
	{
		return(~EINVAL);
	}

	while(1)
	{
		/* 受信データの有り/無し確認 */
		read_cnt = SioDrv_R_chk();
		/* 受信データ有り */
		if (read_cnt != 0)
		{
			/* 引数に受信データ設定 */
			copy_to_user(buff, r_sio_buf.rcv_buf, read_cnt);
			/* 受信データ参照位置更新 */
			r_sio_buf.rcv_read += read_cnt;
			return(read_cnt);
		}
		/* 受信データ無し */
		interruptible_sleep_on(&rcv_cmp_wait_q);
	}
}


/*******************************************************************************/
/* 関数名     受信データチェック処理                                           */
/* 機能概要   受信リングバッファにデータがあるか確認する                       */
/* パラメータ なし                                                             */
/* リターン   データ無し：0                                                    */
/*            データ有り：データサイズ                                         */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int SioDrv_R_chk(void)
{

	int	read_cnt = 0;

	/* 受信データ無し*/
	if (r_sio_buf.rcv_read == r_sio_buf.rcv_write)
	{
		return(0);
		
	}
	/* リングバッファが1周した場合 */
	if (r_sio_buf.rcv_read > r_sio_buf.rcv_write)
	{
		read_cnt = (0xFF - r_sio_buf.rcv_read) + r_sio_buf.rcv_write;
	}
	else
	{
		read_cnt = r_sio_buf.rcv_write - r_sio_buf.rcv_read;
	}

	return(read_cnt);
}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのwriteモジュール                                */
/* 機能概要   FPGA_SIOドライバの.writeに関連付けるモジュールである             */
/*            SIOからの受診データを返す                                        */
/* パラメータ arg1 : arg1(in)  ファイルハンドル                                */
/*            arg2 : arg2(in)  送信データ格納領域                              */
/*            arg3 : arg3(in)  送信データサイズ                                */
/*            arg4 : arg4(in)  未使用                                          */
/* リターン   正常：0以上(送信バイト数)                                        */
/*            異常：0未満                                                      */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static int SioDrv_write( struct file * file, const char *buff ,size_t cnt, loff_t *pos)
{

	char	stored_value[CD_SPFRAME_SIZE];

	/* 引数の正常性チェック */
	if(( buff == NULL ) || (cnt == 0) || (cnt > CD_SPFRAME_SIZE))
	{
		return(~EINVAL);
	}
	/* 送信データ設定 */
	copy_from_user(&stored_value, buff, cnt);
	
	/* データ送信 */
	sio_snd_sta(stored_value, cnt);

	interruptible_sleep_on(&snd_cmp_wait_q);

	return(cnt);
}


/*******************************************************************************/
/* 関数名     FPGA_SIOドライバのioctlモジュール                                */
/* 機能概要   FPGA_SIOドライバの.ioctlに関連付けるモジュールである             */
/*            ・SIO通信エラー情報(フレーミング/オーバーラン/パリティ)を返す    */
/*            ・SIO割り込み許可/禁止を設定する                                 */
/* パラメータ arg1 : arg1(in)      ファイルハンドル                            */
/*            arg2 : arg2(in)      リクエストコード                            */
/*            arg3 : arg3(in/out)  パラメータ                                  */
/* リターン   正常：0                                                          */
/*            異常：0未満                                                      */
/* 注意事項   なし                                                             */
/* その他     なし                                                             */
/*******************************************************************************/
static long SioDrv_ioctl( struct file *file, unsigned int cmd , unsigned long arg)
{

	/* エラー情報の取得 */
	if (cmd == TIOCGICOUNT)
	{
		memcpy(&arg, &g_rcv_err, sizeof(CD_RSIO_ERR));
	}
	/* 割り込み解除/禁止*/
	else if (cmd == TIOCSSERIAL)
	{
		/* 禁止 */
		if (arg == 0x01)
		{
			*SCR_REG &= 0x00;
		}
		/* 解除 */
		else
		{
			*SCR_REG |= 0xF4;
		}
	}
	else
	{
		/* DO NOTHING */
	}

	return(0);
}

module_init(SioDrv_probe);
module_exit(SioDrv_remove);

/*                              Copyright 2020 Oki Electric Industry Co.,Ltd. */
