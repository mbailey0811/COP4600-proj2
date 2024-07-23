# Concurrent Hash Table Project

This project implements a concurrent hash table using Jenkins's one_at_a_time hash function. The hash table supports concurrent insert, delete, and search operations using reader-writer locks.

## How to Run
Simply build using the "make" command to generate the chash executable. To run the executable, simply run chash. It is hardcoded to read from the commands.txt file.

## AI Use Policy
AI was used to assist in writing the Makefile for this project. We used AI as a tool to write the Makefile to help us speed up the debugging process and ensure that we were all using the same build command in early development. We believed this was an acceptable use case for the tool because we understand the concept behind it and it is not a concept that was directly taught in class. Thus, using AI for the Makefile does not take away any of our opportunity to practice the concepts we have learned in class.

To generate the file, we first explained our directory structure to ChatGPT. We said that there would be a root folder containing a src folder with hash_table.h/c, locks.h/c, and chash.c. This was enough information to generate the full Makefile. After trying to use it, we realized we needed to make some minor edits. We added the line "LDFLAGS = -lpthread" and added this as a flag when it builds to ensure that threads work properly.

## Team Members
- Megan
- Faramarz
- Dilly
- Franco

## Tasks

### Megan
- [ ] **Review & Refine Existing Code**: Ensure the current implementation in `hash_table.c` and `hash_table.h` is correct and efficient. Address any issues found during the review.
- [ ] **Locks Implementation (Guidance)**: Oversee and guide the implementation of lock functions by other team members.

### Faramarz
- [ ] **Locks Implementation**:
  - Implement lock initialization and destruction functions (`init_locks`, `destroy_locks`).
  - Implement functions to acquire and release read and write locks (`read_lock`, `read_unlock`, `write_lock`, `write_unlock`).
  - Ensure the locks are used correctly in the hash table functions (insert, delete, search).
- [ ] **Documentation**: Add detailed comments in the code explaining the functionality of each part. Ensure the code is easy to understand and maintain.

### Member C (Franco)
- [ ] **Main Program (`chash.c`)**:
  - Create the main program file that reads `commands.txt` and processes each command in a separate thread.
  - Use `pthread_t` to handle multiple threads and ensure the commands are processed concurrently.
  - Ensure correct integration of the hash table and lock functions.
- [ ] **File I/O**:
  - Implement file reading for `commands.txt`.
  - Implement logging of operations, locks, and condition variables to `output.txt`.
  - Ensure the output format matches the assignment requirements.

### Member C (Dilly)
- [ ] **Makefile**: Write a `Makefile` to compile the source files (`hash_table.c`, `locks.c`, `chash.c`) into the `chash` executable. Ensure the Makefile supports modular compilation and handles dependencies correctly.
- [ ] **Testing & Validation**:
  - Develop a set of test cases to verify the correctness and performance of the hash table.
  - Test the program with various input scenarios to ensure robustness and correctness.
  - Validate the multithreading and lock functionality.
- [ ] **README.txt**: Write a comprehensive README file with instructions on how to compile and run the program. Include any relevant information for understanding the project and its implementation. Add citations for any external references or resources used.

## Final Deliverables
- **`chash.c`**: Main program to process commands and produce output.
- **Makefile**: To compile the project into the `chash` executable.
- **Other Source Files**: Additional source files as needed (`locks.c`, etc.).
- **README.txt**: Explanation of the project, compilation instructions, and other relevant notes.
