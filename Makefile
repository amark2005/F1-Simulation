# Variables
CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra
TARGET   := main
SRC      := main.cpp

# Compatibility Flags:
# -static: Links all libraries (including glibc) statically.
# -static-libgcc -static-libstdc++: Specifically targets C++ runtime compatibility.
LDFLAGS  := -static

.PHONY: all clean

# Default target
all: $(TARGET)
	./$(TARGET)

# Link the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean up
clean:
	rm -f $(TARGET)