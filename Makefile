# SPDX-License-Identifier: GPL-2.0
SRC	:= issort.c
SRC	+= list.c
OBJ	:= $(patsubst %.c,%.o,$(SRC))
TEST	:= $(patsubst %.c,%_test,$(SRC))
TESTOBJ	+= $(patsubst %,%.o,$(TEST))
CFLAGS	+= -Wall
CFLAGS	+= -Werror
CFLAGS	+= -g
.PHONY: all test clean
all: $(TEST)
$(TEST): $(OBJ) $(TESTOBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJ)
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<
test: $(TEST)
	@for t in $(TEST);                      \
	do                                      \
		printf "$$t:\t";                \
		if ./$$t 2>$$t.log 1>/dev/null; \
		then                            \
			echo PASS;              \
		else                            \
			echo FAIL;              \
			cat $$t.log;            \
		fi;                             \
	done
clean:
	@$(RM) $(OBJ) $(TESTOBJ) $(TEST) *.log
