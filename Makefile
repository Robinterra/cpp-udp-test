# Define the cross-compiler and flags
CXX=arm-linux-gnueabihf-g++
CXXFLAGS=-Wall -O2

# Define the target executable name
TARGET=bin/out.exe

# Define the source and object files
SRCDIR=src
OBJDIR=bin

# Find all .cpp files in the src directory and its subdirectories
SRCS=$(shell find $(SRCDIR) -name '*.cc')
# Generate the corresponding object files in the bin directory
OBJS=$(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Define the include directories
INCLUDES=-Isrc

# Default target
all: $(TARGET)

# Link the object files into the final executable with static linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ -static

# Compile the source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create the object files directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up generated files
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
	find $(OBJDIR) -type f -name '*.o' -delete

.PHONY: all clean
