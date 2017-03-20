#ifndef _NM_ENTITY_HPP
#define _NM_ENTITY_HPP

#include "component_ref.hpp"
#include "message.hpp"
#include <vector>

namespace NM {
    
    class World;
    
    class Entity {
        
    public:
        Entity(World &w);
        void addComponent(ComponentRef &c);
        void broadcast(Message &m);
        
        
    protected:
        std::vector<ComponentRef> components;
        World &world;
    };
    
    class EntityRef {
    friend class World;
        EntityRef(World &w, int id) : world(w), id(id) {}
        
        World &world;
        const int id;
        
    public:
        Entity& get();
    };
    
}

#endif
