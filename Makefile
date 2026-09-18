# Compiler
CC = gcc

# Source files
SRC = src/main.c src/utils.c

# Output binary
OUT = bin/main

# Libraries
LIBS = -lSDL2

# Default target
all: $(OUT)

# Build rule
$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT) $(LIBS)

# Run the program
run: $(OUT)
	./$(OUT) assets/android.ppm

# Clean build artifacts
clean:
	rm -f $(OUT)
