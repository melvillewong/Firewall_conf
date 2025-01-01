# Variable
CC = gcc                   # Compiler
CFLAGS = -Wall -Werror -g  # Compiler flags (e.g., -Wall for all warnings, -g for debugging info)
# LDFLAGS =                # Linker flags (if needed)

# Targets and rules
# List of source files
SRC = main.c             # Add more .c files here (e.g., helper.c)

# List of object files (this will automatically convert .c files to .o files)
OBJ = $(SRC:.c=.o)

# Default target
all: main

# Rule to build the final executable from object files
main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up object files and executable

run: main
	./main

static-check:
	cppcheck --enable=all --suppress=missingIncludeSystem .

# valgrind: main
# 	valgrind ./main

clean:
	rm -f $(OBJ) main
