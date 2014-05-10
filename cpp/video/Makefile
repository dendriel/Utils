##
# Makefile for Video module.
# Author: vitor.rozsa
# date: 10/05/2014
#
# *Is neeed to start the SDL engine before using the Video class. Take a look at the 
# "Video" project, in my "games" repository, to read an example about this module.
##

TOP = .

CPP = g++
CFLAGS = -Wall -Werror -c

INCLUDE=include
TARGET=libvideo.a
INFO_FILE=video.txt
OBJS_MOD = 755

INC = $(wildcard ./$(INCLUDE)/*.h)
SRCS = $(wildcard ./*.cpp)
OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I$(TOP)/$(INCLUDE)


$(TARGET): $(SRCS) $(INC)
	@echo "> Will create the static library: $(TARGET)"
	$(CPP) $(CFLAGS) $(SRCS) $(INCLUDES)
	ar rvs $(TARGET) $(OBJS)
	@chmod $(OBJS_MOD) $(TARGET)
	@echo "Author: Vitor Rozsa; vitor.rozsa@hotmail.com; github.com/dendriel; Brazil" > $(INFO_FILE)
	@echo "Compiled at" >> $(INFO_FILE)
	@date >> $(INFO_FILE)
	@echo "> Done."

	
.PHONY: clean
clean:
	@echo "rm -f *.o *.a $(INFO_FILE)"
	@rm -f *.o *.a $(INFO_FILE)

.PHONY: help
help:
	@echo "Usage: make"
	@echo "       make clean"
