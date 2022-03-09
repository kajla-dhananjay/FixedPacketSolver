#include <bits/stdc++.h>
#include "chain.h"
#include "declarations.h"
#include "distSelector.h"

/**
 * @brief Runs an instance of the markov chain
 * 
 * @param chain_number Chain number to assign to the thread
 * @return void* Exit value of thread(NULL)
 */

void *runChainParallelInstance(void *ptr)
{
  while(true)
  {
    int i1 = chan->getChain();
    if(i1 != -1) // If queue allows us to make a transition, we go ahead
    {
      X_lock[i1].lock();
      int i2 = distSelector(Cum_P[X_P[i1]]); // Find the next vertex
      if(i2 < 0 || i2 >= n)
      {
        errorHandler err("Bad selection by distSelector");
      }
      int a = X_P[i1];
      int b = i2;
      X_P[i1] = b;
      chan->pushUpdate(i1, std::make_pair(a, b)); // Push update
      X_lock[i1].unlock();
    }
    else
    {
      break;
    }
  }
  pthread_exit(NULL);
}


/**
 * @brief Runs the chain parallely
 * 
 */

void runChainParallel(double e, std::vector<int> x)
{
  chan = new channel(n, s, d, e, x);
  std::vector<std::mutex> mm(d);
  X_lock.swap(mm);
  std::vector<pthread_t> threads(d);
  for(int i = 0; i < d; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, NULL);
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }
  //std::cerr << "Here" << std::endl;
  while(!chan->canStop())
  {
    continue;
  }
}


/**
 * @brief runs the chain 
 */

void runChain()
{
  /***************************** Initialization *******************************/
  X_P.resize(d,s);
  runChainParallel(eps, X_P);
  return;
}