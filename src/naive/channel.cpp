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
  // std::cout << "Initial max time: " << tm->getMax() << std::endl;
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
  // io_lock.lock();
  // ofile << "Id: " << id << " run" << std::endl;
  // io_lock.unlock();
  // io_lock.lock();
  // std::cout << "runInstance Started at: " << id << std::endl;
  // io_lock.unlock();
  if(!old_process.empty() || !old_modified.empty())
  {
    // io_lock.lock();
    // std::cout << "runInstance of: " << id << " at garbage cleaning" << std::endl;
    // io_lock.unlock();
    garbage_lock.lock();
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
    garbage_lock.unlock();
  }
  if(!process_queue.empty()) // processes chain 
  {
    // io_lock.lock();
    // std::cout << "runInstance of: " << id << " at process queue" << std::endl;
    // io_lock.unlock();
    // io_lock.lock();
    // ofile << "Id: " << id << " run to process" << std::endl;
    // io_lock.unlock();
    process_lock.lock();
    queue_lock.lock();
    bool aa = process_queue.empty();
    bool bb = modified_queue.empty();
    queue_lock.unlock();
    if(aa && bb)
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
        // std::cerr << "Before deleting old" << std::endl;
        // delete old;
        // std::cerr << "After deleting old" << std::endl;
        // std::cerr << "Before deleting q" << std::endl;
        // delete q;
        // std::cerr << "After deleting q" << std::endl;
        garbage_lock.lock();
        old_process.push(old);
        old_modified.push(q);
        garbage_lock.unlock();
        modified_queue.pop();
        process_queue.pop();
        old = tolook;
        // io_lock.lock();
        // ofile << "Queue popped by id: " << id << std::endl;
        // io_lock.unlock();
        double z = tm->getMax();
        if(z <= T * D)
        {
          // std::cout << "Max Time: " << z << ", time now: " << T * D << std::endl;
          isDone = true;
          // io_lock.lock();
          // ofile << "Done" << std::endl;
          // io_lock.unlock();
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

        // io_lock.lock();
        // std::cout << v << ": Old mu " << mu[v];

        double r = mu[v] * ot + oq * (nt - ot -1) + nq;
        r /= (double) nt;

        mu[v] = r;

        // std::cout << " | New mu " << mu[v] << std::endl;
        // io_lock.unlock();

        double disc = 4 * std::fabs(nq - mu[v]) * nt;
        disc /= eps;
        disc += 1;

        double tim = 1 + sqrt(disc);
        tim /= 2;

        L[v] = T;

        tm_lock.lock();
        // std::cout << "Max Time before: " << tm->getMax() << std::endl;
        // std::cout << "Vertex updated: " << v << " with before val: " << tm->getVal(v) << " to val: " << ceil(tim) << std::endl;
        tm->setVal(v, std::max((double)500, ceil(tim)));
        // std::cout << "Max Time after: " << tm->getMax() << std::endl;
        tm_lock.unlock();

      }
    }
    return isDone;
  }
  else // runs chain
  {
    // io_lock.lock();
    // std::cout << "runInstance of: " << id << " at run chain at time " << T << std::endl;
    // io_lock.unlock();
    // io_lock.lock();
    // ofile << "Id: " << id << " run to run Chain" << std::endl;
    // io_lock.unlock();
    update_lock.lock();

    // io_lock.lock();
    // std::cout << "runInstance of: " << id << " got update_lock" << std::endl;
    // io_lock.unlock();

    int chain_no = val_chain++;

    // io_lock.lock();
    // std::cout << "runInstance of: " << id << " increased val_chain" << std::endl;
    // io_lock.unlock();
    if(val_chain <= D)
    {
      // io_lock.lock();
      // std::cout << "runInstance of: " << id << " at val_chain <= D" << std::endl;
      // io_lock.unlock();
      update_lock.unlock();
      // io_lock.lock();
      // std::cout << "runInstance of: " << id << " released update lock" << std::endl;
      // io_lock.unlock();
      int pre_vertex = chain_pos[chain_no];
      int post_vertex = distSelector(CP[pre_vertex]);

      mod_lock.lock();

      (*head)[pre_vertex]--;
      (*head)[post_vertex]++;
      
      head_modified->insert(pre_vertex);
      head_modified->insert(post_vertex);

      chain_pos[chain_no] = post_vertex;


      // io_lock.lock();
      // ofile << "Id: " << id << " Chain: " << chain_no << " moved from " << pre_vertex << " to " << post_vertex << std::endl;
      // ofile << "Val_chain: " << val_chain << ", chains_run: " << chains_run << std::endl;
      // io_lock.unlock();
      chains_run++;
      // io_lock.lock();
      // std::cout << "runInstance of: " << id << " at val_chain <= D completed" << std::endl;
      // io_lock.unlock();
      mod_lock.unlock();
    }
    else
    {
      // io_lock.lock();
      // std::cout << "runInstance of: " << id << " at val_chain == D" << std::endl;
      // io_lock.unlock();
      if(chains_run < D)
      {
        update_lock.unlock();
        // io_lock.lock();
        // std::cout << "runInstance of: " << id << " released update lock and returned" << std::endl;
        // io_lock.unlock();
        return isDone;
      }
      // io_lock.lock();
      // ofile << "Tid: " << id << " Chain_Reset" << std::endl;
      // io_lock.unlock();
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
      // io_lock.lock();
      // std::cout << "runInstance of: " << id << " released update lock" << std::endl;
      // io_lock.unlock();
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