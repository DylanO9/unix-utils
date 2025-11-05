# Unix Utilities

## Overview
This repository contains my re-implementations of classic Unix utilities — **`wc`**, **`cat`**, **`grep`**, and **`cp`** — built entirely using low-level **system calls** such as `open`, `read`, `write`, and `close`.  
These projects helped me understand how fundamental Unix tools work under the hood and how to interact directly with the operating system through the C standard library and POSIX interfaces.

## What I Learned

### `wc` (Word Count)
Implementing `wc` taught me how to read from files into buffers, manage file descriptors, and count lines, words, and bytes.  
I used a **state machine** to handle edge cases like words wrapping across multiple lines.

### `grep` (Pattern Search)
I learned **pattern matching** by implementing a naive, byte-by-byte search through files.  
While doing so, I studied the **Boyer–Moore algorithm**, which powers the real `grep` and achieves major performance improvements through clever character-distance skipping.  
I also experimented with using both file descriptors and `FILE*` streams via `fdopen()` and `fgets()`.

### `cat` (Concatenate Files)
Recreating `cat` strengthened my understanding of **Unix system calls** and **error handling**.  
I implemented robust error checking at every stage (open, read, write), learning how to propagate and handle system-level errors gracefully.

### `cp` (Copy Files/Directories)
`cp` introduced me to **directory traversal** using the `DIR` type and `dirent` structures from `<dirent.h>`.  
Implementing recursive copying (`-r` flag) deepened my understanding of **file systems**, **inodes**, and the **stat structure**, which I used to differentiate between files and directories.

## Takeaways
Through these utilities, I learned:
- The importance of **error checking** and **resource management**.  
- How **everything in Unix is represented as a file or inode**.  
- How system calls bridge the gap between software and the operating system.  

This project significantly strengthened my C fundamentals and helped me internalize the design principles behind Unix tools.

