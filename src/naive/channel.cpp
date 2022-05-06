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
  chains_run = 0;
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

  chain_pos = x;

  std::vector<int> rrr((*head).size());
  for(int i = 0; i < D; i++)
  {
    rrr[chain_pos[i]]++;
  }

  head_modified = new std::unordered_set<int>();

  tm = new indexedSet<double>(n); // Initialize the indexed set

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
  io_lock.lock();
  ofile << "Id: " << id << " run" << std::endl;
  io_lock.unlock();
  if(false) // processes chain 
  {
    io_lock.lock();
    ofile << "Id: " << id << " run to process" << std::endl;
    io_lock.unlock();
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
    io_lock.lock();
    ofile << "Id: " << id << " run to run Chain" << std::endl;
    io_lock.unlock();
    update_lock.lock();
    if(process_queue.size() == 10)
    {
      io_lock.lock();
      isDone = true;
      ofile << "The Chain Positions are as follows: " << std::endl;
      std::vector<std::vector<int> > v(mu.size());
      for(int i = 0; i < (int)(*old).size(); i++)
      {
        v[i].push_back((*old)[i]);
      }
      while(!process_queue.empty())
      {
        std::vector<int> vv = *(process_queue.front());
        process_queue.pop();
        if(vv.size() != v.size())
        {
          ofile << "Size Issue" << std::endl;
          exit(2);
        }
        for(int i = 0; i < (int)v.size(); i++)
        {
          v[i].push_back(vv[i]);
        }
      }
      for(int i = 0; i < (int)v.size(); i++)
      {
        ofile << i << ": ";
        for(auto it : v[i])
        {
          ofile << it << " ";
        }
        ofile << std::endl;
      }

      ofile << "The Chain Modifications are as follows: " << std::endl;
      int cnt = 0;
      while(!modified_queue.empty())
      {
        ofile << cnt << std::endl;
        std::unordered_set<int> *q = modified_queue.front();
        modified_queue.pop();
        for(auto it : *q)
        {
          ofile << it << ' ';
        }
        ofile << std::endl;
        cnt++;
      }
      ofile.close();
      io_lock.unlock();
      exit(0);
    }

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


      io_lock.lock();
      ofile << "Id: " << id << " Chain: " << chain_no << " moved from " << pre_vertex << " to " << post_vertex << std::endl;
      ofile << "Val_chain: " << val_chain << ", chains_run: " << chains_run << std::endl;
      io_lock.unlock();
      chains_run++;
      mod_lock.unlock();
    }
    else
    {
      while(chains_run < D)
      {
        continue;
      }
      io_lock.lock();
      ofile << "Tid: " << id << " Chain_Reset" << std::endl;
      io_lock.unlock();
      process_queue.push(head);
      modified_queue.push(head_modified);

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