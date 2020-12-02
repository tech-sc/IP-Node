
LEGAL_PLATFORMS := p b i s grocx
ifneq ($(PLATFORM_TYPE), $(findstring $(PLATFORM_TYPE),$(LEGAL_PLATFORMS)))
$(error The PLATFORM_TYPE "$(PLATFORM_TYPE)" is not supported. Legal values are: $(LEGAL_PLATFORMS))
endif

ifndef PLATFORM_TYPE
echo Warning: PLATFORM_TYPE unspecified !!!
$(warning ****Warning: PLATFORM_TYPE unspecified !!!!)
endif

LEGAL_APPS := ac488_cvt ac483_cvt ac494_tulip_fxss ac494_sdp_ipp ac494_sdp_ata ac494_smb ceva 320_ipp ac494_tulip2_40 ac494_tulip2_21 ac494_orchid ac494_orchid_ata
ifneq ($(APP_TYPE), $(findstring $(APP_TYPE),$(LEGAL_APPS)))
$(error  The APP_TYPE "$(APP_TYPE)" is not supported. Legal values are: $(LEGAL_APPS))
endif

ifndef APP_TYPE
echo Warning: APP_TYPE unspecified !!!
$(warning ****Warning: APP_TYPE unspecified !!!!)
endif

ifndef SLIC_DEVICE
SLIC_DEVICE := legerity
endif

LEGAL_SLIC := legerity silabs
ifneq ($(SLIC_DEVICE), $(findstring $(SLIC_DEVICE),$(LEGAL_SLIC)))
$(error The SLIC_DEVICE "$(SLIC_DEVICE)" is not supported. Legal values are: $(LEGAL_SLIC))
endif

TOPDIR := $(shell pwd)
MAKE:=make


ARCS_TO_COMPILE:=core/acCfg core/md5 core/acValidation

ifeq ($(VOIP_PROT), mgcp)
ARCS_TO_COMPILE += voip/source/mgcp
else
#ARCS_TO_COMPILE += voip/source/sip
endif

ifeq ($(APP_TYPE),320_ipp)
ARCS_TO_COMPILE += ../3xxHD/MSGQ
endif

APPS_TO_COMPILE:=mng_sample_appl acl_main voip gateway_api_sample_application

DIRS_TO_COMPILE:= $(ARCS_TO_COMPILE) $(APPS_TO_COMPILE)


ifeq ($(APP_TYPE), ac488_cvt)
APP_FLAGS += APP_TYPE=ac488_cvt
endif
ifeq ($(APP_TYPE), ac483_cvt)
APP_FLAGS += APP_TYPE=ac483_cvt
endif

ifeq ($(APP_TYPE), ceva)
APP_FLAGS += APP_TYPE=ceva
endif


ifeq ($(PLATFORM_TYPE), p)
APP_FLAGS+=PLATFORM_TYPE=p
endif
ifeq ($(PLATFORM_TYPE), b)
APP_FLAGS+=PLATFORM_TYPE=b
endif
ifeq ($(PLATFORM_TYPE), i)
APP_FLAGS+=PLATFORM_TYPE=i
endif
ifeq ($(PLATFORM_TYPE), grocx)
APP_FLAGS+=PLATFORM_TYPE=grocx
endif
ifeq ($(PLATFORM_TYPE), s)
APP_FLAGS+=PLATFORM_TYPE=s
endif


ifeq ($(PLATFORM_TYPE), s)

# convert APP_TYPE to uppercase
ifeq ($(APP_TYPE), ac494_tulip_fxss)
APP_FLAGS+=APP_TYPE=ac494_tulip_fxss
endif
ifeq ($(APP_TYPE), ac494_sdp_ipp)
APP_FLAGS+=APP_TYPE=ac494_sdp_ipp
endif
ifeq ($(APP_TYPE), 320_ipp)
APP_FLAGS+=APP_TYPE=320_ipp
endif
ifeq ($(APP_TYPE), ac494_sdp_ata)
APP_FLAGS+=APP_TYPE=ac494_sdp_ata
endif
ifeq ($(APP_TYPE), ac494_smb)
APP_FLAGS+=APP_TYPE=ac494_smb
endif
ifeq ($(APP_TYPE), ac494_tulip2_40)
APP_FLAGS+=APP_TYPE=ac494_tulip2_40
endif
ifeq ($(APP_TYPE), ac494_tulip2_21)
APP_FLAGS+=APP_TYPE=ac494_tulip2_21
endif

ifeq ($(APP_TYPE), ac494_orchid)
APP_FLAGS+=APP_TYPE=ac494_orchid
endif

ifeq ($(APP_TYPE), ac494_orchid_ata)
APP_FLAGS+=APP_TYPE=ac494_orchid_ata
endif
endif


ifeq ($(SLIC_DEVICE), legerity)
APP_FLAGS += SLIC_DEVICE=legerity
endif
ifeq ($(SLIC_DEVICE), silabs)
APP_FLAGS += SLIC_DEVICE=silabs
endif

APP_ACTION:=$(ACTION)

ifeq ($(ACTION), clean)
ARC_ACTION:=$(ACTION)
endif


.PHONY: $(DIRS_TO_COMPILE) 

all: $(DIRS_TO_COMPILE) 


$(ARCS_TO_COMPILE):
	$(MAKE) --directory=$@ $(ARC_ACTION) $(APP_FLAGS)
	
$(APPS_TO_COMPILE): core/acCfg 
	$(MAKE) --directory=$@ $(APP_ACTION) $(APP_FLAGS)



