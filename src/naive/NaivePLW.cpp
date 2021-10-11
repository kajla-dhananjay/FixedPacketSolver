/************************* Standard libraries Import **************************/

#include<bits/stdc++.h>

// Compilation Flag Macros : TIMER, DEBUG, PARALLEL

/************************* Basic Macros ***************************************/

#define nll "" //empty string abbr
#define br " " //space abbr
#define nl std::endl //newline abbr

/************************* Input Macros ***************************************/

#define in(a) std::cin >> a; //single input abbr
#define in2(a, b) std::cin >> a >> b; // ...
#define in3(a, b, c) std::cin >> a >> b >> c; // ...

/************************* Output Macros ***************************************/

#define out(a) std::cout << a; // set output
#define out2(a, b) std::cout <<  a << br <<  b; // ...
#define out3(a, b, c) std::cout <<  a << br <<  b << br <<  c; // ..

#define outs(a) out2(a,nll) // ...
#define outn(a) std::cout << a << nl; // ...

/************************* Vector I/O Macros ***********************************/

template<typename T> inline void incontainer(std::vector<T> &v) {for(size_t i = 0; i < v.size(); i++){in(v[i]);}} // Inputs in space separated vectors
template<typename T> inline void outcontainer(std::vector<T> &v) {for(size_t i = 0; i < v.size(); i++){outs(v[i]);}out(nl);} // Outputs space separated vectors
template<typename T> inline void inmatrix(std::vector<std::vector<T> > & v) {for(size_t i = 0; i < v.size(); i++){for(size_t j = 0; j < v[i].size(); j++){in(v[i][j])}}} // Inputs in space separated entries of newline separated vectors forming the matrix
template<typename T> inline void outmatrix(std::vector<std::vector<T> > & v) {for(size_t i = 0; i < v.size(); i++){outcontainer(v[i]); out(nl);}} // Outputs the matrix

/************************* Global Declarations ********************************/

int n; // Number of Nodes in Graph
int m; // Number of Edges in Graph
int s; // Vertex chosen via bootstrapping indicating high stationary prob. state
int u; // Stores the index of the sink vertex


int timer = 0; // Timer for running the chain serially
int N = -1; // Number of samples for bootstrapping
int d = 5; // Stores the nunber of chains to run

double sb; // Stores the sum of non-sink column vectors
double eps = 1; // Stores the bound on error required

#ifdef PARALLEL

class Stopper{
private:
  bool IsDone;
public:
  Stopper()
  {
    IsDone = false; // Initialize currect clock to 0
  }
  void Done()
  {
    IsDone = true;
  }
  bool canStop()
  {
    return IsDone;
  }
};
Stopper *stop;
/*
class Timer{
  private:
    int offset; // Indicates Current time
    std::set<int> setvals;
    std::mutex mut; // Locks the clock
  public:
    Timer()
    {
      offset = 0;
      setvals.clear(); // Initialize currect clock to 0
    }
    bool proceed(int v)
    {
      mut.lock();
      bool flag = false;
      if((int)setvals.size() == d)
      {
        setvals.clear();
        offset++;
        flag = true;
      }
      else if(setvals.find(v) == setvals.end())
      {
        setvals.insert(v);
        flag = true;
      }
      mut.unlock();
      return flag;
    }
    int getOffset()
    {
      return offset;
    }
  };
Timer *timeStruct;
*/
class Channel{
private:
  int timeVal; // Global clock
  int clock; // Countdown to limit queue size before processing
  int timer; // Sets inital value of clock once processed
  std::vector<int> Q; // Vertex occupancy
  std::vector<double> mu; // Error bounding vector. Redundant?
  std::set<int> inQueue; // All vertices that are currently waiting to be processed
  std::queue<std::pair<int, int> > updates; // Updates waiting to be processed
public:
  int batches; //  Tells how many batches can the processing thread can go ahead wtih
  Channel()
  {
    Q.resize(n,0); // Initially all queues empty
    mu.resize(n,0); // Initalize mu
    Q[s] = d; // Place all d packets at s
    mu[s] = d; // Initialize mu
    timer = 10; // Initialize timer
    clock = timer; // Initiate clock counter to timer
    timeVal = 0; // Initialize global clock to 0
  }
  bool canProceed(int chain)
  {
    bool r = (inQueue.find(chain) == inQueue.end()); // The chain already has a transition in the queue
    #ifdef DEBUG
      std::cout << "Chain number : " << chain << " asked for permission to go ahead and was" ;
      if(r)
      {
        std::cout << " accpeted." << std::endl;
      }
      else
      {
        std::cout << " denied." << std::endl;
      }
    #endif
    return r;
  }
  void pushUpdate(int chain, std::pair<int, int> p) // Add a transition in the queue
  {
    #ifdef DEBUG
      std::cout << "Chain number : " << chain << " pushed a transition from: " << p.first << " to " << p.second << std::endl ;
    #endif
    inQueue.insert(chain); // Account for the transition in the queue
    if(inQueue.size() == (size_t)d) // If all chains have transitions in the queue, then we can clear the chain
    {
      inQueue.clear(); // Clear the queue of all threads
    }
    updates.push(p); // Push transition in the queue
    timeVal++; // Increment global time
    clock--; // Decrement countdown
    if(clock == 0)
    {
      batches++; // The queue can be processed for 'batches' number of batches
      updates.push(std::make_pair(-1,timeVal)); // Update Dummy Transition to the queue indicating global time at this moment
      clock = timer; // Reset the countdown
    }
  }
  void process()
  {
    #ifdef DEBUG
      std::cout << "Started Processing for " << batches << " number of batches" << std::endl ;
    #endif
    while(batches > 0)
    {
      batches--; // As this iteration will process one batch
      double z = timeVal; // Current time
      std::set<int> updatedQueues; // Keeps track of all queues that need to be updated
      while(!updates.empty() && updates.front().first != -1) // Updates all transitions
      {
        std::pair<int, int> p = updates.front(); // Check out the front of the queue
        updatedQueues.insert(p.first); // Update p.first as a vertex in need for queue updation
        updatedQueues.insert(p.second); // Update p.second as a vertex in need for queue updation
        Q[p.first]--; // The packet left p.first
        Q[p.second]++; // The packet arrived at p.second
        updates.pop(); // Pop the front of the queue
      }
      if(!updates.empty() && updates.front().first == -1)
      {
        z = updates.front().second; // Global time for the batch
        updates.pop(); // Clear dummy transition
      }
      for(auto it : updatedQueues)
      {
        double r = Q[it]/z; // Average occupancy
        mu[it] += r; // Update mu
        #ifdef DEBUG
          std::cout << "Value of change = " << r << std::endl;
        #endif
        if(r < eps)
        {
          stop->Done(); // Stop the chain
          return;
        }
      }
    }
  }
};
Channel *chan;
#endif


#ifdef TIMER // Declares variables for storing timestamps and durations if -DTIMER is passed as a flag

std::chrono::time_point<std::chrono::high_resolution_clock> start; // Starting Time
std::chrono::time_point<std::chrono::high_resolution_clock> stop1, stop2, stop3, stop4, stop5, stop6; // Various Stop Times
std::chrono::duration<long int, std::ratio<1, 1000000>> dur1, dur2, dur3, dur4, dur5; // Various durations

#endif

std::vector<int> X; // State vector for multi-dimension markov chain
std::vector<int> Q; // Occupancy vector for each node
std::vector<double> mu; // Error vector mu as per definition

#ifdef PARALLEL

std::vector<int> X_P; // State vector for multi-dimension markov chain

#endif

std::vector<double> b; // Column vector b as per definition
std::vector<double> j; // Column vector j as per definition
std::vector<double> D; // Stores total weight sum for the vertices

std::vector<std::pair<double, int> > sources; // Distribution of sources

std::vector<std::vector<int> > adj_list; // Adjacency list for the given graph

std::vector<std::tuple<int, int, double> > edges; // List of edges of the graph

std::vector<std::vector<std::pair<double, int> > > P, Cum_P; // Transition Matrix and Cumulative Transition Matrix


std::map<std::pair<int, int>, double> weightMap; // Contains mapping from pair of nodes to their corresponding edge weight

std::map<std::pair<int, int>, std::vector< std::pair<double, int> > > HittingTable; // Stores the computed hitting table distributions for different pair of nodes

/************************* Function Prototypes ********************************/

/************************* Utility Functions **********************************/

void DFS(int node, std::vector<int> &visited, int &cnt); // Performs DFS on the graph storing connected counter in cnt

bool checkConnected(); // Check if the given graph is connected

template<typename T>
bool cumDist(); // Checks whether the

void throwError(std::string err); // Throws Error and exits the program

template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist); // Sample from a given distribution

void generateHittingTable(int start, int end); // Generates Hitting Table between the two given vertices

#ifdef PARALLEL
void runChainParallelInstance(); // An instance of running chain
#endif

/************************* Useful Functions Definitions ***********************/

void init(); // Initializes the chain

void bootstrap(); // Bootstraps the chain and finds s

void runChainSerial(); // Run the chain serially

#ifdef PARALLEL
void runChainParallel(); // Run the chain parallely
#endif

void runChain(); // Run Phase two

void end(); // Completion Formalities

/************************* Utility Function  **********************************/

void DFS(int node, std::vector<int> &visited, int &cnt)
{
  cnt++; // increase counter indicating total number of connected vertices
  visited[node] = 1; // color of node is color_val

  for(auto it : adj_list[node]) // Iterate through the neighbors of current node
  {
    if(visited[it] != -1) // Node already visited
    {
      continue;
    }
    DFS(it, visited, cnt); // Node not visited, apply DFS recursively
  }
}

bool checkConnected()
{
  std::vector<int> visited(n,-1); // This vector keeps track of visited nodes
  int cnt = 0; // Counter to keep track of number of visited nodes
  DFS(0, visited, cnt); // DFS to update visited counter
  return (cnt == n); // Return true if all nodes have been visited, false otherwise
}

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

void throwError(std::string err)
{
  std::cout << " Fatal Error: " << err << std::endl;
  exit(1);
}

template<typename T>
T distSelector(const std::vector<std::pair<double, T> > &dist)
{
  int i1 = dist.size();

  double d1 = rand();
  d1 /= RAND_MAX; // d1 represents the chosen random value

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

#ifdef PARALLEL

void *runChainParallelInstance(void* chain_number)
{
  int *dim_val = (int*)chain_number; //Extract chain number from void pointer
  int i1, i2; // Temporary variables
  i1 = *dim_val; // i1 is the chain number for this thread
  #ifdef DEBUG
    std::cout << "Chain number : " << i1 << " started" << std::endl;
  #endif
  return NULL;
  while(true)
  {
    if(chan->canProceed(i1)) // If queue allows us to make a transition, we go ahead
    {
      if(stop->canStop()) // If infinite norm of error is bounded, we stop the chain
      {
        #ifdef DEBUG
          std::cout << "Stop signal recieved at thread with chain : " << i1 << std::endl;
        #endif
        break;
      }

      i2 = distSelector(Cum_P[X_P[i1]]); // Find the next vertex
      chan->pushUpdate(i1, std::make_pair(X_P[i1], i2)); // Push update
      X_P[i1] = i2; // Update the chain
    }
  }
  pthread_exit(NULL);
}

void* processorThread(void *x)
{
  #ifdef DEBUG
    std::cout << "Processor Thread Started" << std::endl;
  #endif
  return NULL;
  while(chan->batches > 0 && !stop->canStop())
  {
    chan->process();
  }
  pthread_exit(NULL);
}
#endif

void init()
{

  int i1, i2; // Temporary Int Variables
  double d1; // Temporary Double Variables

  #ifdef TIMER

  start = std::chrono::high_resolution_clock::now(); // Start clock for diagnostic purposes

  #endif

  scanf("%d %d\n", &n, &m); // Input Number of nodes and Number of edges
  edges.resize(m); // Initialize edges vector
  adj_list.resize(n); // Initialize adjacency list vector
  P.resize(n); // Initialize Transition Matrix
  Cum_P.resize(n); // Initialize Cumulative Transition Matrix
  D.resize(n); // Initialize Total node weight tracker

  #ifdef TIMER

  stop1 = std::chrono::high_resolution_clock::now(); // First breakpoint for timer
  dur1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start); // Duration for taking in initial input and initializing vectors
  std::cout << "Base Input Time: " << (double)dur1.count()/((double)1000000) << " seconds" << std::endl; // Print duration on commandline

  #endif

  /*****************************Take Graph Input*******************************/

  for(int i = 0; i < m; i++) // Input the edges
  {
    scanf("%d %d %lf\n", &i1, &i2, &d1); // Takes in the incident vertices and their edge-weight
    adj_list[i1].push_back(i2); // Setup adjacency list
    adj_list[i2].push_back(i1); // Setup adjacency list
    weightMap[std::make_pair(i1, i2)] = d1; // Map given edge weight
    weightMap[std::make_pair(i2, i1)] = d1; // Map given edge weight
    D[i1] += d1; // Add weight of given edges to total node weight
    D[i2] += d1; // Add weight of given edges to total node weight
    edges[i] = std::make_tuple(i1,i2,d1); // append given edge to the edges
  }

  if(!checkConnected()) // Check if the graph is connected
  {
    throwError("Given Graph is not connected");
  }

  #ifdef TIMER

  stop2 = std::chrono::high_resolution_clock::now(); // Second breakpoint for timer
  dur2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - stop1); // Duration for taking in graph input
  std::cout << "Graph Input Time: " << (double)dur2.count()/(double)1000000 << " seconds" << std::endl; // Print duration on commandline

  #endif

  /*****************************Create Transition Matrix***********************/

  for(int i = 0; i < n; i++) // Setting up the transition matrix for our markov chain
  {
    d1 = 0; // Cumulative Probability counter

    if(D[i] < 0.00000001) // Check if cumulative edge weight of a vertex is very low or 0
    {
      throwError("Vertex " + std::to_string(D[i]) + " is either fully or almost disconnected."); // Throw Error and exit
    }

    for(auto it : adj_list[i]) // Scan through all neighboring vertices
    {
      double d2 = weightMap[{i, it}]; // Get the weight of the required edge
      d2 /= D[i]; // Find relative weightage of this edge
      d1 += d2; // Add it's probability to the cumulative value
      if(d2 > 0) // If there's a chance to go from i to it, we push it in our transition matrix
      {
        P[i].push_back(std::make_pair(d2,it)); // Update transition matrix
        Cum_P[i].push_back(std::make_pair(d1,it)); // Update the cumulative transition matrix
      }
    }
  }

  #ifdef TIMER

  auto stop3 = std::chrono::high_resolution_clock::now(); // Third breakpoint for timer
  auto dur3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - stop2); // Duration for setting up transition matrix
  std::cout << "Transition Matrix Computation Time: " << (double)dur3.count()/(double)1000000 << " seconds" << std::endl; // Print duration on commandline

  #endif

  /*****************************Take Column Vector Input***********************/


  scanf("%d\n", &i1); // Input the dimension of column vector, this should equal n

  if(i1 != n)
  {
    throwError("Dimension mismatch between Graph with dimension: " + std::to_string(n) + " and b vector with dimension" + std::to_string(i1));
  }

  b.resize(i1); // Initialize b
  j.resize(i1); // Initialize j

  for(int i = 0; i < i1; i++)
  {
    scanf("%lf\n", &b[i]); // Taking input bi
    if(b[i] < 0) // Finding the sink
    {
      sb = b[i] * -1; // Sum of all non-sink vertices
      u = i; // Indentified u
    }
  }
  double sum = 0; // Cumulative j

  for(int i = 0; i < i1; i++)
  {
    if(b[i] < 0) // Sink vertex
    {
      j[i] = 0; // j_sink = 0 by definition
    }
    else
    {
      j[i] = b[i]/sb; // Using definition of j
    }
    if(j[i] > 0)
    {
      sum += j[i]; // Updating cumulative value
      sources.push_back(std::make_pair(sum, i)); // Making cumulative probability distribution
    }
  }

  d1 = 0; // Culumative j

  P[u].clear(); // Initializing P[u]
  Cum_P[u].clear(); // Initializing Cum_P[u]

  /*****************************Updating Transition Matrix for sink************/

  for(int i = 0; i < n; i++)
  {
    if(j[i] > 0)
    {
      P[u].push_back(std::make_pair(j[i],i)); // Push back non-zero j value into transition matrix
      d1 += j[i]; // Push back non-zero j value into transition matrix
      Cum_P[u].push_back(std::make_pair(d1,i)); // Build cumulative transition matrix for sink
    }
  }

  std::cin >> eps; // Input the error parameter
  std::cout << eps << std::endl;
  #ifdef TIMER

  stop4 = std::chrono::high_resolution_clock::now(); // Fourth breakpoint for timer
  dur4 = std::chrono::duration_cast<std::chrono::microseconds>(stop4 - stop3); // Duration for taking column vector input
  std::cout << "Column Vector Input Time: " << (double)dur4.count()/(double)1000000 << " seconds" << std::endl; // Print duration on commandline

  #endif

}

void bootstrap()
{
  if(N == -1)
  {
    N = 15 * n; // Default Initialization of the chain
  }

  std::unordered_map<int, int> Shat; // Represents the vertices we picked

  for(int t = 0; t < N; t++)
  {
    int what = distSelector(sources); // Picking a source
    if(HittingTable.find(std::make_pair(what, u)) == HittingTable.end()) // No paths between source sink pair
    {
      generateHittingTable(what, u); // Generate a path between source and sink
    }
    int uhat = distSelector(HittingTable[std::make_pair(what,u)]); // Pick a vertex from the given distribution
    Shat[uhat]++; // Increment counter for the chosen vertex
  }
  int maxv = -1; // Keeps check of maximum freq
  int shat = -1; // Keeps track of best vertex
  for(auto it : Shat)
  {
    if(maxv < it.second)
    {
      maxv = it.second; // Update new maximum
      shat = it.first; // Update new winner
    }
  }
  s = shat; // Assign chosen vertex to s
}

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

#ifdef PARALLEL


void runChainParallel()
{
  stop = new Stopper();
  chan = new Channel();
  #ifdef DEBUG
    std::cout << "runChainParallel Started" << std::endl;
  #endif
  std::vector<pthread_t> threads(d);
  pthread_t processor;
  for(int i = 0; i < 1; i++)
  {
    int zzz = i;
    int *xxxx = &zzz;
    void *y = (void*)xxxx;
    #ifdef DEBUG
      std::cout << "Starting Thread " << *((int*)y) << std::endl;
    #endif

    int err = pthread_create(&(threads[0]), NULL, runChainParallelInstance, y);
    if (err != 0)
    {
      std::cout << "Error creating thread at index: " << i << "\nExiting" << std::endl;
      exit(1);
    }
  }
  int err = pthread_create(&processor, NULL, processorThread, NULL);
  if (err != 0)
  {
    std::cout << "Error creating processor thread\nExiting" << std::endl;
    exit(1);
  }
  for(int i = 0; i < d; i++)
  {
    pthread_join (threads[i], NULL);
  }
}

#endif

void runChain()
{
  /***************************** Initialization *******************************/


  #ifdef PARALLEL
    X_P.resize(d,s);
    #ifdef DEBUG
      std::cout << "runChain Started" << std::endl;
    #endif
    runChainParallel();
    return;
  #endif
  X.resize(d, s);
  Q.resize(n, 0);
  mu.resize(n,0);
  Q[s] = d;
  runChainSerial();
  return;
}

void end()
{
  std::cout << s << std::endl;
  #ifdef TIMER

  stop5 = std::chrono::high_resolution_clock::now(); // Final breakpoint for timer
  dur5 = std::chrono::duration_cast<std::chrono::microseconds>(stop5 - stop4); // Duration for bootstrapping
  std::cout << "Bootstrapping Time: " << (double)dur5.count()/(double)1000000 << " seconds" << std::endl; // Print duration on commandline

  dur6 = std::chrono::duration_cast<std::chrono::microseconds>(stop5 - start); // Complete duration for program
  std::cout << "Total Time: " << (double)dur6.count()/(double)1000000 << " seconds" << std::endl; // Print duration on commandline

  #endif
}

int main()
{

  /*****************************Fast I/O Optimization**************************/

  //std::ios_base::sync_with_stdio(false);
  //std::cin.tie(NULL);

  /*****************************Program Start**********************************/

  srand(time(0));

  init();

  #ifdef DEBUG
    std::cout << "Completed Initialization" << std::endl;
  #endif

  bootstrap();

  #ifdef DEBUG
    std::cout << "Completed Bootstrapping" << std::endl;
  #endif

  runChain();

  #ifdef DEBUG
    std::cout << "Completed Running the Chain" << std::endl;
  #endif

  end();

  #ifdef DEBUG
    std::cout << "Completed Final formalities" << std::endl;
  #endif

}
