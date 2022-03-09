#include<bits/stdc++.h>
#include "checkers.h"

/**
 * @brief Runs a Depth first search to mark all connected nodes
 * 
 * @param node current node
 * @param visited array indicating wether each node has been visited or not
 * @param cnt counter indicating number of visited nodes
 */

void DFS(int node, std::vector<int> &visited, int &cnt, std::vector<std::vector<std::pair<double, int> > > &adj)
{
  cnt++; // increase counter indicating total number of connected vertices
  visited[node] = 1; // color of node is color_val

  for(auto it : adj[node]) // Iterate through the neighbors of current node
  {
    if(visited[it.second] != -1) // Node already visited
    {
      continue;
    }
    DFS(it.second, visited, cnt, adj); // Node not visited, apply DFS recursively
  }
}

/**
 * @brief Checks if the input is connected
 * 
 * @return true Input Graph is connected
 * @return false Input Graph is not connected
 */

bool checkConnected(std::vector<std::vector<std::pair<double, int> > > &adj)
{
  std::vector<int> visited(adj.size(),-1); // This vector keeps track of visited nodes
  int cnt = 0; // Counter to keep track of number of visited nodes
  DFS(0, visited, cnt, adj); // DFS to update visited counter
  return (cnt == (int)adj.size()); // Return true if all nodes have been visited, false otherwise
}

/**
 * @brief Checks if the given vector is a Cumulative distribution or not
 * 
 * @tparam T Type of labels on each entity
 * @param dist Candidate distribution
 * @return true Given candidate is a Cumulative distribution
 * @return false Given candidate is not a Cumulative distribution
 */

template<typename T>
bool cumDist(const std::vector<std::pair<double, T> > &dist)
{
  double prev = 0; // checks previous entry
  for(auto it : dist)
  {
    if(it.first < prev) // cumulative prob. distribution is monotonically increasing
    {
      return false;
    }
    prev = it.first; // update previous entry
  }
  return (prev==1); // last entry of the distribution should be 1
}