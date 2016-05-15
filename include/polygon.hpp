#ifndef _POLYGON_HPP
#define _POLYGON_HPP
#include "vector.hpp"
#include <vector>
#include <initializer_list>
#include "collision_result.hpp"

namespace NM {
    typedef Vector Point;
    
    class Polygon {
    public:
        Polygon(std::initializer_list<Point>);
        
        CollisionResult preciseCollision(const Polygon &other);
        
        void translateVia(const Vector &v);
        
        void translateAlong(const Vector &v, double dist) {
            translateVia(v.unitVector() * dist);
        }
        
        const Point& getCentroid() const {
            return centroid;
        }
        
    protected:
        /**
         Perform the scalar projection of each point in this vecot onto the axis, and return the extremes
         @param line the line to project on
         @return (min, max)
         */
        std::tuple<double, double> minAndMaxScalarProjection(NM::Vector line) const;
        /**
          * Calculate the centeroid of the set of points. Called after construction.
         */
        
        void calcCentroid();
        
    private:
        std::vector<Point> points;
        Point centroid;
    };
}
#endif