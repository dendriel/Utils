##
# Makefile for compiling cli.o
# Author: vitor.rozsa
# date: 19/02/2013
##

DIR_TOP := .

COMPILER := gcc
CFLAGS := -Wall -Werror -c

OBJS := cli.o
OBJS_MOD := 755

SOURCE := src/cli.c

INCLUDES :=  -I$(DIR_TOP)/inc

$(OBJS): $(SOURCE)
	@echo "> Will create the objects: $(OBJS)"
	$(COMPILER) $(CFLAGS) $(SOURCE) $(INCLUDES)
	@chmod $(OBJS_MOD) $(OBJS)
	@echo "> Done."

	
.PHONY: clean
clean:
	@echo "rm -f *.o"
	@rm -f *.o

.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"