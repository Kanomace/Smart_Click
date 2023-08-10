# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# set your toolchain prefix
TOOLCHAIN_PREFIX :=
CC     := $(TOOLCHAIN_PREFIX)gcc
LD     := $(TOOLCHAIN_PREFIX)gcc
AR     := $(TOOLCHAIN_PREFIX)ar

DEBUG_CFLAGS	:= -Wall -Wno-format -MMD -DDEBUG -std=gnu99
RELEASE_CFLAGS	:= -Wall -Wno-unknown-pragmas -Wno-format -MMD -O3 -std=gnu99
LIBS			:= -lpthread -lrt

# include files path
# miio
INCS := -I../../miio
INCS += -I../../miio/device
INCS += -I../../miio/device/codec
INCS += -I../../miio/device/typedef
INCS += -I../../miio/uart
INCS += -I../../miio/util
# user
INCS += -I../../user
INCS += -I../../user/app
INCS += -I../../arch/linux
INCS += -I../../user/handler
INCS += -I../../user/ota
INCS += -I../../user/ota/xmodem

# source files path
# miio
SRC_DIRS := ../../miio
SRC_DIRS += ../../miio/device
SRC_DIRS += ../../miio/device/codec
SRC_DIRS += ../../miio/device/typedef
SRC_DIRS += ../../miio/uart
SRC_DIRS += ../../miio/util
# user
SRC_DIRS += ../../user
SRC_DIRS += ../../user/app
SRC_DIRS += ../../arch/linux
SRC_DIRS += ../../user/handler
SRC_DIRS += ../../user/ota
SRC_DIRS += ../../user/ota/xmodem

# lib files path
LIB_DIRS	:=
EXTRA_LIBS	:=  $(foreach dir, $(LIB_DIRS), $(wildcard $(dir)/*.a))

DEBUG_LDFLAGS	:= -g
RELEASE_LDFLAGS	:=

ifeq (YES, ${DEBUG})
   CFLAGS       := ${DEBUG_CFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CFLAGS       := ${RELEASE_CFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
   CFLAGS   += -pg -O3
   LDFLAGS  += -pg
endif

# C MACRO , -DXXXXXXX
DEFS	:=

# Makefile code common to all platforms
CFLAGS	+= ${DEFS}

#****************************************************************************
# start prepare operation
#****************************************************************************
SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJS := $(addsuffix .o,$(basename ${SRCS}))
DEPS := $(addsuffix .d,$(basename ${SRCS}))

#****************************************************************************
# start compile operation
#****************************************************************************
OUTPUT := mcu_demo
.PHONY : all clean distclean execute

all: ${OUTPUT}

${OUTPUT}: ${OBJS}
	${LD} -o $@  ${LDFLAGS} ${OBJS} ${LIBS} ${EXTRA_LIBS} ${INCS}
%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

#****************************************************************************
# start clear operation
#****************************************************************************
clean:
	-rm -f ${OBJS} ${DEPS}
distclean:
	-rm -f ${OBJS} ${OUTPUT} ${DEPS}
execute :
	../../../${OUTPUT}
