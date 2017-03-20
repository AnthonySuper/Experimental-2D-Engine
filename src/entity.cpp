#include "entity.hpp"

namespace NM {
    Entity::Entity(World &w) : world(w) {}
    
    void Entity::broadcast(NM::Message &m) {
        // right now we notify all components, will likely change soon
        for(ComponentRef &cr : components) {
            cr.getUnsafe().receive(m);
        }
    }
    
    void Entity::addComponent(NM::ComponentRef &c) {
        
    }
}
