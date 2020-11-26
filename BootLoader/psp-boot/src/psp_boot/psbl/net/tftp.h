/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission is hereby granted to licensees of Texas Instruments           |**
**| Incorporated (TI) products to use this computer program for the sole     |**
**| purpose of implementing a licensee product based on TI products.         |**
**| No other rights to reproduce, use, or disseminate this computer          |**
**| program, whether in part or in whole, are granted.                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    TFTP header file.
 *******************************************************************************
 * FILE NAME:       tftp.h
 *
 * DESCRIPTION:     TFTP header file.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#ifndef _TFTP_H
#define _TFTP_H

#define TFTP_SVR_PORT     69
#define TFTP_CLIENT_PORT  2000

typedef struct tftp_pkt {
    u2     opcode;
	char   rrq[1];
} tftp_pkt;

Status tftp_req(char *getfile, u2 src_port, u2 dst_port );
Status tftp_send_ack(u2 block_no, u2 src_port, u2 dst_port);
Status tftp_get_file(char *getfile, char *dstfile, u2 dst_port);
Status tftp_in(char *dgram, u2 dst_port, u2 src_port, u4 len);
int tftp(int argc, char *argv[]);

/* IPCS-V4 PG ADD-STA */
Status tftpd_put_file(char *getfile, char *dstfile, u2 dst_port);
int tftpd(int argc, char *argv[]);
/* IPCS-V4 PG ADD-END */

#endif /* _TFTP_H */
