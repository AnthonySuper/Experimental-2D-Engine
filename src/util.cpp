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
    }
}