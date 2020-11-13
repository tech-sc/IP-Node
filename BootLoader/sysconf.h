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
**|         Copyright (c) 1998-2005 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/

/*******************************************************************************
 * FILE PURPOSE:    PSPboot configuration parameters.
 *******************************************************************************
 * FILE NAME:       sysconf.h
 *
 * DESCRIPTION:     PSPboot configuration parameters.
 *
 ******************************************************************************/

#ifndef _SYSCONF_H_
#define _SYSCONF_H_

/* Macros for unit conversions */
#define MEG(x)      ((x) << 20)
#define MHZ(x)      ((x) * 1000 * 1000)
#define KILOBYTE(x) ((x) << 10)
#define KHZ(x)      ((x) * 1000)

/* Definitions for cache types */
#define CACHE_WT_NWA  0     /* write through, no write allocate*/
#define CACHE_WT     1          /* 1050 only, write through, write allocate */
#define CACHE_NONE   2        /* cache disabled */
#define CACHE_WB     3          /* write back, write allocate */

/* Pre-processor macro to get the board name string into the code
 * This depends of BOARD=XXX and XXX=XXX being defined externally 
 * (probably through a makefile). The BOARDNAME macro will return the 
 * string "XXX"
 */
#define BRDSTR(E)   #E
#define MKSTR(X)    BRDSTR(X)
#define BOARDNAME   MKSTR(BOARD)

#include "autoconfig.h"

#endif /* _SYSCONF_H_ */
