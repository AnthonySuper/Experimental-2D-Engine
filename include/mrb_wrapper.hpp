//
//  mrb_wrapper.hpp
//  nmtt
//
//  Created by Anthony Super on 5/16/16.


#ifndef mrb_wrapper_h
#define mrb_wrapper_h
#pragma once

#include <mruby.h>
#include <mruby/data.h>
#include <type_traits>
#include <iostream>

namespace NM::mrb {
    class BadValueConersion : std::runtime_error {
        BadValueConersion(std::string &arg) : std::runtime_error(arg) {};
    };
    
    
    typedef mrb_value (*callable)(mrb_state*, mrb_value);
    
    template<typename T>
    void destructor_value(mrb_state *mrb, void *self) {
        T* type = reinterpret_cast<T*>(self);
        delete type;
    }
    
    namespace traits {
        /**
         Determine if a given type is a struct mrb_data_type
         Pretty much just a helper function
         */
        template<typename T>
        struct is_data_type_struct {
            constexpr static bool value = std::is_same<const struct mrb_data_type, T>::value;
        };
        
        /**
         Type trait which determines if this is a user-defiend object type we can share with MRB.
         */
        template<typename T, typename Enable = void>
        struct is_shared_native {
            constexpr static bool value = false;
        };
        
        template<typename T>
        struct is_shared_native <T,
        typename std::enable_if<is_data_type_struct<decltype(T::mrb_type)>::value>::type> {
            constexpr static bool value = true;
        };
        
        /**
         Type trait which determines if this is any type we can share with MRB, including primitive types
         such as mrb_float, mrb_bool, and so on.
         */
        template<typename T>
        struct is_convertable {
            constexpr static bool value = (is_shared_native<T>::value
                                           || std::is_integral<T>::value
                                           || std::is_same<bool, T>::value
                                           || std::is_floating_point<T>::value);
        };
        
        
    }
    
    template<typename T>
    struct data_type {
        static const mrb_data_type* value() {
            static_assert(traits::is_shared_native<typename std::remove_reference<T>::type>::value,
                          "Can't get an MRB data type for a non-MRB object");
            return &(std::remove_reference<T>::type::mrb_type);
        }
    };
    
    
    /**
     mruby's mrb_get_args function takes a printf-style format string to specify argument types
     This struct allows us to get the format specifier for a single given type.
     In its default, non-specialized form, it has no format specifier.
     We then specialize it based on the types we can convert to and from mruby types.
     */
    template<typename T, typename Extern = void>
    struct param_char {
    };
    
    // o is for object
    template<typename T>
    struct param_char<T, typename std::enable_if<traits::is_shared_native<T>::value>::type> {
        constexpr static const auto value = "o";
    };
    
    // f is for float
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        constexpr static const auto value = "f";
    };
    
    // i is for integer
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        constexpr static auto value = "i";
    };
    
    // b is for bool
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_same<bool, T>::value>::type> {
        constexpr static auto value = "b";
    };
    
    
    template<typename...>
    struct param_format_string;
    /**
     Obtain the printf-style format specifier required by mrg_get_args.
     
     We already defined templates to determine the proper character for arbitrary types, so this
     function returns the entire format string. It does so by folding over the types—
     Take the first type, get its format specifier character, add the string that specifies the rest of
     the types.
     */
    template<typename T, typename ...Rest>
    struct param_format_string<T, Rest...> {
        static std::string value() {
            static_assert(traits::is_convertable<T>::value,
                          "Cannot share this value into mruby");
            return param_char<typename std::remove_reference<T>::type>::value + param_format_string<Rest...>::value();
        }
    };
    
    /**
     This is the "base case" of the format, where we only have a single type.
     */
    template<typename T>
    struct param_format_string<T> {
        static std::string value() {
            return param_char<typename std::remove_reference<T>::type>::value;
        }
    };
    
    template<>
    struct param_format_string<> {
        static std::string value() {
            return "";
        }
    };
    
    template<typename T>
    struct mrb_conversion_helper {
        typedef typename std::remove_pointer<typename std::remove_reference<T>::type>::type contained_type;
        
        mrb_conversion_helper() {
            b = mrb_nil_value();
        }
        
        mrb_value b;
        mrb_state *s;
        operator mrb_value*() {
            return &b;
        }
        
        operator contained_type() {
            void *p = mrb_data_check_get_ptr(s, b, data_type<T>::value());
            return *(reinterpret_cast<contained_type*>(p));
        }
    };
    
    inline mrb_value to_value(mrb_state *mrb, double d) {
        return mrb_float_value(mrb, d);
    }
    
    template<typename T>
    mrb_value to_value(mrb_state *mrb, T obj) {
        static_assert(traits::is_shared_native<T>::value, "Non-shared data structure passed");
        const mrb_data_type *type = data_type<T>::value();
        struct RClass* klass = mrb_class_get(mrb, type->struct_name);
        T *n = new T(obj);
        return mrb_obj_value(Data_Wrap_Struct(mrb, klass, type, n));
    }
    
    
    
    template<typename T, typename Ret, typename ...Args>
    struct binder {
        static_assert(traits::is_convertable<T>::value, "Bound types need to be a convertable type");
        typedef Ret (T::*funcType)(Args...);
        typedef Ret (T::*constFuncType)(Args...) const;
        
        template<funcType func>
        struct bound_method {
            static mrb_value method(mrb_state *mrb, mrb_value self) {
                std::string format = param_format_string<Args...>::value();
                std::tuple<mrb_conversion_helper<Args>...> t;
                fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                void *p =mrb_data_check_get_ptr(mrb, self, data_type<T>::value());
                T *s = reinterpret_cast<T*>(p);
                Ret re = make_call(s, func, t, std::index_sequence_for<Args...>{});
                return to_value(mrb, re);
            }
        };
        
        
        // Functions marked const are weird and extremely annoying
        template<constFuncType func>
        struct const_bound_method {
            static mrb_value method(mrb_state *mrb, mrb_value self) {
                std::string format = param_format_string<Args...>::value();
                std::cout << std::endl << format << std::endl;
                std::tuple<mrb_conversion_helper<Args>...> t;
                fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                void *p =mrb_data_check_get_ptr(mrb, self, data_type<T>::value());
                T *s = reinterpret_cast<T*>(p);
                Ret re = make_const_call(s, func, t, std::index_sequence_for<Args...>{});
                return to_value(mrb, re);
            }
        };
        
        
    private:
        // Use parameter pack expansion to statically iterate over the tuple and pass it as parameters!
        template<class Tuple, std::size_t... indexes>
        static Ret make_call(T *self, funcType func, Tuple &t, std::index_sequence<indexes...>) {
            return (self->*func)(std::get<indexes>(t)...);
        }
        
        /**
         Use parameter pack expansion and an index sequence paramater to call the function
         */
        template<class Tuple, std::size_t... indexes>
        static Ret make_const_call(T *self, constFuncType func, Tuple &t, std::index_sequence<indexes...>) {
            return (self->*func)(std::get<indexes>(t)...);
        }
        
        /**
         Given a tuple of mrb_conversion_helpers, sets each one of their s members to the current mrb_state
         
         Uses that same nasty static paramater pack shit the other things use
         */
        template<class Tuple, std::size_t... indexes>
        static void fill_mrb_values(mrb_state *mrb,
                                    Tuple &t,
                                    std::index_sequence<indexes...>) {
            using swallow = int[];
            // C++ parameter pack syntax: It's... It's not very good
            (void) swallow{0, (void (std::get<indexes>(t).s = mrb), 0)...};
        }
        
        /**
         Nasty paramater packs shit to properly expand this into varargs. So gross.
         */
        template<class Tuple, std::size_t... indexes>
        static void fill_tuple(std::string format,
                               mrb_state *mrb,
                               Tuple &t,
                               std::index_sequence<indexes...>) {
            mrb_get_args(mrb, format.c_str(), ((mrb_value*) std::get<indexes>(t))...);
        }
        
    };
    
}

#endif /* mrb_wrapper_h */
