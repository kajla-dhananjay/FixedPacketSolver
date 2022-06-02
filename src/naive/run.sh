#!/bin/bash
echo "building solver"
make > /dev/null
echo "build complete"
rm -rf input.txt
x=${1:-5}
d="${2:-5}" #Number of chains
b="${3:-40}" #Number of sources
e="${4:-0.000001}" #Epsilon
dat_out="${5:-dat_out.txt}"
#echo "$x $d $b $e"
echo "generating input"
./formatter  ../../data/data${x} input_g.txt input_b.txt "${b}"
echo "input generated"
echo "running solver"
./plw input_g.txt input_b.txt oc.txt ol.txt "${d}" "${e}"
echo "solver run complete"
echo "building analyser"
make analyser > /dev/null
echo "analyser built"
echo "running analysis"
./analyser input_g.txt input_b.txt oc.txt ol.txt "${d}" "${e}" "${dat_out}"
cat "${dat_out}" >> out.txt
echo "run complete"
#make clean
