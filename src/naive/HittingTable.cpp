#include<bits/stdc++.h>
#include <ostream>
int nextStep(std::vector<std::vector<double> > &P, int curr)
{
  int n = P.size();
  double z = rand();
  z /= RAND_MAX;
  double sum = 0;
  for(int i = 0; i < n; i++)
  {
    if(z >= sum && z < sum + P[curr][i])
    {
      return i;
    }
    else
    {
      sum += P[curr][i];
    }
  }
  return n-1;
}
int main()
{
  std::map<std::tuple<int, int, int>, double> index;
  int num_nodes;
  srand(time(0));
  std::cin >> num_nodes;
  std::cin >> num_nodes;
  std::vector<std::vector<double> > P(num_nodes, std::vector<double> (num_nodes, 0));
  for(int i = 0; i < num_nodes; i++)
  {
    for(int j = 0; j < num_nodes; j++)
    {
      std::cin >> P[i][j];
    }
  }
  for(int i = 0; i < num_nodes; i++)
  {
    for(int j = 0; j < num_nodes; j++)
    {
      int start = i;
      int end = j;
      index[std::make_tuple(start,end,start)]++;
      index[std::make_tuple(start,end,end)]++;
      int z = nextStep(P, start);
      while(z != end)
      {
        index[std::make_tuple(start,end,z)]++;
        z = nextStep(P, z);
      }
    }
  }
  for(int i = 0; i < num_nodes; i++)
  {
    for(int j = 0; j < num_nodes; j++)
    {
      for(int k = 0; k < num_nodes; k++)
      {
        std::cout << i << " " << j << " " << k << " " << index[std::make_tuple(i,j,k)] << std::endl;
      }
    }
  }
}
