#include "entity.hpp"

namespace NM {
    Entity::Entity(World *w, int id) :
    world(w),
    id(id) {}
    
    void Entity::broadcast(Message &m) {
        // right now we notify all components, will likely change soon
        for(ComponentRef &cr : components) {
            cr.getUnsafe().receive(m);
        }
    }
    
    EntityRef Entity::makeRef() {
        return {world, id};
    }
    
    void Entity::addComponent(ComponentRef &c) {
        components.emplace_back(c);
    }
    
    ComponentRef Entity::addPhysicsBody() {
        auto c = world->createComponent<PhysicsBody>(this->makeRef());
        addComponent(c);
        return c;
    }
}

