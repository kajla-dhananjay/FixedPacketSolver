/**
 * @file NaivePLW.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief One-sink Laplacian Solver Using Random Walks
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/************************* Standard libraries Import **************************/

#include<bits/stdc++.h>

// Compilation Flag Macros : DEBUG

/************************* Custom libraries Import **************************/

#include "chain.h"
#include "channel.h"
#include "checkers.h"
#include "data.h"
#include "errorHandler.h"
#include "indexedSet.h"
#include "linalg.h"

/************************* Global Declarations ********************************/


graph *input_graph()
{
  graph * g = new graph();
  int a, b;
  std::cin >> a >> b;
  g->n = a;
  g->m = b;
  for(int i = 0; i < b; i++)
  {
    int x, y;
    double z;
    std::cin >> x >> y >> z;
    g->edges.push_back(std::make_tuple(x,y,z));
  }
  return g;
}

std::vector<double> *input_b()
{
  int n;
  std::cin >> n;
  std::vector<double> *b = new std::vector<double>();
  b->resize(n);
  for(int i = 0; i < n; i++)
  {
    double x;
    std::cin >> x;
    (*b)[i] = x;
  }
  return b;
}

double input_eps()
{
  double e;
  std::cin >> e;
  return e;
}

data *init()
{
  // std::cerr << "Init Started" << std::endl;

  graph *g = input_graph();

  // std::cerr << "Graph Input Completed" << std::endl;

  std::vector<double> *b = input_b();

  // std::cerr << "b Input Completed" << std::endl;

  double e = input_eps();

  // std::cerr << "eps Input Completed" << std::endl;


  e = 1;
  e /= ((double)g->n);
  e /= ((double)g->n);
  e /= ((double)g->n);
  
  e = 0.000000001;
  
  data *dat = new data(g, b, e);

  // std::cerr << "Data Generated" << std::endl;

  return dat;
}


/**
 * @brief Completes exit formalities
 * 
 */

void end(channel *chan, data *dat)
{
  std::vector<double> x = chan->getMu();
  int iterations = chan->getT();

  // std::cout << "Final average occupancies: " << std::endl;

  // for(int i = 0; i < (int)x.size(); i++)
  // {
  //   x[i] = 1.0;
  //   std::cout << i << ' ' << x[i] << std::endl;
  // }

  // std::cout << dat->u << std::endl;

  // std::cout << dat->D.size() << std::endl;
  // for(auto it : dat->D)
  // {
  //   std::cout << it << std::endl;
  // }

  double z = x[dat->u];
  x[dat->u] = 0;
  double z_star = 0;

  for(int i = 0; i < dat->n; i++)
  {
    x[i] /= (z * dat->D[i]);
    z_star += x[i];
  }
  z_star /= dat->n;

  // std::cout << z_star << std::endl;

  for(int i = 0; i < dat->n; i++)
  {
    x[i] -= z_star;
    x[i] *= dat->sb;
  }
  

  //std::cout << "Final Result: " << std::endl;

  for(auto it : x)
  {
    std::cout << it << std::endl;
  }

  std::cout << "Total Iterations: " << iterations << std::endl;


  // #ifdef DEBUG

  //  std::vector<double> Lx = matrix_vector_mult(dat->L, x);
  //  std::vector<double> Lx_b = vector_addition(Lx, vector_scalar_mult(dat->b, -1.0));
  //  std::cout << "L1 norm: " << l1_norm(Lx_b) << std::endl;
  // std::cout << "L2 norm: " << l2_norm(Lx_b) << std::endl;
  // std::cout << "Infinity norm: " << inf_norm(Lx_b) << std::endl;
  //  std::cout << "L1 norm(normalized to b): " << l1_norm(Lx_b) / l1_norm(dat->b) << std::endl; 
  // std::cout << "L1 norm(normalized to n): " << l1_norm(Lx_b) / n << std::endl; 
  // std::cout << "L2 norm(normalized to b): " << l2_norm(Lx_b) / l2_norm(b) << std::endl;
  // std::cout << "L2 norm(normalized to n): " << l2_norm(Lx_b) / n << std::endl;
  // std::cout << "Infinity norm(normalized to b): " << inf_norm(Lx_b) / inf_norm(b) << std::endl;
  // std::cout << "Infinity norm(normalized to n): " << inf_norm(Lx_b) / n << std::endl;
  // std::cout << "Averaged L1 norm (L1 norm divided by n): " << l1_norm(Lx_b) / n  << std::endl; 
  //  std::cout << "Averaged L1 norm (L1 norm divided by n) (normalized to b): " << l1_norm(Lx_b) / (dat->n * l1_norm(dat->b)) << std::endl; 
  // #endif

}


/**
 * @brief Main function
 * 
 * @return int Exit status of the program
 */

int main()
{

  /*****************************Fast I/O Optimization**************************/

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  /*****************************Program Start**********************************/

  srand(time(0));

  // std::cerr << "Start" << std::endl;

  data *dat = init();

  // std::cerr << "Init Done" << std::endl;

  channel *chan = runChain(dat);

  // std::cerr << "runChain Done" << std::endl;

  end(chan, dat);

  // std::cerr << "End Done" << std::endl;

}
