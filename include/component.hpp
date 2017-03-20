#ifndef _NM_COMPONENT_HPP
#define _NM_COMPONENT_HPP

#include "message.hpp"
#include "entity.hpp"
#include <iostream>

namespace NM {
    class Component {
    public:
        virtual std::string getName() = 0;
        // this is a real component
        virtual operator bool() { return true; }
        // by default, we don't respond to messages
        virtual void receive(Message& m) {}
        
        inline void attachTo(EntityRef entity) {
            parent = entity;
        }
        
        inline Entity& getEntity() {
            return parent.get();
        }
        
    protected:
        EntityRef parent;
    };
    
    class NullComponent : public Component {
    public:
        virtual std::string getName() override { return "NullComponent"; }
        virtual operator bool() override { return false; }
        
        static NullComponent& getInstance() {
            static NullComponent c;
            return c;
        }
        
        virtual void receive(Message& m) override {
            std::cerr << "NullComponent recieved message: ";
            std::cerr << m.getType() << std::endl;
        }
    };
}

#endif
