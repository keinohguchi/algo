# SPDX-License-Identifier: GPL-2.0
SRC	:= factorial.c
SRC	+= issort.c
SRC	+= list.c
OBJ	:= $(patsubst %.c,%.o,$(SRC))
TEST	:= $(patsubst %.c,%_test,$(SRC))
TESTOBJ	+= $(patsubst %,%.o,$(TEST))
CFLAGS	+= -Wall
CFLAGS	+= -Werror
CFLAGS	+= -g
.PHONY: all fmt build test clean
all: fmt build $(TEST)
$(TEST): $(OBJ) $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJ)
fmt:
	@go fmt *.go
build:
	@go build -race *.go
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<
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
clean:
	@$(RM) $(OBJ) $(TESTOBJ) $(TEST) *.log
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
