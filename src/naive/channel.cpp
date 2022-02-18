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
*/

channel::channel(int n, int s, int d, double e)
{
    T = 0; // Initialize global clock to 0
    eps = e; // Initialize error margin
    Q.resize(n,0); // Initially all queues empty
    S.resize(n); // Initialize update tracking for each vertex
    L.resize(n,0); // Initialize last seen for each vertex
    mu.resize(n, 0); // Initialize average occupancy at each vertex
    chain_positions.resize(d, s);
    Q[s] = d; // Place all d packets at s
    tm = new indexedSet<double>(n); // Initialize the indexed set
    tm->setVal(s, INT_MAX); // Initialize timer at bootstraped vertex to infinity
    isDone = false; // Initialize isdone to false
    for(int i = 0; i < d; i++)
    {
        v_chains.insert(i);
    }
    batch_size = 4;
}

/**
 * @brief Indicates whether a particular chain is allowed to push an update
 * 
 * If a chain already has an update in the queue then that chain will be denied
 * @return chain number of a valid chain to run
 * @return -1 if the thread can exit
 */
int channel::getChain()
{
    m.lock();
    if(R.size() >= v_chains.size() * batch_size)
    {
        //std::cout << "Calling process at queue size: " << R.size() << std::endl;
        process();
        //std::cout << "Queue size after process: " << R.size() << std::endl;
    }
    if(valid_chains.empty())
    {
        valid_chains = v_chains; // Restore such that all chains are now valid
    }
    int x = *(valid_chains.begin());
    valid_chains.erase(valid_chains.begin());
    m.unlock();
    if(isDone)
    {
        return -1;
    }
    return x;
    
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
    m.lock();
    if(chain_positions[chain] != p.first)
    {
        errorHandler err("Bad chain transition");
    }
    chain_positions[chain] = p.second;
    R.push(p); // Push transition in the queue
    m.unlock();
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
 * @brief Once all chains have pushed their updates, the processing thread is activated.
 * 
 * Processes all transitions present in the queue
 * 
 */
void channel::process()
{
    if(isDone)
        return; // Nothing to do
    //std::cout << "Process started" << std::endl;
    std::set<int> updatedVertices;
    while(!R.empty()) // Updates all transitions
    { 
        T++;
        std::pair<int, int> p = R.front(); // Check out the front of the queue
        R.pop(); // Pop the front of the queue

        S[p.first].push(std::make_tuple(Q[p.first]-1, Q[p.first], T, L[p.first]));
        S[p.second].push(std::make_tuple(Q[p.second]+1, Q[p.second], T, L[p.second]));

        Q[p.first]--; // The packet left p.first
        Q[p.second]++; // The packet arrived at p.second

        L[p.first] = T; // Updates last seen for old vertex
        L[p.second] = T; // Updates last seen for new vertex

        updatedVertices.insert(p.first);
        updatedVertices.insert(p.second);
    }
    for(auto w : updatedVertices)
    {
        while(!S[w].empty())
        {
            auto s = S[w].front(); // Current update to be processed
            S[w].pop();

            int nq = std::get<0>(s); // New occupancy at w
            int oq = std::get<1>(s); // Old occupancy at w
            int nt = std::get<2>(s); // Time at which occupancy changed
            int ot = std::get<3>(s); // Last seen time before the change in occupancy

            double z = mu[w] * ot + oq * (nt - ot - 1) + nq;
            z /= nt;

            mu[w] = z; // Updates the average occupancy at this point

            double disc = 1 + 4 * ((fabs(nq-mu[w]) * ot)/eps);
            double temp = sqrt(disc);
            temp += 1;
            temp /= 2;

            tm->setVal(w, temp); // Updates the countdown timer in the indexed set
            //std::cout << "YO" << std::endl;
            //std::cout << "Time: " << nt << " Vertex: " << w << " with nq: " << nq << " oq: " << oq << " nt: " << nt << " ot: " << ot << " mu: " << z << " temp: " << temp << " Timer: " << tm->getMax() << std::endl;  
        }
    }
    /*
    std::cout << "*****OCCUPANCY REPORT*******" << std::endl;
    for(int i = 0; i < (int)Q.size(); i++)
    {
        std::cout << "Vertex: " << i << ", occupancy: " << Q[i] << std::endl;
    }
    std::cout << "*****AVERAGE OCCUPANCY REPORT*******" << std::endl;
    for(int i = 0; i < (int)Q.size(); i++)
    {
        std::cout << "Vertex: " << i << ", average occupancy: " << mu[i] << std::endl;
    }
    std::cout << "*****TIMER REPORT*******" << std::endl;
    for(int i = 0; i < (int)Q.size(); i++)
    {
        std::cout << "Vertex: " << i << ", timer: " << tm->getVal(i) << std::endl;
    }
    */
    // std::cout << "TM: " << tm->getMax() << ", T: " << T << std::endl;
    if(tm->getMax() <= T) // If the maximum time for any vertex to stabilize in occupancy is less than current time, we stop
    {
        isDone = true;
    }
}