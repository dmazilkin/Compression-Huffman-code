# Canonical Huffman code
This repo contains *Canonical Huffman code* implementation from scratch, which can be used for text and files compression.

### Project structure
In the project directory you can find the following:
1. **main.c** - the main file, contains the main program logic,
2. **src** - directory, contains sources for main.c file:
    1. **arg_parser** - parse command line arguments,
    2. **io** - read content from file.

# Algorithm Overview
### Compression steps:
1. Read data:
   - data is read in chunks until eof is reached.
2. Huffman Tree:
   - calculate frequency table,
   - create Huffman Tree with min-heap,
   - assign initial Huffman codes.
3. Canonical Huffman code:
   - sort initial Huffman codes by code length,
   - assign first character code 0 with length of Canonical Huffman code,
   - increase next character code by 0b1,
   - shift next character by length difference.
4. Decode and save data:
   - read data from file in chunks,
   - decode chunk of data with Canonical Huffman code,
   - save chunk of data to file,
5. Save metadata to ".metadata" file:
   - save Canonical Huffman code,
   - save count of shift bits.

### Decompression steps:
1. Read metadata from ".metadata" file:
   - read count of shift bits,
   - read Canonical Huffman code.
2. Read and decode Canonical Huffman codes:
   - read data in chunks,
   - decode chunk of data with Canonical Huffman code,
   - save decoded chunk of data.

# Usage
## Compiling
First you need to compile source files into one executable file. Simply start Makefile script from project folder as follows:
```console
make 
```
after compiling is completed, **main** executable file can be found in directory *build/*. 

## Compression and decompression
To compress files or text by running **main** program you should also provide paths some arguments:
1. use *-i* or *--input* to compress file,
2. use *-m* or *--msg* to compress text from command line,
3. use *-o* or *--output* to provide output file path.

**NOTE**: *-i* and *-m* options are both xor, so you can choose one of them, otherwise program terminates with error.
Option *-o* optional, so if it's not specified the output will be printed to the command line.