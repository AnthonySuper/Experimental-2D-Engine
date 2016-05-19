#ifndef _VECTOR_HPP
#define _VECTOR_HPP

#include <stdint.h>
#include <cmath>
#include <sstream>
#include <mrb_wrapper.hpp>

namespace NM {
    
    /**
     @breif A class representing a vector in 2D space
     
     @discussion This type represents either a vector or a point in space, and is
     used heavily throughout the rest of the engine. It has two members, X and Y.
     Y runs vertically on a screen, X horizontally.
     The top-left corner of the coordinate grid is defined as (0, 0).
     Y increases as you move downwards on the grid, and X increases as you move right.
     This is a vertical flip of the typical coordinate system, mostly because SDL uses the same thing.
     */
    class Vector {
        
        
    public:
        /**
         Funciton to bind this class into an instance of MRB.
         */
        static void bindMRB(mrb_state *mrb);
        
        /**
         Static type information that allows us to use this type from Ruby.
         */
        const static struct mrb_data_type mrb_type;
        
        /**
         * x coordinate of this Vector.
         *
         * Vectors are defined such that the top-left corner of the grid is (0,0).
         * The X axis runs from left to right, increasing as you move right.
         */
        double x;
        

        
        /**
         * @breif y coordinate of this Vector.
         *
         * @remark Vectors are defined such that the top-right corner of the grid is (0,0).
         * The Y axis runs up and down, increasing as you move downwards.
         */
        double y;
        
        
        /**
         @breif Basic construction of a Vector with x and y coordinates
         @param x The x value of the new vector
         @param y The y value of the new vector
         */
        Vector(double x, double y);
        
        /**
         * @brief Default constructor creates the vector {0,0};
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
         @breif Determine if two vectors are not equal
         @see operator==
         */
        bool operator!=(const Vector &);
        
        /**
         Determine the inner product of this vector and another vector.
         */
        double dotProduct(const Vector &o) const;
        
        /**
         @brief Get the absolute distance from one Vector to another.
         
         @remark We define distance in terms of the standard inner product, or geometric distnace.
         
         @param other
                The other Vector to find the absolute distance to
         
         @return the distance
         */
        double absoluteDistance(const Vector &other) const;
        
        /**
         Optain a unit vector in the directio of this vector,
         that is, a vector of length one pointing in the same direction
         as this vector. Useful for all kinds of linear algebra-ey things.
         */
        Vector unitVector() const;
        
    private:
        
        Vector add(const Vector &other) const;
        
        double getY() const { return y; }
        
        double setY(double _y) {
            y = _y;
            return y;
        }
        
        double getX() const { return x; }
        
        double setX(double _x) {
            x = _x;
            return x;
        }
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
    
    inline Vector Vector::add(const Vector& other) const {
        return *this + other;
    }
    
    std::string inspect(const NM::Vector& p);
    
    /*
     --------------
     The following all are declared inline as they are extremely small functions that perform best when inlined.
     I benchmarked this and saw significent speedups when the functions were declared this way as opposed to in a 
     seperate .cpp file. I'm pretty sure that link-time optimization should be inlining these functions anyway
     but it's not doing that, apparently.
     */
    
    inline double Vector::dotProduct(const Vector &o) const {
        return x * o.x + y * o.y;
    }
    
    inline Vector::Vector(double x, double y) {
        this->x = x;
        this->y = y;
    }
    
    inline Vector::Vector() {
        x = 0;
        y = 0;
    }
    
    inline double Vector::absoluteDistance(const NM::Vector &other) const {
        double xdist = x - other.x;
        double ydist = y - other.y;
        return sqrt(xdist * xdist + ydist * ydist);
    }
    
    inline Vector::Vector(const Vector &other) {
        x = other.x;
        y = other.y;
    }
   
    
    inline bool Vector::operator==(const NM::Vector &o) {
        return (x == o.x) && (y == o.y);
    }
    
    inline bool Vector::operator!=(const NM::Vector &o) {
        return ! (*this == o);
    }
    
    inline std::string inspect(const NM::Vector &p) {
        std::stringstream s;
        s << "Vector: (" << p.x << ", " << p.y << ")";
        return s.str();
    }
    
    inline Vector Vector::unitVector() const {
        return (1.0/sqrt(dotProduct(*this))) * (*this);
    }
    
}


#endif
