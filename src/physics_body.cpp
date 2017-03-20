#include "physics_body.hpp"

namespace NM {
    PhysicsBody::PhysicsBody(double mass, Polygon polygon) :
    mass(mass), outline(polygon) {
        
    }
    
    void PhysicsBody::receive(NM::Message &m) {
        // do nothing!
    }
}
