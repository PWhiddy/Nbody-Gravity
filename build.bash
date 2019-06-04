set -x
opt="-O3 -march=native -mtune=native"
#opt="-O2"
#pg="-pg"
#g="-g"
g++ $pg $g -std=c++11 -Wall -Wextra $opt BarnzNhutt.cpp -c -o run.o -fopenmp
g++ $pg $g run.o -o run -fopenmp
