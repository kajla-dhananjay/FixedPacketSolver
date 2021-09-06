#include<bits/stdc++.h>
int n,m,z,N = -1,u;
double sb,eps;

std::vector<double> b;
std::vector<double> J;

std::vector<std::pair<double, int> > sources;
std::vector<double> D;
std::vector<std::vector<std::pair<double, int> > > P, Cum_P;

std::vector<std::vector<int> > adj_list;

std::vector<std::tuple<int, int, double> > edges;
std::map<std::tuple<int,int,int>, int > mpp;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

std::map<std::pair<int, int>, double> weightMap;
std::map<std::pair<int, int>, std::vector< std::pair<double, int> > > HittingTable;

void outcontainer(const std::vector<double> &v)
{
  for(auto it : v)
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
}

void outcontainer(const std::vector<std::pair<double,int> > &v)
{
  for(auto it : v)
  {
    std::cout << it.first << ' ' << it.second << std::endl;
  }
  std::cout << std::endl;
}

void outmatrix(const std::vector<std::vector<double> > &v, std::string s = "")
{
  std::cout << "Printing " << s << std::endl;
  for(auto it : v)
  {
    outcontainer(it);
  }
}

void outmatrix(const std::vector<std::vector<std::pair<double, int> > > &v, std::string s = "")
{
  std::cout << "Printing " << s << std::endl;
  for(auto it : v)
  {
    outcontainer(it);
  }
}

int distSelector(const std::vector<double> &dist)
{
  int n = dist.size();
 
  double z = rand();
  z /= RAND_MAX;

  //std::cout << std::endl << "z = " << z << std::endl; 

  if(z <= dist[0])
  {
    return 0;
  }

  int low = 0;
  int high = n-1;
 
  int mid = 0;

  while(true)
  {
    mid = low + high;
    mid /= 2;
   
    if(high == low - 1)
    {
      if(dist[low] <= z && dist[high] >= z)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1] < z)
    {
      low = mid+1;
    }
    else if(dist[mid] > z)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid] == z && dist[mid+1] == dist[mid] && mid >0)
      {
        mid--;
      }
      break;
    }
  }
  return mid+1;
}

int distSelector(const std::vector<std::pair<double, int> > &dist)
{
  int n = dist.size();
  double z = rand();
  z /= RAND_MAX;

  if(z <= dist[0].first)
  {
    return dist[0].second;
  }

  int low = 0;
  int high = n-1;
 
  int mid = 0;

  while(true)
  {
    mid = low + high;
    mid /= 2;
   
    if(high == low - 1)
    {
      if(dist[low].first <= z && dist[high].first >= z)
      {
        mid = low;
      }
      else
      {
        mid = high;
      }
      break;
    }

    if(dist[mid+1].first < z)
    {
      low = mid+1;
    }
    else if(dist[mid].first > z)
    {
      high = mid-1;
    }
    else
    {
      while(dist[mid].first == z && dist[mid+1].first == dist[mid].first && mid >0)
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
  int z = start;
  long long int sum = 0;
  int maxs = 0;
  int maxv = -1;
  std::cout << "Sampling number: " << sum << " at node: " << z << " with target: " << end << std::endl;
  while(z != end)
  {
    mp[z]++;
    sum++;
    if(maxs < mp[z])
    {
      maxs = mp[z];
      maxv = z;
    }
    z = distSelector(Cum_P[z]); 
    std::cout << "Sampling number: " << sum << " at node: " << z << " with target: " << end << " top frequency: " << maxs << " top node: " << maxv << std::endl;
  }
  mp[z]++;
  sum++;
  std::cout << "Found target at iteration: " << sum << std::endl;
  exit(0);
  std::vector<std::pair<double, int> > dist;
  double r = 0;
  for(auto it : mp)
  {
    double z = it.second;
    z /= sum;
    r += z;
    dist.push_back(std::make_pair(r, it.first));
  }
  HittingTable[std::make_pair(start, end)] = dist;
}

int bootstrap()
{
  if(N == -1)
  {
    N = 5 * n;
  }
  std::unordered_map<int, int> Shat;
  for(int t = 0; t < N; t++)
  {
    std::cout << "At iteration: " << t << " out of " << N << " iterations" << std::endl; 
    int what = distSelector(sources);
    if(HittingTable.find(std::make_pair(what, u)) == HittingTable.end())
    {
      generateHittingTable(what, u);
    }
    //outcontainer(HittingTable[std::make_pair(what, u)]);
    int uHat = distSelector(HittingTable[std::make_pair(what,u)]);
    Shat[uHat]++;
  }
  int maxv = -1;
  int shat = -1;
  for(auto it : Shat)
  {
    if(maxv < it.second)
    {
      //std::cout << it.first << ' ' << it.second;
      maxv = it.second;
      shat = it.first;
    }
  }
  return shat;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  
  srand(time(0));

  auto start = std::chrono::high_resolution_clock::now();

  std::cin >> n >> m;
  edges.resize(m);
  adj_list.resize(n);
  P.resize(n);
  Cum_P.resize(n);
  D.resize(n);
  
  auto stop1 = std::chrono::high_resolution_clock::now();
  auto dur1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start);
  std::cout << "Base Time: " << (double)dur1.count()/((double)1000000) << " seconds" << std::endl;

  for(int i = 0; i < m; i++)
  {
    int a,b;
    double c;
    std::cin >> a >> b >> c;
    adj_list[a].push_back(b);
    adj_list[b].push_back(a);
    weightMap[{a,b}] = c;
    weightMap[{b,a}] = c;
    D[b] += c;
    D[a] += c;
    edges[i] = std::make_tuple(a,b,c);
  }

  auto stop2 = std::chrono::high_resolution_clock::now();
  auto dur2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - stop1);
  std::cout << "Graph Input Time: " << (double)dur2.count()/(double)1000000 << " seconds" << std::endl;
  
  for(int i = 0; i < n; i++)
  {
    double s = 0;
    for(auto it : adj_list[i])
    {
      double z = weightMap[{i, it}];
      z /= D[i];
      s += z;
      if(z > 0)
      {
        P[i].push_back(std::make_pair(z,it));
        Cum_P[i].push_back(std::make_pair(s,it));
      }
    }
  }
 
  auto stop3 = std::chrono::high_resolution_clock::now();
  auto dur3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - stop2);
  std::cout << "Transition Matrix Computation Time: " << (double)dur3.count()/(double)1000000 << " seconds" << std::endl;

  std::cin >> z;
  b.resize(z);
  J.resize(z);
  for(int i = 0; i < z; i++)
  {
    std::cin >> b[i];
    if(b[i] < 0)
    {
      sb = b[i] * -1;
      u = i;
    }
  }
  for(int i = 0; i < z; i++)
  {
    if(b[i] < 0)
    {
      J[i] = 0;
    }
    else
    {
      J[i] = b[i]/sb;
    }
    if(J[i] > 0)
    {
      sources.push_back(std::make_pair(J[i], i));
    }
  }

  double temp = 0;
  P[u].clear();
  Cum_P[u].clear();

  for(int i = 0; i < n; i++)
  {
    if(J[i] > 0)
    {
      P[u].push_back(std::make_pair(J[i],i));
      temp += J[i];
      Cum_P[u].push_back(std::make_pair(temp,i));
    }
  }

  std::cin >> eps;
  
  auto stop4 = std::chrono::high_resolution_clock::now();
  auto dur4 = std::chrono::duration_cast<std::chrono::microseconds>(stop4 - stop3);
  std::cout << "Graph Input Time: " << (double)dur4.count()/(double)1000000 << " seconds" << std::endl;

  std::cout << bootstrap() << std::endl;

  auto stop5 = std::chrono::high_resolution_clock::now();
  auto dur5 = std::chrono::duration_cast<std::chrono::microseconds>(stop5 - stop4);
  std::cout << "Graph Input Time: " << (double)dur5.count()/(double)1000000 << " seconds" << std::endl;

}
