# =========================
# 		Base Config
# =========================

SHELL := /bin/sh

CPP_SRC_EXTENSION := cc
CPP_HEADER_EXTENSION := h

BUILDDIR := ./build
SRCDIR := ./src
TESTDIR := ./test
LIBDIR := ./lib
BIN := game
TEST := game_test

MAIN := $(SRCDIR)/main.$(CPP_SRC_EXTENSION)

CC := clang
CXX := clang++
RM := rm -rf

# =========================
# 	   Compile Flags
# =========================

CPPFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
CPPFLAGS_DEBUG := -g -O0 -fdebug-default-version=4

CPPFLAGS += $(CPPFLAGS_DEBUG)

SRCS := $(shell find $(SRCDIR) -name "*.$(CPP_SRC_EXTENSION)")
OBJS := $(patsubst %.$(CPP_SRC_EXTENSION),$(BUILDDIR)/%.o,$(SRCS))

TEST_SRCS := $(shell find $(TESTDIR) -name "*.$(CPP_SRC_EXTENSION)")

# list testable source files here
TEST_SRCS += $(SRCDIR)/utils/levelloader.$(CPP_SRC_EXTENSION)

TEST_OBJS := $(patsubst %.$(CPP_SRC_EXTENSION),$(BUILDDIR)/%.o,$(TEST_SRCS))

# include all header files
INCFLAGS += $(addprefix -I,$(shell find $(SRCDIR) -type d))

# =========================
#      Library Config
# =========================

SFML_TARGETS := sfml-system sfml-window sfml-graphics

# =========================
#      Library Linking
# =========================

INCFLAGS += -I$(LIBDIR)/sfml/include
INCFLAGS += -I$(LIBDIR)/tomlplusplus/include

TEST_INCFALGS += -I$(LIBDIR)/doctest/include

LDFLAGS += -L$(LIBDIR)/sfml/lib
LDFLAGS += -lsfml-graphics-s
LDFLAGS += -lsfml-window-s
LDFLAGS += -lsfml-system-s
# static dependencies of SFML
LDFLAGS += -lGL -lX11 -ludev -lpthread -lXrandr -lfreetype

# =========================
# 	    Make Targets
# =========================

.PHONY: all lib run clean clean-all build-test

all: build build-test

build: $(BUILDDIR)/$(BIN)

build-test: CPPFLAGS += -DTEST
build-test: INCFLAGS += $(TEST_INCFALGS)
build-test: $(BUILDDIR)/$(TEST)

$(BUILDDIR)/%.o: %.$(CPP_SRC_EXTENSION)
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(INCFLAGS) -MMD -c $< -o $@

$(BUILDDIR)/$(BIN): lib $(OBJS)
	$(CXX) $(OBJS) $(INCFLAGS) $(LDFLAGS) -o $@

$(BUILDDIR)/$(TEST): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) $(INCFLAGS) -o $@

lib:
	cd $(LIBDIR)/sfml && cmake -Wno-dev -DBUILD_SHARED_LIBS=false . > /dev/null . && $(MAKE) -s $(SFML_TARGETS)

run: build
	./seg_wrapper.sh $(BUILDDIR)/$(BIN)
	# $(BUILDDIR)/$(BIN)

test: build-test
	$(BUILDDIR)/$(TEST)

clean:
	$(RM) $(BUILDDIR)

clean-all: clean
	# reset submodules to last commit
	git submodule foreach --recursive git clean -xfd

-include $(shell find $(BUILDDIR) -name "*.d")

# command for debugging variables
print-%  : ; @echo $* = $($*)
