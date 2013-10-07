##
# Description: Basic template to build .t2t- from txt2tags - files. Build html files. Also compile
# extra directories with "make -C" (extra directories like graphics, flowcharts, diagrams, etc)
# Author: vitor.rozsa
# Contact: vitor.rozsa@hotmail.com
# Date: 30/09/2013
#
# Usage: make; make clean
##

# Builder program #
BUILDER=txt2tags
# Output file extension #
EXT=html
# List of .t2t files #
DEPS=$(wildcard ./*.t2t)
# List of .html files #
TARGET=$(DEPS:.t2t=.$(EXT))
# Style file #
STYLE=--style=./style.css

# Compile extra directories #
EXTRA_DIR=graph


all: $(TARGET) $(EXTRA_DIR)

$(TARGET): $(DEPS)
	$(BUILDER) -t $(EXT) $(STYLE) -i $(DEPS) -o $(TARGET)


.PHONY: clean $(EXTRA_DIR)

$(EXTRA_DIR):
	for dir in $(EXTRA_DIR); do make -C $$dir; done


clean:
	for file in $(TARGET); do rm -f $$file; done
	@for dir in $(EXTRA_DIR); do make clean -C $$dir; done
	echo [clean] Done.
