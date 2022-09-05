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

## Total lines of C source code and headers as of 2022-09-05 = 2976 lines

---

Results of a single epoch of training on mnist data
```
Progress: 60000/60000
Acurracy: 0.685811
Acurracy: 0.685817
Validation on a 5 image:
expected:
 0.000000
 0.000000
 0.000000
 0.000000
 0.000000
 1.000000
 0.000000
 0.000000
 0.000000
 0.000000

prediction:
 0.000000
 0.000000
 0.000000
 0.000003
 0.000000
 0.999997
 0.000000
 0.000000
 0.000000
 0.000000

Last layer Z values
-6.212742
-15.317670
 29.143690
-37.539016
 41.817514
-11.402895
 0.583664
-27.166940
-33.834893
```