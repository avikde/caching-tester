# Compiler
CXX = g++

# Target executable name
TARGET = caching-tester

# Source files
SRCS = main.cpp

# Common flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Debug flags
DEBUG_FLAGS = -g -O0 -DDEBUG

# Release flags
RELEASE_FLAGS = -O3 -DNDEBUG

# Default target
.PHONY: all
all: release

# Debug build
.PHONY: debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Release build
.PHONY: release
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(TARGET)

# Help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make          - Build release version (default)"
	@echo "  make release  - Build with O3 optimization"
	@echo "  make debug    - Build with debug symbols and no optimization"
	@echo "  make clean    - Remove built executable"
	@echo "  make help     - Show this help message"
