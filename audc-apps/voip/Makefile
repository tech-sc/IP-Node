#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=audc-voip
PKG_RELEASE:=1
#PKG_SOURCE_TARBALL:=$(PKG_NAME).tgz

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)

define Package/audc-voip
  SUBMENU:=Applications
  CATEGORY:=Audiocodes
  TITLE:=VoIP application
  DEPENDS:=@TARGET_audc +libpthread +PACKAGE_microwindows:microwindows +librt \
									+PACKAGE_audc-ui:audc-ui
endef

ifeq ($(CONFIG_BIG_ENDIAN),y)
  ENDIAN_MODE:=big
else
  ENDIAN_MODE:=little
endif

#RV_VER:=2.5
#RV_VER:=3.0
#RV_NOLOG:=On
#RV_NOLOG:=off
#UI_TYPE:=INTERNAL_UI
#UI_TYPE:=EXTERNAL_UI

define Package/audc-voip/config
	choice PACKAGE_VOIP_RV_VERSION
			prompt "sip stack version"
		   default PACKAGE_RV_3_0

	config PACKAGE_RV_2_5
		   bool "RV 2.5"

	config PACKAGE_RV_3_0
		   bool "RV 3.0"       

	endchoice
endef

ifeq ($(CONFIG_PACKAGE_RV_3_0),y)
  RV_VER:=3.0
else
  RV_VER:=2.5
endif

RV_NOLOG:=on

ifeq ($(CONFIG_PACKAGE_audc-ui),y)
  UI_TYPE:=EXTERNAL_UI
else
  UI_TYPE:=INTERNAL_UI
endif

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/audc-voip/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		OS=openwrt \
		CROSS_COMPILE=$(TARGET_CROSS) \
		STAGING_DIR=$(STAGING_DIR) \
		ENDIAN=$(ENDIAN_MODE) \
		KERNELDIR=$(LINUX_DIR) \
		BOARD=$(SUBTARGET) \
		RV_VER=$(RV_VER) \
		RV_NOLOG=$(RV_NOLOG) \
		UI_TYPE=$(UI_TYPE) \
		INSTALL_DIR=$(PKG_BUILD_DIR)/files/$(SUBTARGET) \
		install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/audc/voip_task
	$(CP) $(PKG_BUILD_DIR)/drivers/ac49x_dsp/*.h $(1)/usr/include/audc/voip_task/
	$(CP) $(PKG_BUILD_DIR)/drivers/include/*.h $(1)/usr/include/audc/voip_task/
	$(CP) $(PKG_BUILD_DIR)/apps/voip/include/common/*.h $(1)/usr/include/audc/voip_task/
endef

define Package/audc-voip/install
	if [ -d $(PKG_BUILD_DIR)/files/$(SUBTARGET) ]; then \
		$(INSTALL_DIR) $(1)/audc/voip/; \
		$(CP) $(PKG_BUILD_DIR)/files/$(SUBTARGET)/* $(1)/audc/voip/; \
		chmod a+x $(1)/audc/voip/preinit.sh; \
		$(INSTALL_DIR) $(1)/$(TARGET_MODULES_DIR); \
		mv $(1)/audc/voip/*.ko $(1)/$(TARGET_MODULES_DIR); \
	fi
	$(CP) ./files/* $(1)/
	if [ -d ./target_files/$(SUBTARGET)/ ]; then \
		$(CP) ./target_files/$(SUBTARGET)/* $(1)/; \
	fi
endef

$(eval $(call BuildPackage,audc-voip,+librt))


