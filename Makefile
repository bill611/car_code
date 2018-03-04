MAKEROOT = $(shell pwd)
# 主程序Makefile

# 填相关编译路径
COMPILER_PATH=
KERNERL_PATH=

# 在指定目录下生成的应用程序
EXE = v
BIN_TARGET = ${BIN_DIR}/${EXE}

SRC_DIR = $(MAKEROOT)/src
OBJ_DIR = $(MAKEROOT)/obj
BIN_DIR = $(MAKEROOT)/out

# INC_DIR 目录下为相应库的头文件
INC_DIR = \
		  $(MAKEROOT)/src/application \
		  $(MAKEROOT)/src/drivers \
		  $(MAKEROOT)/src/gui \
		  $(MAKEROOT)/src/net \
		  $(KERNERL_PATH)/include \
		  $(COMPILER_PATH)/include

SRC = $(wildcard ${SRC_DIR}/application/*.c)
SRC += $(wildcard ${SRC_DIR}/gui/*.c)
SRC += $(wildcard ${SRC_DIR}/drivers/*.c)
SRC += $(wildcard ${SRC_DIR}/net/*.c)

include evn.mk
include path.mk

CFLAGS += ${addprefix -I,${INC_DIR}}

# wildcard:扩展通配符，notdir;去除路径，patsubst;替换通配符

OBJ = $(patsubst %.c,${OBJ_DIR}/%.o,$(notdir ${SRC}))
DEPS = $(patsubst %.c, ${OBJ_DIR}/%.d, $(notdir ${SRC}))

# 链接路径
# -Xlinker编译时可重复查找依赖库，和库的次序无关
# LIB_DIR = \
		  # $(COMPILER_PATH)/lib

# XLINKER = -Xlinker "-(" -lts -lfreetype -lminigui_ths -ldl -lpng -ljpeg  -lm -lpthread  -Xlinker "-)"

export CC LIB_DIR CFLAGS OBJ_DIR INC_DIR DEPS
# $@：表示目标文件，$^：表示所有的依赖文件，$<：表示第一个依赖文件，$?：表示比目标还要新的依赖文件列表
all: make_C ${BIN_TARGET}
	@cp ${BIN_TARGET} $(MAKEROOT)/../nand/nand1-2
	@cp ${BIN_TARGET} ~/work/arm_share/nand2

make_C:
	@mkdir -p ${BIN_DIR}
	@mkdir -p ${OBJ_DIR}
	@make -C src

# 在指定目录下，将所有的.c文件编译为相应的同名.o文件
${BIN_TARGET}:${OBJ}
	@$(COMPILE)

debug:
	make -C src debug

.PHONY:clean
clean:
	@-rm -rf ${BIN_TARGET} obj ${BIN_DIR}
