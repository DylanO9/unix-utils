# cat - Custom File Display Utility

## Overview
This is a simple implementation of the Unix `cat` command written in C.  
It reads data from one or more files (or from standard input if no files are given)  
and writes it to standard output. This version uses buffered I/O with `read()` and  
`write()` system calls to efficiently handle large files.

## Features
- Supports multiple files as arguments (`cat file1 file2`)
- Reads from standard input when no files are provided
- Buffered reads and writes using a 1024-byte buffer
- Graceful error handling for `open()`, `write()`, and `close()`
- Minimal memory footprint and no dynamic allocation

## Usage
```bash
./cat

