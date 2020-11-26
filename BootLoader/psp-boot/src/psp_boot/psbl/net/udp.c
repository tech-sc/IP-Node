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
 * FILE PURPOSE:    UDP interfaces.
 *******************************************************************************
 * FILE NAME:       udp.c
 *
 * DESCRIPTION:     UDP interfaces.
 *
 * (C) Copyright 2003, Texas Instruments, Inc
 ******************************************************************************/

#include <psbl/ffs.h>
#include <psbl/stdio.h>
#include <psbl/heapmgr.h>
#include <psbl/debug.h>
#include "net.h"
#include "ether.h"
#include "arp.h"
#include "ip.h"
#ifdef IPV6
#include "ip6.h"
#endif
#include "udp.h"
#include "tftp.h"
#include "tinyip.h"

extern u4 TFTP_LOCAL_PORT; /* Contains dynamic TFTP client port */
extern int dhcp_in(char *frame_ptr, int data_len);

Status udp_send(u1 dst_ip[IP_ADDR_LEN], u2 src_port, u2 dst_port, netbuf frame, u4 sz)
{
	udp_header *udph;

	udph = (udp_header *)(frame + ET_HDR_SZ + iph_sz + 2);

	udph->src_port = hs2net(src_port);
	udph->dst_port = hs2net(dst_port);
	udph->len      = hs2net(sz + UDP_HDR_SZ);
	udph->cksum    = 0;
	udph->cksum    = hs2net( chksum( sys_ip_addr, dst_ip, ( netbuf )udph,
                                     sz + UDP_HDR_SZ, IP_UDP) );
    udph->cksum    = udph->cksum ? udph->cksum : 0xFFFF;

#ifdef IPV6
    if (addr_family == IPA_IPV6)
	    ip6_send (dst_ip, IP_UDP, HOP_LIMIT_DEFAULT, frame, sz + UDP_HDR_SZ);
    else
#endif
	ip_send(dst_ip, IP_UDP, 60, frame, sz + UDP_HDR_SZ);	
	return SBL_SUCCESS;
}

Status udp_in (char *dgram, u1 *src_ip, u1 *dst_ip, int fa)
{
	udp_header    *udph;

#ifdef IPV6
    if ( fa != addr_family) {
        /* discard the incoming packet if its inet family is 
         * different from the sending direction */
        free_netbuf(dgram);   
        return SBL_EFAILURE;
    }
#endif
    udph = (udp_header *)(dgram + iph_sz + sizeof(ether_header));

  /* Check if other side sent UDP checksum */
  if ( udph->cksum )
  {
     /* Calculate UDP checksum. */
     if( 0 != net2hs( chksum( src_ip, dst_ip, ( netbuf )udph,
                              net2hs( udph->len ), IP_UDP) ) )
     {
        sys_printf( "Bad UDP checksum\n" );
        free_netbuf(dgram);   
        return SBL_EFAILURE;
     }    
  }
 
    switch( net2hs(udph->dst_port) )
    {
#ifdef DHCP_SUPPORT			
      case DHCP_CLIENT_PORT:		
  #if UDP_DEBUG			
		sys_printf("Packet type: DHCP\n");
  #endif		
		dhcp_in(dgram, net2hs(udph->len) - UDP_HDR_SZ);		
        break;
#endif
#ifdef TFTP	
      case TFTP_CLIENT_PORT:
	  case TFTP_SVR_PORT:							/* IPCS_V4 PG ADD */
      udp_tftp:      
  #if UDP_DEBUG			
		sys_printf("Packet type: TFTP\n");			
  #endif		
    	tftp_in(dgram, net2hs(udph->dst_port), net2hs(udph->src_port), net2hs(udph->len) - UDP_HDR_SZ);
        break;
#endif	
      default:     
#ifdef TFTP	
      /* Patch: TFTP client port can now be changed */
      if( net2hs( udph->dst_port ) == TFTP_LOCAL_PORT )
        goto udp_tftp;
#endif        
  #if UDP_DEBUG			
		sys_printf("Packet type: Unknown\n");				
  #endif		
        free_netbuf(dgram);   
        break;
	}
	return SBL_SUCCESS;
}

		
