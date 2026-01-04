# Makefile for distributed n-coloring

# Compiler
CXX = mpic++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I../common

# Executable name
TARGET = distributed

# Source files
SOURCES = main.cpp algorithm.cpp ../common/Graph.cpp ../common/Node.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Debug flag
DEBUG ?= 0
ifeq ($(DEBUG), 1)
CXXFLAGS += -DDEBUG=1
endif

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to run the executable
run: all
	mpirun -n 4 ./$(TARGET)

# Target to run with debug logs
debug: clean
	$(MAKE) all DEBUG=1
	mpirun -n 4 ./$(TARGET)

# Target to clean up the project
clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all run debug clean
