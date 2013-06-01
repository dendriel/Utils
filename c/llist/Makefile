##
# Makefile for compiling main
# Author: vitor.rozsa
# date: 14/02/2013
##

TOP := .

ENABLE_EFENCE := #ENABLED

COMPILER := gcc
CFLAGS := -Wall -Werror

TARGET := main
SOURCE := main.c

INCLUDES :=  -I$(TOP)/src/inc

OBJ_ADD := $(TOP)/src/llist.o

LIB_DIR :=

LIBS =

ifeq ($(ENABLE_EFENCE), ENABLED)
	LIBS += -lefence
endif

$(TARGET): $(SOURCE) $(OBJ_ADD)
	$(COMPILER) $(FLAGS) $(SOURCE) $(INCLUDES) $(OBJ_ADD) $(LIBS) -o $(TARGET) 
	
$(OBJ_ADD):
	@make -C $(TOP)/src

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
