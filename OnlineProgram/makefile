# make実行前にSTAGING_DIRを定義するとよい
# $ export STAGING_DIR=./
#
# ビルドファイル構成
# |-- Makefile (This file)
# |-- build
# |   |-- target file.
# |   `-- object files.
# |-- source header files.
# `-- source files.

PROGNAME := linktest

SRCDIR := .
OBJDIR := build
TARGET := $(OBJDIR)/$(PROGNAME)

TOOLCHAIN := ~/sdk.ac49x/barrier_breaker/staging_dir/toolchain-mipsel_mips32_gcc-4.8-linaro_uClibc-0.9.33.2
OPENSSL := ~/sdk.ac49x/barrier_breaker/build_dir/target-mipsel_mips32_uClibc-0.9.33.2/openssl-1.0.2f

INCDIR := -I $(TOOLCHAIN)/include -I $(OPENSSL)/include \
~/googletest/googletest/include
LIBDIR := -L $(TOOLCHAIN)/lib -L $(OPENSSL) \
-L ~/googletest/build/googletest

SRCS := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(addprefix $(OBJDIR)/,$(patsubst %.c,%.o,$(SRCS)))
LIBS := -lpthread -lcrypto

#CC = gcc
CC = $(TOOLCHAIN)/bin/mipsel-openwrt-linux-gcc
CFLAGS = -Wall -O2 $(INCDIR)
CXX = $(TOOLCHAIN)/bin/mipsel-openwrt-linux-g++
CXXFLAGS = -Wall -O2 $(INCDIR)
LDFLAGS = $(LIBDIRS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBDIR) $(LIBS)

$(OBJDIR)/%.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJDIR)

