##
# Description: Basic template to build .dot - from graphviz - files. Build png files.
# Author: vitor.rozsa
# Contact: vitor.rozsa@hotmail.com
# Date: 30/09/2013
#
# Usage: make; make clean
##

# Builder program #
BUILDER=dot
# Output file extension #
EXT=png
# List of .dot files #
DEPS=$(wildcard ./*.dot)
# List of names without extension #
DEPS_STRIPED=$(DEPS:.dot=)
# List of .png files #
TARGET=$(DEPS:.dot=.$(EXT))


all: $(TARGET)

$(TARGET): $(DEPS)
	for t in $(DEPS_STRIPED); do $(BUILDER) -T$(EXT) $$t.dot -o $$t.$(EXT); done
	@echo [make] Done.

.PHONY:clean

clean:
	for t in $(TARGET); do rm -f $$t; done
	@echo [clean] Done.
