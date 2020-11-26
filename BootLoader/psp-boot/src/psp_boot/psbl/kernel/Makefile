#*******************************************************************************
#*+--------------------------------------------------------------------------+**
#*|                            ****                                          |**
#*|                            ****                                          |**
#*|                            ******o***                                    |**
#*|                      ********_///_****                                   |**
#*|                      ***** /_//_/ ****                                   |**
#*|                       ** ** (__/ ****                                    |**
#*|                           *********                                      |**
#*|                            ****                                          |**
#*|                            ***                                           |**
#*|                                                                          |**
#*|         Copyright (c) 1998-2003 Texas Instruments Incorporated           |**
#*|                        ALL RIGHTS RESERVED                               |**
#*|                                                                          |**
#*| Permission is hereby granted to licensees of Texas Instruments           |**
#*| Incorporated (TI) products to use this computer program for the sole     |**
#*| purpose of implementing a licensee product based on TI products.         |**
#*| No other rights to reproduce, use, or disseminate this computer          |**
#*| program, whether in part or in whole, are granted.                       |**
#*|                                                                          |**
#*| TI makes no representation or warranties with respect to the             |**
#*| performance of this computer program, and specifically disclaims         |**
#*| any responsibility for any damages, special or consequential,            |**
#*| connected with the use of this program.                                  |**
#*|                                                                          |**
#*+--------------------------------------------------------------------------+**
#*******************************************************************************

# ******************************************************************************
# * FILE PURPOSE:    PSP Bootloader kernel Makefile
# ******************************************************************************
# * FILE NAME:       Makefile
# *
# * DESCRIPTION:     PSP Bootloader kernel Makefile
# *
# * (C) Copyright 2003, Texas Instruments, Inc
# ******************************************************************************

SRCS_KERNEL_S = platform_init.S srt0.S buasmhooks.S
SRCS_SP_KERNEL_S = mips4k_init.S
SRCS_KERNEL_C = main.c shell.c support.c printf.c \
				exception.c  heapmgr.c files.c fcb.c loadfile.c \
				buhooks.c envalias.c
# IPCS-V4 ADD
SRCS_KERNEL_C += UF7200s2.c

ifeq ($(FLASH_CONFIG), NAND)
SRCS_KERNEL_C += env.c nand_env.c
SRCS_KERNEL_C += nand_fmm.c
ifeq ($(BOARD), TNETV1051SDB)
SRCS_KERNEL_C += nor_env.c
endif
else
SRCS_KERNEL_C += nor_env.c env.c
SRCS_KERNEL_C += nor_fmm.c
endif


ifeq ($(BOARD), SEAD2)
SRCS_KERNEL_C += seadsio.c
else
SRCS_KERNEL_C += sio.c
endif

ifeq ($(BOARD),UR8EVM)
SRCS_KERNEL_C += yamuna_clk.c
else
SRCS_KERNEL_C += clkc.c
endif 

# Optional Module Support
ifeq ($(FFS), YES)
ifeq ($(FLASH_CONFIG), NAND)
SRCS_KERNEL_C += ffs_nandutil.c
else
SRCS_KERNEL_C += ffs_norutil.c
endif
SRCS_KERNEL_C += ffs.c ffs_nand.c ffs_nor.c mms.c
else
ifeq ($(FLASH_CONFIG), NAND)
SRCS_KERNEL_C += nand_rawfs.c
else
SRCS_KERNEL_C += rawfs.c
endif
endif

ifeq  ($(CONF_OMOD_TIBINARY), YES)
SRCS_KERNEL_C += load_bin.c
else
SRCS_KERNEL_C += load_bin_filler.c
endif
ifeq  ($(CONF_OMOD_ELF), YES)
SRCS_KERNEL_C += load_elf.c
else
SRCS_KERNEL_C += load_elf_filler.c
endif

ifeq  ($(CONF_OMOD_NSP_SINGLE_IMAGE), YES)
SRCS_KERNEL_C += cksum.c
ifeq ($(FLASH_CONFIG), NAND)
SRCS_KERNEL_C += nand_single_img.c
else
SRCS_KERNEL_C += single_img.c
endif
endif

# Optional Command Support.
VPATH		+= cmd
include cmd/cmd.mak

# Flash driver 
VPATH 		+= flash
ifeq	("$(SRCS_FLASH_C)", "")
include flash/flash.mak
endif

SRCS_SP_KERNEL_C = $(SRCS_FLASH_C)

OBJS_KERNEL = $(SRCS_KERNEL_S:%.S=%.o) $(SRCS_KERNEL_C:%.c=%.o)
OBJS_SP_KERNEL = $(SRCS_SP_KERNEL_S:%.S=%.o) $(SRCS_SP_KERNEL_C:%.c=%.o)
OBJS_FLASH = $(SRCS_FLASH_C:%.c=%.o)
	
OBJS_LIBCMD = $(SRCS_CMD_C:%.c=%.o)

all: libkernel_b.a libcmd_b.a

libkernel_b.a: clean $(OBJS_KERNEL) $(OBJS_SP_KERNEL)
ifeq ($(BOARD), TNETV1051SDB)
	$(LD) $(LDFLAGS) -r -o flashop.o $(OBJS_FLASH)
else
	$(CP) $(OBJS_FLASH) flashop.o
endif
	$(AR) -r $@ $(OBJS_KERNEL)

libcmd_b.a: clean $(OBJS_LIBCMD)
	$(AR) -r $@ $(OBJS_LIBCMD)

.PHONY: clean
clean:
	$(RM) $(OBJS_KERNEL) $(OBJS_SP_KERNEL) $(OBJS_FLASH) libkernel_b.a
	$(RM) $(OBJS_LIBCMD) libcmd_b.a
	$(RM) flashop.o
include $(TOPDIR)/rules.mak

