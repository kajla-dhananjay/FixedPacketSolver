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

#include "indexedSet.h"
#include "channel.h"
#include "errorHandler.h"
#include "bootstrap.h"
#include "declarations.h"

/************************* Function Prototypes ********************************/

/************************* Utility Functions **********************************/

void DFS(int node, std::vector<int> &visited, int &cnt); 

bool checkConnected(); 

template<typename T>
bool cumDist(); 

template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist); 

void generateHittingTable(int start, int end);

void *runChainParallelInstance(void *ptr); 

/************************* Useful Functions Definitions ***********************/

void init(); 

void bootstrap(); 

void runChainSerial();

void runChainParallel(double eps, std::vector<int> x);

void runChain(); 

void end();

/************************* Utility Functions **********************************/

/**
 * @brief Runs a Depth first search to mark all connected nodes
 * 
 * @param node current node
 * @param visited array indicating wether each node has been visited or not
 * @param cnt counter indicating number of visited nodes
 */

void DFS(int node, std::vector<int> &visited, int &cnt)
{
  cnt++; // increase counter indicating total number of connected vertices
  visited[node] = 1; // color of node is color_val

  for(auto it : Cum_P[node]) // Iterate through the neighbors of current node
  {
    if(visited[it.second] != -1) // Node already visited
    {
      continue;
    }
    DFS(it.second, visited, cnt); // Node not visited, apply DFS recursively
  }
}

/**
 * @brief Checks if the input is connected
 * 
 * @return true Input Graph is connected
 * @return false Input Graph is not connected
 */

bool checkConnected()
{
  std::vector<int> visited(n,-1); // This vector keeps track of visited nodes
  int cnt = 0; // Counter to keep track of number of visited nodes
  DFS(0, visited, cnt); // DFS to update visited counter
  return (cnt == n); // Return true if all nodes have been visited, false otherwise
}

/**
 * @brief Checks if the given array is a Cumulative distribution or not
 * 
 * @tparam T Type of labels on each entity
 * @param dist Candidate distribution
 * @return true Given candidate is a Cumulative distribution
 * @return false Given candidate is not a Cumulative distribution
 */

template<typename T>
bool cumDist(const std::vector<std::pair<double, T> > &dist)
{
  double prev = 0; // checks previous entry
  for(auto it : dist)
  {
    if(it.first < prev) // cumulative prob. distribution is monotonically increasing
    {
      return false;
    }
    prev = it.first; // update previous entry
  }
  return (prev==1); // last entry of the distribution should be 1
}

/**
 * @brief Selects a given label from the distribution
 * 
 * @tparam T Type of label in the disrtibution
 * @param dist Cumulative probability disrtibution
 * @return T 
 */

template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist)
{
  int i1 = dist.size();

  double d1 = rand(); // d1 represents the chosen random value
  d1 /= RAND_MAX; 

  if(d1 <= dist[0].first) // first entry is the selected entry
  {
    return dist[0].second;
  }

  int low = 0, high = i1-1, mid = 0; // Temp variables for binary search

  while(true) // Binary search for the interval of distribution in which d1 falls
  {
    mid = (low + high)/2;

    if(high == low - 1)
    {
      if(dist[low].first <= d1 && dist[high].first >= d1)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1].first < d1)
    {
      low = mid+1;
    }
    else if(dist[mid].first > d1)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid].first == d1 && dist[mid+1].first == dist[mid].first && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  return dist[mid+1].second;
}

template<typename T>
std::vector<T> matrix_vector_mult(std::vector<std::vector<T> > mat, std::vector<T> v)
{
  std::vector<T> ans;
  if(mat.empty())
    return ans;
  int n = mat[0].size();
  if(n != (int)v.size() || n == 0)
  {
    errorHandler err("Matrix vector multiplication: bad dimensions");
  }
  for(int i = 0; i < (int)mat.size(); i++)
  {
    if(n != (int)mat[i].size())
    {
      errorHandler err("Matrix vector multiplication: not a matrix");
    }
    T r = 0;
    for(int j = 0; j < n; j++)
    {
      r += mat[i][j] * v[j]; 
    }
    ans.push_back(r);
  }
  return ans;
}

template<typename T>
std::vector<T> vector_scalar_mult(std::vector<T> v, T val)
{
  for(auto &it : v)
  {
    it *= val;
  }
  return v;
}

template<typename T>
std::vector<T> vector_addition(std::vector<T> a, std::vector<T> b)
{
  if(a.size() != b.size())
  {
    errorHandler err("Vector addition dimension mismatch");
  }
  for(int i = 0; i < (int)a.size(); i++)
  {
    a[i] += b[i];
  }
  return a;
}

template<typename T>
T l1_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    r += std::abs(it);
  }
  return r;
}

template<typename T>
T l2_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    r += (it * it);
  }
  r = sqrt(r);
  return r;
}

template<typename T>
T inf_norm(std::vector<T> a)
{
  T r = 0;
  for(auto it : a)
  {
    if(r < std::abs(it))
    {
      r = std::abs(it);
    }
  }
  return r;
}

/************************* Core Functions ***********************************/


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
  P.resize(n, std::vector<double>(n,0));
  L.resize(n, std::vector<double>(n,0));
  D.resize(n); // Initialize Total node weight tracker
  Shat.resize(n);
  std::mutex tmp;
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
    P[i1][i2] = d1;
    P[i2][i1] = d1;
    L[i1][i2] = -1 * d1;
    L[i2][i1] = -1 * d1;
  }

  if(!checkConnected()) // Check if the graph is connected
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
    for(int j = 0; j < n; j++)
    {
      P[i][j] = P[i][j] / D[i];
    }
    L[i][i] = D[i];
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

  P[u] = j;
  Cum_P[u] = sources; // Initializing Cum_P[u]

  scanf("%lf", &eps); // Input the error parameter
  eps = 1;
  eps /= (double)n;
  eps /= (double)n;
}


/**
 * @brief Generates Hitting table for vertices starting from start and ending at end
 * 
 * @param start Starting vertex
 * @param end Ending vertex
 */

void generateHittingTable(int start, int end)
{
  std::unordered_map<int, int> mp;

  int i1 = start; // Initial vertex
  mp[start]++; // start has been visited
  long long int sum = 1; // Total number of vertices in the walk

  while(i1 != end) // Continue walking until we hit our target
  {
    i1 = distSelector(Cum_P[i1]); // Select next vertex from the transition matrix
    mp[i1]++; // increment occurences of i1
    sum++; // increment vertex counter for the walk
  }

  std::vector<std::pair<double, int> > dist; // Vector to generate Hitting table distribution

  double r = 0;
  for(auto it : mp) // Generate Hitting Table Distribution
  {
    double z = it.second;
    z /= sum;
    r += z;
    dist.push_back(std::make_pair(r, it.first));
  }
  HittingTable[std::make_pair(start, end)] = dist;
}


/**
 * @brief Runs an instance of the markov chain
 * 
 * @param chain_number Chain number to assign to the thread
 * @return void* Exit value of thread(NULL)
 */

void *runChainParallelInstance(void *ptr)
{
  while(true)
  {
    int i1 = chan->getChain();
    if(i1 != -1) // If queue allows us to make a transition, we go ahead
    {
      X_lock[i1].lock();
      int i2 = distSelector(Cum_P[X_P[i1]]); // Find the next vertex
      if(i2 < 0 || i2 >= n)
      {
        errorHandler err("Bad selection by distSelector");
      }
      int a = X_P[i1];
      int b = i2;
      X_P[i1] = b;
      chan->pushUpdate(i1, std::make_pair(a, b)); // Push update
      X_lock[i1].unlock();
    }
    else
    {
      break;
    }
  }
  pthread_exit(NULL);
}


void *bootstrapParallel(void *ar)
{
  io_lock.lock();
  //std::cout << "Thread created " << q << std::endl;
  io_lock.unlock();
  /*
  for(int i = 0; i < n; i++)
  {
    int what = distSelector(sources); // Picking a source
    sourceLock[what].lock();
    if(HittingTable.find(std::make_pair(what, u)) == HittingTable.end()) // No paths between source sink pair
    {
      generateHittingTable(what, u); // Generate a path between source and sink
    }
    sourceLock[what].unlock();
    int uhat = distSelector(HittingTable[std::make_pair(what,u)]); // Pick a vertex from the given distribution
    Shat[uhat]++; // Increment counter for the chosen vertex
  }
  */
  io_lock.lock();
  //std::cout << "Thread destroyed " << q << std::endl;
  io_lock.unlock();
  pthread_exit(NULL);
}


/**
 * @brief Bootstrap runs the bootstrapping algorithm and finds the ideal vertex to start the chain from
 * 
 */

void bootstrap()
{
  if(N == -1)
  {
    N = N_mult * n; // Default Initialization of the chain
  }
  
  std::vector<pthread_t> threads(N_mult);  
  

  for(int t = 0; t < N_mult; t++)
  {
    int err = pthread_create(&(threads[t]), NULL, bootstrapParallel, NULL);
    if (err != 0)
    {
      errorHandler err("Error creating thread");
    }
    /*else 
    {
      std::cout << "Thread " << i << "created." << std::endl;
    }*/
  }
  std::cerr << "Here" << std::endl;
  for(int t = 0; t < N_mult; t++)
  {
    pthread_join(threads[t], NULL);
  }

  int maxv = -1; // Keeps check of maximum freq
  int shat = -1; // Keeps track of best vertex
  for(int i = 0; i < n; i++)
  {
    if(maxv < Shat[i])
    {
      maxv = Shat[i]; // Update new maximum
      shat = i; // Update new winner
    }
  }
  s = shat; // Assign chosen vertex to s
  std::cerr << "Chosen vertex: " << s << std::endl;
}

void checkBootStrap()
{
  std::cout << n << std::endl << n << std::endl;
  for(auto it : P)
  {
    for(auto jt : it)
    {
      std::cout << jt << ' ';
    }
    std::cout << std::endl;
  }
  std::vector<int> v(n,0);
  Eigen::MatrixXd A(n+1,n);
  Eigen::VectorXd B = Eigen::ArrayXd::Zero(n+1);
  for(int i = 0; i < n; i++)
  {
    for(int q = 0; q < n; q++)
    {
      A(i,q) = P[q][i];
    }
    A(i, i) = -1;
  }
  for(int q = 0; q < n; q++)
  {
    A(n,q) = 1; 
  }
  B(n) = 1;
}

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

/**
 * @brief Runs the chain parallely
 * 
 */

void runChainParallel(double e, std::vector<int> x)
{
  chan = new channel(n, s, d, e, x);
  std::vector<std::mutex> mm(d);
  X_lock.swap(mm);
  std::vector<pthread_t> threads(d);
  for(int i = 0; i < d; i++)
  {
    int err = pthread_create(&(threads[i]), NULL, runChainParallelInstance, NULL);
    if (err != 0)
    {
      errorHandler err("Error Creating Thread while running the chain");
    }
  }
  //std::cerr << "Here" << std::endl;
  while(!chan->canStop())
  {
    continue;
  }
}

void initChain_ht()
{
  X_P.resize(d);
  for(int i = 0; i < d; i++)
  {
    X_P[i] = distSelector(sources);
  }
  runChainParallel(eps, X_P);
  std::vector<double> res = chan->getMu();
  int maxi = 0;
  double maxv = res[0];
  for(int i = 0; i < (int) res.size(); i++)
  {
    if(maxv < res[i])
    {
      maxv = res[i];
      maxi = i;
    }
  }
  X_P.resize(d, maxi);
}

void initChain()
{
  X_P.resize(d,s);
}

/**
 * @brief runs the chain 
 */

void runChain()
{
  /***************************** Initialization *******************************/
  initChain();
  runChainParallel(eps, X_P);
  return;
}

/**
 * @brief Completes exit formalities
 * 
 */

void end()
{
  std::vector<double> x = chan->getMu();
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
  //outcontainer(x);
  std::vector<double> Lx = matrix_vector_mult(L, x);
  //outcontainer(Lx);
  std::vector<double> Lx_b = vector_addition(Lx, vector_scalar_mult(b, -1.0));
  //outcontainer(Lx_b);
  //outcontainer(Lx_b);
  std::cout << "L1 norm: how to change " << l1_norm(Lx_b) << std::endl;
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
}

/**
 * @brief Main function
 * 
 * @return int Exit status of the program
 */

int main()
{

  /*****************************Fast I/O Optimization**************************/

  //std::ios_base::sync_with_stdio(false);
  //std::cin.tie(NULL);

  /*****************************Program Start**********************************/

  srand(time(0));

  init();

  //bootstrap();

  //checkBootStrap();

  //return 0;

  s = 0;

  runChain();

  end();


}
