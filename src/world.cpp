#include "world.hpp"
#include "entity.hpp"

namespace NM {
    Component& World::getUnsafe(std::type_index idx, int it) {
        if(idx == typeid(PhysicsBody)) {
            return physics.getForId(it);
        }
        else {
            throw std::runtime_error("No such component!");
        }
    }
    
    ComponentRef World::wrap(std::type_index idx, int id) {
        return {idx, id, this};
    }
    
    World::World() : physics(this) {}
    
    EntityRef World::createEntity() {
        int id = entities.size();
        entities.emplace_back(this, id);
        return EntityRef(this, id);
    }
    
    Entity& World::getEntity(int id) {
        return entities.at(id);
    }
    
    const Entity& World::getEntity(int id) const {
        return entities.at(id);
    }
    
}
