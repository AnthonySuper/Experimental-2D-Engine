//
//  mrb_wrapper.hpp
//  nmtt
//
//  Created by Anthony Super on 5/16/16.
//
//

#ifndef mrb_wrapper_h
#define mrb_wrapper_h

#include <mruby.h>
#include <mruby/data.h>

namespace NM {
    template<typename T>
    void mrb_destructor_value(mrb_state *mrb, void *self) {
        T* type = reinterpret_cast<T*>(self);
        delete type;
    }
    
    /**
     Determine if a given type is an mrb_data_type
     */
    template<typename T>
    struct is_mrb_data_type {
        constexpr static bool value = std::is_same<const struct mrb_data_type, T>::value;
    };
    
    /**
     Type trait to determine if a given type can be shared with MRB.
     */
    template<typename T, typename Enable = void>
    struct is_mrb_shared {
        constexpr static bool value = false;
    };
    
    template<typename T>
    struct is_mrb_shared<T,
    typename std::enable_if<is_mrb_data_type<decltype(T::mrb_type)>::value>::type> {
        constexpr static bool value = true;
    };
    
    
}

#endif /* mrb_wrapper_h */
