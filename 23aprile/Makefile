# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Define include directories
INCLUDES = -I.

# Define the target executable
TARGET = cub3d

# Define source files
SRC = main.c main2.c functions.c

# Define object files
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(INCLUDES)

# Rule to create object files from C source files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to clean up the build
clean:
	rm -f $(OBJ) $(TARGET)

# Specify rules that do not correspond to files
.PHONY: all clean
