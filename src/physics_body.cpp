#include "physics_body.hpp"

namespace NM {
    PhysicsBody::PhysicsBody() :
    PhysicsBody(EntityRef(nullptr, -1))
    {}
    
    
    PhysicsBody::PhysicsBody(EntityRef e) :
    Component(e), mass(0), outline{{0, 0}, {0, 1}, {1, 0}}
    {}
    
    
    
    void PhysicsBody::receive(NM::Message &m) {
        // do nothing!
    }
}
