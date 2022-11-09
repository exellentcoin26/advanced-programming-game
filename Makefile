SHELL := /bin/sh

CPP_SRC_EXTENSION := cc
CPP_HEADER_EXTENSION := h

BUILDDIR := ./build
SRCDIR := ./src
LIBDIR := ./lib
BIN := game

MAIN := $(SRCDIR)/main.$(CPP_SRC_EXTENSION)

CC := clang
CXX := clang++
RM := rm -rf

CPPFLAGS ?= -std=c++11 -Wall -Wextra -Wpedantic
CPPFLAGS_DEBUG := -g -Og

SRCS := $(shell find $(SRCDIR) -name "*.$(CPP_SRC_EXTENSION)")
OBJS := $(patsubst %.$(CPP_SRC_EXTENSION),$(BUILDDIR)/%.o,$(SRCS))

INCFLAGS += $(addprefix -I,$(shell find $(SRCDIR) -type d))

build: $(BUILDDIR)/$(BIN)

$(BUILDDIR)/%.o: %.$(CPP_SRC_EXTENSION)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@

$(BUILDDIR)/$(TEST): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $@

.PHONY:
run: build
	$(BUILDDIR)/$(BIN)

.PHONY:
clean:
	$(RM) $(BUILDDIR)

# command for debugging variables
print-%  : ; @echo $* = $($*)
