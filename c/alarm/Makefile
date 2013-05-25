##
# Makefile for compiling main
# Author: vitor.rozsa
# date: 14/02/2013
##

TOP = .

ENABLE_EFENCE = #ENABLED

CC = gcc
CFLAGS = -Wall -Werror

TARGET = main
SOURCE = main.c

INCLUDES = -I$(TOP)/src/inc

OBJS = $(TOP)/src/alarm.o

LDFLAGS =
LDLIBS = -lpthread -lrt

ifeq ($(ENABLE_EFENCE), ENABLED)
	LDLIBS += -lefence
endif

$(TARGET): $(SOURCE) $(OBJS)
	$(CC) $(LDFLAGS) $(SOURCE) $(INCLUDES) $(OBJS) $(LDLIBS) -o $(TARGET) 
	
$(OBJS):
	@cd $(TOP)/lib/ && make


.PHONY: clean
clean:
	@echo "rm -f $(TARGET)"
	@rm -f $(TARGET)
	@echo "rm -f $(OBJS)"
	@rm -f $(OBJS)
	
#  ".PHONY" specifies that target help is not a file,
# even if there is a "help" file in the directory.
.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"
