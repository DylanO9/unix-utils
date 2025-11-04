# Custom cp Command Implementation

## Overview
This project is a custom implementation of the Unix cp command written in C.  
It supports copying individual files, multiple files to a directory, and entire directories recursively using the -r flag.  

The program replicates key functionality of the native cp utility while including safe memory handling, recursion, and error checking.

---

## Features
- Copy a single file to another file  
- Copy multiple files into a directory  
- Recursively copy directories using -r  
- Safe dynamic memory allocation via safe_malloc()  
- Prevent copying a file onto itself  
- Handle invalid paths and directories gracefully  
- Clear error messages and exit codes  

Planned future extensions:
- -p : Preserve metadata (permissions, timestamps, ownership)
- -i : Prompt before overwriting
- -n : Skip overwriting existing files

---

## Implementation Details

Key Components:
- cp() : Core logic that determines whether the operation is file-to-file, file-to-directory, or directory copy
- copy_file() : Handles buffered file copying using an 8KB buffer
- dir_walk() : Traverses directories recursively and recreates folder structures
- safe_malloc() : Wrapper around malloc() that terminates the program safely on allocation failure
- deconstruct_file_path() : Extracts and stores file and directory info into a File_Info struct
- is_valid_dir() : Checks if a given path points to a valid directory

---

### Build
Compile using gcc:
```bash
gcc -Wall -Wextra -g main.c -o cp

