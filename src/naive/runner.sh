#!/bin/bash
make
./graph_gen < graph_gen_input.txt > graph.txt
./b_gen < b_gen_input.txt > b.txt
./GTL < graph.txt > laplacian.txt
cat laplacian.txt > eq.txt; cat b.txt >> eq.txt
./TransitionMatrix < eq.txt > p.txt
cat graph.txt > plw_input.txt
cat b.txt >> plw_input.txt
./HittingTable < p.txt >> plw_input.txt
cat eps.txt >> plw_input.txt
./NaivePLW < plw_input.txt > data.txt
