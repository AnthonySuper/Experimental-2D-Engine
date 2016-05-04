#ifndef _POSITION_HPP
#define _POSITION_HPP

#include <stdint.h>

namespace NM {
    class Position {
        
    public:
        int64_t x;
        int64_t y;
        /**
         * Basic construction of a position with x and y coordinates
         */
        Position(int64_t x, int64_t y);
        /**
         * Default constructor sets both values to 0
         */
        Position();
    };
}
#endif