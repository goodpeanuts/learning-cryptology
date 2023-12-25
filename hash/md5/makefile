# 指定编译器
CC = gcc

# 指定源文件
SRCS = main.c md5.c

# 指定目标文件
TARGET = md5

# 默认目标
all: $(TARGET)

# 规则：如何从源文件生成目标文件
$(TARGET): $(SRCS)
	mkdir -p target
	$(CC) $^ -o target/$@

# 清理目标
clean:
	rm -rf target