#include <bits/stdc++.h>
#include "chain.h"
#include "channel.h"
#include "errorHandler.h"
#include "distSelector.h"

/**
 * @brief Runs an instance of the markov chain
 * 
 * @param chain_number Chain number to assign to the thread
 * @return void* Exit value of thread(NULL)
 */

std::mutex io_lock;

void *runChainParallelInstance(void *ptr)
{

  std::tuple<channel*, int> *p = (std::tuple<channel*, int> *)ptr;
  channel *ch = std::get<0>(*p);
  int q = std::get<1>(*p);
  io_lock.lock();
  //std::cerr << "Thread: " << q << " started." << std::endl;
  io_lock.unlock();
  bool canStop = false;

  while(!canStop)
  {
    canStop = ch->runInstance(q);
  }

  pthread_exit(NULL);
}



/**
 * @brief runs the chain 
 */

channel* runChain(data *dat)
{

  // std::cerr << "runChain Started" << std::endl;

  int ss = 0;

  std::vector<int> x(dat->d, ss);

  // std::cerr << "Before Channel init" << std::endl;

  channel *ch = new channel(dat->n, ss, dat->d, dat->eps, x, dat->Cum_P, 0.5);

  // std::cerr << "After Channel init" << std::endl;

  std::vector<pthread_t> threads(dat->d);

  //std::pair<channel *, data*> p = std::make_pair(ch, dat);

  std::vector<std::tuple<channel*, int> > vtt;

  int num_threads = dat->d;

  num_threads = 8;

  for(int i = 0; i < num_threads; i++)
  {
    vtt.push_back(std::make_tuple(ch,i));
  }

  // std::cerr << "Before running threads" << std::endl;


  for(int i = 0; i < num_threads; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, &vtt[i]);
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }

  // std::cerr << "Chains Running" << std::endl;

  for(int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  // std::cerr << "Chains Exit" << std::endl;

  return ch;
}