#ifndef _POSITION_HPP
#define _POSITION_HPP

#include <stdint.h>
#include <cmath>

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
        
        /**
         * Copy constructor
         */
        Position(const Position&);
        
        /**
         * Assignment Operator just uses the default
         */
        Position& operator=(const Position &) = default;
        
        bool operator==(const Position &);
        
        bool operator!=(const Position &);
        
        double absoluteDistance(const Position &other) const;
    };
    
    template<typename T>
    NM::Position operator*(T o, NM::Position p) {
        static_assert(std::is_arithmetic<T>::value,
                      "Can only multiply by scalar values");
        return Position(o * p.x, o * p.y);
    }
    
    template<typename T>
    NM::Position operator*(NM::Position p, T o) {
        return o * p;
    }
}


#endif