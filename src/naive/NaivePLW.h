#ifndef A0F547E1_B2D5_4820_9CDD_4ADA291248FE
#define A0F547E1_B2D5_4820_9CDD_4ADA291248FE


#include<bits/stdc++.h>

#include "chain.h"
#include "channel.h"
#include "checkers.h"
#include "data.h"
#include "errorHandler.h"
#include "indexedSet.h"
#include "linalg.h"

graph *input_graph(std::string graph_in);
std::vector<double> *input_b(std::string b_in);
data *init(std::string graph_in, std::string b_in, int dd, double e);
void end(std::ofstream &oc, std::ofstream &ol, const std::vector<double> &p);
void solve(data *input, std::vector<double> &p);
void solver(std::string in_graph, std::string b, std::string out_coord, std::string out_logs, int d, double e);
std::vector<double> solver(graph* g, std::vector<double> &b, int d, double e);

#endif /* A0F547E1_B2D5_4820_9CDD_4ADA291248FE */
