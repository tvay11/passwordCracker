# Password Cracking in C using Pthreads

## Overview

This program is designed to perform password cracking using brute force. It uses the `crypt` library to encrypt candidate passwords and compare them to the target hash. Multi-threading is utilized via Pthreads to speed up the cracking process.

**Note**: This is only intended for educational purposes.

## Dependencies

- `crypt.h`: The crypt library for the cryptographic function.
- `pthread.h`: Pthreads library for multi-threading.
- `stdio.h`, `stdlib.h`, `string.h`: Standard C libraries for I/O and string manipulation.

## Compilation

To compile the code, run:

```bash
gcc -o crack crack.c -lcrypt -lpthread

```

## Run the program with three arguments:

    The number of threads you want to use.
    The size of the key to crack.
    The target password hash.
  ```bash
  ./crack <number_of_threads> <key_size> <target_hash>
  ```
Example
  ```bash
./crack 4 2 "cdfnIXMyMCpPg"
  ```
