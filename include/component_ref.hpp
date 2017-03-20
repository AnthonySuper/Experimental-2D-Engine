#ifndef _NM_COMPONENT_REF_HPP
#define _NM_COMPONENT_REF_HPP

#include <typeindex>
#include <stdexcept>
#include <string>

#include "component.hpp"

namespace NM {
    class World;
    
    class ComponentRef {
    protected:
        const std::type_index type;
        int idx;
        World *world;
        
    public:
        template<typename T>
        T& get();
        
        Component& getUnsafe();
        
        template<typename T>
        bool is();
        
        ComponentRef(const std::type_index, int, World*);
        
    };
    
    struct ComponentCastError : std::runtime_error {
        ComponentCastError(std::string message) :
        runtime_error(message) {}
    };
    
    template<typename T>
    T& ComponentRef::get() {
        if(is<T>()) {
            return static_cast<T&>(getUnsafe());
        }
        else {
            throw ComponentCastError(std::string(type.name()) + " cannot convert to " + typeid(T).name());
        }
    }
    
    template<typename T>
    bool ComponentRef::is() {
        return type == typeid(T);
    }
}


#endif
