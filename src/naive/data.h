#ifndef BC291FBE_75E9_4E26_89CD_75989319BA00
#define BC291FBE_75E9_4E26_89CD_75989319BA00

#include<bits/stdc++.h>

class graph{
public:
    int n;
    int m;
    std::vector<std::tuple<int, int, int> > edges;
    graph();
};

class laplacian{
public:
    std::vector<std::vector<int> > L;
    std::vector<std::vector<int> > b;
};

class data{
public:
    int n; ///< Number of Nodes in Graph
    int m; ///< Number of Edges in Graph

    int u = -1; ///< The index of the sink vertex
    int d = 5; ///< Stores the nunber of chains to run

    double sb = 0; ///< Stores the sum of non-sink column vectors
    double eps = 1; ///< Stores the bound on error required


    std::vector<double> b; ///< Column vector b as per definition
    std::vector<double> j; ///< Column vector j as per definition
    std::vector<double> D; ///< Stores total weight sum for the vertices

    std::vector<std::pair<double, int> > sources; ///< Distribution of sources

    std::vector<std::vector<std::pair<double, int> > > Cum_P; ///< Cumulative Transition Matrix

    data();
    data(graph *g, std::vector<double> *v, double e);
};

#endif /* BC291FBE_75E9_4E26_89CD_75989319BA00 */
