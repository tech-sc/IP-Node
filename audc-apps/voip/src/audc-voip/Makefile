AC494_BOARDS:=sdp, tulip, smb, m495l, 310hd, 320hd, 405hd, 420hd, 430hd, 440hd
AC494E_BOARDS:= orchid, orchid_ata, 450hd 
BOARDS= $(AC494_BOARDS) $(AC494_BOARDS)
ARCH=mips
ENDIAN=little
BOARD=310hd
DSP=AC495xxx
OS=openwrt
PWD=$(shell pwd)
MAKE=make
RV_NOLOG=on
RV_RELEASE=on
RV_VER=2.5
UI_TYPE=INTERNAL_UI

#APPS_DEBUG := -DACL_SIP_CALL_MNGR_DEBUG
#APPS_DEBUG += -DAC_DSP_TELEPHONY_COMMAND_DEBUG
#APPS_DEBUG += -DAC_DSP_TELEPHONY_EVENTS_DEBUG
#APPS_DEBUG += -DCONFIG_RG_VOIP_RV_SIP
#APPS_DEBUG += -DAC_LINE_CONNECTION_DEBUG
#APPS_DEBUG += -DACL_RV_SIP_API_DEBUG
#APPS_DEBUG += -DACL_ADV_SIP_FEATURES_DEBUG
#APPS_DEBUG += -DVOIP_CONF_DEBUG
#APPS_DEBUG += -DACL_OLSTUN_MSG_DEBUG
#APPS_DEBUG += -DAC_VOIP_MEDIA_HANDLER_DEBUG
#APPS_DEBUG += -DAC_VOIP_RTP_UTILS_DEBUG
#APPS_DEBUG += -DAC_VOIP_STATUS_HANDLER_DEBUG
#APPS_DEBUG += -DAC_VOIP_UTILS_DEBUG

ifeq ($(BOARD), $(findstring $(BOARD),$(AC494_BOARDS)))
SOC_FAMILY=AC494
endif
ifeq ($(BOARD), $(findstring $(BOARD),$(AC494E_BOARDS)))
SOC_FAMILY=AC494E
endif


ifeq ($(ENDIAN), little)
ENDIAN_MODE=CONFIG_AC49X_LITTLE_ENDIAN
else
ENDIAN_MODE=CONFIG_AC49X_BIG_ENDIAN
endif

ifeq ($(RV_VER), 2.5)
APPS_RV_VER=RV_2_5
else
APPS_RV_VER=RV_3_0
endif

ifeq ($(RV_NOLOG), on)
RV_ARCHIVE_FILES:=archive_files
else
RV_ARCHIVE_FILES=archive_files_with_logs
endif
ifeq ($(ENDIAN), little)
TARGET_CPU_ENDIAN_LINE := RV_ARCH_LITTLE_ENDIAN
RADVISION_FOLDER=radvision_$(RV_VER)/sip
else
TARGET_CPU_ENDIAN_LINE := RV_ARCH_BIG_ENDIAN
RADVISION_FOLDER=radvision_$(RV_VER)/sip_be
endif

ifeq ($(OS), openwrt)
SDK_DIR=$(PWD)/../barrier_breaker/
INCLUDE_PATH=-I$(PWD)/drivers/include/

#little endian
ifeq ($(ENDIAN), little)
CROSS_COMPILE:= $(SDK_DIR)/staging_dir/toolchain-mipsel_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-
TARGET_CPU_FAMILY=$(SDK_DIR)/staging_dir/toolchain-mipsel_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-uclibc
STAGING_DIR=$(SDK_DIR)/staging_dir/target-mipsel_mips32_uClibc-0.9.33.2
KERNELDIR=$(SDK_DIR)/build_dir/target-mipsel_mips32_uClibc-0.9.33.2/linux-audc_$(BOARD)/linux-3.10.49/
endif

#big endian
ifeq ($(ENDIAN), big)
CROSS_COMPILE:= $(SDK_DIR)/staging_dir/toolchain-mips_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-
TARGET_CPU_FAMILY=$(SDK_DIR)/staging_dir/toolchain-mips_mips32_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-uclibc
STAGING_DIR=$(SDK_DIR)/staging_dir/target-mips_mips32_uClibc-0.9.33.2
KERNELDIR=$(SDK_DIR)/build_dir/target-mips_mips32_uClibc-0.9.33.2/linux-audc_$(BOARD)/linux-3.10.49/
endif

endif

ifeq ($(OS), buildroot)
SDK_DIR=/home/mark/work/sdk/buildroot/
CROSS_COMPILE:= $(SDK_DIR)/staging/bin/mipsel-linux-uclibc-
TARGET_CPU_FAMILY=$(SDK_DIR)/staging/bin/mipsel-linux-uclibc
STAGING_DIR=$(SDK_DIR)/staging/
KERNELDIR=$(SDK_DIR)/build_mipsel_nofpu/linux-2.6.10_dev-malta-mips2_fp_len/
INCLUDE_PATH=-I$(KERNELDIR)/include/asm/mach-avalanche/generic -I$(KERNELDIR)/include/asm/mach-avalanche/titan -I$(PWD)/drivers/include/
endif

ifeq ($(BOARD), sdp)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DKEYMAP_SDP 
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), tulip)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_TULIP -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1 -DCONFIG_NUM_SLIC_DEVICES=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_tulip_fxss
APPS_CUST_CFLAGS=$(COMM_FLAGS)
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_tulip
endif

ifeq ($(BOARD), smb)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_SMB -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=4 -DCONFIG_SLIC_LE88_MODULE -DCONFIG_NUM_SLIC_DEVICES=4 
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_smb
APPS_CUST_CFLAGS=$(COMM_FLAGS)
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_smb
endif

ifeq ($(BOARD), 310hd)
DSP=AC495xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS := $(COMM_FLAGS) -DKEYMAP_310HD 
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), 320hd)
DSP=AC495xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS := $(COMM_FLAGS) -DKEYMAP_320HD
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) CUST_KCFLAGS='-DCONFIG_SDP_IPPHONE -D$(ENDIAN_MODE) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), 420hd)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp 
DRVS_TYPE_FLAGS:=BOARD=$(BOARD)
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DLCD_LIDDFB -DKEYMAP_420HD -DNANOX
APPS_CUST_INCLUDEFLAGS=-I$(STAGING_DIR)/usr/include/nanox/
APPS_CUST_LDFLAGS=-lnano-X
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
endif

ifeq ($(BOARD), 430hd)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp 
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DKEYMAP_430HD $(INCLUDE_PATH)
DRVS_TYPE_FLAGS:=BOARD=$(BOARD)
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), 440hd)
DSP=AC494xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp 
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DKEYMAP_440HD $(INCLUDE_PATH)
DRVS_TYPE_FLAGS:=BOARD=$(BOARD)
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), 405hd)
DSP=AC495xxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE  -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DLCD_ST7567 -DKEYMAP_405HD -DNANOX -D$(UI_TYPE)
APPS_CUST_INCLUDEFLAGS=-I$(STAGING_DIR)/usr/include/nanox/
APPS_CUST_LDFLAGS=-lnano-X
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
endif

ifeq ($(BOARD), m495l)
DSP=AC495Lxxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_sdp_ipp
APPS_CUST_CFLAGS= $(COMM_FLAGS) -DLCD_ST7567 -DKEYMAP_M495L -DNANOX
APPS_CUST_INCLUDEFLAGS=-I$(STAGING_DIR)/usr/include/nanox/
APPS_CUST_LDFLAGS=-lnano-X
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
endif

ifeq ($(BOARD), orchid)
DSP=AC494Exxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -DCONFIG_ORCHID -DCONFIG_ORCHID_MAIN -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid
APPS_CUST_CFLAGS= $(COMM_FLAGS) -DGLCD_CPT480272 -DKEYMAP_ORCHID_MAIN -DNANOX
APPS_CUST_INCLUDEFLAGS=-I$(STAGING_DIR)/usr/include/nanox/
APPS_CUST_LDFLAGS=-lnano-X
DRVS_TYPE_FLAGS:=BOARD=$(BOARD)
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid
endif

ifeq ($(BOARD), orchid_ata)
DSP=AC494Exxx
COMM_FLAGS=-DCONFIG_ORCHID_ATA -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1 \
	-DCONFIG_SLIC_LE88_MODULE -DCONFIG_NUM_SLIC_DEVICES=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid_ata
APPS_CUST_CFLAGS=$(COMM_FLAGS)
DRVS_TYPE_FLAGS:=BOARD=$(BOARD)			
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid_ata
endif

ifeq ($(BOARD), 450hd)
DSP=AC494Exxx
COMM_FLAGS=-DCONFIG_SDP_IPPHONE -D$(DSP) -D$(ENDIAN_MODE) -DCONFIG_NUMBER_OF_DSP=1
APPS_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid
APPS_CUST_CFLAGS=$(COMM_FLAGS) -DGLCD_CPT480272 -DCONFIG_ORCHID -DKEYMAP_450HD -DNANOX
APPS_CUST_INCLUDEFLAGS=-I$(STAGING_DIR)/usr/include/nanox/
APPS_CUST_LDFLAGS=-lnano-X
DRVS_TYPE_FLAGS:=BOARD=$(BOARD) 
DRVS_CUST_KCFLAGS='$(COMM_FLAGS) $(INCLUDE_PATH)'
STACK_TYPE_FLAGS:=PLATFORM_TYPE=s APP_TYPE=ac494_orchid
endif

APPS_CUST_CFLAGS += -D$(APPS_RV_VER)
APPS_CUST_CFLAGS += $(APPS_DEBUG)


LOCAL_INSTALL_DIR=$(PWD)/files/$(BOARD)
OPENWRT_INSTALL_DIR=$(SDK_DIR)/package/audc/audc-apps/voip/$(BOARD)/
#INSTALL_DIR=$(LOCAL_INSTALL_DIR)
INSTALL_DIR=$(OPENWRT_INSTALL_DIR)

export STAGING_DIR
export KERNELDIR

.PHONY: apps 
.PHONY: drivers

all: drivers apps

clean: drivers_clean apps_clean

help:
	@echo available commands:
	@echo make apps
	@echo make apps_clean
	@echo make drivers
	@echo make drivers_clean
	@echo make all
	@echo make clean
	@echo make install	

apps:
	$(MAKE) -C apps CROSS_COMPILE=$(CROSS_COMPILE) \
				$(APPS_TYPE_FLAGS) \
				CUST_CFLAGS='$(APPS_CUST_CFLAGS)' \
				CUST_INCLUDEFLAGS='$(APPS_CUST_INCLUDEFLAGS)' \
				CUST_LDFLAGS='$(APPS_CUST_LDFLAGS)' \
				DRV_DIR=$(PWD)/drivers/ \
				RADVISION_FOLDER=$(RADVISION_FOLDER) \
				RV_ARCHIVE_FILES=$(RV_ARCHIVE_FILES) \
				TARGET_CPU_ENDIAN_LINE=$(TARGET_CPU_ENDIAN_LINE) \
				ACTION=release
apps_clean:
	$(MAKE) -C apps $(APPS_TYPE_FLAGS) ACTION=clean
	
drivers:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD)/drivers \
			$(DRVS_TYPE_FLAGS) \
			CUST_KCFLAGS=$(DRVS_CUST_KCFLAGS) \
			modules
	
drivers_clean:	
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(PWD)/drivers \
			$(DRVS_TYPE_FLAGS) \
			CUST_KCFLAGS=$(DRVS_CUST_KCFLAGS) \
			clean

stack:
	$(MAKE) -C radvision_linux_$(RV_VER) $(STACK_TYPE_FLAGS) NOLOG=$(RV_NOLOG) release=$(RV_RELEASE) \
					PROTOCOL=SIP TARGET_CPU_FAMILY=$(TARGET_CPU_FAMILY) TARGET_CPU_ENDIAN=$(ENDIAN)

stack_clean:
	$(MAKE) -C radvision_linux_$(RV_VER) $(STACK_TYPE_FLAGS) NOLOG=$(RV_NOLOG) release=$(RV_RELEASE) \
					PROTOCOL=SIP TARGET_CPU_FAMILY=$(TARGET_CPU_FAMILY) TARGET_CPU_ENDIAN=$(ENDIAN) clean
	
install_sdp:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	cp -f $(PWD)/apps/acl_main/Release/ac494main_sdp_IPP $(INSTALL_DIR)/
	cp -f $(PWD)/apps/mng_sample_appl/Releasemng_sample_appl $(INSTALL_DIR)/
	
install_tulip:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/ac49x_mpi/ac49x_mpi.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/le88/le88.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	
install_smb:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/ac49x_mpi/ac49x_mpi.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/le88/le88.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/

install_310hd:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	cp -f $(PWD)/apps/acl_main/Release/ac494main_sdp_IPP $(INSTALL_DIR)/
	cp -f $(PWD)/apps/mng_sample_appl/Release/mng_sample_appl $(INSTALL_DIR)/

install_320hd:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	cp -f $(PWD)/apps/acl_main/Release/ac494main_sdp_IPP $(INSTALL_DIR)/
	cp -f $(PWD)/apps/mng_sample_appl/Release/mng_sample_appl $(INSTALL_DIR)/	
	
install_420hd:
	chmod a+x $(INSTALL_DIR)/preinit.sh	
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/liddfb/liddfb_glcd.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/

install_430hd:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/	
	
install_405hd:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/st7567_lcd/st7567.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	cp -f $(PWD)/apps/acl_main/Release/ac494main_sdp_IPP $(INSTALL_DIR)/
	cp -f $(PWD)/apps/mng_sample_appl/Release/mng_sample_appl $(INSTALL_DIR)/
	
install_m495l:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/st7567_lcd/st7567.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	cp -f $(PWD)/apps/acl_main/Release/ac494main_sdp_IPP $(INSTALL_DIR)/
	cp -f $(PWD)/apps/mng_sample_appl/Release/mng_sample_appl $(INSTALL_DIR)/	
	
install_orchid:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/
	
install_orchid_ata:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/ac49x_mpi/ac49x_mpi.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/le88/le88.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/lu_proc $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/fpga_sio/fpga_sio.ko $(INSTALL_DIR)/
	cp -f $(PWD)/drivers/watchdog/watchdog.ko $(INSTALL_DIR)/

install_450hd:
	cp -f $(PWD)/drivers/ac49x_dsp/ac49x_dsp.ko $(INSTALL_DIR)/
	cp -f $(PWD)/apps/voip/Release/voip_task $(INSTALL_DIR)/	
	
install: drivers apps install_$(BOARD)
	
