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
        double x;
        
        /**
         * y coordinate of this Vector.
         *
         * Vectors are defined such that the top-right corner of the grid is (0,0).
         * The Y axis runs up and down, increasing as you move downwards.
         */
        double y;
        
        /**
         * Basic construction of a Vector with x and y coordinates
         */
        Vector(double x, double y);
        
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
        
        /**
         Determine if a vector is equal to another by comparing their components.
         */
        bool operator==(const Vector &);
        
        /**
         Determine if two vectors are not equal
         */
        bool operator!=(const Vector &);
        
        /**
         Determine the inner product of this vector and another vector.
         */
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
    
    /**
     Provide scalar multiplication of vectors
     */
    template<typename T>
    inline NM::Vector operator*(T o, NM::Vector p) {
        static_assert(std::is_arithmetic<T>::value,
                      "Can only multiply by scalar values");
        return Vector(o * p.x, o * p.y);
    }
    
    /**
     Scalar multiplication should work for either order of arguments
     (scalar * vector) or (vector * scalar) so we provide this.
     */
    template<typename T>
    inline NM::Vector operator*(NM::Vector p, T o) {
        return o * p;
    }
    
    inline NM::Vector operator-(NM::Vector f, NM::Vector s) {
        return NM::Vector(f.x - s.x, f.y - s.y);
    }
    
    inline NM::Vector operator+(const NM::Vector &v, const NM::Vector &s) {
        return NM::Vector(v.x + s.x, v.y + s.y);
    }
    
    
    std::string inspect(const NM::Vector& p);
}


#endif