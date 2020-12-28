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
PROJ_SRCDIR := ~/sdk.ac49x.uf7200/barrier_breaker/package/audc/audc-apps/voip/src/audc-voip/apps/voip/source/common
PROJ_INCDIR := ~/sdk.ac49x.uf7200/barrier_breaker/package/audc/audc-apps/voip/src/audc-voip/apps/voip/include/common
OBJDIR := build
TARGET := $(OBJDIR)/$(PROGNAME)

TOOLCHAIN := /usr
OPENSSL := /usr/local/src/openssl-1.0.2f

INCDIR := -I $(TOOLCHAIN)/include -I $(OPENSSL)/include \
-I /usr/local/src/googletest/googletest/include -I $(PROJ_INCDIR)
LIBDIR := -L $(TOOLCHAIN)/lib -L $(OPENSSL) \
-L /usr/local/src/googletest/build/googletest

SRCS := com_mem.c lulog_tsk.c supp_tsk.c
SRCS += tftp_tsk.c wave_sub.c
OBJS := $(SRCS:%=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)
LIBS := -lpthread -lcrypto

#CC = gcc
CC = $(TOOLCHAIN)/bin/gcc
#CFLAGS = -Wall -O2 -DDEBUG -DTEMP_HEADER -MMD -MP $(INCDIR)
CFLAGS = -Wall -O2 -DDEBUG -MMD -MP $(INCDIR)
CXX = $(TOOLCHAIN)/bin/g++
#CXXFLAGS = -Wall -O2 -DDEBUG -DTEMP_HEADER -MMD -MP $(INCDIR)
CXXFLAGS = -Wall -O2 -DDEBUG -MMD -MP $(INCDIR)
LDFLAGS = $(LIBDIR)
ARFLAGS = crsv

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(PROGLIB) $^
#	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# c source
$(OBJDIR)/%.c.o:$(PROJ_SRCDIR)/%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c $<

# c++ source
$(OBJDIR)/%.cpp.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) -o $@ -c $<


clean:
	rm -rf $(OBJDIR)
	rm $(PROGLIB)

rebuild:
	make clean && make

-include $(DEPS)