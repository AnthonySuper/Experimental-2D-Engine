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
}