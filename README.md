# C-Machine-Learning
Machine learning from scratch in C. Fully connected network for predicting images of hand written digits.

---

# How to Build
Requires `gcc` and optionally `make` 
  
Without make:
```c
// debug
gcc *.c -o main.exe
./main.exe

// release
gcc *.c -o main.exe -DNDEBUG
./main.exe
```
  
With make:
```c
// debug (default make command)
make debug
./main.exe
make clean

// release
make release
./main.exe
make clean
```
  
Note: If using make on a non-Windows platform, output file will be `main.out`. This can be reconfigured in the makefile.
