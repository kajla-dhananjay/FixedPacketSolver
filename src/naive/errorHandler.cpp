/**
 * @file errorHandler.cpp
 * @author Dhananjay Kajla (kajla.dhananjay@gmail.com)
 * @brief A trivial error handling class
 * @version 1.0
 * @date 2021-2-5
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <bits/stdc++.h>
#include "errorHandler.h"

errorHandler::errorHandler()
{
    std::cerr << "Undefined Error" << std::endl;
    exit(1);
}

errorHandler::errorHandler(std::string s)
{
    std::cerr << "Error: " << s << std::endl;
    exit(1);
}

errorHandler::errorHandler(std::string s, int x)
{
    std::cerr << "Error: " << s << std::endl;
    exit(x);
}