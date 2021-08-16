#include<bits/stdc++.h>

int n, m, a, b;
double w;
std::map<std::pair<int, int>, double> weightMapping;
std::map<int, double> nodeSum;

std::vector<std::vector<double> > getD()
{
  std::vector<std::vector<double> > v(n, std::vector<double>(n,0));
  for(int i = 0; i < n; i++)
  {
    v[i][i] = nodeSum[i];
  }
  return v;
}

std::vector<std::vector<double> > getA()
{ 
  std::vector<std::vector<double> > A(n, std::vector<double>(n,0));
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      A[i][j] = weightMapping[std::make_pair(i,j)];
    }
  }
  return A;
}

std::vector<std::vector<double> > matrixMultiplier(std::vector<std::vector<double> > a, double b)
{
  std::vector<std::vector<double> > product(a.size(), std::vector<double>(a[0].size(),0));
  for(int i = 0; i < a.size(); i++)
  {
    for(int j = 0; j < a[i].size(); j++)
    {
      product[i][j] = a[i][j] * b;
    }
  }
  return product;
}

std::vector<std::vector<double> > matrixAdder(std::vector<std::vector<double> > a, std::vector<std::vector<double> > b, int n, int m)
{
  std::vector<std::vector<double> > addedMatrix(n, std::vector<double>(m,0));
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      addedMatrix[i][j] = a[i][j] + b[i][j];
    }
  }
  return addedMatrix;
}

std::vector<std::vector<double> > getLaplacian()
{
  std::vector<std::vector<double> > D = getD();
  std::vector<std::vector<double> > A = getA();
  std::vector<std::vector<double> > L = matrixAdder(D, matrixMultiplier(A,-1), n, n);
  return L;
}

int main()
{
  std::cin >> n;
  std::cin >> m;
  for(int i = 0; i < m; i++)
  {
    std::cin >> a >> b >> w;
    weightMapping[std::make_pair(a,b)] = w;
    nodeSum[a] += w;
    weightMapping[std::make_pair(b,a)] = w;
    nodeSum[b] += w;
  }
  std::vector<std::vector<double> > L = getLaplacian();
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
