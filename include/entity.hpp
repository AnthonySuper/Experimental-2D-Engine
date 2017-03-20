#ifndef _NM_ENTITY_HPP
#define _NM_ENTITY_HPP

#include "component_ref.hpp"
#include "message.hpp"
#include "world.hpp"
#include <vector>

namespace NM {
    class Entity {
    public:
        Entity(World *world, int id);
        void addComponent(ComponentRef &c);
        void broadcast(Message &m);
        
        ComponentRef addPhysicsBody();
        
        template<typename T>
        typename std::enable_if<std::is_base_of<Component, T>::value, T&>::type getComponent() {
            for(auto c: components) {
                if(c.is<T>()) {
                    return c.get<T>();
                }
            }
            throw std::runtime_error("No component of that type!");
        }
        
        template<typename T>
        typename std::enable_if<is_component_v<T>, ComponentRef>::type createComponent();
        
        
        template<typename T>
        typename std::enable_if<std::is_base_of<Component, T>::value,
        std::vector<T*>>::type getComponents() {
            std::vector<T*> comps;
            for(auto &c: components) {
                if(c.is<T>()) {
                    comps.emplace_back(&c);
                }
            }
            return comps;
        }
        
        EntityRef makeRef();
        
        
    protected:
        std::vector<ComponentRef> components;
        World *world;
        const int id;
    };
    
    template<>
    inline ComponentRef Entity::createComponent<PhysicsBody>() {
        return addPhysicsBody();
    }
}

#endif
