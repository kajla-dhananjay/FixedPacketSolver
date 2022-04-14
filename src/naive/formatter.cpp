#include<bits/stdc++.h>

void DFS(int node, int color_val, std::vector<std::set<int> > &adj_list, std::vector<int> &color, std::unordered_map<int, int> &colormap)
{
  if(color[node] != -1)
  {
    return; // Node already colored
  }

  color[node] = color_val; // color of node is color_val

  for(auto it : adj_list[node])
  {
    if(color[it] != -1)
    {
      continue; // Node already colored
    }
    DFS(it, color_val, adj_list, color, colormap);
  }
}

void outcontainer(const std::vector<int> &v)
{
  for(auto it : v)
  {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
}

std::pair<int, int> DFS_Util(int N, std::vector<std::tuple<int, int, double> > &edges, std::vector<std::set<int> > &adj_list, std::vector<int> &color, std::unordered_map<int, int> &colormap)
{
  int color_cnt = 0; // Total colors used

  for(int i = 0; i < N; i++)
  {
    if(color[i] == -1)
    {
      colormap[color_cnt] = i; // ith node is a representative of component with color = color_cnt
      DFS(i, color_cnt++, adj_list, color, colormap); // Recursive DFS
    }
  }
  //std::cerr << "Total Components = " << color_cnt << " ";
  std::vector<int> cc(color_cnt);
  int maxv = -1, maxo = -1;
  for(int i = 0; i < N; i++)
  {
    if(color[i] < 0 || color[i] >= color_cnt)
    {
      std::cout << "BIG ERROR" << std::endl;
    }
    cc[color[i]]++;
    if(cc[color[i]] > maxv)
    {
      maxv = cc[color[i]];
      maxo = color[i];
    }
  }
  //std::cerr << "| Nodes = " << maxv << " ";
  return {maxo,maxv};
}

int main()
{
  std::string s;
  std::getline(std::cin, s);
  int n = 0,m = 0;
  std::vector<std::tuple<int, int, double> > edges;
  int a,b;
  while(std::cin >> a >> b)
  {
    double c=1;
    n = std::max(n, std::max(a,b));
    a--;
    b--;
    //std::cout << a << ' ' << b << std::endl;
    edges.push_back(std::make_tuple(a,b,c));
  }
  m=edges.size();
  std::vector<std::set<int> > adj_list(n);
  for(int t = 0; t <m; t++)
  {
    int a = std::get<0>(edges[t]);
    int b = std::get<1>(edges[t]);
    //std::cout << t << ' ' << a << ' ' << b << std::endl;
    adj_list[a].insert(b);
    adj_list[b].insert(a);
  }
  //std::cout << "Here" << std::endl;

  std::vector<int> color(n,-1); // Represents the color of the node.
  std::unordered_map<int, int> colormap; // Represents a representative vertex for every color

  std::vector<std::tuple<int, int, double> > new_edges;
  std::pair<int,int> color_dfs = DFS_Util(n, edges, adj_list, color, colormap);
  int color_val = color_dfs.first;
  int new_n = color_dfs.second;
#ifdef Debug
  std::cerr << "Total Edges : " << edge_count << std::endl;
  std::cerr << "Max Edges : " << e << std::endl;
  std::cerr << "Total probability = " << ((double)edge_count) / e << " Expected Probability = " << p/PRECISION << std::endl;
#endif
  int val = 0;
  std::map<int, int> mppp;
  for(auto it : edges)
  {
    int a = std::get<0>(it);
    int b = std::get<1>(it);
    if(color[a] != color[b])
    {
      std::cout << "SAME COMPONENT. COLOR MISMATCH." << std::endl;
      exit(1);
    }
    if(color[a] == color_val)
    {
      if(mppp.find(a) == mppp.end())
      {
        mppp[a] = val++;
      }
      if(mppp.find(b) == mppp.end())
      {
        mppp[b] = val++;
      }
      new_edges.push_back(std::make_tuple(mppp[a], mppp[b], std::get<2>(it)));
    }
  }
  std::cout << new_n << " " << new_edges.size() << std::endl;
  for(auto it : new_edges)
  {
    std::cout << std::get<0>(it) << " " << std::get<1>(it) << " " << std::get<2>(it) << std::endl;
  }
  //std::cerr << "| Edges = " << new_edges.size() << std::endl;


  n = new_n;
  int r = n/2;
  double z = 0;
  std::vector<double> v(r, 0); // r to n 
  int t = 0;
  int num_sources = std::min(40,r-1); // r to n
  for(; t < num_sources; t++)
  {
    double q = 1;
    z += q;
    v[t] = q;
  }
  v[t] = -1 * z;

  std::random_shuffle(v.begin(), v.end());


  for(int i = r+1; i < n; i++) // remove this loop
  {
    v.push_back(0);
  }


  std::cout << n << std::endl;
  for(auto it : v)
  {
    std::cout << it << std::endl;
  }


  std::cout << "0.1123" << std::endl;
}
