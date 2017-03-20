#ifndef _NM_MESSAGE_HPP_
#define _NM_MESSAGE_HPP_

#include <string>
#include <typeinfo>
#include <type_traits>

namespace NM {
    class Message {
    protected:
        std::string messageType;
        
        template<typename T>
        typename std::enable_if<std::is_base_of<Message, T>::value, T&>::type cast();
        
    public:
        std::string getType() const;
        Message(std::string type);
        
    };
    
    struct BadMessageCast : std::runtime_error {
        BadMessageCast(std::string msg) : runtime_error(msg) {}
    };
    
    template<typename T>
    typename std::enable_if<std::is_base_of<Message,
                                            T>::value, T&>::type
    Message::cast() {
        if(typeid(T) == typeid(*this)) {
            return static_cast<T&>(this);
        }
        else {
            throw BadMessageCast("Could not convert types!");
        }
    }
}

#endif
