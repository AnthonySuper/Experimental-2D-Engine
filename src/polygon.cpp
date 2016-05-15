#include "polygon.hpp"
#include <iostream>

namespace NM {
    Polygon::Polygon(std::initializer_list<Point> _points) {
        points = std::vector<Point>(_points);
        calcCentroid();
    }
    
    void Polygon::translateVia(const NM::Vector &v) {
        
        for(auto p: points) {
            p.x += v.x;
            p.y += v.y;
        }
        // Also move the origin with respect to the translation
        centroid.x += v.x;
        centroid.y += v.y;
    }
    
    /*
     Formula generously provided by wikipedia
     https://en.wikipedia.org/wiki/Centroid#Centroid_of_polygon
     */
    void Polygon::calcCentroid() {
        double area = 0;
        double cx = 0;
        double cy = 0;
        for(int i = 0; i < points.size(); i++) {
            auto first = points.at(i);
            // Loop around
            auto second = points.at((i == points.size() - 1) ? 0 : i + 1);
            // x_i y_{i+1} - x_{i + 1} y_{i}
            double abit = (first.x * second.y - second.x * first.y);
            area += abit;
            cx += (first.x + second.x) * abit;
            cy += (first.y + second.y) * abit;
        }
        area /= 2;
        cx /= (area * 6);
        cy /= (area * 6);
        centroid = {cx, cy};
    }
    
    
    CollisionResult Polygon::preciseCollision(const NM::Polygon &other) {
        double minOverlap = std::numeric_limits<double>::max();
        Vector mtv = {0, 0};
        for(int i = 0; i < points.size(); ++i) {
            Point &first = points.at(i);
            Point &second = (i == (points.size() - 1)) ? points.at(0) : points.at(i + 1);
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
        double a = points[0].dotProduct(normal);
        double b = points[1].dotProduct(normal);
        double min = std::min(a, b);
        double max = std::max(a, b);
        for(int i = 2; i < points.size(); ++i) {
            double r = points[i].dotProduct(normal);
            min = std::min(min, r);
            max = std::max(max, r);
        }
        return {min, max};
    }
    
}