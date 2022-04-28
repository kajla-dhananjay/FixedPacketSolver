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

std::mutex mut;

void *runChainParallelInstance(void *ptr)
{
  std::pair<channel*, data*> *p = (std::pair<channel*, data*> *)ptr;
  channel *ch = p->first;
  data *dat = p->second;

  int q = rand()%100;

  mut.lock();
  std::cerr << "Thread " << q << " Initialized" << std::endl;
  mut.unlock();

  while(!ch->canStop())
  {
    mut.lock();
    std::cerr << "Thread " << q << " asking for command" << std::endl;
    mut.unlock();
    std::pair<bool, std::pair<int, int> > z = ch->getCommand();
    if(z.first)
    {
      mut.lock();
      std::cerr << "Thread " << q << " got command to process" << std::endl;
      mut.unlock();
      ch->processQueue();
      mut.lock();
      std::cerr << "Thread " << q << " done processing" << std::endl;
      mut.unlock();
    }
    else
    {
      int chain = z.second.first;
      int curr = z.second.second; 
      mut.lock();
      std::cerr << "Thread " << q << " got command to run chain: " << chain << " currently at pos: " << curr  << std::endl;
      mut.unlock();
      int pos = distSelector(dat->Cum_P[curr]); // Find the next vertex
      mut.lock();
      std::cerr << "Thread " << q << " ran chain: " << chain << " to pos: " << pos  << std::endl;
      mut.unlock();
      if(pos < 0 || pos >= dat->n)
      {
        errorHandler err("Bad selection by distSelector");
      }
      mut.lock();
      std::cerr << "Thread " << q << " pushing update " << pos  << std::endl;
      mut.unlock();
      ch->pushUpdate(chain, std::make_pair(curr, pos)); // Push update
      mut.lock();
      std::cerr << "Thread " << q << " pushed update to position: " << pos  << std::endl;
      mut.unlock();
    }
  }
  mut.lock();
  std::cerr << "Thread " << q << " exiting "  << std::endl;
  mut.unlock();
  pthread_exit(NULL);
}



/**
 * @brief runs the chain 
 */

channel* runChain(data *dat)
{

  std::cerr << "runChain Started" << std::endl;

  int ss = 0;

  std::vector<int> x(dat->d, ss);

  std::cerr << "Before Channel init" << std::endl;

  channel *ch = new channel(dat->n, ss, dat->d, dat->eps, x, 0.5);

  std::cerr << "After Channel init" << std::endl;

  std::vector<pthread_t> threads(dat->d);

  std::pair<channel *, data*> p = std::make_pair(ch, dat);

  std::cerr << "Before running threads" << std::endl;

  int num_threads = dat->d;

  for(int i = 0; i < num_threads; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, &p);
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }

  std::cerr << "Chains Running" << std::endl;

  while(!ch->canStop())
  {
    continue;
  }

  for(int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i],NULL);
  }

  std::cerr << "Chains Exiting" << std::endl;

  return ch;
}