#ifndef _NM_WORLD_HPP
#define _NM_WORLD_HPP

#include "physics_system.hpp"
#include "component_ref.hpp"
#include "entity.hpp"
#include <typeinfo>
#include <string>
#include <typeindex>

namespace NM {
    
    class World {
    public:
        World();
        
        template<typename Return>
        ComponentRef createComponent(EntityRef entity);
        
        ComponentRef wrap(std::type_index idx, int id);
        
        EntityRef createEntity();
        
        Entity& getEntity(int id);
        
        const Entity& getEntity(int id) const;
        
        Component& getUnsafe(std::type_index idx, int id);
     
    private:
        PhysicsSystem physics;
        std::vector<Entity> entities;
        
    };
    
    template<>
    inline ComponentRef World::createComponent<PhysicsBody>(EntityRef entity) {
        auto c = physics.createChild(entity);
        return c;
    }
    
}

#endif
