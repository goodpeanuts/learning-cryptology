# 指定编译器
CC = gcc

# 获取仓库根目录
ROOT_DIR = $(shell git rev-parse --show-toplevel)

# 指定源文件
SRCS = main.c md5.c

# 指定目标文件
TARGET = $(ROOT_DIR)/target/md5

# 默认目标
all: $(TARGET)

# 规则：如何从源文件生成目标文件
$(TARGET): $(SRCS)
	mkdir -p $(ROOT_DIR)/target
	$(CC) $^ -o $@

# 清理目标
clean:
	rm -f $(TARGET)