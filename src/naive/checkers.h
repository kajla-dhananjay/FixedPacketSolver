#ifndef _CHECKERS_H_
#define _CHECKERS_H_
#include<bits/stdc++.h>

void DFS(int node, std::vector<int> &visited, int &cnt, std::vector<std::vector<std::pair<double, int> > > &adj); 
bool checkConnected(std::vector<std::vector<std::pair<double, int> > > &adj); 
template<typename T> bool cumDist(const std::vector<std::pair<double, T> > &dist);

#endif