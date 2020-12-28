# File: drivers/char/fpga_sio/Makefile
#
# Makefile for the Linux fpga_sio driver.
#

# File: driversfpga_sio/Makefile
#
# Makefile for the fpga_sio driver.
#
EXTRA_CFLAGS:=$(CUST_KCFLAGS)
#EXTRA_CFLAGS += $(CUST_INCDIR)

INCDIR := -I $(TOOLCHAIN)/include -I $(OPENSSL)/include

obj-m += fpga_sio.o

fpga_sio-objs := sio_drv.o sio.o


#obj-m += ac49x_dsp.o
#ac49x_dsp-objs := pal_vlynq.o pal_vlynqDev.o pal_vlynqIoctl.o dsp_drv.o AC49xDrv_Api.o  AC49xDrv_UserDefinedFunctions.o

