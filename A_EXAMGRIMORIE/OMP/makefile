# Makefile to compile numbered .c files (e.g., 0.c, 1.c, ...) into executables

CC = gcc
CFLAGS = -fopenmp -Wall -O2

# Rule to compile any .c file into an executable with the same base name
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean target: removes all numbered executables (0, 1, 2, ...)
clean:
	rm -f [0-9] [0-9][0-9] [0-9][0-9][0-9]
