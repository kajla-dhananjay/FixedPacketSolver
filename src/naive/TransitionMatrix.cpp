#include<bits/stdc++.h>

int main()
{
  int n,m;
  std::cin >> n >> m;
  std::vector<std::vector<double> > L(n, std::vector<double>(m,0));
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      std::cin >> L[i][j];
    }
  }
  int nn;
  std::cin >> nn;
  if(nn != n)
  {
    std::cerr << "Inequal sizes of b and L" << std::endl;
  }
  std::vector<double> b(nn);
  for(int i = 0; i < n; i++)
  {
    std::cin >> b[i];
  }
  double sb = 0;
  for(int i = 0; i < n; i++)
  {
    if(b[i] < 0)
    {
      continue;
    }
    sb += b[i];
  }
  std::vector<double> j(n);
  for(int i = 0; i < n; i++)
  {
    if(b[i] < 0)
    {
      j[i] = 0;
      continue;
    }
    else
    {
      j[i] = b[i] / sb;
    }
  }
  for(int i = 0; i < n; i++)
  {
    if(b[i] < 0)
    {
      L[i] = j;
      continue;
    }
    double z = -1;
    z *= L[i][i];
    for(int j = 0; j < n; j++)
    {
      L[i][j] /= z;
    }
    L[i][i] = 0;
  }
  std::cout << n << std::endl << n << std::endl;
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      std::cout << L[i][j] << ' ';
    }
    std::cout << std::endl;
  }
}
