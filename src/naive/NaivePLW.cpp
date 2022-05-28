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
#include<fstream>

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

std::chrono::time_point<std::chrono::high_resolution_clock> t0, t1;

graph *input_graph(std::string graph_in)
{
  std::ifstream gin(graph_in);
  graph * g = new graph();
  int a, b;
  gin >> a >> b;
  g->n = a;
  g->m = b;
  for(int i = 0; i < b; i++)
  {
    int x, y;
    double z;
    gin >> x >> y >> z;
    g->edges.push_back(std::make_tuple(x,y,z));
  }
  gin.close();
  return g;
}

std::vector<double> *input_b(std::string b_in)
{
  std::ifstream bin(b_in);
  int n;
  bin >> n;
  std::vector<double> *b = new std::vector<double>();
  b->resize(n);
  for(int i = 0; i < n; i++)
  {
    double x;
    bin >> x;
    (*b)[i] = x;
  }
  bin.close();
  return b;
}


data *init(std::string graph_in, std::string b_in, int dd, double e)
{

  // std::cerr << "Init Started" << std::endl;

  graph *g = input_graph(graph_in);

  // std::ofstream outfile;
  // outfile.open("output.txt");
  // outfile << g->n << " " << g->m << std::endl;
  // for(auto it : g->edges)
  // {
  //   outfile << std::get<0>(it) << " " << std::get<1>(it) << " " << std::get<2>(it) << std::endl;
  // }

  // std::cerr << "Graph Input Completed" << std::endl;

  std::vector<double> *b = input_b(b_in);

  // outfile << (*b).size() << std::endl;
  // for(auto it : (*b))
  // {
  //   outfile << it << std::endl;
  // }

  // std::cerr << "b Input Completed" << std::endl;

  
  data *dat = new data(g, b, e, dd);

  // std::cout << "Cumulative Transition Matrix: " << dat->Cum_P.size() << std::endl;

  // for(int i = 0; i < (int)dat->Cum_P.size(); i++)
  // {
  //   std::cout << i << ": ";
  //   for(auto jt : dat->Cum_P[i])
  //   {
  //     std::cout << "{" << jt.first << ", " << jt.second << "} ";
  //   }
  //   std::cout << std::endl;
  // }

  // std::cerr << "Data Generated" << std::endl;

  return dat;
}


/**
 * @brief Completes exit formalities
 * 
 */

void end(channel *chan, data *dat, std::ofstream &oc, std::ofstream &ol, std::vector<double> &p)
{
  std::chrono::duration<double> diff = t1 - t0;
  double timev = diff.count();


  // std::cerr << "End Started" << std::endl;
  std::vector<double> x = chan->getMu();
  // std::cerr << "Got getMu" << std::endl;
  // int iterations = chan->getT();
  // std::cout << "Total Iterations: " << iterations << std::endl;
  // std::cerr << "Got iterations" << std::endl;

  // std::cout << "Final average occupancies: " << std::fixed << std::setprecision(5) << std::endl;

  // for(int i = 0; i < (int)x.size(); i++)
  // {
  //   std::cout << i << ' ' << x[i] << std::endl;
  // }

  // std::cout << dat->u << std::endl;

  // std::cout << dat->D.size() << std::endl;
  // for(auto it : dat->D)
  // {
  //   std::cout << it << std::endl;
  // }

  double sumv = 0;

  for(int i = 0; i < (int)x.size(); i++)
  {
    if(i == dat->u)
    {
      continue;
    }
    double q = x[i];
    q /= dat->D[i];
    sumv += q;
  }

  double uus = x[dat->u];

  // std::cout << "Final Solution: " << std::endl;


  for(int i = 0; i < (int)x.size(); i++)
  {
    if(i == dat->u)
    {
      p[i] = -1.0 * dat->sb;
      p[i] /= dat->n;
      p[i] /= uus;
      p[i] *= sumv;
    }
    else 
    {
      p[i] = dat->sb;
      p[i] /= uus;
      double rr = x[i];
      rr /= dat->D[i];
      double ss = sumv;
      ss /= dat->n;
      rr -= ss;
      p[i] *= rr;
    }
  }
  

  oc << std::fixed << std::setprecision(5);

  oc << p.size() << std::endl;

  for(int i = 0; i < (int)p.size(); i++)
  {
    oc << p[i] << std::endl;
  }
  oc << std::endl;

  ol << timev << std::endl;

  return;

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
  // std::cout << "Averaged L1 norm (L1 norm divided by n) (normalized to b): " << l1_norm(Lx_b) / (dat->n * l1_norm(dat->b)) << std::endl; 
  // #endif

}


/**
 * @brief Main function
 * 
 * @return int Exit status of the program
 */

int main(int argc, char *argv[])
{

  /*****************************Fast I/O Optimization**************************/

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  /*****************************Program Start**********************************/

  if(argc != 7)
  {
    std::cout << "Expected 6 arguments, got: " << argc-1 << std::endl;
    return 1;
  }

  std::string in_graph = std::string(argv[1]);
  std::string b = std::string(argv[2]);
  std::string out_coord = std::string(argv[3]);
  std::string out_logs = std::string(argv[4]);
  int d = std::stoi(std::string(argv[5]));
  double e = std::stod(std::string(argv[6]));

  std::ofstream oc, ol;
  oc.open(out_coord);
  ol.open(out_logs);

  srand(time(0));

  // std::cerr << "Start" << std::endl;


  data *dat = init(in_graph, b, d, e);

  std::vector<double> solution(dat->n);

  t0 = std::chrono::high_resolution_clock::now();

  // std::cerr << "Init Done" << std::endl;

  channel *chan = runChain(dat);

  // std::cerr << "runChain Done" << std::endl;

  t1 = std::chrono::high_resolution_clock::now();


  end(chan, dat, oc, ol, solution);

  oc.close();
  ol.close();
  // std::cerr << "End Done" << std::endl;

}
