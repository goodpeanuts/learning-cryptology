# 指定编译器
CXX = g++

# 获取仓库根目录
ROOT_DIR = $(shell git rev-parse --show-toplevel)

# 指定源文件
SRCS = $(wildcard *.cpp)

# 指定目标文件
OBJS = $(patsubst %.cpp, $(ROOT_DIR)/target/%, $(SRCS))

# 默认目标
all: $(OBJS)

# 规则：如何从 .cpp 文件生成可执行文件
$(ROOT_DIR)/target/%: %.cpp
	mkdir -p $(ROOT_DIR)/target
	$(CXX) $< -o $@ -lcryptopp -lssl -lcrypto -lgmp

# 清理目标
clean:
	rm -f $(OBJS)