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

// Compilation Flag Macros : TIMER, DEBUG, PARALLEL, STEPBYSTEP

/************************* Custom libraries Import **************************/

#include "bootstrap.h"
#include "chain.h"
#include "channel.h"
#include "checkers.h"
#include "declarations.h"
#include "distSelector.h"
#include "end.h"
#include "errorHandler.h"
#include "indexedSet.h"
#include "initialization.h"
#include "linalg.h"
#include "serialChain.h"

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

  init();

  runChain();

  end();

}
