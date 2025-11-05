# Custom grep Command Implementation

## Overview
This project is a custom implementation of the Unix **grep** command written in C.  
It searches files for lines containing a specified pattern and prints all matching lines to standard output.  

The program focuses on core functionality file validation, line-by-line pattern searching, and efficient memory management while maintaining simplicity and clarity inspired by K&R C principles.

---

## Features
- Search for a given pattern in one or multiple files  
- Print every line containing the pattern  
- Basic string-matching logic without reliance on external libraries  
- Manual file reading using system calls (`open`, `read`, `fgetc`, etc.)  
- Memory-safe handling for matched lines and buffers  
- Clear error handling for invalid files or paths  

Planned future extensions:
- `-i` : Case-insensitive matching  
- `-n` : Display line numbers of matches  
- `-c` : Count the number of matching lines  
- `-r` : Recursively search directories  
- Support for regular expressions  

---

## Implementation Details

Key Components:
- **`my_grep()`** : Core function that handles file opening, reading, and pattern searching  
- **`my_getline()`** : Custom line reader for buffered input from a file stream  
- **`is_valid_file()`** : Verifies that the given path points to a valid file using `stat()`  
- **`matched_lines` buffer** : Stores dynamically allocated strings for all matching lines  
- **Manual string comparison** : Iteratively matches characters to detect pattern occurrences  

This implementation intentionally avoids `strstr()` or regex libraries to reinforce manual algorithmic understanding and low-level C memory management.

---

## Build
Compile using gcc:
```bash
gcc -Wall -Wextra -g main.c -o grep

