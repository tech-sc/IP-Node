/*# MODULE_HEADER #*/
/* This is free software, licensed under the GNU General Public License v2. */
/* See /LICENSE for more information. */
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : COM                                                      #*/
/*#                                                                         #*/
/*#     日本語名 : 共通関数                                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <linux/if_link.h>
#include <pthread.h>
#include <mqueue.h>

#include "def.h"
#include "str.h"
#include "str_comdt.h"
#include "prot.h"
#include "str_boot.h"
#include "str_start.h"
#include "com_mem.h"

extern LU_BOOT_DAT	lu_boot_data;					/*# ＩＰＬブートデータ #*///2004.01 LU32B追加
extern XS_POOL_DAT pool_dat[COM_POOL_MAX];
extern XS_POOL_ATR pool_atr_tbl[COM_POOL_MAX];
extern XS_TASK_ATTR task_attr[COM_TASK_MAX_N];
extern XS_TASK_STATUS task_status[COM_TASK_MAX_N];
extern struct mq_attr lu_mq_attr_v;
extern XS_ECB_STTR ecb_attr[COM_ECB_MAX];
extern mqd_t ecb_mq_desc[COM_ECB_MAX];
extern XS_QUE_CTL com_que_ctl[COM_QUE_MAX];
extern XS_DBGPRINT_CTL dbgprint_ctl[COM_TASK_MAX_N];
extern XS_DBGPRINT_CTL com_dbgprint_ctl;
extern LU_START lu_start;
extern FLS_CA_DATA			fls_ca_data;				/*# CA?؂????? #*/
extern	COM_FLSDATA_STS	com_flsdata_sts;
extern	S_LUNUM_DAT lun_data;						/*# ＬＵ番号通知データ管理 #*/
extern	COM_DATA	com_data;						/*# 共通参照データ #*/
extern  COM_FLSDATA     com_flsdata;
/**************************** モジュールローカル変数 ****************************/
static pthread_mutex_t pool_atrtbl_lock;  // pool_atrtblテーブル保護用MUTEX
static pthread_mutex_t que_ctl_lock;      // que_ctlテーブル保護用MUTEX
/***************************************************************************************************/

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_syserr                                               #*/
/*#                                                                         #*/
/*#     日本語名 : システムエラー                                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : システムエラー処理                                       #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : タスクＩＤ(BYTE)                                         #*/
/*#              : エラーコード(BYTE)                                       #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/03/03 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_syserr(BYTE	taskid,BYTE	errcd)
{
	FILE *fpout;
	int fd_w =0;
	uint_8 prev_p;

	/* ＬＵ再開情報を更新する */
	lu_start.sys_err_cnt++;								/*# システムエラー回数更新 #*/
	lu_start.sys_err_id=(DWORD)taskid;						/*# システムエラータスクＩＤ #*/
	lu_start.sys_err_inf=(DWORD)errcd;						/*# システムエラー付加情報 #*/
	lu_start.pool0_cnt=(DWORD)(POOL0_CNT - pool_atr_tbl[POOL0].get_count);	/*# ＰＯＯＬ０残り数 #*/
	lu_start.pool1_cnt=(DWORD)(POOL1_CNT - pool_atr_tbl[POOL1].get_count);	/*# ＰＯＯＬ１残り数 #*/
	/* ＬＵ再開情報をＦｌａｓｈに書きこむ */
	fd_w = open("/flsdata/lu_start.dat", O_WRONLY | O_SYNC );
	if (fd_w != -1) {
		fpout = fdopen(fd_w, "wb");
		if (fpout == NULL) {
			dbg_print(COM_ID, LOG_ERR, "%s lu_start fdopen error %s\n", __func__, strerror(errno));
			close(fd_w);
		}
		else {
			if (fwrite((const void *)&lu_start, 1, (size_t)sizeof(lu_start), fpout) < sizeof(lu_start)) {
				dbg_print(COM_ID, LOG_ERR, "%s lu_start fwrite error %s\n", __func__, strerror(errno));
			}
			fflush(fpout);
			fsync(fd_w);
			fclose(fpout);
			close(fd_w);
		}
	}
	else {
		dbg_print(COM_ID, LOG_ERR, "%s lu_start open error %s\n", __func__, strerror(errno));
	}
	/* CA切替情をＦｌａｓｈに書きこむ */
		fd_w = open("/flsdata/ca_data.dat", O_WRONLY | O_SYNC );
	if (fd_w != -1) {
		fpout = fdopen(fd_w, "wb");
		if (fpout == NULL) {
			dbg_print(COM_ID, LOG_ERR, "%s ca_data fdopen error %s\n", __func__, strerror(errno));
			close(fd_w);
		}
		else {
			if (fwrite((const void *)&fls_ca_data, 1, (size_t)sizeof(fls_ca_data), fpout) < sizeof(fls_ca_data)) {
				dbg_print(COM_ID, LOG_ERR, "%s fls_ca_data fwrite error %s\n", __func__, strerror(errno));
			}
			fflush(fpout);
			fsync(fd_w);
			fclose(fpout);
			close(fd_w);
		}
	}
	else {
		dbg_print(COM_ID, LOG_ERR, "%s ca_data open error %s\n", __func__, strerror(errno));
	}

	/* com_flsdata_stsをＦｌａｓｈに書きこむ */
		fd_w = open("/flsdata/flsdata_sts.dat", O_WRONLY | O_SYNC );
	if (fd_w != -1) {
		fpout = fdopen(fd_w, "wb");
		if (fpout == NULL) {
			dbg_print(COM_ID, LOG_ERR, "%s flsdata_sts fdopen error %s\n", __func__, strerror(errno));
			close(fd_w);
		}
		else {
			if (fwrite((const void *)&com_flsdata_sts, 1, (size_t)sizeof(com_flsdata_sts), fpout) < sizeof(com_flsdata_sts)) {
				dbg_print(COM_ID, LOG_ERR, "%s flsdata_sts fwrite error %s\n", __func__, strerror(errno));
			}
			fflush(fpout);
			fsync(fd_w);
			fclose(fpout);
			close(fd_w);
		}
	}
	else {
		dbg_print(COM_ID, LOG_ERR, "%s flsdata_sts open error %s\n", __func__, strerror(errno));
	}

	/* EDを障害中の表示に変更する。 */
	if(com_FpgaLED(LED_BLINK | LED_LU | LED_BLUE, ON_BLINK_MASK | LED_CTRL_MASK | COLOR_MASK,&prev_p) != OK) {
		dbg_print(COM_ID, LOG_ERR, "%s com_FpgaLED prev_p=%02x\n", __func__, prev_p);
	}
	/* ログ情報(syslog、Corefileを含む)をFlashに書きこむ */
	system("cp /var/log/messages.* /lu_log/.");
	system("cp tmp/*.core /lu_log/.");
	fflush(NULL);
	fsync(fd_w);
	system("sync");

	reboot(RB_AUTOBOOT);
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_poolinit                                             #*/
/*#                                                                         #*/
/*#     日本語名 : メモリ管理イニシャル                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : プール管理情報の初期設定                                 #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_poolinit(void)
{
	BYTE index;
	pthread_mutex_init(&pool_atrtbl_lock, NULL);/* ミューテックス初期化 */

	/* プール属性の生成 */
	for (index=0;index<COM_POOL_MAX;++index) {
          pool_atr_tbl[index].PoolSize = pool_dat[index].PoolSize;
          pool_atr_tbl[index].PoolNum  = pool_dat[index].PoolNum;
          pool_atr_tbl[index].get_count= 0;
          pool_atr_tbl[index].pool_adr = NULL;
	}
}
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_poolget[タスク起動]                                  #*/
/*#                                                                         #*/
/*#     日本語名 : メモリブロック取得                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたIDのメモリブロックを取得する                   #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : メモリブロックID(BYTE)                                   #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : MSGアドレス(BYTE*) [adr,NG:NULL]                         #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
BYTE	*com_poolget(BYTE poolid)
{
  BYTE  *adr = NULL;
  DWORD blocksize;
  XS_POOL_INFO *pinfo;
  BYTE  *ret_addr = (BYTE *)0xFFFFFFFF;

	if (poolid >= COM_POOL_MAX) {
		/* メモリブロックID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_01);
	}
	
	/* スレッド排他制御(Lock) */
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&pool_atrtbl_lock );
	pthread_mutex_lock( &pool_atrtbl_lock );

	/* shurokusumiブロック数をチェックし、最大数取得済の場合は、プールなしでリターンする */
	if (pool_atr_tbl[poolid].PoolNum == pool_atr_tbl[poolid].get_count)
	{
		// 既にメモリプールを使い切っている場合
		goto com_poolget_EXIT;
	}

	// 確保サイズ取得
	blocksize = pool_atr_tbl[poolid].PoolSize;

	// メモリ確保
	adr = (BYTE *)malloc( (blocksize+sizeof(XS_POOL_INFO)) );

  if (adr == NULL)
    {
		goto com_poolget_EXIT;
    }

	pool_atr_tbl[poolid].get_count++;

	// 確保空間を0で初期化
	memset( adr, 0, (blocksize+sizeof(XS_POOL_INFO)));

	pinfo = (XS_POOL_INFO*)adr;
	pinfo->AllocAddr = &adr[sizeof(XS_POOL_INFO)];
	pinfo->Index = poolid;
	pinfo->pool_next_adr = NULL;

	if ( pool_atr_tbl[poolid].pool_adr == NULL ) {
		pool_atr_tbl[poolid].pool_adr = pinfo;
	}
	else {
		XS_POOL_INFO *pooladr = pool_atr_tbl[poolid].pool_adr;
		for ( ; pooladr; pooladr = pooladr->pool_next_adr) {
			if(!pooladr->pool_next_adr) {
			break;
			}
		}
		pooladr->pool_next_adr = pinfo;
	}
	ret_addr = pinfo->AllocAddr;
com_poolget_EXIT:	
	/* スレッド排他制御(Unlock) */
	pthread_mutex_unlock( &pool_atrtbl_lock );
	pthread_cleanup_pop( 0 );

	return ret_addr;
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_ipoolget[割込みから起動]                             #*/
/*#                                                                         #*/
/*#     日本語名 : メモリブロック取得                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたIDのメモリブロックを取得する                   #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : メモリブロックID(BYTE)                                   #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : MSGアドレス(BYTE*) [adr,NG:NULL]                         #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
BYTE	*com_ipoolget(BYTE poolid)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	return com_poolget(poolid);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_poolput メモリブロック解放[タスク起動]               #*/
/*#                                                                         #*/
/*#     日本語名 : メモリブロック解放                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : メモリブロックの解放を行う                               #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : メモリブロックID(BYTE)                                   #*/
/*#              : 解放アドレス(BYTE*)                                      #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_poolput(BYTE poolid,BYTE *adr)
{

  pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&pool_atrtbl_lock );
  pthread_mutex_lock( &pool_atrtbl_lock );

  {
    XS_POOL_INFO *pinfo = (XS_POOL_INFO*)(adr - sizeof(XS_POOL_INFO));
    int index = pinfo->Index;
	if (index >= COM_POOL_MAX) {
		/* メモリブロックID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_02);
	}

  	if( pinfo->AllocAddr != adr ) {
		/* メモリブロックアドレス異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_03);
    }

    {
      XS_POOL_INFO *mae = NULL;
      XS_POOL_INFO *pooladr = pool_atr_tbl[index].pool_adr;
      for ( ; pooladr; pooladr = pooladr->pool_next_adr) {
        if(pooladr == pinfo) {
          if(mae == NULL) {
            pool_atr_tbl[index].pool_adr = pooladr->pool_next_adr;
          }
          else {
            mae->pool_next_adr = pooladr->pool_next_adr;
          }
        break;
        }
        mae = pooladr;
      } 
    }

    pool_atr_tbl[pinfo->Index].get_count--;

    free( pinfo );
  }
  pthread_mutex_unlock( &pool_atrtbl_lock );
  pthread_cleanup_pop( 0 );
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_ipoolput [割込みから起動]                            #*/
/*#                                                                         #*/
/*#     日本語名 : メモリブロック解放                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : メモリブロックの解放を行う                               #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : メモリブロックID(BYTE)                                   #*/
/*#              : 解放アドレス(BYTE*)                                      #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_ipoolput(BYTE poolid,BYTE *adr)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	com_poolput(poolid, adr);
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_get_pool_status  メモリプール状態取得                #*/
/*#                                                                         #*/
/*#     日本語名 : メモリプール状態取得                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたIDの状態を収集する。                           #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : メモリブロックID(BYTE)                                   #*/
/*#              : メモリプール数格納領域アドレス(WORD*)                    #*/
/*#              : 使用中メモリプール数格納領域アドレス(WORD*)              #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/272 - 小澤  美津晴                                 #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_get_pool_status(BYTE poolid, WORD *PoolNum, WORD *get_count)
{

	if (poolid >= COM_POOL_MAX) {
		/* メモリブロックID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_28);
	}
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&pool_atrtbl_lock );
	pthread_mutex_lock( &pool_atrtbl_lock );
  
	/* メモリプール数を格納する */
	*PoolNum = pool_atr_tbl[poolid].PoolNum;

	/* 使用中メモリプール数を格納する */
	*get_count = pool_atr_tbl[poolid].get_count;
	
	pthread_mutex_unlock( &pool_atrtbl_lock );
	pthread_cleanup_pop( 0 );
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_iptdis                                               #*/
/*#                                                                         #*/
/*#     日本語名 : 割込み禁止 [タスクから起動]                              #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 割込み禁止実行                                           #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : マスク情報(BYTE)                                         #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_iptdis(void)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_iptena                                               #*/
/*#                                                                         #*/
/*#     日本語名 : 割込み禁止解除 [タスクから起動]                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 割込み禁止解除実行                                       #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : マスク情報(BYTE)                                         #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void com_iptena(BYTE pri)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_strcopy                                              #*/
/*#                                                                         #*/
/*#     日本語名 : 文字列コピー（バイト）                                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 文字列コピー実行                                         #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : コピー先アドレス(*BYTE)                                  #*/
/*#              : コピー元アドレス(*BYTE)                                  #*/
/*#              : コピーサイズ(WORD)                                       #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_strcopy(BYTE *dst,BYTE *src,WORD cnt)
{
	WORD	i;
	BYTE	wk;

	for(i = 0;i < cnt;i++,dst++,src++){
		wk = *src;
		*dst = wk;
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_wordcopy                                             #*/
/*#                                                                         #*/
/*#     日本語名 : データコピー（ワード）                                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ワードデータコピー実行                                   #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : コピー先アドレス(*WORD)                                  #*/
/*#              : コピー先アドレス(*WORD)                                  #*/
/*#              : コピーサイズ(WORD)                                       #*/
/*#              : コピーパターン(BYTE;＝０：そのまま転送                   #*/
/*#                                    ＝１：ＬＯＷ＜－＞ＨＩＧＨ変換転送   #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/02/24 - 青木  康浩                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_wordcopy(WORD *dst,WORD *src,WORD cnt,BYTE ptn)
{
	WORD	i;
	WORD	wk;
	for(i = 0;i < cnt;i++,dst++,src++){
			wk = *src;
        if(ptn == COM_NOMAL){                  /*# そのまま転送の場合 #*/
			*dst = wk;
        }
        else{                                  /*# LOW<->HIGH変換転送の場合 #*/
          *dst = com_chgword(wk);
        }
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_dwordcopy                                            #*/
/*#                                                                         #*/
/*#     日本語名 : 文字列コピー（32bit）                                    #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 文字列コピー実行                                         #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : コピー先アドレス(*DWORD)                                 #*/
/*#              : コピー元アドレス(*DWORD)                                 #*/
/*#              : コピーサイズ(WORD)                                       #*/
/*#     出    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_dwordcopy(DWORD *dst,DWORD *src,WORD cnt,BYTE ptn)
{
	WORD	i;
	DWORD	wk;

	for(i = 0;i < cnt;i++,dst++,src++){
		wk = *src;
		if(ptn == COM_NOMAL){                  /*# そのまま転送の場合 #*/
		*dst = wk;
		}
		else{                                  /*# LOW<->HIGH変換転送の場合 #*/
			*dst = com_chgdword(wk);
		}
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_ecbinit                                              #*/
/*#                                                                         #*/
/*#     日本語名 : ECB初期化                                                #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ECBを初期化する                                          #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_ecbinit(void)
{
	BYTE ecb_no;

	/* ECB属性の生成 */
	for (ecb_no=0;ecb_no<COM_ECB_MAX;++ecb_no) {
		ecb_mq_desc[ecb_no] = mq_open(ecb_attr[ecb_no].name, ecb_attr[ecb_no].oflag, ecb_attr[ecb_no].mode, ecb_attr[ecb_no].attr);
		if (ecb_mq_desc[ecb_no] == -1) {
			dbg_print(COM_ID, LOG_ERR, "%s mq_open[%s] error %s\n", __func__, ecb_attr[ecb_no].name, strerror(errno));
			/* mq_open異常終了時は、システムエラーを起動 */
			com_syserr(COM_ID, COM_ABORT_04);
		}
	}
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_sndmsg                                               #*/
/*#                                                                         #*/
/*#     日本語名 : メッセージ送信 [タスクから起動]                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : メッセージ送信処理実行                                   #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : ECB-ID(BYTE)                                             #*/
/*#              : 送信メッセージポインタ(*BYTE)                            #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_sndmsg(BYTE ecbno,BYTE *msg)
{
	unsigned int msg_prio=0;
	mqd_t mqdesc;
	XS_POOL_INFO *pool_info;
	
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
	
	/* 入力メッセージを解放する */
	pool_info = (XS_POOL_INFO *)msg;
	com_poolput(pool_info->Index, (BYTE *)pool_info->AllocAddr);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_isndmsg [割込みから起動]                             #*/
/*#                                                                         #*/
/*#     日本語名 : メッセージ送信                                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : メッセージ送信処理実行                                   #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : ECB-ID(BYTE)                                             #*/
/*#              : 送信メッセージポインタ(*BYTE)                            #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_isndmsg(BYTE ecbno,BYTE *msg)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	com_sndmsg(ecbno,msg);
#ifdef LU_LOG
	log_trace((BYTE)ecbno,(BYTE*)msg);
#endif
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_rcvmsg [タスクから起動]                              #*/
/*#                                                                         #*/
/*#     日本語名 : メッセージ受信(Waitあり)                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : メッセージ受信実行                                       #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : ECB-ID(BYTE)                                             #*/
/*#              : タイマカウント値(BYTE)                                   #*/
/*#     リターン : 送信メッセージポインタ(*BYTE)                            #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
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
	pool_info = (XS_POOL_INFO *)msg;
	for (cnt=0;cnt<100;++cnt) {
		ret_msg = com_poolget(pool_info->Index);
		if (ret_msg != (BYTE *)0xFFFFFFF) {
			break;
		}
	}
	if (cnt == 100) {
		dbg_print(COM_ID, LOG_ERR, "%s com_poolget pool_id=%d %s\n", __func__, pool_info->Index, strerror(errno));
		/* mq_open異常終了時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_11);
	}

	memcpy((void *)ret_msg, (void *)(msg + sizeof(XS_POOL_INFO)), len-sizeof(XS_POOL_INFO));
#ifdef LU_LOG
	log_trace((BYTE)ecbno,(BYTE*)ret_msg);
#endif
	return(ret_msg);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_queinit                                              #*/
/*#                                                                         #*/
/*#     日本語名 : キュー管理イニシャル                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : キュー管理関連の初期化を行う                             #*/
/*#     入    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_queinit(void)
{

	/* ミューテックス初期化 */
	pthread_mutex_init(&que_ctl_lock, NULL);

	/* キュー管理テーブルを全て0で初期化する */
	memset((void *)com_que_ctl, 0, sizeof(com_que_ctl));
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_queput                                               #*/
/*#                                                                         #*/
/*#     日本語名 : キュー接続処理[タスク処理からコールする]                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたMSGのキュー接続を行う。                        #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#              : MSGアドレス(BYTE*)                                       #*/
/*#              : 接続種別(BYTE) [=0:QUE_TOP,=1:QUE_TAIL]                  #*/
/*#     リターン : Ret(DWORD) [=0:OK,=-1(FFFFFFFFh):NG]                     #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	com_queput(WORD queid,BYTE *adr,BYTE kind)
{
	XS_QUE_CTL *quectl_p;
	XS_POOL_INFO *poolinfo_p;
	
	/* キューIDをチェック */
	if (queid >= COM_QUE_MAX) {
		/* キューID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_12);
	}
	
	quectl_p = (XS_QUE_CTL *)&com_que_ctl[queid];
	poolinfo_p = (XS_POOL_INFO *)(adr - sizeof(XS_POOL_INFO));
	
	/* スレッド排他制御(Lock) */
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&que_ctl_lock );
	pthread_mutex_lock( &que_ctl_lock );
	
	if (quectl_p->Que_num == 0) {
		/* 登録数0の場合 */
		quectl_p->Que_Head = quectl_p->Que_Tail = (XS_POOL_INFO *)adr;
		poolinfo_p->link_next = poolinfo_p->link_pre = NULL;
	}
	else if (kind == QUE_TOP) {
		/* キューの先頭に接続 */
		poolinfo_p->link_pre = NULL;
		poolinfo_p->link_next = (void *)quectl_p->Que_Head;
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Head;
		poolinfo_p->link_pre = (void *)(adr - sizeof(XS_POOL_INFO));
		quectl_p->Que_Head = (XS_POOL_INFO *)(adr - sizeof(XS_POOL_INFO));
	}
	else {
		/* キューの末尾に接続 */
		poolinfo_p->link_pre = (void *)quectl_p->Que_Tail;
		poolinfo_p->link_next = NULL;
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Tail;
		poolinfo_p->link_next = (void *)(adr - sizeof(XS_POOL_INFO));
		quectl_p->Que_Tail = (XS_POOL_INFO *)(adr - sizeof(XS_POOL_INFO));
	}
	
	/* キュー登録数更新 */
	++quectl_p->Que_num;
	
	/* スレッド排他制御(Unlock) */
	pthread_mutex_unlock( &que_ctl_lock );
	pthread_cleanup_pop( 0 );

	return(OK);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_iqueput                                              #*/
/*#                                                                         #*/
/*#     日本語名 : キュー接続処理[割り込み処理からコールする]               #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたMSGのキュー接続を行う。                        #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#              : MSGアドレス(BYTE*)                                       #*/
/*#              : 接続種別(BYTE) [=0:QUE_TOP,=1:QUE_TAIL]                  #*/
/*#     リターン : Ret(DWORD) [=0:OK,=-1(FFFFFFFFh):NG]                     #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	com_iqueput(WORD queid,BYTE *adr,BYTE kind)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	return(com_queput(queid, adr, kind));
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_queget                                               #*/
/*#                                                                         #*/
/*#     日本語名 : キュー取り出し処理[タスク処理からコールする]             #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : キュー先頭からキュー取り出しを行う。                     #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#              : 接続種別(BYTE) [=0:QUE_TOP,=1:QUE_TAIL]                  #*/
/*#     リターン : MSGアドレス(BYTE*) [=Adr:OK,=-1(FFFFFFFFh):NG]           #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
BYTE	*com_queget(WORD	queid,BYTE	kind)
{
	XS_QUE_CTL *quectl_p;
	XS_POOL_INFO *poolinfo_p;
	BYTE *ret_addr=(BYTE *)0xFFFFFFFF;
	
	/* キューIDをチェック */
	if (queid >= COM_QUE_MAX) {
		/* キューID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_13);
	}
	
	quectl_p = (XS_QUE_CTL *)&com_que_ctl[queid];
	
	/* スレッド排他制御(Lock) */
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&que_ctl_lock );
	pthread_mutex_lock( &que_ctl_lock );
	
	if (quectl_p->Que_num == 0) {
		/* メッセージ無しでリターンする */
		return((BYTE *)0xFFFFFFFF);
	}
	if (quectl_p->Que_num == 1) {
		/* キューから取り出し */
		ret_addr = (BYTE *)quectl_p->Que_Head + sizeof(XS_POOL_INFO);
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Head;
		poolinfo_p->link_pre = poolinfo_p->link_next = NULL;
		quectl_p->Que_Head = quectl_p->Que_Tail = NULL;
	}
	else if (kind == QUE_TOP) {
		/* キューの先頭から取り出し */
		ret_addr = (BYTE *)quectl_p->Que_Head + sizeof(XS_POOL_INFO);
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Head;
		quectl_p->Que_Head = (XS_POOL_INFO *)poolinfo_p->link_next;
		poolinfo_p->link_pre = poolinfo_p->link_next = NULL;
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Head;
		poolinfo_p->link_pre = NULL;
	}
	else {
		/* キューの末尾から取り出し */
		ret_addr = (BYTE *)quectl_p->Que_Tail + sizeof(XS_POOL_INFO);
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Tail;
		quectl_p->Que_Tail = (XS_POOL_INFO *)poolinfo_p->link_pre;
		poolinfo_p->link_pre = poolinfo_p->link_next = NULL;
		poolinfo_p = (XS_POOL_INFO *)quectl_p->Que_Tail;
		poolinfo_p->link_next = NULL;
	}
	
	/* キュー登録数更新 */
	--quectl_p->Que_num;
	
	/* スレッド排他制御(Unlock) */
	pthread_mutex_unlock( &que_ctl_lock );
	pthread_cleanup_pop( 0 );
	return(ret_addr);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_iqueget                                              #*/
/*#                                                                         #*/
/*#     日本語名 : キュー取り出し処理[割り込み処理からコールする]           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : キュー先頭からキュー取り出しを行う。                     #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#              : 接続種別(BYTE) [=0:QUE_TOP,=1:QUE_TAIL]                  #*/
/*#     リターン : MSGアドレス(BYTE*) [=Adr:OK,=-1(FFFFFFFFh):NG]           #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : 20/11/27 - 小澤  美津晴                                  #*/
/*#                                                                         #*/
/*###########################################################################*/
BYTE	*com_iqueget(WORD	queid,BYTE	kind)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	return(com_queget(queid, kind));
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_quesns                                               #*/
/*#                                                                         #*/
/*#     日本語名 : キュー状況調査[タスク処理からコールする]                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : キューの登録数を調査する                                 #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#     リターン : 調査結果(DWORD)                                          #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	com_quesns(WORD queid)
{

	DWORD ret_num;
	
	/* キューIDをチェック */
	if (queid >= COM_QUE_MAX) {
		/* キューID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_14);
	}
	
	/* スレッド排他制御(Lock) */
	pthread_cleanup_push( (void(*)(void *))pthread_mutex_unlock,(void *)&que_ctl_lock );
	pthread_mutex_lock( &que_ctl_lock );
	
	ret_num = com_que_ctl[queid].Que_num;

	/* スレッド排他制御(Unlock) */
	pthread_mutex_unlock( &que_ctl_lock );
	pthread_cleanup_pop( 0 );
	
	return(ret_num);
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_iquesns                                              #*/
/*#                                                                         #*/
/*#     日本語名 : キュー状況調査[割り込み処理からコールする]               #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : キューの登録数を調査する                                 #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : キューID(WORD)                                           #*/
/*#     リターン : 調査結果(DWORD)                                          #*/
/*#                                                                         #*/
/*#     作    成 : 99/01/22 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	com_iquesns(WORD queid)
{
	dbg_print(COM_ID, LOG_DEBUG, "%s activated\n", __func__);
	return(com_quesns(queid));
}

#if 0 /* IPCS_V4 <PD><DEL> */
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_chgword                                              #*/
/*#                                                                         #*/
/*#     日本語名 : ワードデータ変換処理                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたワードデータをビッグエンディアン形式から       #*/
/*#                リトルエンディアン形式に変換する。                       #*/
/*#     入    力 : 変換データ(WORD)                                         #*/
/*#     リターン : 変換結果(WORD)                                           #*/
/*#                                                                         #*/
/*#     作    成 : 99/02/09 - 森　昌生                                      #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
WORD	com_chgword( WORD	input )
{
	union{										/*# 変換用ワーク(インプット用) #*/
		WORD	work_w;
		BYTE	work_b[2];
	}input_work;
	union{										/*# 変換用ワーク(アウトプット用) #*/
		WORD	work_w;
		BYTE	work_b[2];
	}output_work;
	
	input_work.work_w = input;					/*# インプットデータ設定 #*/
												/*# 変換処理 #*/
	output_work.work_b[0] = input_work.work_b[1];
	output_work.work_b[1] = input_work.work_b[0];
	
	return( output_work.work_w );
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_chgdword                                             #*/
/*#                                                                         #*/
/*#     日本語名 : ダブルワードデータ変換処理                               #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたダブルワードデータをビッグエンディアン形式から #*/
/*#                リトルエンディアン形式に変換する。                       #*/
/*#     入    力 : 変換データ(DWORD)                                        #*/
/*#     リターン : 変換結果(DWORD)                                          #*/
/*#                                                                         #*/
/*#     作    成 : 99/02/09 - 森　昌生                                      #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	com_chgdword( DWORD	input )
{
	union{										/*# 変換用ワーク(インプット用) #*/
		DWORD	work_dw;
		BYTE	work_b[4];
	}input_work;
	union{										/*# 変換用ワーク(アウトプット用) #*/
		DWORD	work_dw;
		BYTE	work_b[4];
	}output_work;
	
	input_work.work_dw = input;					/*# インプットデータ設定 #*/
												/*# 変換処理 #*/
	output_work.work_b[0] = input_work.work_b[3];
	output_work.work_b[1] = input_work.work_b[2];
	output_work.work_b[2] = input_work.work_b[1];
	output_work.work_b[3] = input_work.work_b[0];
	
	return( output_work.work_dw );
}
#endif /* IPCS_V4 <PD><DEL-END> */
/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : bcd_bin                                                  #*/
/*#                                                                         #*/
/*#     日本語名 : ＢＣＤ－バイナリ変換処理                                 #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ＢＣＤデータをバイナリ（１６進数値）に変換する。         #*/
/*#                ex.  0x12345678 -->  0x00bc614e                          #*/
/*#     入    力 : 変換ＢＣＤデータ(DWORD)                                  #*/
/*#     リターン : 変換結果(DWORD)                                          #*/
/*#                                                                         #*/
/*#     作    成 : 99/07/07 -                                               #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
DWORD	bcd_bin(DWORD bcd)
{
	DWORD	bin;

												/*# 変換処理 #*/
	bin= ((bcd >> 28) & 0x0000000f)*10000000 +
		 ((bcd >> 24) & 0x0000000f)*1000000 +
		 ((bcd >> 20) & 0x0000000f)*100000 +
		 ((bcd >> 16) & 0x0000000f)*10000 +
		 ((bcd >> 12) & 0x0000000f)*1000 +
		 ((bcd >>  8) & 0x0000000f)*100 +
		 ((bcd >>  4) & 0x0000000f)*10 +
		 (bcd & 0x0000000f);

	return bin;
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : lu_num_set                                               #*/
/*#                                                                         #*/
/*#     日本語名 : ＬＵ番号出力データ設定                                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : -                                                        #*/
/*#     作    成 : 99/07/29 -                                               #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void	lu_num_set(DWORD lu_num, DWORD vlan_id)
{
	DWORD			bit;
	DWORD			vlan_dat;
	

	for(bit=0; bit<8; bit++)
	{
		lun_data.lun_out_bit[bit]=1;					/*# ガードビット設定 #*/
	}

	lun_data.lun_out_bit[8]=0;						  /*# スタートビット設定 #*/
	for(bit=9; bit<=16; bit++)
	{
		if((lu_num & 0x00000001) == 1)
		{
			lun_data.lun_out_bit[bit]=1;			/*# ＬＵ番号ビット１設定 #*/
		}
		else
		{
			lun_data.lun_out_bit[bit]=0;			/*# ＬＵ番号ビット０設定 #*/
		}
		lu_num=lu_num>>1;
	}
	lun_data.lun_out_bit[17]=1;						  /*# ストップビット設定 #*/

	vlan_dat=(vlan_id & 0xff);						  /*# VLAN ID下位８ビット #*/
	lun_data.lun_out_bit[18]=0;						  /*# スタートビット設定 #*/
	for(bit=19; bit<=26; bit++)
	{
		if((vlan_dat & 0x00000001) == 1)
		{
			lun_data.lun_out_bit[bit]=1;			/*# ＬＵ番号ビット１設定 #*/
		}
		else
		{
			lun_data.lun_out_bit[bit]=0;			/*# ＬＵ番号ビット０設定 #*/
		}
		vlan_dat=vlan_dat>>1;
	}
	lun_data.lun_out_bit[27]=1;						  /*# ストップビット設定 #*/

	vlan_dat=vlan_id>>8;							  /*# VLAN ID上位８ビット #*/
	lun_data.lun_out_bit[28]=0;						  /*# スタートビット設定 #*/
	for(bit=29; bit<=36; bit++)
	{
		if((vlan_dat & 0x00000001) == 1)
		{
			lun_data.lun_out_bit[bit]=1;			/*# ＬＵ番号ビット１設定 #*/
		}
		else
		{
			lun_data.lun_out_bit[bit]=0;			/*# ＬＵ番号ビット０設定 #*/
		}
		vlan_dat=vlan_dat>>1;
	}
	lun_data.lun_out_bit[37]=1;						  /*# ストップビット設定 #*/

	lun_data.lun_out_cnt=0;						/*# ＬＵ番号出力データ初期化 #*/
	lun_data.lun_out_ctrl=1;							/*# ＬＵ番号出力開始 #*/

}
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : in_usnet_mac_rev                                         #*/
/*#                                                                         #*/
/*#     日本語名 : MACアドレスBIT反転                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : MAC ADDRESS BIT REVERSE                                  #*/
/*#     更    新 :  99/02/23 - 毛塚  照明                                   #*/
/*#                                                                         #*/
/*###########################################################################*/
void in_usnet_mac_rev(BYTE	*macadr)
{
	DWORD	i,bit;
	BYTE	dat;

	for(i=0; i<6; i++){
		dat = 0;
		for(bit=0; bit<8; bit++){
			if((0x01<<bit) & *(macadr + i)){
				dat |= 0x80>>bit;
			}
		}
 		*(macadr + i) = dat;
	}
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_hardver_set                                          #*/
/*#                                                                         #*/
/*#     日本語名 : ハードバージョン設定                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ハードバージョンを読み出し共通エリアに設定する           #*/
/*#     作    成 : 00/09/-- - V2.1 追加                                     #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_hardver_set( )
{

#if 0 /* Kari Ozawa */
	volatile WORD	*io_ptr;							/*# IOポインタ #*/

	if( com_data.lu_type == LU_MASTER ){

		io_ptr = (WORD*)IO_HARD_VER;					/*# ハードバージョンアドレス #*/
		com_data.hard_ver=(BYTE)(*io_ptr)	;			/*# ハードバージョン値保存 #*/

		if(com_data.hard_ver == 0xff)					/*# V1.1ハードのみ特殊（0xffに見える） #*/
			com_data.hard_ver = E_V1_1_HARD;

	}
	else
#endif /* Kari Ozawa */
		com_data.hard_ver = 0;							/*# オンライン通知で使用するため値変更注意 #*/

}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : lu_num_init                                              #*/
/*#                                                                         #*/
/*#     日本語名 : ＬＵ番号通知準備処理                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 01/01/18 -                                               #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
// 2001.1.18 ＬＵ番号／ＶＬＡＮ番号化け対策------全面追加
void	lu_num_init()
{
#if 0 /* IPCS_V4 <PD><DEL-STA> */

	volatile WORD	*io_addr2;
	volatile DWORD	*io_addr4;

	io_addr2=(WORD *)IO_WRT_PRO;
	*io_addr2=(WORD)0x0055;								  /*# プロテクト解除 #*/
	io_addr2=(WORD *)IO_RST_SLV;
	*io_addr2=(WORD)0x0000;							  /*# スレーブκリセット #*/
	io_addr2=(WORD *)IO_WRT_PRO;
	*io_addr2=(WORD)0x00ff;								  /*# プロテクト設定 #*/

	io_addr4=(DWORD *)IO_ILCR1;						  /*# ＩＲＱレベル制御１ #*/
	*io_addr4=(DWORD)0x00000050;		 /*# MAC/DSP/CRINT=0/5/0 DSP以外停止 #*/
#endif /* IPCS_V4 <PD><DEL-END> */
	com_data.timer_cnt=0;					   /*# 16bitタイマカウンタ初期化 #*/
	com_data.lu_slv_restart=1;		  	/*# ＬＵスレーブリスタート要求セット #*/

}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 :lu_hdver_make                                             #*/
/*#                                                                         #*/
/*#     日本語名 : ハードバージョン作成                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ハードバージョンの文字列を作成する                       #*/
/*#     作    成 : 00/09/-- V2.1追加 ＳＡ                                   #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : BYTE *sptr：ハードバージョンデータへのポインタ           #*/
/*#              : BYTE *dptr：データ格納エリアへのポインタ                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
void	lu_hdver_make( BYTE *sptr, BYTE *dptr )
{

	dptr[0] = 0 + 0x30;
	dptr[1] = ((sptr[0]&0xf0)>>4) + 0x30;
	dptr[2] = ((sptr[0]&0x0f)>>0) + 0x30;
	dptr[3] = 0 + 0x30;

}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 :lu_flver_make                                             #*/
/*#                                                                         #*/
/*#     日本語名 : ファイルバージョン作成                                   #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : ファイルバージョンの文字列を作成する                     #*/
/*#     作    成 : 00/--/-- V2.1追加 ＳＡ                                   #*/
/*#     更    新 : --/--/-- -                                               #*/
/*#     入    力 : BYTE *sptr：ファイルバージョンデータへのポインタ         #*/
/*#              : BYTE *dptr：データ格納エリアへのポインタ                 #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/
//void	lu_flver_make( BYTE *sptr, BYTE *dptr )
BYTE	lu_flver_make( BYTE *sptr, BYTE *dptr )		//2000.12.26 V2.2変更
{
	int	s;			/*##*/
	int	d;
	BYTE	f_flg;							//2000.12.26 V2.2追加

	d = 0;
//	f_flg = OFF;							//2000.12.26 V2.2変更
	f_flg = 0;								//2001.01.29 V2.2変更
	for(s=0; s<16; s++){
		switch(s){
			case 9:
			case 10:
			case 12:
			case 13:
				if(sptr[s] == ' ')			//2000.09.01 V2.1DEBUG
					dptr[d] = '0';			//2000.09.01 V2.1DEBUG
				else						//2000.09.01 V2.1DEBUG
					dptr[d] = sptr[s];
				d++;
				break;
			case 14:
				if(sptr[s] == '*'){			//2000.12.26 V2.2追加
//					f_flg = ON;				//2000.12.26 V2.2追加
					f_flg = 1;				//2001.01.29 V2.2変更
					break;
				}
				if(sptr[s] == '#'){			//2001.01.29 V2.2追加
					f_flg = 2;				//2001.01.29 V2.2追加
					break;
				}
			default:
				break;
		}
	}
	return(f_flg);							//2000.12.26 V2.2追加
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : lu_boot_save                                             #*/
/*#                                                                         #*/
/*#     日本語名 : ＬＵデータ保存                                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : -                                                        #*/
/*#     作    成 : 99/04/14 -                                               #*/
/*#     更    新 : 00/11/18 V2.1 オンライン側でもセーブできるようIPLより移動#*/
/*#                遠隔BOOTとデバック時のDRAM設定対策                       #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/

void	lu_boot_save(void)
{
	BYTE	*cp;
	int		 ccuip = OFF;								/*# ＣＣＵ ＩＰアドレス有無 #*/


	lu_boot_data.ccu1_ip=0;								// 2001.08 V2.3 追加
	lu_boot_data.ccu2_ip=0;								// 2001.08 V2.3 追加

														/*# ＬＵ ＩＰアドレス #*/
	memcpy( (char *)&lu_boot_data.yiaddr,				/* 転送先     */
			(char *)&lu_boot_data.btp_rep.btp_yiaddr,	/* 転送元     */
			4 );										/* 転送サイズ */

														/*# ファイルサーバ ＩＰアドレス #*/
	memcpy( (char *)&lu_boot_data.fiaddr,				/* 転送先     */
			(char *)&lu_boot_data.btp_rep.btp_siaddr,	/* 転送元     */
			4 );										/* 転送サイズ */

	memcpy( (char *)&lu_boot_data.ccu1_ip,				/* 転送先     */
			(char *)&lu_boot_data.btp_rep.btp_siaddr,	/* 転送元     */
			4 );										/* 転送サイズ */	// 2001.08 V2.3 追加

														/* ゲートウェイ */
	memcpy( (char *)&lu_boot_data.giaddr,				/* 転送先     */
			(char *)&lu_boot_data.btp_rep.btp_giaddr,	/* 転送元     */
			4 );										/* 転送サイズ */

	if (com_chgdword(lu_boot_data.btp_rep.btp_vend.v_magic) ==  BOOTP_MAGIC) {
	    cp = (BYTE *)&lu_boot_data.btp_rep.btp_vend.v_fields; /* point to individual fields*/
	    while ((*cp != END_TAG) && (cp <= &(lu_boot_data.btp_rep.btp_vend.v_fields[59]))) 
		{
			switch(*cp) {
/* Fixed lengths fields */
			case PAD_TAG:
				cp++;
				break;
			case END_TAG:
				break;
			case SUBNET_TAG: /* Subnet Mask */
				memcpy( (char *)&lu_boot_data.ipmask,	/* 転送先     */
						(char *)&cp[2],				 	/* 転送元     */
						4 );							/* 転送サイズ */
				cp += *(cp+1)+2;
				break;
/* Variable lengths fields */
			case GATEWAY_TAG: /* ゲートウェイ（最初の１つだけ保存） */
				memcpy( (char *)&lu_boot_data.giaddr,	/* 転送先     */
						(char *)&cp[2],				 	/* 転送元     */
						4 );							/* 転送サイズ */
				cp += *(cp+1)+2;
				break;
			case VEND_VLAN_TAG:
				lu_boot_data.vlan_id=cp[2];				/*# ＶＬＡＮ ＩＤ #*/
				lu_boot_data.class_a=cp[3];				/*# ＣＬＡＳＳ Ａ #*/
				lu_boot_data.class_b=cp[4];				/*# ＣＬＡＳＳ Ｂ #*/
				lu_boot_data.class_c=cp[5];				/*# ＣＬＡＳＳ Ｃ #*/
				lu_boot_data.class_d=cp[6];				/*# ＣＬＡＳＳ Ｄ #*/
				cp += *(cp+1)+2;
				break;
			case VEND_CCUIP_TAG: /*# ＣＣＵ ＩＰアドレス #*/
				memcpy( (char *)&lu_boot_data.siaddr,	/*# 転送先     #*/
						(char *)&cp[2],				 	/*# 転送元     #*/
						VEND_CCUIP_SIZE );				/*# 転送サイズ #*/
				memcpy( (char *)&lu_boot_data.ccu1_ip,	/*# 転送先     #*/	// 上書きする 
						(char *)&cp[2],				 	/*# 転送元     #*/
						VEND_CCUIP_SIZE );				/*# 転送サイズ #*/	// 2001.08 V2.3 追加
				ccuip = ON;
				cp += *(cp+1)+2;
				break;
			case VEND_CCUVER_TAG:
				lu_boot_data.ccu_ver = (WORD)(cp[2]&0x0f)<<8 | (WORD)(cp[3]&0x0f);
				cp += *(cp+1)+2;
				break;
			case VEND_CCU2_TAG: /*# ＣＣＵ２ ＩＰアドレス #*/
				memcpy( (char *)&lu_boot_data.ccu2_ip,	/*# 転送先     #*/
						(char *)&cp[2],				 	/*# 転送元     #*/
						VEND_CCUIP_SIZE );				/*# 転送サイズ #*/	// 2001.08 V2.3 追加
				cp += *(cp+1)+2;
				break;
			default:
				cp += *(cp+1)+2; /* pass over variable tag data */
				break;
			} /* end of switch */
		} /* end of while */
	}/* end of RFC_1084 if */

	/*# V1.1CCUではベンダ情報にCCUIPアドレスはなく、BOOTPサーバ＝ファイルサーバである #*/
	if(ccuip == OFF){
										/*# ファイルサーバ ＩＰアドレス #*/
		memcpy( (char *)&lu_boot_data.siaddr,				/* 転送先     */
				(char *)&lu_boot_data.btp_rep.btp_siaddr,	/* 転送元     */
				4 );										/* 転送サイズ */
	}


}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : lu_boot_make                                             #*/
/*#                                                                         #*/
/*#     日本語名 : ＢＯＯＰデータ作成                                       #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : BOOTPSRV機能でも使用するデータを作成する。               #*/
/*#                遠隔設定時にのみ考慮される事を前提                       #*/
/*#     作    成 : 00/09/-- - V2.1追加 ＳＡ                                 #*/
/*#     更    新 : 00/11/18 V2.1 オンライン側でも作成できるようIPLより移動  #*/
/*#                遠隔BOOTとデバック時のDRAM設定対策                       #*/
/*#     入    力 : -                                                        #*/
/*#     出    力 : -                                                        #*/
/*#     入 出 力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*###########################################################################*/

void	lu_boot_make(void)
{
	BYTE	*cp;
	int		 lcnt;

	const BYTE tag_tbl[] =	{ SUBNET_TAG,		GATEWAY_TAG,		VEND_VLAN_TAG,
							  VEND_CCUIP_TAG,	VEND_CCUVER_TAG,	VEND_CCU2_TAG,
							  END_TAG, 0};

	const BYTE size_tbl[] =	{ VEND_SUBNET_SIZE,	VEND_GATEWAY_SIZE,	VEND_VLAN_SIZE,
							  VEND_CCUIP_SIZE,	VEND_CCUVER_SIZE,	VEND_CCU2_SIZE,
						      0, 0};

														/*# ＬＵ ＩＰアドレス #*/
	memcpy( (char *)&lu_boot_data.btp_rep.btp_yiaddr,	/* 転送先     */
			(char *)&com_flsdata.data.yiaddr,			/* 転送元     */
			4 );										/* 転送サイズ */

														/*# ファイルサーバ ＩＰアドレス #*/
	memcpy( (char *)&lu_boot_data.btp_rep.btp_siaddr,	/* 転送先     */
			(char *)&com_flsdata.data.siaddr,			/* 転送元     */
			4 );										/* 転送サイズ */

														/* ゲートウェイ */
	memcpy( (char *)&lu_boot_data.btp_rep.btp_giaddr,	/* 転送先     */
			(char *)&com_flsdata.data.giaddr,			/* 転送元     */
			4 );										/* 転送サイズ */

	lu_boot_data.btp_rep.btp_vend.v_magic = BOOTP_MAGIC_LE;
	cp = (BYTE *)&lu_boot_data.btp_rep.btp_vend.v_fields; /* point to individual fields*/
	for( lcnt=0; tag_tbl[lcnt]; lcnt++ ){

		*cp     = tag_tbl[lcnt];
		*(++cp) = size_tbl[lcnt];
		++cp;											//2000.11.22 V2.1DEBUG

		switch(tag_tbl[lcnt]) {
		case SUBNET_TAG: /* Subnet Mask */
			memcpy( (char *)cp,							/* 転送先     */
					(char *)&com_flsdata.data.ipmask,	/* 転送元     */
					size_tbl[lcnt] );					/* 転送サイズ */
			break;
		case GATEWAY_TAG: /* ゲートウェイ（最初の１つだけ保存） */
			memcpy( (char *)cp,	/* 転送先     */
					(char *)&com_flsdata.data.giaddr,	/* 転送元     */
					size_tbl[lcnt] );					/* 転送サイズ */
			break;
		case VEND_VLAN_TAG:
			cp[0]   = com_flsdata.data.vlan_id;			/*# ＶＬＡＮ ＩＤ #*/
			cp[1]   = com_flsdata.data.class_a;			/*# ＣＬＡＳＳ Ａ #*/
			cp[2]   = com_flsdata.data.class_b;			/*# ＣＬＡＳＳ Ｂ #*/
			cp[3]   = com_flsdata.data.class_c;			/*# ＣＬＡＳＳ Ｃ #*/
			cp[4]   = com_flsdata.data.class_d;			/*# ＣＬＡＳＳ Ｄ #*/
			lu_boot_data.garp_vlan_id = cp[0];			/*# ＧＡＲＰ ＶＬＡＮ－ＩＤ #*/
														/*# BOOTPサーバ動作ありの場合はBOOTしない為fusion:boot()処理の代行 #*/
			break;
		case VEND_CCUIP_TAG: /*# ＣＣＵ ＩＰアドレス #*/
			memcpy( (char *)cp,							/*# 転送先     #*/
					(char *)&com_flsdata.data.siaddr,	/*# 転送元     #*/
					size_tbl[lcnt] );					/*# 転送サイズ #*/
			break;
		case VEND_CCUVER_TAG:
			memcpy( (char *)cp,							/*# 転送先     #*/
					(char *)&com_flsdata.data.ccu_ver,	/*# 転送元     #*/	//フラッシュに保存していないため不定値
					size_tbl[lcnt] );					/*# 転送サイズ #*/	//ＩＰＬでのバージョン参照なし
			break;
		case VEND_CCU2_TAG: /*# ＣＣＵ２ アドレス #*/
			if((com_flsdata.data.siaddr2 != 0) &&(com_flsdata.data.siaddr2 != 0xffffffff)){
				memcpy( (char *)cp,							/*# 転送先     #*/
						(char *)&com_flsdata.data.siaddr2,	/*# 転送元     #*/
						size_tbl[lcnt] );					/*# 転送サイズ #*/
			}
			else{
				cp -= (VEND_CCU2_SIZE+2);
			}
			break;
		case END_TAG:
			break;
		default:
			break;
		} /* end of switch */
		cp += size_tbl[lcnt];
	} /* end of while */
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 :                                             				 #*/
/*#                                                                         #*/
/*#     日本語名 : D01判定                               					#*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :  BOOT_DATAの拡張フラグをみてD01ビットが立ってるならONを返す#*/
/*#     入    力 : --                                       				#*/
/*#     リターン : ON(D01)またはOFF(CCU)                                        #*/
/*#                                                                         #*/
/*#     作    成 : 04/01/05 - LU32B追加SA                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/

BYTE	lu_D01( )
{

//D01 1コール用にデバッグ時は強制的にONを返すように変更 2004.1.6//デバッグルート変更2004.04 IP-CS
#ifndef V2_1_DBG
	if((lu_boot_data.extension & EXT_D01) == 1){
//	if((com_flsdata.data.extension & EXT_D01) == 1){//2004.05 IP-CS

		return ON;
	}else{
		return OFF;
	}
#else
//	return ON;
	if((com_flsdata.data.extension & EXT_D01) == 1){//2004.05 IP-CS
		return ON;
	}else{
		return OFF;
	}


#endif		
}

/*# SOURCE_HEADER #*/
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 :                                             				 #*/
/*#                                                                         #*/
/*#     日本語名 : SS9100判定                               					#*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : com_flsdataの拡張フラグをみてSS9100ビットが立ってるならONを返す#*/
/*#     入    力 : --                                       				#*/
/*#     リターン : ON(SS9100)またはOFF                                        #*/
/*#                                                                         #*/
/*#     作    成 : 04/08/24 - SS9100追加SA                                    #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/

BYTE	lu_SS9100( )
{

	if(com_flsdata.data.extension & EXT_SS9100){//2004.08 SS9100
		return ON;
	}else{
		return OFF;
	}

}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_pthreadinit                                          #*/
/*#                                                                         #*/
/*#     日本語名 : スレッド初期化                                           #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : スレッド状態テーブルを0で初期化する。                    #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : -                                                        #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_pthreadinit(void)
{
	/* タスク状態テーブルの全スレッドの状態を停止中に設定 */
	memset((void *)task_status, 0, sizeof(task_status));
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_pthreadstart                                         #*/
/*#                                                                         #*/
/*#     日本語名 : スレッドスタート                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたスレッドIDのスレッドを生成し、起動する。       #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : スレッドID(BYTE)                                         #*/
/*#                スレッド起動時の引数(int *)                              #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_threadstart(BYTE thread_id, void *arg)
{
	int ret_code;
	struct sched_param param;

	/* スレッドIDをチェック */
	if (thread_id >= COM_TASK_MAX_N) {
		/* スレッドID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_15);
	}
	/* スレッド状態をチェック */
	if (task_status[thread_id].status == TASK_ACTIVE) {
		/* スレッドが既に起動されている時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_16);
	}
	
	/* スレッド属性エリアを初期化する */
	ret_code = pthread_attr_init(&task_status[thread_id].thread_attr);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_attr_init ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_17);
	}
	/* 属性をPTHREAD_CREATE_JOINABLE(スレッドは join 可能な状態で作成される)に変更する */
	ret_code = pthread_attr_setdetachstate(&task_status[thread_id].thread_attr, PTHREAD_CREATE_JOINABLE);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_attr_setdetachstate ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_18);
	}
	/* 属性をPTHREAD_EXPLICIT_SCHED(スレッド属性オブジェクトで 指定された値からスケジューリング属性を取得する)に変更する */
	ret_code = pthread_attr_setinheritsched(&task_status[thread_id].thread_attr, PTHREAD_EXPLICIT_SCHED);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_attr_setinheritsched ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_19);
	}
	/* スケジューリングポリシー属性をSCHED_FIFOに変更する */
	ret_code = pthread_attr_setschedpolicy(&task_status[thread_id].thread_attr, SCHED_FIFO);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_attr_setschedpolicy ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_20);
	}
	/* 優先度属性を登録された値に変更する */
	param.sched_priority = task_attr[thread_id].task_pri;
	ret_code = pthread_attr_setschedparam(&task_status[thread_id].thread_attr, &param);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_attr_setschedparam ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_21);
	}
	/* スレッドを生成&起動する */
	ret_code = pthread_create(&task_status[thread_id].thread_id, &task_status[thread_id].thread_attr, (void *)task_attr[thread_id].start_routine, arg);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_create ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_22);
	}
	/* スレッド状態を起動中に変更する */
	task_status[thread_id].status = TASK_ACTIVE;
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : com_pthreadstop                                          #*/
/*#                                                                         #*/
/*#     日本語名 : スレッド停止                                             #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : 指定されたスレッドIDを停止する。                         #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : スレッドID(BYTE)                                         #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	com_threadstop(BYTE thread_id)
{
	int ret_code;

	/* スレッドIDをチェック */
	if (thread_id >= COM_TASK_MAX_N) {
		/* スレッドID異常時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_23);
	}
	/* スレッド状態をチェック */
	if (task_status[thread_id].status == TASK_INACTIVE) {
		/* スレッドが未起動の場合時は、システムエラーを起動 */
		com_syserr(COM_ID, COM_ABORT_24);
	}
	
	/* スレッドを停止する */
	ret_code = pthread_cancel(task_status[thread_id].thread_id);
	if (ret_code != 0) {
		dbg_print(COM_ID, LOG_ERR, "%s thread_id=%d pthread_cancel ret_code=%d\n", __func__, thread_id, ret_code);
		com_syserr(COM_ID, COM_ABORT_25);
	}
	/* スレッド状態を非起動中に変更する */
	task_status[thread_id].status = TASK_INACTIVE;
}

/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dbg_print_set                                            #*/
/*#                                                                         #*/
/*#     日本語名 : デバッグプリント設定                                     #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : デバッグプリントの設定を行う。                           #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : スレッドID(BYTE)                                         #*/
/*#                ログレベル(DWORD)                                        #*/
/*#                出力先(DWORD)                                            #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void	dbg_print_set(BYTE thread_id, DWORD log_level, DWORD output_dst)
{
	if (thread_id == COM_ID) {
		/* スレッドIDがCOMの場合 */
		com_dbgprint_ctl.log_level = log_level;
		com_dbgprint_ctl.output_dst = output_dst;
	}
	else {
		/* スレッドIDをチェック */
		if (thread_id >= COM_TASK_MAX_N) {
			/* スレッドID異常時は、システムエラーを起動 */
			com_syserr(COM_ID, COM_ABORT_26);
		}
		/* スレッドIDがCOM以外の場合 */
		dbgprint_ctl[thread_id].log_level = log_level;
		dbgprint_ctl[thread_id].output_dst = output_dst;
	}
}
	
/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 : dbg_print                                                #*/
/*#                                                                         #*/
/*#     日本語名 : デバッグプリント                                         #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 : デバッグプリントを設定されている出力先に出力する。       #*/
/*#                                                                         #*/
/*#                PID          -                                           #*/
/*#                Priorty      -                                           #*/
/*#                Queue Kind   -                                           #*/
/*#     入    力 : スレッドID(BYTE)                                         #*/
/*#                ログレベル(DWORD)                                        #*/
/*#                出力フォーマット(const char *)                           #*/
/*#                可変長引数                                               #*/
/*#     リターン : -                                                        #*/
/*#                                                                         #*/
/*#     作    成 : 20/11/27 - 小澤  美津晴                                  #*/
/*#     更    新 : --/--/-- - -                                             #*/
/*#                                                                         #*/
/*###########################################################################*/
void dbg_print(BYTE thread_id, int log_level, const char *format, ...)
{
	int _log_level;
	DWORD _output_dst;
	va_list ap;

	if (thread_id == COM_ID) {
		/* スレッドIDがCOMの場合 */
		_log_level = com_dbgprint_ctl.log_level;
		_output_dst = com_dbgprint_ctl.output_dst;
	}
	else {
		/* スレッドIDをチェック */
		if (thread_id >= COM_TASK_MAX_N) {
			/* スレッドID異常時は、システムエラーを起動 */
			com_syserr(COM_ID, COM_ABORT_27);
		}
		/* スレッドIDがCOM以外の場合 */
		_log_level = dbgprint_ctl[thread_id].log_level;
		_output_dst = dbgprint_ctl[thread_id].output_dst;
	}

	/* レベルをチェックし、出力不要な場合はリターンする */
	if (log_level > _log_level) {
		return;
	}
	/* 可変長引数を１個の変数にまとめる */
	va_start( ap, format );
	/* 出力先がコンソールの場合は、コンソールに出力する */
	if ((_output_dst & LOGDST_CNS) == LOGDST_CNS) {
		vfprintf(stdout, format, ap );
	}
	/* 出力先がsyslogの場合は、syslogに出力する */
	if ((_output_dst & LOGDST_SYSLOG) == LOGDST_SYSLOG) {
		vsyslog((int)log_level, format, ap );
	}
    va_end( ap );
}
/*        Copyright (C) 1999 Oki Electric Industry Co.,Ltd.        */
