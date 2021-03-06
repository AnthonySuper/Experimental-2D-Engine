#include "polygon.hpp"
#include <iostream>
#include <limits>
#include <tuple>

namespace NM {
    Polygon::Polygon(std::initializer_list<Point> _points) {
        points = std::vector<Point>(_points);
        calcCentroid();
    }
    
    Polygon::Polygon(std::vector<Point> ps) {
        points = ps;
        calcCentroid();
    }
    
    Polygon::Polygon(const Polygon &other) {
        points = other.points;
        centroid = other.centroid;
    }
    
    void Polygon::translateVia(const NM::Vector &v) {
        for(auto p: points) {
            p.x += v.x;
            p.y += v.y;
        }
        // Also move the origin with respect to the translation
        centroid.x += v.x;
        centroid.y += v.y;
        boundingBox.topLeft.x += v.x;
        boundingBox.topLeft.y += v.y;
        boundingBox.bottomRight.x += v.x;
        boundingBox.bottomRight.y += v.y;
    }
    
    /*
     Formula generously provided by wikipedia
     https://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
     */
    void Polygon::calcCentroid() {
        Point topLeft = points[0];
        Point bottomRight = points[0];
        float area = 0;
        float cx = 0;
        float cy = 0;
        for(int i = 0; i < points.size(); i++) {
            auto first = points.at(i);
            // Loop around
            auto second = points.at((i == points.size() - 1) ? 0 : i + 1);
            // x_i y_{i+1} - x_{i + 1} y_{i}
            float abit = (first.x * second.y - second.x * first.y);
            area += abit;
            cx += (first.x + second.x) * abit;
            cy += (first.y + second.y) * abit;
            topLeft = {std::min(topLeft.x, first.x),
                std::min(topLeft.y, first.y)};
            bottomRight = {std::max(bottomRight.x, first.x),
                std::max(bottomRight.y, first.y)};
        }
        area /= 2;
        cx /= (area * 6);
        cy /= (area * 6);
        centroid = {cx, cy};
        boundingBox = {topLeft, bottomRight};
    }
    
    
    CollisionResult Polygon::preciseCollision(const NM::Polygon &other) const {
        if(other.points.size() < points.size()) {
            return other.preciseCollision(*this);
        }
        double minOverlap = std::numeric_limits<double>::max();
        Vector mtv = {0, 0};
        size_t size = points.size();
        for(int i = 0; i < size; ++i) {
            const Point &first = points[i];
            const Point &second = (i == (size - 1)) ? points[0] : points[i + 1];
            Vector axis = (first - second);
            auto ourPoints = minAndMaxScalarProjection(axis.unitVector());
            auto theirPoints = other.minAndMaxScalarProjection(axis.unitVector());
            double ourMin, ourMax, theirMin, theirMax;
            std::tie(ourMin, ourMax) = ourPoints;
            std::tie(theirMin, theirMax) = theirPoints;
            if(ourMax < theirMin || theirMax < ourMin) {
                return CollisionResult();
            }
            else {
                double ol = std::min(ourMax, theirMax) - std::max(ourMin, theirMin);
                if(ol < minOverlap) {
                    minOverlap = ol;
                    mtv = axis.unitVector();
                }
            }
        }
        return CollisionResult(mtv * minOverlap);
    }
    
    // TODO: Optimize this
    std::tuple<double, double> Polygon::minAndMaxScalarProjection(NM::Vector line) const {
        auto normal = line.unitVector();
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::min();
        for(auto &point: points) {
            double r = point.dotProduct(normal);
            min = std::min(min, r);
            max = std::max(max, r);
        }
        return std::make_tuple(min, max);
    }

}
