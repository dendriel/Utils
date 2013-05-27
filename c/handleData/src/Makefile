##
# Makefile for compiling handleData.o
# Author: vitor.rozsa
# date: 27/05/2013
##

TOP = .

CC = gcc
CFLAGS = -Wall -Werror #-lefence

SRCS = $(wildcard *.c)
TARGET = $(SRCS:.c=.o)
INCDEP = $(wildcard $(TOP)/inc/*.h)

INCLUDES = -I$(TOP)/inc


$(TARGET): $(SRCS) $(INCDEP)
	@echo "> Will create the objects: $(TARGET)"
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDES)
	@echo "> Done."


.PHONY: clean
clean:
	@echo $(SRCS) $(TARGET)
	@echo "rm -f *.o"
	@rm -f *.o
	@echo "rm -f *.dat"
	@rm -f *.dat

.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"
