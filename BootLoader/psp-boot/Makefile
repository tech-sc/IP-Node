#
# Copyright (C) 2013-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=audc-pspboot
PKG_RELEASE:=1
#PKG_SOURCE_TARBALL:=psp_boot.tgz

PKG_BUILD_DIR := $(KERNEL_BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

define Package/audc-pspboot
  SECTION:=boot
  CATEGORY:=Audiocodes
  SUBMENU:=Boot Loaders
  DEPENDS:=@TARGET_audc
  TITLE:=Audiocodes PSP-Boot
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	echo #####################################
	$(CP) ./src/psp_boot/* $(PKG_BUILD_DIR)
	$(CP) ./files/$(SUBTARGET)/bootcfg.mak $(PKG_BUILD_DIR)/
endef

define Build/Configure

endef

ifeq ($(CONFIG_BIG_ENDIAN),y)
  ENDIAN_MODE:=BE
else
  ENDIAN_MODE:=LE
endif

USER_CFLAGS=-Daudc_$(SUBTARGET) -DCONFIG_BAUDRATE=115200

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		ENDIAN=$(ENDIAN_MODE) \
		USER_CFLAGS="$(USER_CFLAGS)" \
		all
endef

define Package/audc-pspboot/install
	$(INSTALL_DIR) $(BIN_DIR)/$(ARCH)_$(BOARD)_$(SUBTARGET)_$(PROFILE)
	$(CP) $(PKG_BUILD_DIR)/bin/*.bin $(BIN_DIR)/$(ARCH)_$(BOARD)_$(SUBTARGET)_$(PROFILE)
	$(CP) ./files/$(SUBTARGET)/bootbdi.cfg $(BIN_DIR)/$(ARCH)_$(BOARD)_$(SUBTARGET)_$(PROFILE)/bootbdi.cfg
endef 

$(eval $(call BuildPackage,audc-pspboot))