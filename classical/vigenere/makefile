# 指定编译器
CXX = g++

# 指定源文件
SRCS = $(wildcard *.cpp)

# 指定目标文件
OBJS = $(patsubst %.cpp, target/%, $(SRCS))

# 默认目标
all: $(OBJS)

# 规则：如何从 .cpp 文件生成可执行文件
target/%: %.cpp
	mkdir -p target
	$(CXX) $< -o $@

# 清理目标
clean:
	rm -rf target