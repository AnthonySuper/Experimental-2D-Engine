#ifndef _NM_PHYSICS_WORLD_HPP
#define _NM_PHYSICS_WORLD_HPP

#include "physics_body.hpp"
#include "vector.hpp"
#include <vector>
#include <set>

namespace NM {
    class world;
    
    class PhysicsWorld {
    public:
        PhysicsWorld(World *world);
        
        template<typename ...Args>
        int constructMember(Args... args) {
            bodies.emplace_back(std::forward<Args>(args)...);
            return bodies.size() - 1;
        }
        
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
