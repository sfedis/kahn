# Kahn

Parallel implementation of Kahn's topological sorting using OpenMP

## Compiling

### Automatic (Recommended)

You can make use of the provided makefile commands to compile:

* `make`: Compile without runtime optimizations (-O0)
* `make 3`: Compile with maximum execution time optimizations (-O3)
* `make clean`: Clean the compiled source

### Manual 

You can compile manually by running: `gcc -o kahn.o -fopenmp kahn.c -O0`

## Running

Documentation will be updated soon.

## Contributing

### Linting

You can use the clang-format-9 tool to automatically lint the code files.
First make sure to install it (if not already installed) by running:
`sudo apt-get install clang-format-9`

Then you can use the formatter from the makefile executing:
`make lint`