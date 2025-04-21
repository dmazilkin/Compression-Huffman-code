# Canonical Huffman code
This repo contains *Canonical Huffman code* implementation from scratch, which can be used for text and files compression.

## Project structure
In the project directory you can find the following:
1. **main.c** - the main file, contains the main program logic,
2. **src** - directory, contains sources for main.c file:
    1. **arg_parser** - parse command line arguments,
    2. **io** - read content from file.
# Usage

## Compiling
First you need to compile source files into one executable file. Simply start Makefile script from project folder as follows:
```console
make all
```
after compiling is completed, **main** executable file can be found in directory *build/*. 

## Compression
To compress files or text by running **main** program you should also provide paths some arguments:
1. use *-i* or *--input* to compress file,
2. use *-m* or *--msg* to compress text from command line,
3. use *-o* or *--output* to provide output file path.

**NOTE**: *-i* and *-m* options are both xor, so you can choose one of them, otherwise program terminates with error.
Option *-o* optional, so if it's not specified the output will be printed to the command line.