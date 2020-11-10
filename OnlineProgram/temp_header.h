typedef struct  T_MSG_HEADER{           /*# 内部メッセージヘッダ #*/
    DWORD       sys;                    /*# システムデータ（ＯＳ用） #*/
    BYTE        id;                     /*# 送信元タスクＩＤ #*/
    BYTE        div;                    /*# メッセージ区分 #*/
    BYTE        kind;                   /*# メッセージ種別 #*/
    BYTE        no;                     /*# ＰＫＧ／ＣＮ＿ＮＯ #*/
    BYTE        *link;                  /*# ２次リンクアドレス #*/
}MSG_HEADER;

typedef struct  T_INNER_MSG{        /*# 内部メッセージフォーマット #*/
    MSG_HEADER  msg_header;
    union   {
/*          BYTE    user[16];   # ＵＳＥＲ領域 #*/
            BYTE    user[32];   /*# ＵＳＥＲ領域 #*//* 2003.10.23 tamu kari */
        }user_data;
}INNER_MSG;

#define I_SERVER_REQ	0xf0
#define I_SERVER_RESP	0xf1
#define I_CLIENT_REQ	0xf2
#define I_CLIENT_RESP	0xf3
#define I_WRITE_REQ	0xf4
#define I_WRITE_RESP	0xf5

#define DWL_ID	0x10
#define WRT_ID	0x11

#define	LOG_EMERG	0		/* system is unusable */
#define	LOG_ALERT	1		/* action must be taken immediately */
#define	LOG_CRIT		2		/* critical conditions */
#define	LOG_ERR		3		/* error conditions */
#define	LOG_WARNING	4		/* warning conditions */
#define	LOG_NOTICE	5		/* normal but significant condition */
#define	LOG_INFO		6		/* informational */
#define	LOG_DEBUG	7		/* debug-level messages */

#define LOGDST_CNS    1 /* Output to console */
#define LOGDST_SYSLOG 2 /* Output to syslog */

#define WRITER_ECB	0x30

extern BYTE *com_poolget(BYTE id);
extern void com_poolput(BYTE id, BYTE *p);
extern void com_sndmsg(BYTE ecb, BYTE *g);
extern BYTE *com_rcvmsg(BYTE ecb, WORD tim);
extern void com_threadstart(BYTE id, void *p);
extern void dbg_print_set(BYTE i,BYTE v,BYTE z);
extern void dbg_print(BYTE i,BYTE v,char *p,...);

extern BYTE mnt_config_dat(char *path, BYTE write);


