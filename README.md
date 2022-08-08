# C-Machine-Learning
Machine learning from scratch in C. Fully connected network for predicting images of hand written digits.

---

# How to Use
Currently still in development so it simply runs tests in test mode and does nothing in non-test mode. If you don't care to build the project then you can simply run the `main.exe` included in the project for the latest build. Build intructions below otherwise.

---

# How to Build
Requires `gcc` and optionally `make` 
  
With make:
```
// debug (default make command)
make debug
./main.exe
make clean

// release
make release
./main.exe
make clean

// for running tests prepend test_ to the recipe name
// example
make test_release
```

Without make:
```
// debug
gcc *.c matrix/*.c -o main.exe
./main.exe

// release
gcc *.c matrix/*.c -o main.exe -DNDEBUG
./main.exe

// to run tests, include the following
tests/*.c -DRUN_TESTS

// example
gcc *.c matrix/*.c tests/*.c -DRUN_TESTS -o main.exe -DNDEBUG
```
  
Note: If using make on a non-Windows platform, output file will be `main.out`. This can be reconfigured in the makefile.

---

## Total lines of C source code and headers as of 2022-08-07 = 1531 lines
