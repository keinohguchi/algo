# SPDX-License-Identifier: GPL-2.0
SRC	:= factorial.c
SRC	+= issort.c
SRC	+= list.c
SRC	+= stack.c
SRC	+= queue.c
SRC	+= set.c
SRC	+= hash.c
SRC	+= tree.c
SRC	+= avl.c
SRC	+= heap.c
ASM	:= $(patsubst %.c,%.s,$(SRC))
OBJ	:= $(patsubst %.c,%.o,$(SRC))
TEST	:= $(patsubst %.c,%_test,$(SRC))
TESTOBJ	:= $(patsubst %,%.o,$(TEST))
INT	:= $(patsubst %.asm,%_int,$(wildcard *.asm))
CFLAGS	+= -Wall
CFLAGS	+= -Werror
CFLAGS	+= -g
.PHONY: all fmt build clean test
all: build $(DIS) $(ASM) $(INT) $(TEST)
$(TEST): $(OBJ) $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJ)
fmt:
	@go fmt *.go
build:
	@go build -race *.go
clean:
	@$(RM) $(OBJ) $(TESTOBJ) $(ASM) $(INT) $(TEST) *.o *.lst *.log
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<
%.s: %.c
	$(CC) $(CFLAGS) -O3 -S -masm=intel $<
%_int: %.asm
	yasm -f elf64 -g dwarf2 -l $@.lst -o $@.o $<
	$(CC) $(CFLAGS) -g -static -o $@ $@.o
test: $(TEST)
	-@echo "Go tests"
	-@echo "========"
	@go test -race -v *.go
	@go test -cover *.go
	-@echo "C tests"
	-@echo "======="
	@for t in $(TEST);                      \
	do                                      \
		printf "$$t:\t";                \
		if ./$$t 2>$$t.log 1>/dev/null; \
		then                            \
			echo PASS;              \
		else                            \
			echo FAIL;              \
			cat $$t.log;            \
			exit 1;                 \
		fi;                             \
	done
# CI targets.
.PHONY: arch64 ubuntu64
arch64: arch64-image
	docker run -v $(PWD):/home/build algo/$@ make all clean
ubuntu64: ubuntu64-image
	docker run -v $(PWD):/home/build algo/$@ make all clean
%-arch64: arch64-image
	docker run -v $(PWD):/home/build algo/arch64 make $* clean
%-ubuntu64: ubuntu64-image
	docker run -v $(PWD):/home/build algo/ubuntu64 make $* clean
%-image:
	docker build -t algo/$* -f Dockerfile.$* .
