gcc ./main.c -o main -Ofast -fwhole-program -funroll-all-loops -mavx2 -march=native -msse4.2 -msse4.1 -msse3 -msse2 -ffast-math
time ./main