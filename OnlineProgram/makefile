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

## Directory defines
TOOLCHAIN = ~/sdk.ac49x/barrier_breaker/staging_dir/toolchain-mipsel_mips32_gcc-4.8-linaro_uClibc-0.9.33.2
BUILDDIR = ./build
OBJDIR = $(BUILDDIR)
SRCDIR = .
INCDIRS = -I -I $(TOOLCHAIN)/include \
~/googletest/googletest/include
LIBDIRS = -L $(TOOLCHAIN)/lib \
-L ~/googletest/build/googletest

## Target name
TARGET = $(BUILDDIR)/stat
STAGING_DIR = ./

## Compiler options
CC = $(TOOLCHAIN)/bin/mipsel-openwrt-linux-gcc
#CC = gcc
CFLAGS = -O2 -Wall
CXX = $(TOOLCHAIN)/bin/mipsel-openwrt-linux-g++
#CXX = g++
CXXFLAGS = -O2 -Wall
LDFLAGS =

SRCS := $(shell find $(SRCDIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS = -lpthread

#INCLUDE := $(shell find $(INCDIRS) -type d)
#INCLUDE := $(addprefix -I,$(INCLUDE))
INCLUDE := $(INCDIRS)

CPPFLAGS := $(INCLUDE) -MMD -MP
LDFLAGS += $(LIBDIRS) $(LIBS)

# Target
default:
	make all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# assembly
$(OBJDIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(OBJDIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(OBJDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
	
.PHONY: all clean rebuild

clean:
	$(RM) -r $(BUILDDIR)

rebuild:
	make clean && make

-include $(DEPS)

MKDIR_P = mkdir -p
