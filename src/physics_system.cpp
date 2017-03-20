#include "physics_system.hpp"
#include "world.hpp"

namespace NM {
    
    void PhysicsSystem::update() {
        // do some updating or something
    }
    
    void PhysicsSystem::calcCollisions() {
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
    
    PhysicsSystem::PhysicsSystem(World *world) : world(world) {}
    
    ComponentRef PhysicsSystem::createChild(EntityRef ref) {
        int id = bodies.size();
        bodies.emplace_back(ref);
        return {typeid(PhysicsBody), id, world};
    }
    
}
