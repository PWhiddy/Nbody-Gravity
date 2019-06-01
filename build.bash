set -x
opt="-O3 -march=native -mtune=native"
#pg="-pg"
#opt="-O2"
g++ $pg -std=c++11 -Wall -Wextra $opt BarnzNhutt.cpp -c -o run.o -fopenmp
g++ $pg run.o -o run -fopenmp
