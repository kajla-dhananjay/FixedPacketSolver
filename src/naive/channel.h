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

/**
 * @brief Channel class runs the chain while storing occupancies, etc. 
 */
class channel
{
private:

  bool isDone; //!< Indicator to check if all the processing is complete
  double eps; //!< Error Margin
  int T; //!< Global clock
  int D; //!< Number of Chains
  int val_chain; //!< Chain number to assign to runner threads
  int max_queue_size; //!< Maximum size of queue until runner threads can run
  int chains_run; //!< Chains run until a new transition

  std::mutex process_lock; //!< Mutex to lock val_chain
  std::mutex update_lock; //!< Mutex to lock modified values
  std::mutex mod_lock; //!< Mutex to lock IO
  std::mutex io_lock; //!< Lock to print debugging stuff with thread safety
  std::mutex tm_lock; //!< Lock to update termination timers
  std::mutex queue_lock; //!< Lock the queue 
  std::mutex garbage_lock; //!< Lock the garbage collector

  std::vector<std::vector<std::pair<double, int> > > CP; //!< Cumulative transition matrix

  std::vector<int> L; //!< Indicates when was the last time a given vertex was updated

  std::vector<double> mu; //!< Average vertex occupancy until processing the start of queue
  indexedSet<double>* tm; //!< Indexed priority queue to keep track of the bottleneck vertex(termination timers). Equivalent to policy based reb-black tree structures.

  std::vector<int> *old; //!< Tracks last known occupancies
  std::vector<int> *head; //!< Tracks current occupancies
  
  std::unordered_set<int> *head_modified; //!< Keeps track of vertices modified by the latest transitions

  std::queue<std::vector<int> *> old_process; //!< Keeps track of memory used by queue occupancies which is no longer useful and can be freed up 
  std::queue<std::unordered_set<int> *> old_modified; //!< Keeps track of memory used by changed vertices which is no longer useful and can be freed up 

  std::queue<std::vector<int> *> process_queue; //!< Queue occupancies at consecutive points in time
  std::queue<std::unordered_set<int> *> modified_queue; //!< Queue occupancy changes at consecutive points in time

  std::vector<int> chain_pos; //!< Position of various changes at the most recent time

public:
  channel(); //!< Default Constructor
  channel(int n, int s, int d, double e, std::vector<int> x, std::vector<std::vector<std::pair<double, int> > > &cp); //!< Constructor to initialize channel

  bool canStop(); //!< Function to tell whether the channel has converged and can be stopped
  bool runInstance(int id); //!< Function for thread with id to run/process/garbage collect 

  int getT(); //!< Get most recent time block
  std::vector<double> getMu(); //!< Get the average occupancy

};
#endif