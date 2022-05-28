#include<bits/stdc++.h>
#include "data.h"
using namespace std;
std::ifstream reader;
std::ofstream writer;

graph *read_graph()
{
  graph *g = new graph();
  int a, b;
  reader >> a >> b;
  g->n = a;
  g->m = b;
  for(int i = 0; i < b; i++)
  {
    int x, y;
    double z;
    reader >> x >> y >> z;
    g->edges.push_back(std::make_tuple(x,y,z));
  }
  return g;
}

void write_graph(graph *g)
{
  writer << g->n << " " << g->m << std::endl;
  for(auto it : g->edges)
  {
    writer << std::get<0>(it) << " " << std::get<1>(it) << " " << std::get<2>(it) << std::endl;
  }
}

laplacian *read_laplacian()
{
  int n;
  reader >> n;
  std::vector<std::vector<double> > v(n, std::vector<double>(n));
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < n; j++)
    {
      reader >> v[i][j];
    }
  }
  laplacian *l = new laplacian(v);
  return l;
}

void write_laplacian(laplacian *l)
{
  std::vector<std::vector<double> > v = l->getL();
  writer << v.size() << std::endl;
  for(int i = 0; i < (int)v.size(); i++)
  {
    if(v[i].size())
    {
      writer << v[i][0];
    }
    for(int j = 1; j < (int)v[i].size(); j++)
    {
      writer << " " << v[i][j] ;
    }
    writer << std::endl;
  }
  return;
}

int main(int argc, char *argv[])
{
  std::string conv_type, in_file, out_file;
  if(argc != 4) 
  {
    std::cout << "Expected 3 commandline arguments, found: " << argc-1 << std::endl;
    return 0;
  }
  conv_type = std::string(argv[1]);
  in_file = std::string(argv[2]);
  out_file = std::string(argv[3]);
  reader.open(in_file.c_str());
  writer.open(out_file.c_str());
  if(conv_type == "0")
  {
    write_laplacian(new laplacian(read_graph()));
  }
  else 
  {
    write_graph(new graph(read_laplacian()));
  }
  reader.close();
  writer.close();
}