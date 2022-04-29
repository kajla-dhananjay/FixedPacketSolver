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


void *runChainParallelInstance(void *ptr)
{
  std::tuple<channel*, data*, int> *p = (std::tuple<channel*, data*, int> *)ptr;
  channel *ch = std::get<0>(*p);
  data *dat = std::get<1>(*p);
  int q = std::get<2>(*p);

  std::ofstream ofile;
  ofile.open(("thread_log_"+std::to_string(q) + ".txt").c_str());

  ofile << "Thread Initialized" << std::endl;

  while(!ch->canStop())
  {
    ofile << "Thread asking for command" << std::endl;
    std::pair<bool, std::pair<int, int> > z = ch->getCommand();
    ofile << "Thread got command" << std::endl;
    if(z.first)
    {
      if(z.second.first == 0)
      {
        ofile << "Thread got command to process via full val_chain" << std::endl;
      }
      else 
      {
        ofile << "Thread got command to process through usual means" << std::endl;
      }
      ch->processQueue(q, ofile);
      ofile << "Thread done processing" << std::endl;
    }
    else
    {
      int chain = z.second.first;
      int curr = z.second.second; 
      ofile << "Thread got command to run chain: " << chain << " currently at pos: " << curr  << std::endl;
      int pos = distSelector(dat->Cum_P[curr]); // Find the next vertex
      ofile << "Thread ran chain: " << chain << " from pos: " << curr <<  " to pos: " << pos  << std::endl;
      if(pos < 0 || pos >= dat->n)
      {
        errorHandler err("Bad selection by distSelector");
      }
      ofile << "Thread pushing update for chain " << chain  << std::endl;
      ch->pushUpdate(q, ofile, chain, std::make_pair(curr, pos)); // Push update
      ofile << "Thread pushed update for chain: " << chain  << std::endl;
    }
  }
  ofile << "Thread exiting "  << std::endl;
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

  //std::pair<channel *, data*> p = std::make_pair(ch, dat);

  std::vector<std::tuple<channel*, data*, int> > vtt;

  int num_threads = dat->d;

  num_threads = 2;

  for(int i = 0; i < num_threads; i++)
  {
    vtt.push_back(std::make_tuple(ch,dat,i));
  }

  std::cerr << "Before running threads" << std::endl;


  for(int i = 0; i < num_threads; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, &vtt[i]);
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