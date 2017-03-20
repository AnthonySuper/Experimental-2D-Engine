#include "physics_world.hpp"
#include "world.hpp"

namespace NM {
    
    void PhysicsWorld::update() {
        // do some updating or something
    }
    
    void PhysicsWorld::calcCollisions() {
        // Right now we have the worst collisions in the world!
        std::vector<CollisionResult> collisions;
        for(int i = 0; i < bodies.size(); ++i) {
            const auto &p = bodies[i].getOutline();
            for(int j = i + 1; i < bodies.size(); ++j) {
                const auto &p2 = bodies[j].getOutline();
                auto c = p.preciseCollision(p2);
                if(c) {
                    
                }
            }
        }
    }
    
    PhysicsWorld::PhysicsWorld(World *world) : world(world) {}
    
}
