#include "position.hpp"

namespace NM {
    Position::Position(int64_t x, int64_t y) {
        this->x = x;
        this->y = y;
    }
    
    Position::Position() {
        x = 0;
        y = 0;
    }
    
    double Position::absoluteDistance(const NM::Position &other) const {
        double xdist = x - other.x;
        double ydist = y - other.y;
        return sqrt(xdist * xdist + ydist * ydist);
    }
    
    Position::Position(const Position &other) {
        x = other.x;
        y = other.y;
    }
    
    bool Position::operator==(const NM::Position &o) {
        return (x == o.x) && (y == o.y);
    }
    
    bool Position::operator!=(const NM::Position &o) {
        return ! (*this == o);
    }
    
    std::string inspect(const NM::Position &p) {
        std::stringstream s;
        s << "Position: (" << p.x << ", " << p.y << ")";
        return s.str();
    }
    
    
}