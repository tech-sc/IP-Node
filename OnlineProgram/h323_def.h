/*###########################################################################*/
/*#                                                                         #*/
/*# モジュール名 :                                                          #*/
/*#                                                                         #*/
/*#     日本語名 :                                                          #*/
/*#     参照番号 : -                                                        #*/
/*#     単位区分 : -                                                        #*/
/*#     概    要 :                                                          #*/
/*#     作    成 : 99/02/13 - 毛塚  照明                                    #*/
/*#     更    新 : --/--/--                                                 #*/
/*#                                                                         #*/
/*###########################################################################*/

#ifndef _H232_DEF_H
#define _H232_DEF_H
extern			COM_DATA	com_data;
extern	LU_BOOT_DAT	lu_boot_data;					/*# ＩＰＬブートデータ #*/
extern	DSPINT_VOICECTL	dspint_voicectl[];			/*# 音声制御用メモリ #*/
extern	RCV_RTP_FRM		rcv_rtp_buf[];				/*# RTP受信音声バッファ #*/	// 99.10.21
extern	LU_LOG	lu_log_inf;								/*# ＬＵログ情報 #*/	// 2001.03.15追加

typedef void (*H323FuncAddr)( BYTE * );

extern	void	h323_snd00(BYTE*);
extern	void	h323_snd11(BYTE*);
extern	void	h323_snd21(BYTE*);
extern	void	h323_snd30(BYTE*);
extern	void	h323_snd31(BYTE*);
extern	void	h323_snd99(BYTE*);

extern	void	h323_rcv00(BYTE*);
extern	void	h323_rcv11(BYTE*);
extern	void	h323_rcv21(BYTE*);
extern	void	h323_rcv31(BYTE*);
extern	void	h323_rcv41(BYTE*);
extern	void	h323_rcv51(BYTE*);//SS9100追加 2004.10
extern	void	h323_rcv99(BYTE*);

extern	void	h323(INNER_MSG *msg);
extern	void	time_led(char);

extern	void	h323_mkrtcprr(BYTE,BYTE**);
extern	void 	h323_mkrtcpsdes_rcv(BYTE,BYTE**);

//SS9100 2004.11.10
//extern	void 	h323_mkrtcprrb(BYTE,BYTE*);
extern	void 	h323_mkrtcprrb(BYTE,BYTE*,BYTE);
extern	BYTE 	h323_sndrcv_you_chk(BYTE);

/* IPCS_V4 <PD><ADD-STA> */
void	h323_rcvcallback(DWORD sd,DWORD ip,WORD port,BYTE *msg,DWORD len);
void	h323_rcvcallback_rtcp(DWORD sd,DWORD ip,WORD port,BYTE *msg,DWORD len);
void	h323_voicesnd(BYTE *msg);
void	h323_socketng_post(BYTE	cn,BYTE	result);
void	h323_sdata_cls(BYTE	cn);
void	h323_rdata_cls(BYTE	cn);
void	h323_channel_info_cls(BYTE	cn);
/* IPCS_V4 <PD><ADD-END> */



#define			H323_END		0
#define			H323_SND		1

#define			H323_NORMAL		0
#define			H323_LOOP		1
#define			H323_MULTI		2
#define			H323_MLOOP		3

//#define			H323_CNMAX		LU_CHANNEL_MAX	/*#  #*/
#define			H323_CNMAX		CNMAX	/*# 2004.1.15 DEFINE戻し #*/

#define			H323_SNDPASS	0		/*# 送信=0、受信=1、双方向=2#*/
#define			H323_RCVPASS	1
#define			H323_BOTHPASS	2

#define			H323_SDESSND	5		/*# SDES送出間隔(単位秒) #*/
//#define			H323_SDESRCV	30		/*# SDES受信ﾀｲﾑｱｳﾄ(単位秒) #*/
#define			H323_SDESRCV	10		/*# SDES受信ﾀｲﾑｱｳﾄ(単位秒) #*/
#define			H323_RRSND		5		/*# RR送信ﾀｲﾑｱｳﾄ(単位秒) #*///SS9100追加 2004.10

#define			IPV4			0
#define			IPV6			1

#define			H323_OPENNG		1
#define			H323_SDESTO		2
#define			H323_BYERCV		3
#define			H323_CLOSENG	4
#define			H323_ERRPKT	5

/*# RTCP Packet Type #*/
#define		RT_RTCP_PT_SR		200
#define		RT_RTCP_PT_RR		201
#define		RT_RTCP_PT_SDES		202
#define		RT_RTCP_PT_BYE		203
#define		RT_RTCP_PT_APP		204

//#define		RT_SR_HEAD			0x80
#define		RT_SR_HEAD			0x81//SS9100変更 2004.10

#define		RT_SDES_HEAD		0x81
#define		RT_BYE_HEAD			0x81
#define		RT_RTCPRCV_HEAD		0x80
#define		RT_RTCPRCV_VPRC		0x81//SS9100追加 2004.10
#define		RT_RR_HEAD			0x81//SS9100追加 2004.10
#define		RT_RTP_HEAD			0x80
#define		RT_RTP_PT_ULAW		0
#define		RT_RTP_PT_ALAW		0x08

//圧縮LU対応。コーデックヘッダ追加。2003.09.07
#define		RT_RTP_PT_G723L_LAW		0x04
#define		RT_RTP_PT_G723H_LAW		0x04
#define		RT_RTP_PT_G729_LAW		0x12

#define		RT_ITEM_NUM			9
#define		RT_ITEM_SIZE		32
#define		RT_ITEM_CNAME		1
#define		RT_DATSIZEMAX		1472
#define		RT_RTCPSIZEMAX		256
#define		RTP_HEAD_SIZE	12

typedef struct T_RTCPDAT{
	BYTE	head;						/*# 0x00:V(2)/P(1)/SC(5) #*/
	BYTE	pt;							/*# 0x01:Packet Type #*/
	WORD	len;						/*# 0x02:length #*/
	BYTE	dat[1];						/*# 0x0c:data area #*/
}RTCPDAT;

#define		LUAQNTC_BC		35			/*#  音声情報通知イベントBC #*///SS9100追加 2004.10
//SS9100追加 start 2004.10
typedef	struct	T_H323_CHANNEL_INFO{
	BYTE		notice_flg;
	BYTE		overflow;
	WORD		seq_expcted;
	DWORD 		rtp_rcvcnt;
	DWORD 		rtp_sndcnt;
	DWORD 		delay_min;
	DWORD 		delay_max;
	DWORD		seq_gap_cnt;
	DWORD 		sr_ntp32;
	DWORD 		rrrcv_cnt;
	DWORD 		delay_total;

}H323_CHANNEL_INFO;//SS9100追加 2004.10

typedef struct T_RTCP_COMMON_T{
	BYTE		vprc;
	BYTE		pt;
	WORD		length;
	DWORD		ssrc;

} RTCP_COMMON_T;



typedef struct T_RECEIVER_RPT_B {
	DWORD			ssrc_1;       /* data source being reported */
	DWORD			lost;   /* cumulative number of packets lost */
	DWORD			recieved;   /* cumulative number of packets received */
	DWORD			jitter;     /* interarrival jitter */
	DWORD			lsr;        /* last SR packet from this source */
	DWORD			dlsr;       /* delay since last SR packet */
} RECEIVER_RPT_B;

typedef struct T_SENDER_RPT{
	DWORD 		ntp_sec;    /* NTP timestamp */
	DWORD 		ntp_frac;
	DWORD 		rtp_ts;     /* RTP timestamp */
	DWORD 		psent;      /* packets sent */
	DWORD 		osent;      /* octets sent */ 
		
	RECEIVER_RPT_B 	rr[1];		/* variable-length list */
} SENDER_RPT;

/* one RTCP packet */
typedef struct T_RTCP_T{
	RTCP_COMMON_T	common;     /* common header */
	
	union 
		{
		SENDER_RPT		sr;		/* sender report (SR) */
		RECEIVER_RPT_B	rr[1];		/* reception report (RR) */
		} r;
} RTCP_T;
//SS9100追加 end 2004.10
/* IPCS_V4 <PD><ADD-STA> */
typedef struct	H323_DATA{
	BYTE	sts;											/*# 状態 #*/
	BYTE	timerid;										/*# rtcp制御用 #*/
	BYTE	timerid2;										//SS9100追加 2004.10
	BYTE	exe_sts;										/*# 動作種別 #*/
	BYTE	byercv;											/*# BYE受信 #*/
	BYTE	rcvflg;											/*# 初回受信FLG #*/
	BYTE	acm_flg;										/*#  #*/
	BYTE	sndcnt;											/*# パケット送信周期 #*/
	BYTE	mult_ttl;										/*# マルチキャストTTL #*//* IPCS_V4 <PD><ADD> */
	BYTE	dmyb[3];										/* IPCS_V4 <PD><CHG> */
	WORD	myport;											/*# 自PORT番号 #*/
	WORD	youport;										/*# 相手PORT番号 #*/
	WORD	rtp_seq;										/*# ｼｰｹﾝｽ番号 #*/
	WORD	dmyw;
//	DWORD	ssrc;											/*# SSRC #*/
//SS9100追加 start 2004.10
	DWORD	my_ssrc;											/*# 自SSRC #*/
	DWORD	you_ssrc;											/*# 相手SSRC #*/
//SS9100追加 end 2004.10

	DWORD	myip;											/*# 自IPアドレス #*/
	DWORD	youip;											/*# 相手IPアドレス #*/
	DWORD	rtp_time;										/*# ﾀｲﾑｽﾀﾝﾌﾟ #*/
	DWORD	rtp_sd;											/*# RTPｿｹｯﾄﾃﾞｨｽｸﾘﾌﾟﾀ #*/
	DWORD	rtcp_sd;										/*# RTCPｿｹｯﾄﾃﾞｨｽｸﾘﾌﾟﾀ #*/
	DWORD	rtp_err;										/*# rtpﾊﾟｹｯﾄｴﾗｰ数(V1) #*/
	DWORD	rtcp_err;										/*# rtcpﾊﾟｹｯﾄｴﾗｰ数(V1) #*/
	DWORD	timer_cnt;//SS9100追加 2004.10					/*# 送：呼開始時刻、受：最終SR到着時刻 #*/
	BYTE	cname[32];										/*# CNMAE #*/
	BYTE	sdes_item[RT_ITEM_NUM][RT_ITEM_SIZE];			/*# sdes_item #*/
	//圧縮LU追加。20040209
	BYTE	rtcp_exe;										/*# RTCP実施/未実施状態 #*/
}h323_data;
/* IPCS_V4 <PD><ADD-END> */
#endif /* _H232_DEF_H */
