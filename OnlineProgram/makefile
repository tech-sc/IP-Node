# ビルドファイル構成
# |-- Makefile (This file)
# |-- build
# |   |-- target file.
# |   `-- object files.
# |-- source header files.
# `-- source files.

## make program name
PROGNAME := apptest
## $(PROGLIB)は、googletest用のテストモジュールにリンクするためのlibファイル
PROGLIB := lib$(PROGNAME).a
export STAGING_DIR=

## Directory defines
SRCDIR := .
OBJDIR := build
TARGET := $(OBJDIR)/$(PROGNAME)

TOOLCHAIN := /usr
OPENSSL := /usr/local/src/openssl-1.0.2f

INCDIR := -I $(TOOLCHAIN)/include -I $(OPENSSL)/include \
-I /usr/local/src/googletest/googletest/include
LIBDIR := -L $(TOOLCHAIN)/lib -L $(OPENSSL) \
-L /usr/local/src/googletest/build/googletest

SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.s)
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS := -lpthread -lcrypto

#CC = gcc
CC = $(TOOLCHAIN)/bin/gcc
CFLAGS = -Wall -O2 -DDEBUG -DTEMP_HEADER -MMD -MP $(INCDIR)
CXX = $(TOOLCHAIN)/bin/g++
CXXFLAGS = -Wall -O2 -DDEBUG -DTEMP_HEADER -MMD -MP $(INCDIR)
LDFLAGS = $(LIBDIR)
ARFLAGS = crsv

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(PROGLIB) $^
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# ssembly
$(OBJDIR)/%.s.o:%.s
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(AS) $(ASFLAGS) -o $@ -c $<

# c source
$(OBJDIR)/%.c.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

# c++ source
$(OBJDIR)/%.cpp.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<


clean:
	rm -rf $(OBJDIR)

rebuild:
	make clean && make

-include $(DEPS)
