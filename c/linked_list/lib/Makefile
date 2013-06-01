##
# Makefile for compiling linked_list.o
# Author: vitor.rozsa
# date: 14/02/2013
##

TOP = .

CC = gcc
CFLAGS = -Wall -Werror -c

TARGET = linked_list.o
OBJS_MOD = 755

SRCS = linked_list.c

INCLUDES = -I$(TOP)/inc



$(TARGET): $(SRCS)
	@echo "> Will create the objects: $(TARGET)"
	$(CC) $(CFLAGS) $(SRCS) $(INCLUDES)
	@chmod $(OBJS_MOD) $(TARGET)
	@echo "> Done."

	
.PHONY: clean
clean:
	@echo "rm -f *.o"
	@rm -f *.o

.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"
