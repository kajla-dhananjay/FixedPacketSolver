#include<bits/stdc++.h>
int n,m,z,N,u;
double sb,eps;
std::vector<std::tuple<int, int, double> > edges;
std::map<std::tuple<int,int,int>, int > mpp;
std::vector<double> b;
int bootStrap()
{
  srand(time(0));
  std::vector<double> dist1(n), dist2(n);
  std::map<int, int> S;
  for(int i = 0; i < n; i++)
  {
    dist1[i] = b[i] / sb;
  }
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
  int n,m;
  std::cin >> n >> m;
  N = 100;
  edges.resize(m);
  for(int i = 0; i < m; i++)
  {
    int a,b;
    double c;
    std::cin >> a >> b >> c;
    edges[i] = std::make_tuple(a,b,c);
  }
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
  std::cin >> n;
  for(int i = 0; i < n * n * n; i++)
  {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    mpp[std::make_tuple(a,b,c)] = d;
  }
  std::cin >> eps;
}
