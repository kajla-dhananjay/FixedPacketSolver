/**
 * @file chain.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Functions to run the chains
 * @version 2.0
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <bits/stdc++.h>
#include "chain.h"
#include "channel.h"
#include "errorHandler.h"
#include "distSelector.h"

/**
 * @brief Runs an instance of the markov chain
 * 
 * @param ptr Passes the channel information and the thread id as a tuple
 * @return void* Exit value of thread(NULL)
 */

std::mutex io_lock;

void *runChainParallelInstance(void *ptr)
{
  std::tuple<channel*, int> *p = (std::tuple<channel*, int> *)ptr;
  channel *ch = std::get<0>(*p);
  int q = std::get<1>(*p);
  bool canStop = false;

  while(!canStop)
  {
    canStop = ch->runInstance(q);
  }

  pthread_exit(NULL);
}



/**
 * @brief runs the chain 
 * @param dat Data about the graph and the column vector
 * @return channel* Returns pointer to channel which processed the chain
 */

channel* runChain(data *dat)
{

  int ss = 0;
  std::vector<int> x(dat->d, ss);
  channel *ch = new channel(dat->n, ss, dat->d, dat->eps, x, dat->Cum_P);

  std::vector<pthread_t> threads(dat->d); //!< Thread vector
  std::vector<std::tuple<channel*, int> > vtt; //!< Vector for storing channel information and id for each thread

  int num_threads = dat->d; //!< Number of threads to run

  num_threads = 8; //!< Limiting the number of threads by physical capacity

  for(int i = 0; i < num_threads; i++)
  {
    vtt.push_back(std::make_tuple(ch,i)); //!< Initialize thread information vector
  }

  for(int i = 0; i < num_threads; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, &vtt[i]); //!< Create threads to run the chain
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }

  for(int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL); //!< Join threads before ending the function
  }

  return ch;
}