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

  std::ofstream ofile; // Output file to log stuff

  bool isDone; // Indicator to check if all the processing is complete
  double eps; // Error Margin
  double p; // Probability of commanding a thread to act as a processor
  int T; // Global clock
  int D; // Number of Chains
  int val_chain; // Chain number to assign to runner threads
  int max_queue_size; // Maximum size of queue until runner threads can run
  int chains_run; // Chains run until a new transition

  std::mutex chain_lock; // Mutex to lock val_chain
  std::mutex mod_lock; // Mutex to lock modified values
  std::mutex io_lock; // Mutex to lock IO

  std::vector<int> L; // Indicates when was the last time a given vertex was updated

  std::vector<double> mu; // Average vertex occupancy until processing the start of queue
  indexedSet<double>* tm; // Indexed priority queue to keep track of the bottleneck vertex


  std::vector<std::pair<double, bool> > selector; // Chooses whether to assign a thread to process or to run


  std::vector<int> *old; // Tracks last known occupancies
  std::vector<int> *head; //Tracks current occupancies
  
  std::unordered_set<int> *head_modified;

  std::queue<std::vector<int> *> process_queue;
  std::queue<std::unordered_set<int> *> modified_queue;

  std::vector<int> chain_pos;

public:
  channel();
  channel(int n, int s, int d, double e, std::vector<int> x, double pr);
  ~channel();

  bool canStop();

  std::pair<bool, std::pair<int, int> > getCommand(); 
  void pushUpdate(int id, std::ofstream &tfile, int chain, std::pair<int, int> p);
  void processQueue(int id, std::ofstream &tfile);


  int getT();
  std::vector<double> getMu();

};
#endif