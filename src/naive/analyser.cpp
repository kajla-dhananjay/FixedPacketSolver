#include<bits/stdc++.h>
#include "data.h"
#include "linalg.h"

#define DEBUG

using namespace std;

std::ifstream rg, rb, rx, rt;
std::ofstream writer;

graph *read_graph()
{
  graph *g = new graph();
  int a, b;
  rg >> a >> b;
  g->n = a;
  g->m = b;
  for(int i = 0; i < b; i++)
  {
    int x, y;
    double z;
    rg >> x >> y >> z;
    g->edges.push_back(std::make_tuple(x,y,z));
  }
  return g;
}

std::vector<double> *read_b()
{
  int n;
  rb >> n;
  std::vector<double> *b = new std::vector<double>();
  b->resize(n);
  for(int i = 0; i < n; i++)
  {
    double x;
    rb >> x;
    (*b)[i] = x;
  }
  return b;
}

std::vector<double> *read_x()
{
  int n;
  rx >> n;
  std::vector<double> *b = new std::vector<double>();
  b->resize(n);
  for(int i = 0; i < n; i++)
  {
    double x;
    rx >> x;
    (*b)[i] = x;
  }
  return b;
}

int main(int argc, char *argv[])
{
  std::string g_in, b_in, x_in, t_in, dat_out;
  if(argc != 8) 
  {
    std::cout << "Expected 7 commandline arguments, found: " << argc-1 << std::endl;
    return 0;
  }
  g_in = std::string(argv[1]);
  b_in = std::string(argv[2]);
  x_in = std::string(argv[3]);
  t_in = std::string(argv[4]);
  int num_chains = std::stoi(std::string(argv[5]));
  double e = std::stod(std::string(argv[6]));
  dat_out = std::string(argv[7]);


  rg.open(g_in.c_str());
  rb.open(b_in.c_str());
  rx.open(x_in.c_str());
  rt.open(t_in.c_str());

  writer.open(dat_out.c_str());

  double timev;

  rt >> timev;

  // std::cout << timev << std::endl;

  rt.close();

  laplacian *l = new laplacian(read_graph());
  rg.close();
  std::vector<std::vector<double> > L = l->getL();
  std::vector<double> b = *read_b();
  rb.close();
  std::vector<double> x = *read_x();
  rx.close();

  //std::cout << c_in << std::endl;
  
  

  double n = L.size();

  int nb = 0;
  for(auto it : b)
  {
    if(it > 0.0000001)
    {
      nb++;
    }
  }

  std::vector<double> Lx = matrix_vector_mult(L, x);
  std::vector<double> Lx_b = vector_addition(Lx, vector_scalar_mult(b, -1.0));

  #ifdef DEBUG

  std::cout << "L: " << std::endl;

  std::cout << L.size() << std::endl;
  for(auto it : L)
  {
    for(auto jt : it)
    {
      std::cout << jt << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "x: " << std::endl;

  std::cout << x.size() << std::endl;
  for(auto it : x)
  {
    std::cout << it << std::endl;
  }
  std::cout << std::endl;

  std::cout << "b: " << std::endl;

  std::cout << b.size() << std::endl;
  for(auto it : b)
  {
    std::cout << it << std::endl;
  }
  std::cout << std::endl;
  std::cout << "Lx_b" << std::endl;
  std::cout << Lx_b.size() << std::endl;
  for(int i = 0; i < (int)Lx_b.size(); i++)
  {
    std::cout << Lx[i] << " " << Lx_b[i] << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
  
  #endif

  double l1_n = l1_norm(Lx_b);
  // double l2_n = l2_norm(Lx_b);
  // double inf_n = inf_norm(Lx_b);
  double l1_n_b = l1_n / l1_norm(b);
  // double l1_n_nn = l1_norm(Lx_b) / n;
  double l1_n_nb = l1_n_b / n;

  // std::cout << "L2 norm: " << l2_norm(Lx_b) << std::endl;
  // std::cout << "Infinity norm: " << inf_norm(Lx_b) << std::endl;
  //  std::cout << "L1 norm(normalized to b): " << l1_norm(Lx_b) / l1_norm(dat->b) << std::endl; 
  // std::cout << "L1 norm(normalized to n): " << l1_norm(Lx_b) / n << std::endl; 
  // std::cout << "L2 norm(normalized to b): " << l2_norm(Lx_b) / l2_norm(b) << std::endl;
  // std::cout << "L2 norm(normalized to n): " << l2_norm(Lx_b) / n << std::endl;
  // std::cout << "Infinity norm(normalized to b): " << inf_norm(Lx_b) / inf_norm(b) << std::endl;
  // std::cout << "Infinity norm(normalized to n): " << inf_norm(Lx_b) / n << std::endl;
  // std::cout << "Averaged L1 norm (L1 norm divided by n): " << l1_norm(Lx_b) / n  << std::endl; 
  // std::cout << "Averaged L1 norm (L1 norm divided by n) (normalized to b): " << l1_norm(Lx_b) / (dat->n * l1_norm(dat->b)) << std::endl; 
  
  writer << std::fixed;

  writer << n << " " << nb << " " << num_chains << " " << e << " " << timev << " " << l1_n << " " << l1_n_b << " " << l1_n_nb << std::endl;
  
  writer.close();
}