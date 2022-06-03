/**
 * @file channel.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Channel data structure to fecilitate running of the chain
 * @version 2.0
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <bits/stdc++.h>
#include "channel.h"
#include "errorHandler.h"
#include "chain.h"
#include "distSelector.h"



/**
* @brief Default Constructor
*/
channel::channel()
{
  
}

/**
 * @brief Creates a new channel object for given number of nodes, given start node, given number of chains
 * @param n Number of nodes
 * @param s Bootstrapped vertex
 * @param d Number of chains
 * @param e Error bound
 * @param x Initial position of chains
*/


channel::channel(int n, int s, int d, double e, std::vector<int> x, std::vector<std::vector<std::pair<double, int> > > &cp)
{
  isDone = false; //!< Initialize isdone to false
  eps = e; //!< Initialize error margin
  T = 1; //!< Initialize global clock to 1
  D = d; //!< Initialize number of chains
  val_chain = 0; //!< increments the moment the thread starts running a chain
  chains_run = 0; //!< increments the moment the thread is done running the chain
  max_queue_size = 10; //!< Maximum queue size before which all threads are sent to processing

  CP = cp; //!< Cumulative transition matrix

  L.resize(n,0); //!< Initialize last seen for each vertex
  mu.resize(n, 0); //!< Initialize average occupancy at each vertex

  std::vector<int> r(n,0);

  tm = new indexedSet<double>(n); //!< Keeps track of all termination timers

  for(auto it : x)
  {
    r[it]++; // Increase occupancy at occupied vertices
    tm->setVal(it, INT_MAX); // Set timer of occupied vertices to infinity
  }
  
  old = new std::vector<int>(); //!< Special pointer to keep track of the most recently pushed out vector
  head = new std::vector<int>(); //!< Keeps track of the head of runner threads

  *old = r;
  *head = r;

  chain_pos = x;

  head_modified = new std::unordered_set<int>(); //!< Keeps track of modified vertices at each iteration
}

/**
 * @brief Indicates whether a particular chain is allowed to push an update
 * 
 * If a chain already has an update in the queue then that chain will be denied
 * @param id Id of the thread running the chain (passed for debugging purposes)
 * @return chain number of a valid chain to run
 * @return -1 if the thread can exit
 */
bool channel::runInstance(int id)
{
  if(!old_process.empty() || !old_modified.empty()) //!< If there's garbage to free, the thread will free it up first
  {
    garbage_lock.lock(); //!< Accquiring garbage lock
    while(!old_process.empty()) 
    {
      std::vector<int> *old_p = old_process.front();
      delete(old_p);
      old_process.pop();
    }
    while(!old_modified.empty())
    {
      std::unordered_set<int> *old_m = old_modified.front();
      delete(old_m);
      old_modified.pop();
    }
    garbage_lock.unlock(); //!< Releasing garbage lock
  }

  if(!process_queue.empty()) //!< If there's something to process, the thread will processes the chain 
  {
    process_lock.lock(); //!< Accquire process lock to keep other threads from processing and changing queue information 
    queue_lock.lock(); //!< Lock the queue to keep sync
    bool aa = process_queue.empty();
    bool bb = modified_queue.empty();
    queue_lock.unlock(); //!< Unlock the queue
    if(aa && bb) //!< If there's nothing to process
    {
      process_lock.unlock();
      return isDone;
    }
    else if(aa || bb)
    {
      errorHandler err("Process_queue and Modified_queue not in sync " + std::to_string(aa) + " " + std::to_string(bb));
    }
    else 
    {
      std::vector<int> *tolook = process_queue.front();
      std::unordered_set<int> *q = modified_queue.front();
      if(q->empty())
      {
        garbage_lock.lock(); //!< Accquire garbage lock to push redundant memory pointers in the trash
        old_process.push(old);
        old_modified.push(q);
        garbage_lock.unlock(); //!< Unlock garbage lock
        modified_queue.pop();
        process_queue.pop();
        old = tolook;
        double z = tm->getMax();
        if(z <= T * D) //!< All timers have expired
        {
          isDone = true;
        }
        T++;
        process_lock.unlock();
      }
      else 
      {
        int v = *((*q).begin()); //!< Vertex to process

        q->erase(q->begin());

        double ot = L[v] * D;
        double nt = T * D;
        double oq = (*old)[v];
        double nq = (*tolook)[v];

        process_lock.unlock();

        double r = mu[v] * ot + oq * (nt - ot -1) + nq;
        r /= (double) nt;

        mu[v] = r;

        double disc = 4 * std::fabs(nq - mu[v]) * nt;
        disc /= eps;
        disc += 1;

        double tim = 1 + sqrt(disc);
        tim /= 2;

        L[v] = T; //!< Update Last seen

        tm_lock.lock();
        tm->setVal(v, std::max((double)500, ceil(tim))); //!< Update termination timer with 500 as a reserve time
        tm_lock.unlock();

      }
    }
    return isDone;
  }
  else
  {
    update_lock.lock();
    int chain_no = val_chain++;
    if(val_chain <= D)
    {
      update_lock.unlock();
      int pre_vertex = chain_pos[chain_no];
      int post_vertex = distSelector(CP[pre_vertex]);

      mod_lock.lock();

      (*head)[pre_vertex]--;
      (*head)[post_vertex]++;
      
      head_modified->insert(pre_vertex);
      head_modified->insert(post_vertex);

      chain_pos[chain_no] = post_vertex;
      chains_run++;
      mod_lock.unlock();
    }
    else
    {
      if(chains_run < D)
      {
        update_lock.unlock();
        return isDone;
      }
      queue_lock.lock();
      process_queue.push(head);
      modified_queue.push(head_modified);
      queue_lock.unlock();

      std::vector<int> *tmp = head;
      head = new std::vector<int>();
      *head = *tmp;
      head_modified = new std::unordered_set<int>();

      val_chain = 0;
      chains_run = 0;
      update_lock.unlock();
    }

    return isDone;
  }
}

/**
 * @brief Indicates whether the computation is complete
 * 
 * @return true If average occupancies have stabilized
 * @return false If average occupancies have not yet stabilized
 */

bool channel::canStop()
{
  return isDone;
}


/**
 * @brief gets the Average Occupancies
 * 
 * @return mu The average occupancies in the graph
 */
std::vector<double> channel::getMu()
{
  return mu;
}


/**
 * @brief returns the total time
 * 
 * @return T The total time
 */
int channel::getT()
{
  return T;
}