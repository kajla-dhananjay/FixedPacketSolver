#include <bits/stdc++.h>
#include "NaivePLW.h"
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

  solver(in_graph, b, out_coord, out_logs, d, e);

}
