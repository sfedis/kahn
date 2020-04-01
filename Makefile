CC=gcc
CFLAGS=-fopenmp
OBJ=kahn

# Compile without optimizations
make:
	$(CC) -o $(OBJ).o $(CFLAGS) $(OBJ).c

# Compile using maximum optimizations
3:
	$(CC) -o $(OBJ).o $(CFLAGS) $(OBJ).c -O3

# Clean the source files
clean:
	rm -f *.o

# Lint based on the included configuration using Clang
lint:
	find . -regex '.*\.\(c\|h\)' -exec clang-format-9 -style=file -i {} \;