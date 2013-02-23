##
# Makefile for compiling main
# Author: vitor.rozsa
# date: 19/02/2013
##

DIR_TOP := .

ENABLE_EFENCE := #ENABLED

COMPILER := gcc
CFLAGS := -Wall -Werror

TARGET := main
SOURCE := main.c

INCLUDES :=  -I$(DIR_TOP)/lib/inc

OBJ_ADD := $(DIR_TOP)/lib/cli.o

LIB_DIR :=

LIBS = -lpthread

ifeq ($(ENABLE_EFENCE), ENABLED)
	LIBS += -lefence
endif

$(TARGET): $(SOURCE) $(OBJ_ADD)
	$(COMPILER) $(CFLAGS) $(SOURCE) $(INCLUDES) $(OBJ_ADD) $(LIBS) -o $(TARGET) 
	
$(OBJ_ADD):
	@cd $(DIR_TOP)/lib/ && make


.PHONY: clean
clean:
	@echo "rm -f $(TARGET)"
	@rm -f $(TARGET)
	@echo "rm -f $(OBJ_ADD)"
	@rm -f $(OBJ_ADD)
	
#  ".PHONY" specifies that target help is not a file,
# even if there is a "help" file in the directory.
.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"