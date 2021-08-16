#include<bits/stdc++.h>
#include <random>
int main()
{
  std::map<std::tuple<int, int, int>, double> index;
  int num_nodes;
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
      std::map<int, int> mp;
      int start = i
    }
  }
}
