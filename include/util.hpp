#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <vector>
#include <string>
#include <fstream>
#include "errors.hpp"

namespace NM {
    namespace Util {
        long factorial(long);
        std::vector<char> readFile(std::string pathname);
        std::string readFileString(std::string pathname);
    }
}


#endif