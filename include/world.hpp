#ifndef _NM_WORLD_HPP
#define _NM_WORLD_HPP

#include "physics_world.hpp"
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
        using component_physics = typename std::enable_if<std::is_same<Return, PhysicsBody>::value, ComponentRef>::type;
        
        template<typename Return, typename ...Args>
        component_physics<Return> create(Args ...args) {
            int id = physics.constructMember(std::forward<Args>(args)...);
            return ComponentRef{typeid(PhysicsBody), id, this};
        }
        
        template<typename Return>
        component_physics<Return> create(double mass,
                                      std::initializer_list<Vector> polygon) {
            int id = physics.constructMember(mass, polygon);
            return ComponentRef{typeid(PhysicsBody), id, this};
        }
        
        ComponentRef wrap(std::type_index idx, int id);
        
        EntityRef createEntity();
        
        Entity& getEntity(int id);
        
        const Entity& getEntity(int id) const;
        
        Component& getUnsafe(std::type_index idx, int id);
     
    private:
        PhysicsWorld physics;
        std::vector<NM::Entity> entities;
    };
    
    
}

#endif
