#ifndef _PHYSICS_BODY_HPP
#define _PHYSICS_BODY_HPP

#include "polygon.hpp"
#include "component.hpp"
#include "message.hpp"
#include "component_ref.hpp"

namespace NM {
    class PhysicsBody : public Component {
    public:
        PhysicsBody(double mass, Polygon polygon);
        std::string getName() override {
            return "PhysicsBody";
        }
        
        inline double getMass() const {
            return mass;
        }
        
        inline const Polygon& getOutline() const {
            return outline;
        }
        
        inline Polygon& getOutline() {
            return outline;
        }
        
    protected:
        double mass;
        Polygon outline;
    };
   
    class CollisionMessage : public Message {
    public:
        CollisionMessage(CollisionResult r, ComponentRef other) :
         Message("object_collided"), otherBody(other), res(r) {
            
        }
        
        ComponentRef otherBody;
        CollisionResult res;
    };
}


#endif
