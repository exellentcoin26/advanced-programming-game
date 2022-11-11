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

CPPFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS_DEBUG := -g -Og

CPPFLAGS += $(CPPFLAGS_DEBUG)

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

INCFLAGS += -I$(LIBDIR)/sfml/include

LDFLAGS += -L$(LIBDIR)/sfml/lib
LDFLAGS += -lsfml-graphics-s
LDFLAGS += -lsfml-window-s
LDFLAGS += -lsfml-system-s
# static dependencies of SFML
LDFLAGS += -lGLU -lGL -lX11 -ludev -lpthread -lXrandr

# =========================
# 	    Make Targets
# =========================

.PHONY: all lib run clean clean-all

all: build

build: $(BUILDDIR)/$(BIN)

$(BUILDDIR)/%.o: %.$(CPP_SRC_EXTENSION)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) -c $< -o $@

$(BUILDDIR)/$(BIN): lib $(OBJS)
	$(CXX) $(OBJS) $(INCFLAGS) $(LDFLAGS) -o $@

lib:
	cd $(LIBDIR)/sfml && cmake -DBUILD_SHARED_LIBS=false . > /dev/null . && make -s -j$(NUM_COMP_THREADS) $(SFML_TARGETS)

run: build
	./seg_wrapper.sh $(BUILDDIR)/$(BIN)

clean:
	$(RM) $(BUILDDIR)

clean-all: clean
	# reset submodules to last commit
	git submodule foreach --recursive git clean -xfd

# command for debugging variables
print-%  : ; @echo $* = $($*)
