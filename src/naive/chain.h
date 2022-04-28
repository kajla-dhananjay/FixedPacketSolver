#ifndef _CHAIN_H_
#define _CHAIN_H_
#include <bits/stdc++.h>
#include "channel.h"
#include "data.h"

void *runChainParallelInstance(void *ptr); 
channel *runChain(int n, int s, int d, double eps, std::vector<std::vector<std::pair<double, int> > > &CP);
channel *runChain(data *dat);
#endif 