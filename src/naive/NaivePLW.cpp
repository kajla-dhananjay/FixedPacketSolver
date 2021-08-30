#include<bits/stdc++.h>
int n,m,z,N,u;
double sb,eps;
std::vector<std::tuple<int, int, double> > edges;
std::map<std::tuple<int,int,int>, int > mpp;
std::vector<double> b;
std::vector<std::vector<double> > P, Cum_P;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

std::unordered_map<std::pair<int, int>, std::vector< std::pair<double, int> >, hash_pair > HittingTable;

int distSelector(const std::vector<double> &dist)
{
  int n = dist.size();
 
  for(auto it : dist)
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;

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
  //std::cout << "Chosen value: " << mid << " | Chosen Interval: " << dist[mid] << " - " << dist[mid+1] << std::endl;
  return mid+1;
}

int distSelector(const std::vector<std::pair<double, int> > &dist)
{
  int n = dist.size();
 
  /*
  for(auto it : dist)
  {
    std::cout << it.second << ' ' << it.first << std::endl;
  }
  std::cout << std::endl;
  */

  double z = rand();
  z /= RAND_MAX;

  if(z <= dist[0].first)
  {
    return dist[0].second;
  }

  //std::cout << std::endl << "z = " << z << std::endl; 

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
  //std::cout << "Chosen value: " << dist[mid+1].second << " | Chosen Interval: " << dist[mid].first << " - " << dist[mid+1].first << std::endl;
  return dist[mid+1].second;
}

void generateHittingTable(int start, int end)
{

  std::map<int, int> mp;
  int z = start;
  while(z != end)
  {
    mp[z]++;
    z = distSelector(Cum_P[z]); 
  }
}

void outcontainer(std::vector<double> v)
{
  std::cout << v.size() << std::endl;
  for(auto it : v)
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
}

int bootStrap()
{
  std::cout << n << std::endl;
  srand(time(0));
  std::vector<double> dist1(n), dist2(n);
  std::map<int, int> S;
  for(int i = 0; i < n; i++)
  {
    dist1[i] = b[i] / sb;
  }
  //outcontainer(dist1);
  for(int i = 0; i < N; i++)
  {
    double z = rand();
    z /= RAND_MAX;
    double sd = 0;
    int j = 0;
    for(; j < n; j++)
    {
      if(z >= sd && z < sd + dist1[j])
      {
        break;
      }
      sd += dist1[j];
    }
    int start = j;
    int end = u;
    int tot = 0;
    for(int i = 0; i < n; i++)
    {
      tot += mpp[std::make_tuple(start, end, i)];
    }
    for(int i = 0; i < n; i++)
    {
      dist2[i] = mpp[std::make_tuple(start, end, i)];
      dist2[i] /= tot;
    }
    z = rand();
    z /= RAND_MAX;
    sd = 0;
    j = 0;
    for(; j < n; j++)
    {
      if(z >= sd && z < sd + dist2[j])
      {
        break;
      }
      sd += dist2[j];
    }
    std::cout << "On trial: " << i << ", the algo selected : " << j << std::endl;
    S[j]++;
  }
  int maxv = -1, maxs = 0;
  for(auto it : S)
  {
    if(maxv < it.second)
    {
      maxv = it.second;
      maxs = it.first;
    }
  }
  return maxs;
}

int main()
{
  //Handling Graph Input
  std::cin >> n >> m;
  edges.resize(m);
  for(int i = 0; i < m; i++)
  {
    int a,b;
    double c;
    std::cin >> a >> b >> c;
    edges[i] = std::make_tuple(a,b,c);
  }

  //Handling Column vector Input
  std::cin >> z;
  b.resize(z);
  for(int i = 0; i < z; i++)
  {
    std::cin >> b[i];
    if(b[i] < 0)
    {
      sb = b[i] * -1;
      u = i;
    }
  }
  
  //Handling Epsilon Input
  std::cin >> eps;
  std::cout << bootStrap() << std::endl;
}
