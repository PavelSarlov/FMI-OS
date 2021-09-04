
ifndef CC
	CC=gcc
endif
CFLAGS=-std=c99 -Werror -Wall -Wpedantic -Wextra
SRCS=$(wildcard *.c)
OBJS=$(SRC:.c=.o)
EXES=$(subst .c,,$(SRCS))
RM=rm -f

all: $(OBJS) $(EXES)

clean:
	$(RM) $(OBJS) *.o
