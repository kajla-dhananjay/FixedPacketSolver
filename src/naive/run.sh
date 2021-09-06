#!/bin/bash
make > /dev/null
rm -rf input.txt
./formatter < ${1:-data40} > input.txt
#./graph_gen < graph_gen_input.txt >> input.txt
#./b_gen < graph_gen_input.txt >> input.txt
#echo "0.1" >> input.txt
./NaivePLW < input.txt
