# Advanced File Processing, Disk Scheduling, and System Command Simulations

This repository contains C programs that implement solutions for file processing, disk scheduling, file system operations, and system command simulations. The project includes multiple tasks that demonstrate the usage of system calls (`mmap`, `lseek`, `dup`, etc.), multithreading, and efficient file management.

---

## Table of Contents

1. [Tasks Overview](#tasks-overview)
   - [Q1: Weather Station Data Processing](#q1-weather-station-data-processing)
   - [Q2: Shortest Seek Time First (SSTF) Disk Scheduling](#q2-shortest-seek-time-first-sstf-disk-scheduling)
   - [Q3: Random File Access with `lseek`](#q3-random-file-access-with-lseek)
   - [Q4: Output Redirection with `dup`](#q4-output-redirection-with-dup)
   - [Q5: Command Simulations (`ls`, `wc`, `cp`, `mv`)](#q5-command-simulations-ls-wc-cp-mv)
2. [Performance Analysis: `fread` vs. `mmap`](#performance-analysis-fread-vs-mmap)
3. [How to Compile and Run](#how-to-compile-and-run)
4. [System Requirements](#system-requirements)
5. [License](#license)

---

## Tasks Overview

### Q1: Weather Station Data Processing

**Description:**  
This program reads a large dataset containing weather station temperature records and calculates the minimum, mean, and maximum temperature for each station. Two implementations are provided:
1. Using `fread`.
2. Using `mmap`.

**Features:**
- Input: Text file with records in the format: `station_name;temperature`
- Output: Min, mean, and max temperature for each station and the total processing time for each method.

**Deliverables:**
- `Q1.c`: Implementation using both `fread` and `mmap`.
- Makefile: Compiles the program.
- Performance analysis of `fread` vs. `mmap`.
- Strace logs for system call analysis.

---

### Q2: Shortest Seek Time First (SSTF) Disk Scheduling

**Description:**  
This program simulates the SSTF disk scheduling algorithm, calculating seek time and rotational latency for a given set of requests.

**Features:**
- Input: Disk request queue and initial head position.
- Output: Seek time and rotational latency for each request.

**Deliverables:**
- `Q2.c`: Implementation of the SSTF algorithm.
- Makefile: Compiles the program.

---

### Q3: Random File Access with `lseek`

**Description:**  
This program demonstrates random file access using the `lseek` system call. It indexes and retrieves specific records from a binary file containing student records.

**Features:**
- Input: Student ID.
- Output: Name of the student corresponding to the given ID.

**Deliverables:**
- `Q3.c`: Implementation of the program.
- `students.rec`: Binary file containing student records.
- Makefile: Compiles the program.

---

### Q4: Output Redirection with `dup`

**Description:**  
This program simulates output redirection using the `dup` family of system calls (`dup`, `dup2`, `dup3`). It consists of two parts:
1. Simple output redirection to a file.
2. Interactive simulation of shell-like commands with redirected `stdout` and `stderr`.

**Features:**
- Input: Commands to execute and output file names.
- Output: Results of the commands written to the specified files.

**Deliverables:**
- `Q4.c`: Implementation of both parts.
- Makefile: Compiles the program.
- Example output files: `output_p1.txt`, `output_p2.txt`.

---

### Q5: Command Simulations (`ls`, `wc`, `cp`, `mv`)

**Description:**  
This program simulates basic Linux commands (`ls`, `wc`, `cp`, `mv`) using individual C programs and executes them as child processes through a parent program.

**Features:**
- `list.c`: Lists files in the specified directory.
- `countw.c`: Counts words in a file.
- `copy.c`: Copies a file to a directory.
- `move.c`: Moves files or directories to another location.
- `main.c`: Executes these commands using `fork` and `exec`.

**Deliverables:**
- `q5/`: Folder containing all the source files and a Makefile.
- Makefile: Builds the individual programs and the `main` program.

---

## Performance Analysis: `fread` vs. `mmap`

### Overview
This analysis compares the performance of `fread` and `mmap` for processing large datasets.

### Observations
1. **`fread`:**
   - Invokes multiple `read` system calls, leading to high overhead.
   - Buffered I/O incurs additional latency for large files.
   - Strace log: ~37,662 `read` calls.
2. **`mmap`:**
   - Maps the file into memory, avoiding repeated system calls.
   - Efficient for sequential access with `madvise(MADV_SEQUENTIAL)`.
   - Strace log: ~38 `mmap` calls and 4 `munmap` calls.

### Conclusion
`mmap` is more efficient for large files due to reduced system calls and direct memory mapping.

---

## How to Compile and Run

### Compilation
- Compile programs using the provided Makefiles:
  - For Q1 to Q4:
    ```bash
    make
    ```
  - For Q5:
    ```bash
    cd q5
    make
    ```

### Execution
- Run individual programs:
  ```bash
  ./q1        # Weather Station Data Processing
  ./q2        # SSTF Disk Scheduling
  ./q3        # Random File Access
 ./q4 -p1 output_p1.txt   # Output Redirection (Part 1)
 ./q4 -p2 "ls -l" output_p2.txt   # (e.g. )Output Redirection (Part 2)
  cd q5 && ./main # Command Simulations
