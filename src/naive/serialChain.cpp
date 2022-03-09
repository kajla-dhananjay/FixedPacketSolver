#include <bits/stdc++.h>
#include "serialChain.h"
#include "declarations.h"
#include "distSelector.h"

/**
 * @brief Runs the chain serially
 * 
 */

void runChainSerial()
{
  int i1, i2;
  double d1, d2, d3;
  timer = 0;
  while(true)
  {
    timer++; // t = t+1

    i1 = ((rand()) % d); // Choosing a dimension to update
    d1 = rand(); // Check whether the update is lazy
    d1 /= RAND_MAX;

    if(d1 <= 0.5)
    {
      i2 = distSelector(Cum_P[X[i1]]); // Select new vertex from distribution

      Q[i2]++; // Increment occupancy for new vertex
      Q[X[i1]]--; // Decrement occupancy for previous vertex

      d2 = ((double)Q[X[i1]])/((double)timer); // Increment in mu for previous vertex
      d3 = ((double)Q[i2])/((double)timer); // Increment in mu for new vertex

      mu[X[i1]] += d2; // Update mu[previous vertex]
      mu[i2] += d3; // Update mu[new vertex]

      X[i1] = i2; // Update state of multi-dim markov chain
    }

    if(d2 < eps || d3 < eps)
    {
      std::cout << d2 << ' ' << d3 << ' ' << eps << "| Time taken: " << timer << std::endl;
      return;
    }
  }
}