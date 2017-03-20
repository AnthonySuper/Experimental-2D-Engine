#include "entity_ref.hpp"
#include "world.hpp"

namespace NM {
    Entity& EntityRef::get() {
        if(checkNulls) {
            if(world) {
                return world->getEntity(id);
            }
            else {
                throw std::runtime_error("Null world deref!");
            }
        }
        else {
            return world->getEntity(id);
        }
    }
    
    EntityRef::operator bool() const {
        return world != nullptr && id != -1;
    }
    
    bool EntityRef::operator==(const EntityRef &other) const {
        return other.world == world && other.id == id;
    }
    
    EntityRef::operator Entity&() {
        return get();
    }
}
