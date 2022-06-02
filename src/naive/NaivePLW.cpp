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

void end(std::ofstream &oc, std::ofstream &ol, const std::vector<double> &p)
{
  std::chrono::duration<double> diff = t1 - t0;
  double timev = diff.count();

  oc << std::fixed << std::setprecision(5);
  oc << p.size() << std::endl;

  for(int i = 0; i < (int)p.size(); i++)
  {
    oc << p[i] << std::endl;
  }

  oc << std::endl;
  ol << timev << std::endl;
  return;
}

void solve(data *input, std::vector<double> &p)
{
  channel *chan = runChain(input);

  std::vector<double> x = chan->getMu();
  double uus = x[input->u];
  
  p.clear();
  p.resize(x.size());

  double sumv = 0;

  for(int i = 0; i < (int)x.size(); i++)
  {
    if(i == input->u)
    {
      continue;
    }
    double q = x[i];
    q /= input->D[i];
    sumv += q;
  }



  for(int i = 0; i < (int)x.size(); i++)
  {
    if(i == input->u)
    {
      p[i] = -1.0 * input->sb;
      p[i] /= input->n;
      p[i] /= uus;
      p[i] *= sumv;
    }
    else 
    {
      p[i] = input->sb;
      p[i] /= uus;
      double rr = x[i];
      rr /= input->D[i];
      double ss = sumv;
      ss /= input->n;
      rr -= ss;
      p[i] *= rr;
    }
  }

  return;
}

void solver(std::string in_graph, std::string b, std::string out_coord, std::string out_logs, int d, double e)
{
  std::ofstream oc, ol;

  oc.open(out_coord);
  ol.open(out_logs);

  srand(time(0));

  data *dat = init(in_graph, b, d, e);

  std::vector<double> output(dat->n);

  t0 = std::chrono::high_resolution_clock::now();
  
  solve(dat, output);
  
  t1 = std::chrono::high_resolution_clock::now();
  
  end(oc, ol, output);
  
  oc.close();
  ol.close();
}

std::vector<double> solver(graph* g, std::vector<double> &b, int d, double e)
{
  srand(time(0));
  data *dat = new data(g, &b, e, d);
  std::vector<double> output(dat->n);
  solve(dat, output);
  return output;
}

