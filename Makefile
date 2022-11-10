# =========================
# 		Base Config
# =========================

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

# =========================
# 	   Compile Flags
# =========================

CPPFLAGS ?= -std=c++11 -Wall -Wextra -Wpedantic
CPPFLAGS_DEBUG := -g -Og

SRCS := $(shell find $(SRCDIR) -name "*.$(CPP_SRC_EXTENSION)")
OBJS := $(patsubst %.$(CPP_SRC_EXTENSION),$(BUILDDIR)/%.o,$(SRCS))

# include all header files
INCFLAGS += $(addprefix -I,$(shell find $(SRCDIR) -type d))

# =========================
#      Library Config
# =========================

NUM_COMP_THREADS ?= 12

SFML_TARGETS := sfml-system sfml-window sfml-graphics

# =========================
#      Library Linking
# =========================

INCFLAGS += -I$(LIBDIR)/sfml/include/SFML

LDFLAGS += -L$(LIBDIR)/sfml/lib
LDFLAGS += -lsfml-system-s
LDFLAGS += -lsfml-window-s
LDFLAGS += -lsfml-graphics-s

# =========================
# 	    Make Targets
# =========================

.PHONY: all lib run clean clean-all

all: build

build: lib $(BUILDDIR)/$(BIN)

$(BUILDDIR)/%.o: %.$(CPP_SRC_EXTENSION)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) -c $< -o $@

$(BUILDDIR)/$(BIN): $(OBJS)
	$(CXX) $(OBJS) $(INCFLAGS) $(LDFLAGS) -o $@

lib:
	cd $(LIBDIR)/sfml && cmake -DBUILD_SHARED_LIBS=false > /dev/null . && make -s -j$(NUM_COMP_THREADS) $(SFML_TARGETS)

run: build
	$(BUILDDIR)/$(BIN)

clean:
	$(RM) $(BUILDDIR)

clean-all: clean
	# reset submodules to last commit
	git submodule foreach --recursive git clean -xfd

# command for debugging variables
print-%  : ; @echo $* = $($*)
