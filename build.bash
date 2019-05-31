set -x
opt="-pg -O3 -march=native -mtune=native" 
#opt="-O2"
g++ -std=c++11 -Wall -Wextra $opt BarnzNhutt.cpp -c -o run.o -fopenmp
g++ -pg run.o -o run -fopenmp

