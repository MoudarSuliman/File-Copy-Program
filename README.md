# File-Copy-Program

## Overview

This program provides a utility for copying files on Unix-like systems. It supports two modes of operation: using standard read and write system calls, and using memory mapping for potentially faster performance. The program parses command-line options to determine the mode of operation and the source and destination files.

## Features

- **Standard Copying:** Utilizes `read()` and `write()` functions for file copying.
- **Memory-Mapped Copying:** Uses `mmap()` and `memcpy()` for efficient file copying.
- **Command-Line Interface:** Offers `-m` option for memory-mapped copying and `-h` for displaying help information.

## Compilation

To compile the program, ensure you have GCC or another compatible C compiler installed on your system. Use the following command in the terminal:

```sh
gcc -o copy file_copy.c
```

Replace `file_copy.c` with the actual name of your source file.

## Usage

After compilation, you can run the program with the following syntax:

```sh
./copy [-m] <file_name> <new_file_name>
./copy [-h]
```

- **Without `-m` Option:** The program uses `read()` and `write()` system calls to copy the file contents from `<file_name>` to `<new_file_name>`.

- **With `-m` Option:** The program maps the source file to a memory region and copies its contents to the destination file using `memcpy()`, offering potentially faster copying for large files.

- **`-h` Option:** Displays help information about how to use the program.

### Examples

- Copying a file using standard I/O:
  
  ```sh
  ./copy source.txt destination.txt
  ```

- Copying a file using memory-mapped I/O:
  
  ```sh
  ./copy -m source.txt destination.txt
  ```

- Displaying help information:
  
  ```sh
  ./copy -h
  ```

## Important Remarks

- The program uses the `getopt()` function to process command-line options and arguments, providing flexibility in how the program is called.
- Error checks are performed after each system call to ensure the program behaves as expected under various conditions.
- This utility is designed to work on Unix-like operating systems and requires permissions to read from the source file and write to the destination file.

## Dependencies

The program requires the following include files:

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
```

Make sure your development environment supports these headers and the associated system calls.
