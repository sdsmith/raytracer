EXECUTABLE = raytracer
BUILD ?= debug
BUILD_DIR ?= build

CXX ?= clang++
CXXFLAGS += -std=c++14 -pthread # Required
CXXFLAGS.debug := -g -O0
CXXFLAGS.release := -O2
CXXFLAGS += $(CXXFLAGS.$(BUILD))
CXXFLAGS += -Wall -Werror -Wextra -Weffc++ -Wfloat-equal -Werror-implicit-function-declaration -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wno-unused-result
CXXFLAGS += -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code # Potentially noisy and not worth it
CXXFLAGS += -ftrapv # -fsanitize=address # asan not available on windows...
# Warnings to ignore for now
CXXFLAGS += -Wno-unused-parameter -Wno-float-equal
#CXXFLAGS += -Wshadow-compatible-local # CI using gcc5 w/o this flag, clang does not have this flag

LDFLAGS := $(CXXFLAGS)
# Remove -pthread from clang to avoid unsued linker flag
ifeq ($(findstring clang,$(CXX)),clang)
LDFLAGS := $(filter-out -pthread,$(CXXFLAGS))
endif

ifeq ($(BUILD),$(filter $(BUILD),debug release))
BUILD_DIR := $(BUILD_DIR)/$(CXX)/$(BUILD)
else
$(error Unknown build type)
endif

SOURCE_DIR = src
INCLUDE_DIRS := -I$(SOURCE_DIR)
CPP_FILES := $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJ_FILES = $(addprefix $(BUILD_DIR)/,$(CPP_FILES:%.cpp=%.o))
DEP_FILES = $(OBJ_FILES:%.o=%.d)

.PHONY: all header clean

all: header $(BUILD_DIR)/$(EXECUTABLE)

# Include .d dependencies
-include $(DEP_FILES)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< -MMD $(INCLUDE_DIRS) $(CXXFLAGS)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(INCLUDE_DIRS) $(LDFLAGS)

header:
	@echo "==== $(CXX) $(BUILD) build"
	@echo "== $(CXX) version:"
	$(CXX) -v
	@echo "== build"

clean:
	rm -r $(BUILD_DIR)
