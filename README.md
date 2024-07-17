# Concurrent Hash Table Project

This project implements a concurrent hash table using Jenkins's one_at_a_time hash function. The hash table supports concurrent insert, delete, and search operations using reader-writer locks.

## Team Members
- Megan
- Faramarz
- Dilly
- Franco

## Tasks

### Megan
- [x] **Review & Refine Existing Code**: Ensure the current implementation in `hash_table.c` and `hash_table.h` is correct and efficient. Address any issues found during the review.
- [x] **Locks Implementation (Guidance)**: Oversee and guide the implementation of lock functions by other team members.

### Faramarz
- [ ] **Locks Implementation**:
  - Implement lock initialization and destruction functions (`init_locks`, `destroy_locks`).
  - Implement functions to acquire and release read and write locks (`read_lock`, `read_unlock`, `write_lock`, `write_unlock`).
  - Ensure the locks are used correctly in the hash table functions (insert, delete, search).
- [ ] **Documentation**: Add detailed comments in the code explaining the functionality of each part. Ensure the code is easy to understand and maintain.

### Member C (Dilly OR Franco)
- [ ] **Main Program (`chash.c`)**:
  - Create the main program file that reads `commands.txt` and processes each command in a separate thread.
  - Use `pthread_t` to handle multiple threads and ensure the commands are processed concurrently.
  - Ensure correct integration of the hash table and lock functions.
- [ ] **File I/O**:
  - Implement file reading for `commands.txt`.
  - Implement logging of operations, locks, and condition variables to `output.txt`.
  - Ensure the output format matches the assignment requirements.

### Member C (Dilly OR Franco)
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
