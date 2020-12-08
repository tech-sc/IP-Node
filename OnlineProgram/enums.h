/********************************************************************
 * AudioCodes improved application enumerators definitions          *
 *                                                                  *
 * File Name:	enums.h                                             *
 *                                                                  *
 * Copyright (C) 2008 AudioCodes Ltd.                               *
 *                                                                  *
 * Modifications :                                                  *
 *                                                                  *
 * 25/02/08 - Coding started.                                       *
 *																	*
 ********************************************************************/


#ifndef	_ENUMS_H
#define	_ENUMS_H

#include "voip_gen_defs.h"

typedef enum {
	VOIP_PROT_SIP = 0, /*"sip", "SIP"*/
	VOIP_PROT_H323 = 1, /*"h.323", "H.323"*/
	VOIP_PROT_MGCP = 2  /*"mgcp", "MGCP"*/    
} voip_protocols_t;

typedef enum {
	/* Note: the following enum values are the real voip payload-type values */
	VOIP_SIP_TRANSPORT_UDP = 1, /*"udp", "UDP"*/
	VOIP_SIP_TRANSPORT_TCP = 2, /*"tcp", "TCP"*/	
	/*  EA - 071220 - VI#59630 support SIP over TLS */
    	VOIP_SIP_TRANSPORT_TLS = 3 /*"tls", "TLS"*/
} voip_sip_transport_prot_t;

typedef enum {

	ILLEGAL_RTP_PAYLOAD = -1,
		
	RTP_PAYLOAD_PCMU = 0,   /*"PCMU", "pcmu", "g711u", "G711U", "g.711u", "G.711U"*/
	RTP_PAYLOAD_G726_32 = 2, /*"g726-32", "G726-32", "g.726-32", "G.726-32"*///Nirp Vocoders 726 has 2 for payload type according to 3551
	RTP_PAYLOAD_G723 = 4,   /*"g723", "G723", "g.723", "G.723"*/
	RTP_PAYLOAD_PCMA = 8,   /*"PCMA", "pcma", "g711a", "G711A", "g.711a", "G.711A"*/
	RTP_PAYLOAD_G722 = 9,   /*"G722", "g722", "G.722", "g.722"*/
	RTP_PAYLOAD_CN = 13,  /*"CN", "cn"*/ /* Comfort Noise */    
	RTP_PAYLOAD_G728 = 15,  /*"G728", "g728", "G.728", "g.728"*/
	RTP_PAYLOAD_G729 = 18,  /*"G729", "g729", "G.729", "g.729"*/
#if 0//NIRP 070905
	RTP_PAYLOAD_ILBC = 97,  /*"iLBC", "ILBC", "ilbc"*///Nirp Vocoders dynamic, must be >= 96
#endif
	RTP_PAYLOAD_G726_16 = 98,  /*"g726-16", "G726-16", "g.726-16", "G.726-16"*///Nirp Vocoders dynamic, must be >= 96
#if 0//NIRP 070905
	RTP_PAYLOAD_AMR = 99,  /*"AMR"*///Nirp Vocoders dynamic, must be >= 96
#endif
	RTP_PAYLOAD_DTMF = 101 /*"telephone-event"*/ /* dynamic, must be >= 96 */

} rtp_payload_type_t;


typedef enum {

	CALL_STATE_IDLE = 0, /*"Idle"*/
	CALL_STATE_DIALING = 1, /*"Dialing"*/
	CALL_STATE_CALLING = 2,/*"Calling"*/
	CALL_STATE_RINGING = 3, /*"Ringing"*/
	CALL_STATE_ANSWERING = 4, /*"Answering"*/
	CALL_STATE_CONNECTED = 5, /*"Connected"*/
	CALL_STATE_HOLD = 6, /*"Hold"*/
	CALL_STATE_REORDER = 7, /*"Reorder"*/
	CALL_STATE_DISCONNECTED = 8 /*"Disconnected"*/
} call_state_t;

typedef enum {
	CALL_ORIGIN_INCOMING = 0, /*"Incoming"*/
	CALL_ORIGIN_OUTGOING =1, /*"Outgoing"*/
	CALL_ORIGIN_UNKNOWN = 2 /*"Unknown"*/
} call_origin_t;

typedef enum {
	SIP_REG_REGISTERED = 0, /*"Registered"*/
	SIP_REG_UNREGISTERED = 1, /*"Not Registered"*/
	SIP_REG_ERROR = 2, /*"Registration Error"*/
	SIP_REG_UNAUTHENTICATED = 3, /*"Authentication Error"*/
	SIP_REG_INPROG = 4 /*"In Progress..."*/
} sip_reg_state_t;


typedef enum {
	PHONE_STATE_DISABLED = 0, /*"Disabled"*/
	PHONE_STATE_ONHOOK = 1, /*"On Hook"*/
	PHONE_STATE_OFFHOOK = 2, /*"Off Hook"*/
	PHONE_STATE_RINGING = 3 /*"Ringing"*/
} phone_state_t;

typedef enum {
	CALL_TYPE_VOICE = 0, /*"Voice"*/
	CALL_TYPE_FAX = 1 /*"Fax"*/
} call_type_t;

typedef enum {
	OUT_OF_SERVICE_TONE_NONE = 0, /*"No Tone"*/
	OUT_OF_SERVICE_REORDER_TONE = 1, /*"Reorder Tone"*/
	OUT_OF_SERVICE_DIAL_TONE = 2	/* Dial Tone */
} out_of_service_tone_t;

typedef enum {
	FAX_MAX_RATE_2_4_KBPS = 0, /*"2.4 Kbps"*/
	FAX_MAX_RATE_4_8_KBPS = 1, /*"4.8 Kbps"*/
	FAX_MAX_RATE_7_2_KBPS = 2, /*"7.2 Kbps"*/
	FAX_MAX_RATE_9_6_KBPS = 3, /*"9.6 Kbps"*/
	FAX_MAX_RATE_12_KBPS = 4,  /*"12 Kbps"*/
	FAX_MAX_RATE_14_4_KBPS = 5/*"14.4 Kbps"//default value*/
} fax_max_rate_t;

typedef enum {
	KEY_SEQ_FLASH_ONLY = 0, /*"Flash only"*/
	KEY_SEQ_FLASH_AND_DIGITS = 1, /*"Flash + digits"*/
	KEY_SEQ_REMOTE_FLASH = 2,  /*"Send Flash Hook via SIP"*//* ACL NB 070516 - AC#53280 Send Hook-flash */
	KEY_SEQ_FLASH_FOR_MGCP = 3	/* AlexChen SDK upgrade 20090415 */
} key_sequence_style_t;

typedef enum {
	ANALOG_PORT_TYPE_FXS = 0, /*"Analog Port Type FXS"*/
	ANALOG_PORT_TYPE_FXO = 1, /*"Analog Port Type FXO"*/
	ANALOG_PORT_TYPE_FXO_FXS = 2  /*"Analog Port Type FXO and FXS"*/
} analog_port_type_t;

typedef enum {
	CFW_CFU = 0, /*"Unconditional"*/
	CFW_CFB = 1, /*"Busy"*/
	CFW_CFNR = 2 /*"No-Reply"*/
} cfw_type_t;

typedef enum {
	DTMF_INBAND = 0, /*"inband"*/
	DTMF_RFC2833 = 1, /*"rfc2833"*/
	DTMF_VIA_SIP = 2 /*"via SIP"*/
} dtmf_transport_mode_t;

typedef enum {
	T38_TRANSPORT_DISABLE = 0, /*"Transparent"*/
	T38_TRANSPORT_RELAY = 1, /*"T.38Relay"*/
	T38_VOICE_BAND_DATA = 2, /*"Voice-Band-Data"*/
	T38_TRANSPORT_BYPASS = 3 /*"Bypass"*/
} t38_transport_mode_t;

/* ACL NIRP 070401 VI 51493 */
/* ACL NB 061213 */
typedef enum {
	DATA_TRANSPORT_DISABLE = 0x10, /*"Transparent"*/
	DATA_VOICE_BAND_DATA = 0x20, /*"Voice-Band-Data"*/
	DATA_TRANSPORT_BYPASS = 0x40 /*"Bypass"*/
} data_transport_mode_t;

typedef enum {
	FAX_TRANSPORT_DISABLE = 0, /*"Transparent"*/
	FAX_TRANSPORT_RELAY = 1, /*"T.38Relay"*/
	FAX_VOICE_BAND_DATA = 2, /*"Voice-Band-Data"*/
	FAX_TRANSPORT_BYPASS = 3 /*"Bypass"*/
} fax_transport_mode_t;

typedef enum {
    CW_SIP_REPLY_RINGING = 180,   /*"Ringing"*/
    CW_SIP_REPLY_QUEUED = 182,    /*"Queued"*/
} cw_sip_reply_t;

typedef enum {
    CONF_LOCAL = 0,     /*"Local"*/
    CONF_REMOTE = 1,    /*"Remote"*/
} conf_mode_t;

typedef enum {
    VOIP_DESTINATION_TYPE_PROXY = 0,    /*"proxy", "VOIP_DESTINATION_TYPE_PROXY"*/
    VOIP_DESTINATION_TYPE_LOCAL = 1,    /*"local", "VOIP_DESTINATION_TYPE_LOCAL"*/
    VOIP_DESTINATION_TYPE_DIRECT = 2,   /*"direct", "VOIP_DESTINATION_TYPE_DIRECT"*/
} voip_destination_type_t;

typedef enum {
    AUTO_GAIN_CTL_REMOTE = 0, /*"For Remote User"*/
    AUTO_GAIN_CTL_LOCAL = 1, /*"For Local User"*/
} auto_gain_ctl_mode_location_t;

extern code2str_ex_t rtp_payload_type_t_str[];
extern code2str_ex_t voip_sip_transport_prot_t_str[];
extern code2str_ex_t voip_protocols_t_str[];
extern code2str_ex_t dtmf_transport_mode_t_str[];
extern code2str_ex_t key_sequence_style_t_str[];
extern code2str_ex_t analog_port_type_t_str[];
extern code2str_ex_t cw_sip_reply_t_str[];
extern code2str_ex_t conf_mode_t_str[];
extern code2str_ex_t cfw_type_t_str[];
extern code2str_ex_t out_of_service_tone_t_str[];
extern code2str_ex_t fax_max_rate_t_str[];
extern code2str_ex_t t38_transport_mode_t_str[];
extern code2str_ex_t data_transport_mode_t_str[];
extern code2str_ex_t voip_destination_type_t_str[];
extern code2str_ex_t auto_gain_ctl_mode_location_t_str[];

#endif /*_ENUMS_H */

