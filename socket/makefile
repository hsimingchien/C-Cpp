CFLAGS += -g

SRC := $(wildcard *.c)
$(info "SRC: $(SRC)")
BIN := $(SRC:%.c=%)
$(info "BIN: $(BIN)")
all: $(BIN)

#server: server.c
#client: client.c

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $@.c -lpthread

define two-lines
    foo
    foo1
endef

a:
	@echo "$(two-lines)"


# .PHONY意思表示clean是一个“伪目标”
.PHONY: clean
clean:
# -rm表示命令执行有错的话, 忽略错误, 继续执行
	-rm $(BIN)
