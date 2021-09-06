#!/bin/bash
make
rm -rf input.txt
./max_component_extractor < in_raw.txt > input.txt
#./graph_gen < graph_gen_input.txt >> input.txt
#./b_gen < graph_gen_input.txt >> input.txt
#echo "0.1" >> input.txt
./NaivePLW < input.txt
