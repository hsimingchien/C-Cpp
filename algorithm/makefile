SRCS:=$(wildcard *.c)
SRCS+=$(wildcard *.cpp)
$(info "SRCS:$(SRCS)")
OBJS:=$(SRCS:%.c=%)
OBJS:=$(OBJS:%.cpp=%)
$(info "OBJS:$(OBJS)")

all: $(OBJS)



# .PHONY意思表示clean是一个“伪目标”
.PHONY: clean
clean:
# -rm表示命令执行有错的话, 忽略错误, 继续执行
	-rm $(OBJS)
