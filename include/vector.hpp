#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <stdint.h>
#include <cmath>
#include <sstream>

namespace NM {
    class Vector {
        
    public:
        /**
         * x coordinate of this Vector.
         *
         * Vectors are defined such that the top-left corner of the grid is (0,0).
         * The X axis runs from left to right, increasing as you move right.
         */
        int64_t x;
        
        /**
         * y coordinate of this Vector.
         *
         * Vectors are defined such that the top-right corner of the grid is (0,0).
         * The Y axis runs up and down, increasing as you move downwards.
         */
        int64_t y;
        
        /**
         * Basic construction of a Vector with x and y coordinates
         */
        Vector(int64_t x, int64_t y);
        
        /**
         * Default constructor sets both values to 0
         */
        Vector();
        
        /**
         * Copy constructor
         */
        Vector(const Vector&);
        
        /**
         * Assignment Operator just uses the default
         */
        Vector& operator=(const Vector &) = default;
        
        bool operator==(const Vector &);
        
        bool operator!=(const Vector &);
        
        double dotProduct(const Vector &) const;
        
        /**
         Get the absolute pythagorean distance from one Vector to another.
         @param other
                The other Vector to find the absolute distance to
         
         @return the distance
         */
        double absoluteDistance(const Vector &other) const;
        
        Vector unitVector() const;
    };
    
    template<typename T>
    inline NM::Vector operator*(T o, NM::Vector p) {
        static_assert(std::is_arithmetic<T>::value,
                      "Can only multiply by scalar values");
        return Vector(o * p.x, o * p.y);
    }
    
    template<typename T>
    inline NM::Vector operator*(NM::Vector p, T o) {
        return o * p;
    }
    
    inline NM::Vector operator-(NM::Vector f, NM::Vector s) {
        return NM::Vector(f.x - s.x, f.y - s.y);
    }
    
    
    std::string inspect(const NM::Vector& p);
}


#endif