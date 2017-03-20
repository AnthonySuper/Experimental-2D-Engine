#include "message.hpp"

namespace NM {
    std::string Message::getType() const {
        return messageType;
    }
    
    Message::Message(std::string type) :
    messageType(type) {
        
    }
}
