#include "util.hpp"

namespace NM{
    namespace Util {
        long factorial(long in, long acc) {
            if(in == 1) {
                return acc;
            }
            return factorial(in - 1, acc * in);
        }
        
        long factorial(long in){
            return factorial(in, 1);
        }
        
        std::vector<char> readFile(std::string pathname) {
            using isitr = std::istreambuf_iterator<char>;
            std::ifstream st(pathname.c_str(), std::ios::binary);
            st >> std::noskipws;
            if(! st.is_open()) {
                throw FileNotFoundError(pathname);
            }
            std::vector<char> vec;
            std::copy(isitr(st), isitr(), std::back_inserter(vec));
            return vec;
        }
        
        std::string readFileString(std::string pathname) {
            auto c = readFile(pathname);
            return std::string(c.begin(), c.end());
        }
    }
}
