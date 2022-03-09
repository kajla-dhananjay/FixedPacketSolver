#ifndef _CHAIN_H_
#define _CHAIN_H_
#include <bits/stdc++.h>
#include "channel.h"

void *runChainParallelInstance(void *ptr); 
channel *runChain(int n, int s, int d, int eps, std::vector<std::vector<std::pair<double, int> > > &CP);

#endif 