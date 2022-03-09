#include "initialization.h"
#include "checkers.h"

/**
 * @brief Initializes the variables, i.e. Takes input
 * 
 */

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
  std::vector<std::mutex> mm(n);
  sourceLock.swap(mm); // Lock sources


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
