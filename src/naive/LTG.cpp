#include<bits/stdc++.h>

int n, m, a, b;
double w;
std::vector<std::vector<double> > L;
std::vector<std::tuple<int, int, double> > E;

bool checkLaplacian()
{
  for(int i = 0; i < n; i++)
  {
    double rowsum = 0;
    double rowpos = 0;
    for(int j = 0; j < m; j++)
    {
      if(L[i][j] != L[j][i])
      {
        return false;
      }
      rowsum += L[i][j];
      rowpos += (int)(L[i][j] > 0);
      if(i == j)
      {
        continue;
      }
      else if(j > i)
      {
        E.push_back(std::make_tuple(i,j,-1 * L[i][j]));
      }
    }
    if(rowsum > 0.05 || rowpos != 1)
    {
      std::cout << "ROWSUM: " << rowsum << std::endl;
      std::cout << "ROWPOS: " << rowpos << std::endl;
      return false;
    }
  }
  return true;
}

int main()
{
  std::cin >> n;
  std::cin >> m;
  L.resize(n, std::vector<double>(m,0));
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      std::cin >> L[i][j];
    }
  }
  if(!checkLaplacian())
  {
    std::cerr << "Bad Laplacian" << std::endl;;
  }
  std::cout << n << std::endl;
  std::cout << E.size() << std::endl;
  for(auto it : E)
  {
    std::cout << std::get<0>(it) << " " << std::get<1>(it) << " " << std::get<2>(it) << std::endl;
  }
}
