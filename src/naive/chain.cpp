#include <bits/stdc++.h>
#include "chain.h"
#include "channel.h"
#include "errorHandler.h"

/**
 * @brief Runs an instance of the markov chain
 * 
 * @param chain_number Chain number to assign to the thread
 * @return void* Exit value of thread(NULL)
 */

int nn, ss, dd;
double ee;
std::vector<int> xx;
std::vector<std::mutex> X_lock;
std::vector<std::vector<std::pair<double, int> > > cp;
channel *ch;
std::mutex io_lock;


template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist)
{
  int i1 = dist.size();

  double d1 = rand(); // d1 represents the chosen random value
  d1 /= RAND_MAX; 

  if(d1 <= dist[0].first) // first entry is the selected entry
  {
    return dist[0].second;
  }

  int low = 0, high = i1-1, mid = 0; // Temp variables for binary search

  while(true) // Binary search for the interval of distribution in which d1 falls
  {
    mid = (low + high)/2;

    if(high == low - 1)
    {
      if(dist[low].first <= d1 && dist[high].first >= d1)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1].first < d1)
    {
      low = mid+1;
    }
    else if(dist[mid].first > d1)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid].first == d1 && dist[mid+1].first == dist[mid].first && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  return dist[mid+1].second;
}

void *runChainParallelInstance(void *ptr)
{
  while(true)
  {
    int i1 = ch->getChain();

    if((i1 >= dd || i1 < 0) && i1 != -1 )
    {
      std::cerr << "Bad chain allocation " + std::to_string(i1) << std::endl;
      errorHandler("Bad chain allocation " + std::to_string(i1));
    }

    if(i1 != -1) // If queue allows us to make a transition, we go ahead
    {
      X_lock[i1].lock();
      int i2 = distSelector(cp[xx[i1]]); // Find the next vertex
      if(i2 < 0 || i2 >= nn)
      {
        errorHandler err("Bad selection by distSelector");
      }
      int a = xx[i1];
      int b = i2;
      xx[i1] = b;
      ch->pushUpdate(i1, std::make_pair(a, b)); // Push update
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
 * @brief runs the chain 
 */

channel* runChain(int n, int s, int d, double eps, std::vector<std::vector<std::pair<double, int> > > &CP)
{
  /***************************** Initialization *******************************/
  nn = n;
  ss = s;
  dd = d;
  ee = eps;
  cp = CP;

  xx.resize(d,s);

  std::vector<std::mutex> tmp(d);
  X_lock.swap(tmp);

  // std::cerr << n << ' ' << eps << std::endl;

  ch = new channel(nn, ss, dd, ee, xx);

  // std::cerr << "Before Chain Running" << std::endl;

  std::vector<pthread_t> threads(d);
  for(int i = 0; i < d; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, NULL);
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }

  //std::cerr << "Chains Running" << std::endl;

  while(!ch->canStop())
  {
    continue;
  }

  return ch;
}