/*#####################################################*/
/**
 * @file　usr_drv.h
 * @brief template of control driver for kernel 2.6
*/
#ifndef USR_DRV_H_
#define USR_DRV_H_

#include <linux/ioctl.h>

/* status定数 */
#define USR_DRV_NORMAL		0		/* 正常状態 */
#define USR_DRV_ERROR		-1		/* 異常状態 */
#define USR_DRV_NOT_INIT	-2		/* 未初期化状態 */

/* 戻り値用定数 */
#define RET_OK				0		/* 正常終了 */
#define RET_ERR				-1		/* 異常終了 */

/* 通知要求パラメータ */
typedef struct {
	int		kind;					/* 通知登録(1)／削除(0) */
	int		signo;					/* シグナル番号     */
	pid_t	pid;					/* 通知先プロセスＩＤ       */
} NOTICE_REQ;
#define NOTICE_SET			1		/* 通知登録 */
#define NOTICE_CLEAR		0		/* 通知クリア */


/*====================================================================*
** IOCTLコマンド定義
**/
/** IOCTLマジックナンバー */
//#define USR_DRV_IOC_MAGIC   0xE0

///** Get Status command */
//#define USR_DRV_IOCTL_GET_STATUS        _IOWR( USR_DRV_IOC_MAGIC, 0,  int )
//#define USR_DRV_IOCTL_SET_NOTICE        _IOWR( USR_DRV_IOC_MAGIC, 1,  int )

///** IOCTLコマンド最大番号 */
//#define USR_DRV_IOC_MAXNR   1

#endif /*USR_DRV_H_*/

