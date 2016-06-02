#ifndef _ACTOR_HPP
#define _ACTOR_HPP

#include <mruby.h>
#include <SDL.h>
#include <stdexcept>
#include <vector>
#include <mrb_wrapper.hpp>
#include <bitset>

namespace NM {
    class Actor {
        
    public:
        class NoExistingBehaviorClassError : std::runtime_error {
        public:
            NoExistingBehaviorClassError(std::string name) :
            std::runtime_error("Could not find ruby class named " + name) {}
        };
        
        Actor(mrb_state *mrb,
              std::string behavior_class_name);
        
        /**
         A ruby class which models this actor's behavior
         
         This must be a sub-class (in ruby) of the Actor class, so that
         methods defined on all actors can be called on this specialization.
         */
        RClass *behaviorClass;
        
        mrb_value _self;
        
        const static struct mrb_data_type mrb_type;
        
    private:
        Actor(const Actor&) = delete;
        
    };
    
    namespace mrb {
        mrb_value to_value(mrb_state *, NM::Actor *);
    }
}
#endif