#ifndef BC291FBE_75E9_4E26_89CD_75989319BA00
#define BC291FBE_75E9_4E26_89CD_75989319BA00

#include<bits/stdc++.h>

class laplacian;

class graph{
public:
    int n;
    int m;
    std::vector<std::tuple<int, int, double> > edges;
    graph();
    graph(laplacian *l);
};

class laplacian{
private:
    std::vector<std::vector<double> > L;
public:
    laplacian();
    laplacian(graph *g);
    laplacian(std::vector<std::vector<double> > l);
    std::vector<std::vector<double> > getL();
};

class vec{
public:
    std::vector<double> b;
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

    // std::vector<std::vector<double> > L; ///< Graph Laplacian [Optional]

    data();
    data(graph *g, std::vector<double> *v, double e, int dd=5);
};

#endif /* BC291FBE_75E9_4E26_89CD_75989319BA00 */
