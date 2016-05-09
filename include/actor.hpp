#ifndef _ACTOR_HPP
#define _ACTOR_HPP
#include "util.hpp"
#include "player.hpp"

namespace NM {
    
    class Actor {
        
    public:
        PlayerBitmask controlMask;
        PlayerBitmask visibilityMask;
        
        bool visibleBy(const Player&) const;
        bool controllableBy(const Player&) const;
    };
}
#endif