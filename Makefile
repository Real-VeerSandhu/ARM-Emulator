# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -Iinclude -g -fsanitize=address,undefined

# Source files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp) src/cpu.cpp
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = arm_emulator

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Run the emulator
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run