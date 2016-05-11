#include "polygon.hpp"

namespace NM {
    Polygon::Polygon(std::initializer_list<Point> _points) {
        points = std::vector<Point>(_points);
    }
    
    bool Polygon::preciseCollision(const NM::Polygon &other) {
        for(int i = 0; i < points.size(); ++i) {
            Point &first = points.at(i);
            Point &second = (i == (points.size() - 1)) ? points.at(0) : points.at(i + 1);
            Vector axis = (first - second).unitVector();
            auto ourPoints = minAndMaxScalarProjection(axis);
            auto theirPoints = other.minAndMaxScalarProjection(axis);
            double ourMin, ourMax, theirMin, theirMax;
            std::tie(ourMin, ourMax) = ourPoints;
            std::tie(theirMin, theirMax) = theirPoints;
            if(ourMax < theirMin || theirMax < ourMin) {
                return false;
            }
        }
        return true;
    }
    
    std::tuple<double, double> Polygon::minAndMaxScalarProjection(NM::Vector axis) const {
        double a = points[0].dotProduct(axis);
        double b = points[1].dotProduct(axis);
        double min = std::min(a, b);
        double max = std::max(a, b);
        for(int i = 2; i < points.size(); ++i) {
            double r = points[i].dotProduct(axis);
            min = std::min(min, r);
            max = std::max(max, r);
        }
        return {min, max};
    }
    
}