#include<bits/stdc++.h>
#include "data.h"
#include "checkers.h"
#include "errorHandler.h"

graph::graph()
{

}

data::data()
{

}

data::data(graph *g, std::vector<double> *v, double e)
{
    std::cerr << "Graph Input BP-1" << std::endl;

    this->n = g->n;
    this->m = g->m;

    this->Cum_P.resize(this->n); // Initialize Cumulative Transition Matrix

    this->D.resize(n); // Initialize Total node weight tracker

    std::cerr << "Graph Input BP-2" << std::endl;

    // this->L.resize(n, std::vector<double>(n));

    for(auto it : g->edges) // Input the edges
    {
        int i1 = std::get<0>(it);
        int i2 = std::get<1>(it);
        double d1 = std::get<2>(it);

        D[i1] += d1; // Add weight of given edges to total node weight
        D[i2] += d1; // Add weight of given edges to total node weight
        Cum_P[i1].push_back(std::make_pair(D[i1], i2)); // Add i2 to cumulative distribution of i1
        Cum_P[i2].push_back(std::make_pair(D[i2], i1)); // Add i1 to cumulative distribution of i2

        // L[i1][i2] = -1 * d1;
        // L[i2][i1] = -1 * d1;
    }

    std::cerr << "Graph Input BP-3" << std::endl;

    if(!checkConnected(Cum_P)) // Check if the graph is connected
    {
        errorHandler err("Given Graph is not connected");
    }

    std::cerr << "Graph Input BP-4" << std::endl;


  /*****************************Create Cumulative Transition Matrix***********/

    for(int i = 0; i < n; i++) // Setting up the transition matrix for our markov chain
    {
        for(auto &it : Cum_P[i])
        {
            it.first /= D[i];
        }
        // L[i][i] = D[i];
    }

    std::cerr << "Graph Input BP-5" << std::endl;
  

  /*****************************Take Column Vector Input***********************/


    int i1 = v->size();

    if(i1 != n)
    {
        errorHandler err("Dimension mismatch between Graph with dimension: " + std::to_string(n) + " and b vector with dimension" + std::to_string(i1));
    }

    this->b.resize(n); // Initialize b
    this->j.resize(n); // Initialize j

    this->sb = 0;

    std::cerr << "Graph Input BP-6" << std::endl;

    for(int i = 0; i < (int)v->size(); i++)
    {
      std::cerr << i << ' ' << (*v)[i] << std::endl;
        this->b[i] = (*v)[i];

        if(this->b[i] < 0 && u == -1) // Finding the sink
        {
          std::cerr << "YO" << std::endl;
          exit(0);
            u = i; // Indentified u 
        }
        else if(b[i] < 0) // Identified Multiple sinks
        {
            errorHandler err("Multiple Sinks");
        }
        else // Possible source
        {
            sb += b[i]; // Sum of all b cordinates
        }
    }

    std::cerr << "Graph Input BP-7" << std::endl;

  double d1 = 0; // Cumulative j

  for(int i = 0; i < n; i++)
  {
    if(b[i] <= 0) // non_source vertex
    {
      j[i] = 0; // j = 0 by definition
    }
    else
    {
      j[i] = b[i]/sb; // Using definition of j
      d1 += j[i]; // Updating cumulative value
      sources.push_back(std::make_pair(d1, i)); // Making cumulative probability distribution
    }
  }

  std::cerr << "Graph Input BP-8" << std::endl;

  Cum_P[u] = sources; // Initializing Cum_P[u]

  std::cerr << "Graph Input BP-9" << std::endl;

  this->eps = e;
}
