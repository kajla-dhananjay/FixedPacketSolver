/**
 * @file data.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief Data structures to stores graphs, laplacians and other data
 * @version 2.0
 * @date 2021-05-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<bits/stdc++.h>
#include "data.h"
#include "checkers.h"
#include "errorHandler.h"

graph::graph()
{

}

graph::graph(laplacian *l)
{
  std::vector<std::vector<double> > ll = l->getL();
  n = ll.size();
  edges.clear();
  for(int i = 0; i < n; i++)
  {
    for(int j = i+1; j < n; j++)
    {
      if(abs(ll[i][j]) > 0.0000001)
      edges.push_back({i, j, ll[i][j]});
    }
  }
  m = edges.size();
}

laplacian::laplacian()
{

}

laplacian::laplacian(std::vector<std::vector<double> > l)
{
  L = l;
}

laplacian::laplacian(graph *g)
{
  L.resize(g->n, std::vector<double>(g->n, 0));
  std::vector<double> D(g->n);
  for(auto it : g->edges)
  {
    L[std::get<0>(it)][std::get<1>(it)] = -1 * std::get<2>(it);
    L[std::get<1>(it)][std::get<0>(it)] = -1 * std::get<2>(it);
    L[std::get<0>(it)][std::get<0>(it)] += std::get<2>(it);
    L[std::get<1>(it)][std::get<1>(it)] += std::get<2>(it);
  }
}
    
std::vector<std::vector<double> > laplacian::getL()
{
  return L;
}

data::data()
{

}

data::data(graph *g, std::vector<double> *v, double e, int dd)
{
    this->n = g->n;
    this->m = g->m;
    this->d = dd;

    this->Cum_P.resize(this->n); //!< Initialize Cumulative Transition Matrix

    this->D.resize(n); //!< Initialize Total node weight tracker

    //!< this->L.resize(n, std::vector<double>(n));

    for(auto it : g->edges) // Input the edges
    {
        int i1 = std::get<0>(it);
        int i2 = std::get<1>(it);
        double d1 = std::get<2>(it);

        D[i1] += d1; //!< Add weight of given edges to total node weight
        D[i2] += d1; //!< Add weight of given edges to total node weight
        Cum_P[i1].push_back(std::make_pair(D[i1], i2)); //!< Add i2 to cumulative distribution of i1
        Cum_P[i2].push_back(std::make_pair(D[i2], i1)); //!< Add i1 to cumulative distribution of i2
    }

    if(!checkConnected(Cum_P)) //!< Check if the graph is connected
    {
        errorHandler err("Given Graph is not connected");
    }

  /*****************************Create Cumulative Transition Matrix***********/

    for(int i = 0; i < n; i++) //!< Setting up the transition matrix for our markov chain
    {
        for(auto &it : Cum_P[i])
        {
            it.first /= D[i];
        }
    }
  
  /*****************************Take Column Vector Input***********************/

    int i1 = v->size();

    if(i1 != n)
    {
        errorHandler err("Dimension mismatch between Graph with dimension: " + std::to_string(n) + " and b vector with dimension" + std::to_string(i1));
    }

    this->b.resize(n); //!< Initialize b
    this->j.resize(n); //!< Initialize j

    this->sb = 0;

    for(int i = 0; i < (int)v->size(); i++)
    {
        this->b[i] = (*v)[i];

        if(this->b[i] < 0 && u == -1) //!< Finding the sink
        {
          u = i; //!< Indentified u 
        }
        else if(b[i] < 0) //!< Identified Multiple sinks
        {
          errorHandler err("Multiple Sinks");
        }
        else //!< Possible source
        {
          sb += b[i]; //!< Sum of all b cordinates
        }
    }

  double d1 = 0; //!< Cumulative j

  for(int i = 0; i < n; i++)
  {
    if(b[i] <= 0) //!< non_source vertex
    {
      j[i] = 0; //!< j = 0 by definition
    }
    else
    {
      j[i] = b[i]/sb; //!< Using definition of j
      d1 += j[i]; //!< Updating cumulative value
      sources.push_back(std::make_pair(d1, i)); //!< Making cumulative probability distribution
    }
  }

  Cum_P[u] = sources; //!< Initializing Cum_P[u]

  this->eps = e; //!< Error parameter
}

