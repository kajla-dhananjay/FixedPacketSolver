/**
 * @file NaivePLW.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief One-sink Laplacian Solver Using Random Walks
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/************************* Standard libraries Import **************************/

#include<bits/stdc++.h>

// Compilation Flag Macros : TIMER, DEBUG, PARALLEL, STEPBYSTEP

/************************* Custom libraries Import **************************/

#include "chain.h"
#include "channel.h"
#include "checkers.h"
#include "errorHandler.h"
#include "indexedSet.h"
#include "initialization.h"
#include "linalg.h"
#include "serialChain.h"


/************************* Program Wide Macros ********************************/

//#define TIMER
//#define DEBUG
//#define PARALLEL
//#define STEPBYSTEP

/************************* Global Declarations ********************************/

int n; ///< Number of Nodes in Graph
int m; ///< Number of Edges in Graph
int s = 0; ///< Vertex chosen via bootstrapping indicating high stationary prob. state


int u = -1; ///< The index of the sink vertex
int timer = 0; ///< Timer for running the chain serially
int N_mult = 64; //< Multiplier for number of bootstrapping runs to have
int N = -1; ///< Number of samples for bootstrapping
int d = 5; ///< Stores the nunber of chains to run
int iterations = 0;

double sb; ///< Stores the sum of non-sink column vectors
double eps = 1; ///< Stores the bound on error required
 
channel *chan;

std::vector<double> b; ///< Column vector b as per definition
std::vector<double> j; ///< Column vector j as per definition
std::vector<double> D; ///< Stores total weight sum for the vertices

std::vector<std::pair<double, int> > sources; ///< Distribution of sources

#ifdef DEBUG
std::vector<std::vector<double> > P; ///< Transition Matrix
std::vector<std::vector<double> > L; ///< Laplacian Matrix for the given graph
#endif

std::vector<std::vector<std::pair<double, int> > > Cum_P; ///< Cumulative Transition Matrix


void init()
{

  int i1, i2; // Temporary Int Variables
  double d1; // Temporary Double Variables


  scanf("%d %d\n", &n, &m); // Input Number of nodes and Number of edges

  Cum_P.resize(n); // Initialize Cumulative Transition Matrix

  #ifdef DEBUG
  P.resize(n, std::vector<double>(n,0));
  L.resize(n, std::vector<double>(n,0));
  #endif

  D.resize(n); // Initialize Total node weight tracker


  /*****************************Take Graph Input*******************************/

  for(int i = 0; i < m; i++) // Input the edges
  {
    scanf("%d %d %lf\n", &i1, &i2, &d1); // Takes in the incident vertices and their edge-weight
    D[i1] += d1; // Add weight of given edges to total node weight
    D[i2] += d1; // Add weight of given edges to total node weight
    Cum_P[i1].push_back(std::make_pair(D[i1], i2)); // Add i2 to cumulative distribution of i1
    Cum_P[i2].push_back(std::make_pair(D[i2], i1)); // Add i1 to cumulative distribution of i2

    #ifdef DEBUG
    P[i1][i2] = d1;
    P[i2][i1] = d1;
    L[i1][i2] = -1 * d1;
    L[i2][i1] = -1 * d1;
    #endif

  }

  if(!checkConnected(Cum_P)) // Check if the graph is connected
  {
    errorHandler err("Given Graph is not connected");
  }


  /*****************************Create Cumulative Transition Matrix***********/

  for(int i = 0; i < n; i++) // Setting up the transition matrix for our markov chain
  {
    for(auto &it : Cum_P[i])
    {
      it.first /= D[i];
    }

    #ifdef DEBUG

    for(int j = 0; j < n; j++)
    {
      P[i][j] = P[i][j] / D[i];
    }
    L[i][i] = D[i];

    #endif
  }
  

  /*****************************Take Column Vector Input***********************/


  scanf("%d\n", &i1); // Input the dimension of column vector, this should equal n

  if(i1 != n)
  {
    errorHandler err("Dimension mismatch between Graph with dimension: " + std::to_string(n) + " and b vector with dimension" + std::to_string(i1));
  }

  b.resize(n); // Initialize b
  j.resize(n); // Initialize j

  for(int i = 0; i < n; i++)
  {
    scanf("%lf\n", &b[i]); // Taking input bi

    if(b[i] < 0 && u == -1) // Finding the sink
    {
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

  d1 = 0; // Cumulative j

  for(int i = 0; i < n; i++)
  {
    if(b[i] <= 0) // Sink vertex
    {
      j[i] = 0; // j_sink = 0 by definition
    }
    else
    {
      j[i] = b[i]/sb; // Using definition of j
      d1 += j[i]; // Updating cumulative value
      sources.push_back(std::make_pair(d1, i)); // Making cumulative probability distribution
    }
  }

  d1 = 0; // Culumative j

  #ifdef DEBUG
  P[u] = j;
  #endif 

  Cum_P[u] = sources; // Initializing Cum_P[u]

  scanf("%lf", &eps); // Input the error parameter
  eps = 1;
  eps /= (double)n;
  eps /= (double)n;
}


/**
 * @brief Completes exit formalities
 * 
 */

void end()
{
  std::vector<double> x = chan->getMu();
  iterations = chan->getT();
  double z = x[u];
  x[u] = 0;
  double z_star = 0;

  for(int i = 0; i < n; i++)
  {
    x[i] /= (z * D[i]);
    z_star += x[i];
  }
  z_star /= n;

  for(int i = 0; i < n; i++)
  {
    x[i] -= z_star;
    x[i] *= sb;
  }
  
  std::cout << "Total Iterations: " << iterations << std::endl;

  #ifdef DEBUG

  std::vector<double> Lx = matrix_vector_mult(L, x);
  std::vector<double> Lx_b = vector_addition(Lx, vector_scalar_mult(b, -1.0));
  std::cout << "L1 norm: " << l1_norm(Lx_b) << std::endl;
  std::cout << "L2 norm: " << l2_norm(Lx_b) << std::endl;
  std::cout << "Infinity norm: " << inf_norm(Lx_b) << std::endl;
  std::cout << "L1 norm(normalized to b): " << l1_norm(Lx_b) / l1_norm(b) << std::endl; 
  std::cout << "L1 norm(normalized to n): " << l1_norm(Lx_b) / n << std::endl; 
  std::cout << "L2 norm(normalized to b): " << l2_norm(Lx_b) / l2_norm(b) << std::endl;
  std::cout << "L2 norm(normalized to n): " << l2_norm(Lx_b) / n << std::endl;
  std::cout << "Infinity norm(normalized to b): " << inf_norm(Lx_b) / inf_norm(b) << std::endl;
  std::cout << "Infinity norm(normalized to n): " << inf_norm(Lx_b) / n << std::endl;
  std::cout << "Averaged L1 norm (L1 norm divided by n): " << l1_norm(Lx_b) / n  << std::endl; 
  std::cout << "Averaged L1 norm (L1 norm divided by n) (normalized to b): " << l1_norm(Lx_b) / (n * l1_norm(b)) << std::endl; 
  #endif

}


/**
 * @brief Main function
 * 
 * @return int Exit status of the program
 */

int main()
{

  /*****************************Fast I/O Optimization**************************/

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  /*****************************Program Start**********************************/

  srand(time(0));

  std::cerr << "Before Init" << std::endl;

  init();

  std::cerr << "Before Chain" << std::endl;

  chan = runChain(n, s, d, eps, Cum_P);

  std::cerr << "Before End" << std::endl;

  end();

}
