# Custom wc Command Implementation

## Overview
This project is a custom implementation of the Unix **`wc` (word count)** command written in C.  
It supports counting the number of **lines**, **words**, and **bytes** in one or multiple files, closely replicating the behavior and formatted output of the native `wc` utility.

The program uses low-level system calls (`open`, `read`, `close`) to process files efficiently in buffered chunks and applies a state machine to accurately track word boundaries.

---

## Features
- Count **lines**, **words**, and **bytes** in one or more files  
- Display totals across multiple files  
- Handle invalid paths and directories gracefully  
- Read files in buffered chunks for performance  
- Output formatted results aligned similarly to GNU `wc`  
- Robust word counting via a whitespace-based state machine  

Planned future extensions:
- Support for reading from **stdin** (e.g., `./wc < file.txt`)  
- Flags for individual counts (`-l`, `-w`, `-c`)  
- Improved formatting and file permission handling  

---

## Implementation Details

Key Components:
- **`my_wc()`** : Core function that reads file data, updates line, word, and byte counters using a state machine  
- **`print_count()`** : Handles formatted output of counts per file and total summary  
- **`is_valid_file()`** : Validates that a given path refers to an existing regular file  
- **State Machine Logic** :  
  - `IN` and `OUT` states track transitions between whitespace and non-whitespace characters to count words correctly  
- **Buffered Reading** :  
  - Reads in chunks of `MAX_SIZE` (1024 bytes) for efficiency and supports large files  

---

### Build
Compile using **gcc**:
```bash
gcc -Wall -Wextra -g main.c -o wc

