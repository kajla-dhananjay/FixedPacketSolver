#include<bits/stdc++.h>

#define MAX_NODES 400
#define PRECISION 1000000
#define MAX_WEIGHT 10000
/**
 * \mainpage This is the documentation for the implemented code of GHS Algorithm.
 */

/** \brief Generates a connected input graph for given number of nodes and probability of an edge between any two nodes.
 * \file input_generator.cpp
 * \date 8/4/2021
 * \version 0.1
 * \author Dhananjay Kajla
 * \author Vijay Meena
 */

//! \brief Checks validity of given inputs
/** @param N Total number of vertices
 * @param p probability of an edge between two vertices
 * Total number of vertices(N) should be less than MAX_NODES
 * Probaility(p) should be between 0 and 1
 */



bool checkinputs(int N, double p)
{
  if(!(p >= 0 && p <= 1)) //Checks if p is a valid probability
  {
    std::cerr << "Invalid Probability" << std::endl;
    return false;
  }
  if(!(N >= 0 && N <= MAX_NODES)) // Checks whether N is in the allowed range
  {
    std::cerr << "Number of vertices out of range" << std::endl;
    return false;
  }
  return true;
}
/**
 * DFS runs a dfs and colors the nodes into connected components recursively.
 * At the end we have all nodes connected to the current node colored with the same color(color_val).
*/

//! \brief Does DFS on the graph starting from a node
//! \param node index of the current node
//! \param color_val color of the connected component of which node is a part
//! \param adj_list adjacency set of the graph
//! \param color color of connected components of various nodes
//! \param colormap a map from color to one of its representative node

void DFS(int node, int color_val, std::vector<std::set<int> > &adj_list, std::vector<int> &color, std::unordered_map<int, int> &colormap)
{
  if(color[node] != -1)
  {
    return; // Node already colored
  }

  color[node] = color_val; // color of node is color_val

  for(auto it : adj_list[node])
  {
    if(color[it] != -1)
    {
      continue; // Node already colored
    }
    DFS(it, color_val, adj_list, color, colormap);
  }
}

/**
 * DFS_Util runs DFS for all nodes and puts them into connected components.
 * All the connected components are then joined by edges linearly.
 */
//! \brief Uses DFS to make the graph connected
//! \param N index of the current node
//! \param edge_weights Set of edge weights of the graph
//! \param edges Set of edges of the graph point to point
//! \param adj_list adjacency set of the graph
//! \param color color of connected components of various nodes
//! \param colormap a map from color to one of its representative node

void DFS_Util(int N, std::set<int> &edge_weights, std::vector<std::tuple<int, int, double> > &edges, std::vector<std::set<int> > &adj_list, std::vector<int> &color, std::unordered_map<int, int> &colormap)
{
  std::random_device rand_dev; // Uniformly distributed integer RNG
  std::mt19937 generator(rand_dev()); // Mersene twister for RNG for a more uniform distribution
  std::uniform_real_distribution<double> distw(0,MAX_WEIGHT); // Distributes over MAX_WEIGHT uniformly

  int color_cnt = 0; // Total colors used

  for(int i = 0; i < N; i++)
  {
    if(color[i] == -1)
    {
      colormap[color_cnt] = i; // ith node is a representative of component with color = color_cnt
      DFS(i, color_cnt++, adj_list, color, colormap); // Recursive DFS
    }
  }
#ifdef Debug
  std::cerr << "Total Components = " << color_cnt << std::endl;
#endif
  for(int i = 1; i < color_cnt; i++)
  {
    int v0 = colormap[i-1]; // first node
    int v1 = colormap[i]; // second node

    if(adj_list[v0].find(v1) != adj_list[v0].end())
    {
      std::cerr << "Error in connected components" << std::endl; // The two nodes are from different components so this should be impossible
    }

    double r = distw(generator); // random weight

    while(edge_weights.find(r) != edge_weights.end())
    {
      r = distw(generator); // testing out random weights until we find a unique one
    }

    edge_weights.insert(r); // add a new edge
    edges.push_back(std::make_tuple(std::min(v0,v1), std::max(v0,v1), r)); // add a new edge
    adj_list[v0].insert(v1); // change the Adjacency set of the matrix
    adj_list[v1].insert(v0); // change the Adjacency set of the matrix
  }
}

int main()
{
  srand(time(0)); // Seed
  std::random_device rand_dev; // Uniformly distributed integer RNG
  std::mt19937 generator(rand_dev()); // Mersene twister for RNG for a more uniform distribution
  std::uniform_int_distribution<int> dist(0,PRECISION-1); // Distributes over PRECISION uniformly
  int N; // Number of Vertices
  double p; // Probability of an edge
  std::cin >> N >> p;
  std::cout << N << std::endl;
  double e = N;
  e *= (N-1);
  e /= 2; // e is the max number of edges possible

  std::uniform_real_distribution<double> distw(0,MAX_WEIGHT); // Distributes over MAX_WEIGHT uniformly

  p *= PRECISION; // As our range is in integers
  p -= 0.0000001; // This accounts for int to double probability i.e. if p = 5 and PRECISION = 10 the r=5 should count as over p
#ifdef Debug
  std::cerr << "Total Nodes : "<< N << std::endl;
#endif
  std::set<int> edge_weights; // Set of weights of edges in the graph
  std::vector<std::tuple<int, int, double> > edges; // Edges of the graph
  std::vector<std::set<int> > adj_list(N); // Adjacency Matrix of the graph
  std::vector<int> color(N,-1); // Represents the color of the node.
  std::unordered_map<int, int> colormap; // Represents a representative vertex for every color
  for(int i = 0; i < N; i++)
  {
    for(int j = i+1; j < N; j++)
    {
      //std::cout << i << " " << j << std::endl;
      int r = dist(generator);
      if(r <= p)
      {
        double w = 1;
        //double w = distw(generator); // Candidate weight of the edge
        adj_list[i].insert(j); // Adding a new edge
        adj_list[j].insert(i); // Adding a new edge
        //while(edge_weights.find(w) != edge_weights.end())
        //{
        //  w = distw(generator); // Finding another candidate weight of the edge
        //}
        edge_weights.insert(w); // Add the unique weight to the set
        edges.push_back(std::make_tuple(i,j,w)); // Insert the new edge
      }
    }
  }
  DFS_Util(N, edge_weights, edges, adj_list, color, colormap);
  int edge_count = edges.size();
  std::cout << edge_count << std::endl;

#ifdef Debug
  std::cerr << "Total Edges : " << edge_count << std::endl;
  std::cerr << "Max Edges : " << e << std::endl;
  std::cerr << "Total probability = " << ((double)edge_count) / e << " Expected Probability = " << p/PRECISION << std::endl;
#endif
  for(auto it : edges)
  {
    std::cout << std::get<0>(it) << " " << std::get<1>(it) << " " << std::get<2>(it) << std::endl;
  }
}
