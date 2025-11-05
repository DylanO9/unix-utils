# Custom Unix Utilities in C

## Overview
This repository contains custom implementations of classic Unix command-line utilities — **`cat`**, **`cp`**, **`wc`**, and **`grep`** — all written from scratch in **C**.  

Each program closely mirrors the functionality of its native counterpart using **low-level system calls** (`open`, `read`, `write`, `close`) and **manual memory management**, adhering to principles taught in *Kernighan and Ritchie’s* *The C Programming Language (K&R)*.  

The purpose of this project was to deepen understanding of **C fundamentals**, **Unix systems programming**, and **the philosophy of simple, modular tool design** that underpins Unix itself.

---

## Included Tools

| Command | Description | Key Concepts |
|----------|--------------|---------------|
| [`cat`](./cat) | Concatenates and prints files to standard output | Buffered I/O, file descriptors |
| [`cp`](./cp) | Copies files and directories (supports `-r`) | Recursion, directory traversal, safe memory allocation |
| [`wc`](./wc) | Counts lines, words, and bytes | State machines, buffered reading |
| [`grep`](./grep) | Searches for patterns in text files | String matching, dynamic allocation, manual parsing |

Each utility is self-contained with its own **README**, **error handling**, and **system call-based I/O** — avoiding standard library abstractions to focus on the bare-metal operation of the Unix environment.

