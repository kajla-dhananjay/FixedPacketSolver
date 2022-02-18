#!/bin/bash
make > /dev/null
rm -rf input.txt
x=${1:-5}
./formatter < "../../data/data${x}" > input.txt
time ./NaivePLW < input.txt > out.txt
cat out.txt
#make clean
