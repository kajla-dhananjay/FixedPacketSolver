#include<bits/stdc++.h>
int main()
{
  std::cin.tie(NULL);
  std::ios_base::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  std::cout << n << ' ' << m << std::endl;
  for(int i = 0; i < m; i++)
  {
    std::string s;
    std::cin >> s;
    std::cout << s << std::endl;
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
