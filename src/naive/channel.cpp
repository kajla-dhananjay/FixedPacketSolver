/**
 * @file channel.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Channel Data structure which acts as a central information gathering mechanism
 * @version 2.0
 * @date 2021-2-5
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
 * @param p probability of assigning a thread to process the queue
*/


channel::channel(int n, int s, int d, double e, std::vector<int> x, std::vector<std::vector<std::pair<double, int> > > &cp, double pr)
{
  ofile.open("channel_logs.txt");
  
  isDone = false; // Initialize isdone to false
  eps = e; // Initialize error margin
  p = pr; // Initialize probability of processing
  T = 1; // Initialize global clock to 1
  D = d; // Initialize number of chains
  val_chain = 0;
  max_queue_size = 10;

  CP = cp;

  L.resize(n,0); // Initialize last seen for each vertex
  mu.resize(n, 0); // Initialize average occupancy at each vertex

  std::vector<int> r(n,0);

  tm = new indexedSet<double>(n);

  for(auto it : x)
  {
    r[it]++; // Increase occupancy at occupied vertices
    tm->setVal(it, INT_MAX); // Set timer of occupied vertices to infinity
  }
  
  old = new std::vector<int>();
  head = new std::vector<int>();

  *old = r;
  *head = r;

  head_modified = new std::unordered_set<int>();

  tm = new indexedSet<double>(n); // Initialize the indexed set

  chain_pos = x;
}

/**
 * @brief Indicates whether a particular chain is allowed to push an update
 * 
 * If a chain already has an update in the queue then that chain will be denied
 * @return chain number of a valid chain to run
 * @return -1 if the thread can exit
 */
bool channel::runInstance(int id)
{
  if(!process_queue.empty()) // processes chain
  {
    process_lock.lock();
    bool aa = process_queue.empty();
    bool bb = modified_queue.empty();
    if(aa && bb)
    {
      process_lock.unlock();
      return isDone;
    }
    else if(aa || bb)
    {
      errorHandler err("Process_queue and Modified_queue not in sync");
    }
    else 
    {
      std::vector<int> *tolook = process_queue.front();
      std::unordered_set<int> *q = modified_queue.front();

      if(q->empty())
      {
        free(q);
        modified_queue.pop();
        free(old);
        old = process_queue.front();
        process_queue.pop();
        if(tm->getMax() <= T * D)
        {
          isDone = true;
          ofile << "Done" << std::endl;
        }
        T++;
        process_lock.unlock();
      }
      else 
      {
        int v = *((*q).begin());

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

        L[v] = T;

        tm->setVal(v, ceil(tim));

      }
    }
    return isDone;
  }
  else // runs chain
  {
    update_lock.lock();
    int chain_no = val_chain++;
    if(val_chain < D)
    {
      update_lock.unlock();
    }
    int pre_vertex = chain_pos[chain_no];
    int post_vertex = distSelector(CP[pre_vertex]);

    (*head)[pre_vertex]--;
    (*head)[post_vertex]++;
    
    head_modified->insert(pre_vertex);
    head_modified->insert(post_vertex);

    chain_pos[chain_no] = post_vertex;

    if(val_chain >= D)
    {
      process_queue.push(head);
      modified_queue.push(head_modified);

      std::vector<int> *tmp = head;
      head = new std::vector<int>();
      *head = *tmp;
      head_modified = new std::unordered_set<int>();

      val_chain = 0;
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