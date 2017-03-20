#ifndef _NM_COMPONENT_HPP
#define _NM_COMPONENT_HPP

#include "message.hpp"
#include "entity_ref.hpp"
#include <iostream>


namespace NM {
    
    
    class Component {
    public:
        Component(EntityRef entity);
        Component();
        virtual std::string getName() = 0;
        // this is a real component
        virtual operator bool() { return true; }
        virtual void receive(Message& m) = 0;
        Entity& getEntity();
        EntityRef parentEntity;
    };
    
    class NullComponent : public Component {
    public:
        virtual std::string getName() override;
        virtual operator bool() override;
        
        virtual void receive(Message& m) override;
    };
    
    template<typename T>
    constexpr bool is_component_v = std::is_base_of<Component, T>::value;
    
}

#endif
