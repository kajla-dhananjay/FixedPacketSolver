#ifndef _DECLARATIONS_H_
#define _DECLARATIONS_H_

#include "indexedSet.h"
#include "channel.h"
#include "errorHandler.h"
#include "bootstrap.h"

/************************* Program Wide Macros ********************************/

//#define TIMER
//#define DEBUG
//#define PARALLEL
//#define STEPBYSTEP

/************************* Basic Macros ***************************************/

#define nll "" ///< empty string alias
#define br " " ///< space alias
#define nl std::endl ///< newline alias

/************************* Input Macros ***************************************/

#define in(a) std::cin >> a; ///< single input alias
#define in2(a, b) std::cin >> a >> b; ///< double input alias
#define in3(a, b, c) std::cin >> a >> b >> c; ///< triple input alias

/************************* Output Macros ***************************************/

#define out(a) std::cout << a; ///< single output alias
#define out2(a, b) std::cout <<  a << br <<  b; ///< double output alias
#define out3(a, b, c) std::cout <<  a << br <<  b << br <<  c; ///< triple output alias

#define outs(a) out2(a,nll) ///< output with space alias
#define outn(a) std::cout << a << nl; ///< output with newline alias

/************************* Vector I/O Macros ***********************************/

template<typename T> inline void incontainer(std::vector<T> &v) {for(size_t i = 0; i < v.size(); i++){in(v[i]);}} ///< Inputs space separated dynamic array
template<typename T> inline void outcontainer(std::vector<T> &v) {for(size_t i = 0; i < v.size(); i++){outs(v[i]);}out(nl);} ///< Outputs space separated dynamic array
template<typename T> inline void inmatrix(std::vector<std::vector<T> > & v) {for(size_t i = 0; i < v.size(); i++){for(size_t j = 0; j < v[i].size(); j++){in(v[i][j])}}} ///< Inputs space separated entries of newline separated dynamic arrays forming a matrix
template<typename T> inline void outmatrix(std::vector<std::vector<T> > & v) {for(size_t i = 0; i < v.size(); i++){outcontainer(v[i]); out(nl);}} ///< Outputs a matrix

/************************* Global Declarations ********************************/

int n; ///< Number of Nodes in Graph
int m; ///< Number of Edges in Graph
int s = 0; ///< Vertex chosen via bootstrapping indicating high stationary prob. state


int u = -1; ///< The index of the sink vertex
int timer = 0; ///< Timer for running the chain serially
int N_mult = 64; //< Multiplier for number of bootstrapping runs to have
int N = -1; ///< Number of samples for bootstrapping
int d = 5; ///< Stores the nunber of chains to run
int iterations = 0;

double sb; ///< Stores the sum of non-sink column vectors
double eps = 1; ///< Stores the bound on error required

std::mutex io_lock;  ///< Locks STDIN STDOUT manually
 
channel *chan;

std::vector<int> X_P; // State vector for multi-dimension markov chain


std::vector<int> identity; ///< Identity vector
std::vector<int> X; ///< State vector for multi-dimension markov chain
std::vector<int> Q; ///< Occupancy vector for each node
std::vector<double> mu; ///< Error vector mu as per definition


std::vector<double> b; ///< Column vector b as per definition
std::vector<double> j; ///< Column vector j as per definition
std::vector<double> D; ///< Stores total weight sum for the vertices

std::vector<std::pair<double, int> > sources; ///< Distribution of sources

#ifdef DEBUG
std::vector<std::vector<double> > P; ///< Transition Matrix
std::vector<std::vector<double> > L; ///< Laplacian Matrix for the given graph
#endif

std::vector<std::vector<std::pair<double, int> > > Cum_P; ///< Cumulative Transition Matrix

std::map<std::pair<int, int>, std::vector< std::pair<double, int> > > HittingTable; ///< Stores the computed hitting table distributions for different pair of nodes
std::vector<std::mutex> sourceLock; ///< Locks sources to generate hitting table.
std::vector<std::mutex> X_lock;

#endif