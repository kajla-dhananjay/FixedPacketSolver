#include<bits/stdc++.h>
int main()
{
  int n;
  std::cin >> n;
  double z = 0;
  std::vector<double> v(n, 0);
  int i = 0;
  int num_sources = 40;
  for(; i < num_sources; i++)
  { 
    double q = rand();
    q /= RAND_MAX;
    q *= 100;
    z += q;
    v[i] = q;
  }
  v[i] = -1 * z;
  std::random_shuffle(v.begin(), v.end());
  std::cout << n << std::endl;
  for(auto it : v)
  {
    std::cout << it << std::endl;
  }
}
