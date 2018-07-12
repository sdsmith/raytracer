CXX ?= clang++
CXXFLAGS ?= -std=c++14
BUILD_DIR = build
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ${BUILD_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))

.PHONY: all clean setup

all: ${BUILD_DIR}/raytracer

${BUILD_DIR}/%.o: %.cpp setup
	$(CXX) $(CXXFLAGS) -c -o $@ $<

${BUILD_DIR}/raytracer: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

clean:
	rm -r ${BUILD_DIR}

setup:
	mkdir -p ${BUILD_DIR}
