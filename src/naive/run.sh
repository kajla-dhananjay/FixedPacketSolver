#!/bin/bash
make > /dev/null
rm -rf input.txt
x=${1:-40}
./formatter < "../../data/data${x}" > input.txt
./NaivePLW < input.txt
#make clean
