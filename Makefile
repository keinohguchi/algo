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
.PHONY: all fmt test clean
all: fmt $(TEST)
$(TEST): $(OBJ) $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJ)
fmt:
	@go fmt *.go
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<
test: $(TEST)
	@echo "C tests"
	@echo "======="
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
	@echo "Go tests"
	@echo "========"
	@go test -v *.go
clean:
	@$(RM) $(OBJ) $(TESTOBJ) $(TEST) *.log
