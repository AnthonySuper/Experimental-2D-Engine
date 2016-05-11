#include "Vector.hpp"

namespace NM {
    Vector::Vector(int64_t x, int64_t y) {
        this->x = x;
        this->y = y;
    }
    
    Vector::Vector() {
        x = 0;
        y = 0;
    }
    
    double Vector::absoluteDistance(const NM::Vector &other) const {
        double xdist = x - other.x;
        double ydist = y - other.y;
        return sqrt(xdist * xdist + ydist * ydist);
    }
    
    Vector::Vector(const Vector &other) {
        x = other.x;
        y = other.y;
    }
    
    bool Vector::operator==(const NM::Vector &o) {
        return (x == o.x) && (y == o.y);
    }
    
    bool Vector::operator!=(const NM::Vector &o) {
        return ! (*this == o);
    }
    
    std::string inspect(const NM::Vector &p) {
        std::stringstream s;
        s << "Vector: (" << p.x << ", " << p.y << ")";
        return s.str();
    }
    
    double Vector::dotProduct(const NM::Vector &o) const {
        return (x * o.x + y * o.y);
    }
    
    Vector Vector::unitVector() const {
        return (1.0/sqrt(dotProduct(*this))) * (*this);
    }
    
}