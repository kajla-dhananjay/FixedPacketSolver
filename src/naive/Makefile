all: formatter max_component_extractor graph_gen b_gen NaivePLW

formatter: formatter.cpp
	g++ -o formatter formatter.cpp

max_component_extractor: max_component_extractor.cpp
	g++ -o max_component_extractor max_component_extractor.cpp

graph_gen: graph_gen.cpp
	g++ -o graph_gen graph_gen.cpp

b_gen: b_gen.cpp
	g++ -o b_gen b_gen.cpp

#GTL: GTL.cpp
#	g++ -o GTL GTL.cpp

#TransitionMatrix: TransitionMatrix.cpp
#	g++ -o TransitionMatrix TransitionMatrix.cpp

#HittingTable: HittingTable.cpp
#	g++ -o HittingTable HittingTable.cpp

NaivePLW: NaivePLW.cpp
	g++ -o NaivePLW NaivePLW.cpp

clean: graph_gen b_gen NaivePLW
	rm -rf graph_gen b_gen NaivePLW graph.txt b.txt laplacian.txt eq.txt p.txt plw_input.txt data.txt a.out
