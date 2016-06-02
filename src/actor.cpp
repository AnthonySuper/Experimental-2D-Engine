#include "actor.hpp"

namespace NM {
    
    namespace mrb {
        mrb_value to_value(mrb_state *mrb, NM::Actor *actor) {
            return actor->_self;
        }
    }
    
    Actor::Actor(mrb_state *mrb,
                 std::string behavior_class_name) {
        const mrb_data_type *type = &mrb_type;
        behaviorClass = mrb_class_get(mrb, behavior_class_name.c_str());
        if(behaviorClass == nullptr) {
            throw NoExistingBehaviorClassError(behavior_class_name);
        }
        _self = mrb_obj_value(Data_Wrap_Struct(mrb,
                                               behaviorClass,
                                               type, (void *)this));
    }
    
    const struct mrb_data_type Actor::mrb_type = {"Actor", &NM::mrb::destructor_noop};
    
    
}