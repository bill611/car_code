#!/bin/bash

set -e

if [ "$#" == 0 ]; then
	echo 'CC = arm-linux-gcc
STRIP = arm-linux-strip $(BIN_TARGET)
LIB_DIR += $(MAKEROOT)/src/libs
INC_DIR += $(MAKEROOT)/src/include
XLINKER = -Xlinker "-(" -lts -lfreetype -lminigui_ths -ldl -lpng -ljpeg  -lm -lpthread -lrt -Xlinker "-)"
COMPILE = $(CC) -muclibc -o $@ $(OBJ) ${addprefix -L,${LIB_DIR}} ${XLINKER}
CFLAGS = -muclibc -c -DWATCHDOG_DEBUG -O2' > evn.mk
echo '
COMPILER_PATH=/usr/local/arm_linux_4.2/arm-none-linux-gnueabi
KERNERL_PATH=/home/xubin/work/tools/kernel_2.6.35/linux-2.6.35.4
CFLAGS += -g -DWATCHDOG_DEBUG ' > path.mk
else
	echo 'CC = gcc
STRIP=
LIB_DIR += /usr/local/lib 
LIB_DIR += /usr/lib/x86_64-linux-gnu
XLINKER = -Xlinker "-(" -lminigui_ths -ldl -lpng -ljpeg  -lm -lpthread -lrt -Xlinker "-)"
COMPILE = $(CC) -g -o $@ $(OBJ) ${addprefix -L,${LIB_DIR}} ${XLINKER}
CFLAGS = -g -DPC -DWATCHDOG_DEBUG -c -O0' > evn.mk
echo '
COMPILER_PATH=
KERNERL_PATH=
CFLAGS += -g -DWATCHDOG_DEBUG ' > path.mk
fi

make

