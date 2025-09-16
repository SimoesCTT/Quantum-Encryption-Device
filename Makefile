# Quantum Encryption Device (QED) - Makefile
# Hardware-Dependent Cryptographic System Based on Physical Resonance
# Copyright (C) 2025 Americo Simoes. All rights reserved.

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -D_GNU_SOURCE
DEBUG_CFLAGS = -Wall -Wextra -g -O0 -std=c99 -D_GNU_SOURCE -DDEBUG
LDFLAGS = -lssl -lcrypto -lm

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib
TESTDIR = tests

# Target names
TARGET = qed
LIBRARY = libqed.a
SHARED_LIB = libqed.so

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LIB_OBJECTS = $(filter-out $(OBJDIR)/quantum_cli.o, $(OBJECTS))

# Header files
HEADERS = $(wildcard $(INCDIR)/*.h)

# Installation directories
PREFIX = /usr/local
BINDIR_INSTALL = $(PREFIX)/bin
LIBDIR_INSTALL = $(PREFIX)/lib
INCDIR_INSTALL = $(PREFIX)/include

# Default target
all: $(BINDIR)/$(TARGET) $(LIBDIR)/$(LIBRARY) $(LIBDIR)/$(SHARED_LIB)

# Create directories
$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -fPIC -I$(INCDIR) -c $< -o $@

# Build main executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	@echo "Building executable $@..."
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ Built Quantum Encryption Device executable: $@"

# Build static library
$(LIBDIR)/$(LIBRARY): $(LIB_OBJECTS) | $(LIBDIR)
	@echo "Creating static library $@..."
	ar rcs $@ $(LIB_OBJECTS)
	@echo "✅ Built static library: $@"

# Build shared library
$(LIBDIR)/$(SHARED_LIB): $(LIB_OBJECTS) | $(LIBDIR)
	@echo "Creating shared library $@..."
	$(CC) -shared -o $@ $(LIB_OBJECTS) $(LDFLAGS)
	@echo "✅ Built shared library: $@"

# Debug build
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: clean all
	@echo "🐛 Debug build complete"

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)
	@echo "✅ Clean complete"

# Install system-wide
install: all
	@echo "Installing Quantum Encryption Device..."
	install -d $(BINDIR_INSTALL)
	install -m 755 $(BINDIR)/$(TARGET) $(BINDIR_INSTALL)/
	install -d $(LIBDIR_INSTALL)
	install -m 644 $(LIBDIR)/$(LIBRARY) $(LIBDIR_INSTALL)/
	install -m 755 $(LIBDIR)/$(SHARED_LIB) $(LIBDIR_INSTALL)/
	install -d $(INCDIR_INSTALL)
	install -m 644 $(INCDIR)/*.h $(INCDIR_INSTALL)/
	ldconfig
	@echo "✅ Installation complete"
	@echo "   Executable: $(BINDIR_INSTALL)/$(TARGET)"
	@echo "   Libraries: $(LIBDIR_INSTALL)/$(LIBRARY), $(LIBDIR_INSTALL)/$(SHARED_LIB)"
	@echo "   Headers: $(INCDIR_INSTALL)/"

# Uninstall
uninstall:
	@echo "Uninstalling Quantum Encryption Device..."
	rm -f $(BINDIR_INSTALL)/$(TARGET)
	rm -f $(LIBDIR_INSTALL)/$(LIBRARY)
	rm -f $(LIBDIR_INSTALL)/$(SHARED_LIB)
	rm -f $(INCDIR_INSTALL)/quantum_encryption.h
	ldconfig
	@echo "✅ Uninstallation complete"

# Run tests
test: $(BINDIR)/$(TARGET)
	@echo "Running basic functionality tests..."
	@echo "Testing hardware detection..."
	./$(BINDIR)/$(TARGET) --info
	@echo "✅ Basic tests passed"

# Check for dependencies
deps:
	@echo "Checking dependencies..."
	@which gcc > /dev/null || (echo "❌ gcc not found" && exit 1)
	@pkg-config --exists openssl || (echo "❌ OpenSSL development libraries not found" && exit 1)
	@echo "✅ All dependencies satisfied"
	@echo "   GCC: $(shell gcc --version | head -n1)"
	@echo "   OpenSSL: $(shell pkg-config --modversion openssl)"

# Show help
help:
	@echo "Quantum Encryption Device (QED) Build System"
	@echo "Copyright (C) 2025 Americo Simoes. All rights reserved."
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build executable and libraries (default)"
	@echo "  debug      - Build with debug symbols and no optimization"
	@echo "  clean      - Remove all build files"
	@echo "  install    - Install system-wide (requires sudo)"
	@echo "  uninstall  - Remove system installation (requires sudo)"
	@echo "  test       - Run basic functionality tests"
	@echo "  deps       - Check for required dependencies"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Build files will be created in:"
	@echo "  $(OBJDIR)/     - Object files"
	@echo "  $(BINDIR)/     - Executable"
	@echo "  $(LIBDIR)/     - Libraries"

# Show version info
version:
	@echo "Quantum Encryption Device (QED) Build System"
	@echo "Version: 2.0.0"
	@echo "Copyright (C) 2025 Americo Simoes. All rights reserved."

# Create source package
package: clean
	@echo "Creating source package..."
	tar czf qed-2.0.0-src.tar.gz --exclude='.git*' --exclude='*.tar.gz' .
	@echo "✅ Source package created: qed-2.0.0-src.tar.gz"

# Phony targets
.PHONY: all debug clean install uninstall test deps help version package