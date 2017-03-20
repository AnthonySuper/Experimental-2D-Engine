#include "component.hpp"

namespace NM {
    Component::Component(EntityRef entity) : parentEntity(entity) {}
    
    Component::Component() : parentEntity(nullptr, -1) {}
    
    Entity& Component::getEntity() {
        return parentEntity.get();
    }
    
    
    void NullComponent::receive(NM::Message &m) {
        std::cerr << "NullComponent recieved message: ";
        std::cerr << m.getType() << std::endl;
    }
    
    NullComponent::operator bool() {
        return false;
    }
    
    std::string NullComponent::getName() {
        return "NullComponent";
    }
}
