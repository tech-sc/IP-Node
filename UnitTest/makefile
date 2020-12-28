# ビルドファイル構成
# |-- Makefile (This file)
# |-- build
# |   |-- target file.
# |   `-- object files.
# |-- source header files.
# `-- source files.

## make program name
PROGNAME := UnitTest
## $(APPDIR)は、libapptest.aがあるディレクトリを指定する
APPDIR := ../OnlineProgram
APPLIBFL := libapptest.a
APPLIB := apptest
export STAGING_DIR=
export LD_LIBRARY_PATH=/usr/local/src/openssl-1.0.2f

## Directory defines
PROJ_INCDIR := ~/sdk.ac49x.uf7200/barrier_breaker/package/audc/audc-apps/voip/src/audc-voip/apps/voip/include/common
SRCDIR := .
OBJDIR := build
TARGET := $(OBJDIR)/$(PROGNAME)
DBGOPT := -DDEBUG -DTEMP_HEADER

TOOLCHAIN := /usr
OPENSSL := /usr/local/src/openssl-1.0.2f

INCDIR := -I $(TOOLCHAIN)/include -I $(OPENSSL)/include -I $(PROJ_INCDIR) \
-I /usr/local/src/googletest/googletest/include
LIBDIR := -L $(TOOLCHAIN)/lib -L $(OPENSSL) -L $(APPDIR) \
-L /usr/local/src/googletest/build/googletest -lrt

SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.s)
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS := -lpthread -lcrypto -lgtest -lgtest_main -l$(APPLIB)

#CC = gcc
CC = $(TOOLCHAIN)/bin/gcc
CFLAGS = -Wall -O2 $(DBGOPT) -MMD -MP $(INCDIR)
CXX = $(TOOLCHAIN)/bin/g++
CXXFLAGS = -Wall -O2 $(DBGOPT) -MMD -MP $(INCDIR)
LDFLAGS = $(LIBDIR)
ARFLAGS = crsv

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS) $(APPDIR)/$(APPLIBFL)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# ssembly
$(OBJDIR)/%.s.o:%.s
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(AS) $(ASFLAGS) -o $@ -c $<

# c source
$(OBJDIR)/%.c.o:%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) $(LIBS) -o $@ -c $<

# c++ source
$(OBJDIR)/%.cpp.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<


clean:
	rm -rf $(OBJDIR)

rebuild:
	make clean && make

-include $(DEPS)
