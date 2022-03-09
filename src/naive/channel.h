#ifndef _CHANNEL_H_
#define _CHANNEL_H_
/**
 * @file channel.h
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Channel Data structure which acts as a central information gathering mechanism
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<bits/stdc++.h>
#include "indexedSet.h"

class channel
{
private:
  int T; // Global clock
  int batch_size; // Size of batches to process
  int dd;
  int D;
  double eps; // Error Margin
  std::vector<int> Q; // Vertex occupancy at given instant
  std::vector<double> mu; // Average vertex occupancy
  indexedSet<double>* tm; // Indexed priority queue to keep track of the bottleneck vertex
  std::queue<std::pair<int, int> > R; // Queue containing updates waiting to be processed
  std::vector<std::queue<std::tuple<int, int, int, int> > > S; // Data structure to keep track of updates
  std::vector<int> L; // Indicates when was the last time a given vertex was updated
  bool isDone; // Indicator to check if all the processing is complete
  std::mutex m; // Mutex lock for thread safety
  std::vector<int> chain_positions; // A testing metric in order to ensure the order of chain updates is accurate
public:
  channel();
  channel(int n, int s, int d, double e);
  channel(int n, int s, int d, double e, std::vector<int> x);
  int getChain();
  int getT();
  bool canStop();
  void pushUpdate(int chain, std::pair<int, int> p);
  std::vector<double> getMu();
  void process();
};
#endif