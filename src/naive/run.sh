#!/bin/bash
make > /dev/null
rm -rf input.txt
./formatter < ${1:-../../data/data40} > input.txt
./NaivePLW < input.txt
#make clean
