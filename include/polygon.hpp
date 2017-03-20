#ifndef _POLYGON_HPP
#define _POLYGON_HPP
#include "vector.hpp"
#include <initializer_list>
#include <vector>
#include <SDL.h>
#include "collision_result.hpp"

namespace NM {
    
    
    class Polygon {
        using Point = Vector;
    public:
        Polygon(std::initializer_list<Point>);
        
        Polygon(std::vector<Point>);
        
        Polygon(const Polygon&);
        
        CollisionResult preciseCollision(const Polygon &other) const;
        
        void translateVia(const Vector &v);
        
        void translateAlong(const Vector &v, double dist) {
            translateVia(v.unitVector() * dist);
        }
        
        const Point& getCentroid() const {
            return centroid;
        }
        
        const std::vector<Point>& getPoints() const {
            return points;
        }
        
        struct AABB {
            Point topLeft;
            Point bottomRight;
        };
        
        const AABB& getBoundingBox() const {
            return boundingBox;
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
        
        AABB boundingBox;
        
    private:
        std::vector<Point> points;
        Point centroid;
    };
}
#endif
