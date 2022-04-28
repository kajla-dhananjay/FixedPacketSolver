/**
 * @file channel.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Channel Data structure which acts as a central information gathering mechanism
 * @version 1.0
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


channel::channel(int n, int s, int d, double e, std::vector<int> x, double pr)
{
    std::cerr << "Channel BP-0" << std::endl;

    isDone = false; // Initialize isdone to false
    eps = e; // Initialize error margin
    p = pr; // Initialize probability of processing
    T = 1; // Initialize global clock to 1
    D = d; // Initialize number of chains
    val_chain = 0;
    max_queue_size = 10;
    chains_run = 0;

    L.resize(n,0); // Initialize last seen for each vertex
    mu.resize(n, 0); // Initialize average occupancy at each vertex

    selector = {{true, p}, {false, 1}};

    std::cerr << "Channel BP-1" << std::endl;

    std::vector<int> r(n,0);

    tm = new indexedSet<double>(n);

    for(auto it : x)
    {
        r[it]++; // Increase occupancy at occupied vertices
        tm->setVal(it, INT_MAX); // Set timer of occupied vertices to infinity
    }
    
    std::cerr << "Channel BP-2" << std::endl;

    old = new std::vector<int>();
    head = new std::vector<int>();

    *old = r;
    *head = r;

    std::cerr << "Channel BP-3" << std::endl;

    head_modified = new std::unordered_set<int>();

    tm = new indexedSet<double>(n); // Initialize the indexed set

    chain_pos = x;

    std::cerr << "Channel BP-4" << std::endl;

}

/**
 * @brief Indicates whether a particular chain is allowed to push an update
 * 
 * If a chain already has an update in the queue then that chain will be denied
 * @return chain number of a valid chain to run
 * @return -1 if the thread can exit
 */
std::pair<bool, std::pair<int, int> > channel::getCommand()
{
    if(!process_queue.empty() && ((int)process_queue.size() == max_queue_size || distSelector(selector))) {
        return std::make_pair(true, std::make_pair(-1,-1));
    }
    else{
        int x = 0;
        chain_lock.lock();
        if(val_chain == D)
        {
            chain_lock.unlock();
            return std::make_pair(true, std::make_pair(-1,-1));
        }
        x = val_chain++;
        chain_lock.unlock();
        return std::make_pair(false, std::make_pair(x, chain_pos[x]));
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
 * @brief pushes update given by a particular chain into the queue
 * @param chain The chain number pushing the update
 * @param p The update in the form of (old position, new position)
 */
void channel::pushUpdate(int chain, std::pair<int, int> p) // Add a transition in the queue
{
    chains_run++;
    (*head)[p.first]--;
    (*head)[p.second]++;
    (*head_modified).insert(p.first);
    (*head_modified).insert(p.second);
    if(chain_pos[chain] != p.first)
    {
        errorHandler err("Bad Transition");
    }
    chain_pos[chain] = p.second;
    if(chains_run == D){
        process_queue.push(head);
        modified_queue.push(head_modified);
        chains_run = 0;
        val_chain = 0;
        std::vector<int> *tmp = head;
        head = new std::vector<int>();
        *head = *tmp;
        head_modified = new std::unordered_set<int>();
    }
}

void channel::processQueue()
{
    io_lock.lock();
    std::cout << "Process Queue Started" << std::endl;
    io_lock.unlock();
    if(process_queue.empty())
    {
        io_lock.lock();
        std::cout << "Process Queue Empty" << std::endl;
        io_lock.unlock();
        return;
    }
    mod_lock.lock();
    std::vector<int> *tolook = process_queue.front();
    std::unordered_set<int> *q = modified_queue.front();
    if(q->empty())
    {
        io_lock.lock();
        std::cout << "All elements processed" << std::endl;
        io_lock.unlock();
        free(q);
        modified_queue.pop();
        free(old);
        old = process_queue.front();
        process_queue.pop();
        if(tm->getMax() <= T * D)
        {
            isDone = true;
        }
        T++;
        io_lock.lock();
        std::cout << "Time : " << T << std::endl;
        io_lock.unlock();
        mod_lock.unlock();
    }
    else 
    {

        io_lock.lock();
        std::cout << "Process Size: " << q->size() << std::endl;

        int v = *(q->begin());
        q->erase(q->begin());
        mod_lock.unlock();
        
        int ot = L[v] * D;
        int nt = T * D;
        int oq = (*old)[v];
        int nq = (*tolook)[v];

        double r = mu[v] * ot + oq * (nt - ot -1) + nq;
        r /= (double) nt;

        mu[v] = r;

        double disc = 4 * std::fabs(nq - mu[v]) * nt;
        disc /= eps;
        disc += 1;

        double tim = 1 + sqrt(disc);
        tim /= 2;

        L[v] = T;

        std::cout << "Process Size: " << q->size() << std::endl;
        io_lock.unlock();
        tm->setVal(v, ceil(tim));

    }
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