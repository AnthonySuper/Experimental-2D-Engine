#ifndef _POLYGON_HPP
#define _POLYGON_HPP
#include "vector.hpp"
#include <vector>
#include <initializer_list>
#include <experimental/optional>

namespace NM {
    typedef Vector Point;
    
    class Polygon {
    public:
        Polygon(std::initializer_list<Point>);
        bool preciseCollision(const Polygon &other);
        
    protected:
        std::vector<Point> points;
        /**
         Perform the scalar projection of each point in this vecot onto the axis, and return the extremes
         @param axis the axis to project onto. MUST be a unit vector for optimization reasons.
         @return (min, max)
         */
        std::tuple<double, double> minAndMaxScalarProjection(NM::Vector axis) const;
    };
}
#endif