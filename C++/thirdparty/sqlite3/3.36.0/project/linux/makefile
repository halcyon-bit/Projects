# gcc sqlite3.c -lpthread -ldl -fPIC -shared -o libsqlite3.so
CC=gcc

# -c 只激活预处理,编译,和汇编,也就是他只把程序做成obj文件
CFLAGS = -c -O2 -fPIC

MAIN_DIR = ../..

TARGET_BIN_DIR = $(MAIN_DIR)/bin/linux
TARGET_OBJ_DIR = ./tmp

# 创建目录
$(shell if [ ! -e $(TARGET_BIN_DIR) ];then mkdir -p $(TARGET_BIN_DIR); fi)
$(shell if [ ! -e $(TARGET_OBJ_DIR) ];then mkdir -p $(TARGET_OBJ_DIR); fi)

BIN_NAME  = libsqlite3.so

SRC_DIR = $(MAIN_DIR)/src

INCLUDE_PREFIX = -I

LINK_PREFIX = -L

ALL_PATHS = $(SRC_DIR)
ALL_PATHS += $(MAIN_DIR)/include

ALL_INCLUDES = $(addprefix $(INCLUDE_PREFIX), $(ALL_PATHS))

OBJ_CMD = -o 

BIN_OBJS = $(TARGET_OBJ_DIR)/sqlite3.o

$(TARGET_OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@echo ---------------------------------------------------------
	@echo $(ALL_INCLUDES)
	@echo Build OBJECT $(@) from SOURCE $<
	$(CC) $(CFLAGS) $(ALL_INCLUDES) $(OBJ_CMD) $@ $<
	@echo ---------------------------------------------------------

.PHONY: all clean

all:prep bin 

prep:
	@if test ! -d $(TARGET_BIN_DIR); then mkdir $(TARGET_BIN_DIR); fi
	@if test ! -d $(TARGET_OBJ_DIR); then mkdir $(TARGET_OBJ_DIR); fi

bin:$(BIN_OBJS)
	@echo create bin file $(BIN_NAME)
	@$(CC) -lpthread -ldl -fPIC -shared -o $(TARGET_BIN_DIR)/$(BIN_NAME) $^ -s
	
clean:
	@rm -fr $(TARGET_OBJ_DIR) $(TARGET_BIN_DIR) 
