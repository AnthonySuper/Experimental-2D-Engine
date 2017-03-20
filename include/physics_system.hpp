#ifndef _NM_PHYSICS_SYSTEM_HPP
#define _NM_PHYSICS_SYSTEM_HPP

#include "physics_body.hpp"
#include "vector.hpp"
#include <vector>
#include <set>

namespace NM {
    class world;
    
    class PhysicsSystem {
    public:
        PhysicsSystem(World *world);
        
        ComponentRef createChild(EntityRef ref);
        
        PhysicsBody& getForId(int id) {
            return bodies.at(id);
        }
        
        std::vector<PhysicsBody> bodies;
        
        void update();
        
    private:
        void calcCollisions();
        World * const world;
    };
}

#endif
