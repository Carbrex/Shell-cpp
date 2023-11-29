# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall
DEBUGFLAGS = -g
OPTFLAGS = -O2

# Common source files
COMMON_SRCS = cp.cpp rm.cpp ls.cpp mv.cpp cd.cpp Shell.cpp
# Get the current Git branch
GIT_BRANCH := $(shell git rev-parse --abbrev-ref HEAD)

# Source files based on Git branch
ifeq ($(GIT_BRANCH), main)
    BRANCH_SRCS = $(COMMON_SRCS)
else
    $(error Unsupported branch: $(GIT_BRANCH))
endif

# Object files
BRANCH_OBJS = $(BRANCH_SRCS:.cpp=.o)

# Executable
BRANCH_TARGET = result

# Default target
all: debug release

# Rule to build the executable (debug mode)
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BRANCH_TARGET)_debug

# Rule to build the executable (release mode)
release: CXXFLAGS += $(OPTFLAGS)
release: $(BRANCH_TARGET)_release

# Actual build rule for the executable (debug mode)
$(BRANCH_TARGET)_debug: $(BRANCH_OBJS)
	cp Shell.o $(BRANCH_TARGET)_debug

# Actual build rule for the executable (release mode)
$(BRANCH_TARGET)_release: $(BRANCH_OBJS)
	cp Shell.o $(BRANCH_TARGET)_release

# Rule to build object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(BRANCH_OBJS) $(BRANCH_TARGET)_debug $(BRANCH_TARGET)_release

# Clear (clean) everything and rebuild in debug and release modes
clear: clean all
