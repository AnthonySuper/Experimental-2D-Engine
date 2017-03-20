#include "component_ref.hpp"
#include "world.hpp"


namespace NM {

    ComponentRef::ComponentRef(const std::type_index ti,
                   int idx,
                   World *world) :
    type(ti), idx(idx), world(world) {}
    
    
    Component& ComponentRef::getUnsafe() {
        return world->getUnsafe(type, idx);
    }
    
    
}
