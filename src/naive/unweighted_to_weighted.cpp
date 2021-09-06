#include<bits/stdc++.h>
int main()
{
  int n,m;
  std::cin >> n >> m;
  std::cout << n << ' ' << m << std::endl;
  for(int t = 0; t < m; t++)
  {
    int a,b;
    std::cin >> a >> b;
    std::cout << a << ' ' << b << ' ' << 1 << std::endl;
  }
  std::cout << n << std::endl;
  double z = 0;
  std::vector<double> v(n, 0);
  int t = 0;
  int num_sources = std::min(40,n-1);
  for(; t < num_sources; t++)
  {
    double q = 1;
    z += q;
    v[t] = q;
  }
  v[t] = -1 * z;

  std::random_shuffle(v.begin(), v.end());
  std::cout << n << std::endl;
  for(auto it : v)
  {
    std::cout << it << std::endl;
  }
}
