
BUILD ?= debug
BUILD_DIR ?= build
EXECUTABLE = raytracer

ifeq ($(BUILD),$(filter $(BUILD),debug release))
BUILD_DIR := $(BUILD_DIR)/$(BUILD)
else
$(error Unknown build type)
endif

SOURCE_DIR = src
INCLUDE_DIRS := -I$(SOURCE_DIR)
CPP_FILES := $(shell find $(SOURCE_DIR) -name '*.cpp')
OBJ_FILES = $(addprefix $(BUILD_DIR)/,$(CPP_FILES:%.cpp=%.o))
DEP_FILES = $(OBJ_FILES:%.o=%.d)

CXX ?= clang++
CXXFLAGS += -std=c++14 -pthread
CXXFLAGS.debug := -g -O0
CXXFLAGS.release := -O2
CXXFLAGS += $(CXXFLAGS.$(BUILD))

.PHONY: all header clean footer

all: header $(BUILD_DIR)/$(EXECUTABLE) footer

# Include .d dependencies
-include $(DEP_FILES)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $< $(INCLUDE_DIRS)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDE_DIRS)

header:
	@echo "==== $(BUILD) build"

footer:
	@echo "==== done"

clean:
	rm -r $(BUILD_DIR)
