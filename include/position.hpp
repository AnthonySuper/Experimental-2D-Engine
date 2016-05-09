#ifndef _POSITION_HPP
#define _POSITION_HPP

#include <stdint.h>
#include <cmath>
#include <sstream>

namespace NM {
    class Position {
        
    public:
        /**
         * x coordinate of this position.
         *
         * Positions are defined such that the top-left corner of the grid is (0,0).
         * The X axis runs from left to right, increasing as you move right.
         */
        int64_t x;
        
        /**
         * y coordinate of this position.
         *
         * Positions are defined such that the top-right corner of the grid is (0,0).
         * The Y axis runs up and down, increasing as you move downwards.
         */
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
        
        /**
         Get the absolute pythagorean distance from one position to another.
         @param other
                The other position to find the absolute distance to
         
         @return the distance
         */
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
    
    std::string inspect(const NM::Position& p);
}


#endif