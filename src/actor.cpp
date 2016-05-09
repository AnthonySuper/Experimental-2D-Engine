#include "actor.hpp"

namespace NM {
    bool Actor::visibleBy(const NM::Player &p) const {
        return (visibilityMask & p.bitmask).any();
    }
    
    bool Actor::controllableBy(const NM::Player &p) const {
        return (controlMask & p.bitmask).any();
    }
}