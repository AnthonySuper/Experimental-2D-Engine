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
#include <type_traits>
#include <iostream>

namespace NM {
    typedef mrb_value (*mrb_callable)(mrb_state*, mrb_value);
    
    template<typename T>
    void mrb_destructor_value(mrb_state *mrb, void *self) {
        T* type = reinterpret_cast<T*>(self);
        delete type;
    }
    
    /**
     Determine if a given type is a struct mrb_data_type
     Pretty much just a helper function
     */
    template<typename T>
    struct is_mrb_data_type {
        constexpr static bool value = std::is_same<const struct mrb_data_type, T>::value;
    };
    
    /**
     Type trait which determines if this is a user-defiend object type we can share with MRB.
     */
    template<typename T, typename Enable = void>
    struct is_mrb_object {
        constexpr static bool value = false;
    };
    
    template<typename T>
    struct is_mrb_object<T,
    typename std::enable_if<is_mrb_data_type<decltype(T::mrb_type)>::value>::type> {
        constexpr static bool value = true;
    };
    
    template<typename T>
    struct mrb_data_type_finder {
        static const mrb_data_type* data_type() {
            static_assert(is_mrb_object<typename std::remove_reference<T>::type>::value,
                          "Can't get an MRB data type for a non-MRB object");
            return &(std::remove_reference<T>::type::mrb_type);
        }
    };
    
    /**
     Type trait which determines if this is any type we can share with MRB, including primitive types
     such as mrb_float, mrb_bool, and so on.
     */
    template<typename T>
    struct is_mrb_shared {
        constexpr static bool value = (is_mrb_object<T>::value
                                       || std::is_integral<T>::value
                                       || std::is_same<bool, T>::value
                                       || std::is_floating_point<T>::value);
    };
    
    /**
     mruby's mrb_get_args function takes a printf-style format string to specify argument types
     This struct allows us to get the format specifier for a single given type.
     In its default, non-specialized form, it has no format specifier.
     We then specialize it based on the types we can convert to and from mruby types.
     */
    template<typename T, typename Extern = void>
    struct mrb_param_char {
    };
    
    // o is for object
    template<typename T>
    struct mrb_param_char<T, typename std::enable_if<is_mrb_object<T>::value>::type> {
        constexpr static const auto value = "o";
    };
    
    // f is for float
    template<typename T>
    struct mrb_param_char<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        constexpr static const auto value = "f";
    };
    
    // i is for integer
    template<typename T>
    struct mrb_param_char<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        constexpr static auto value = "i";
    };
    
    // b is for bool
    template<typename T>
    struct mrb_param_char<T, typename std::enable_if<std::is_same<bool, T>::value>::type> {
        constexpr static auto value = "b";
    };
    
    /**
     Obtain the printf-style format specifier required by mrg_get_args.
     
     We already defined templates to determine the proper character for arbitrary types, so this
     function returns the entire format string. It does so by folding over the types—
     Take the first type, get its format specifier character, add the string that specifies the rest of
     the types.
     */
    template<typename T, typename ...Rest>
    struct mrb_param_format_string {
        static std::string value() {
            static_assert(NM::is_mrb_shared<T>::value,
                          "Cannot share this value into mruby");
            return mrb_param_char<typename std::remove_reference<T>::type>::value + mrb_param_format_string<Rest...>::value();
        }
    };
    
    /**
     This is the "base case" of the format, where we only have a single type.
     */
    template<typename T>
    struct mrb_param_format_string<T> {
        static std::string value() {
            return mrb_param_char<typename std::remove_reference<T>::type>::value;
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
            void *p = mrb_data_check_get_ptr(s, b, mrb_data_type_finder<T>::data_type());
            return *(reinterpret_cast<contained_type*>(p));
        }
        
    };
    
    
    template<typename T, typename Ret, typename ...Args>
    struct mrb_binder {
        static_assert(is_mrb_shared<T>::value, "must be an MRB type");
        
        typedef Ret (T::*funcType)(Args...);
        
        typedef Ret (T::*constFuncType)(Args...) const;
        
        
        template<funcType func>
        struct bound_method {
            static mrb_value value(mrb_state *mrb, mrb_value self) {
                std::string format = mrb_param_format_string<Args...>::value();
                std::tuple<mrb_conversion_helper<Args>...> t;
                fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                void *p =mrb_data_check_get_ptr(mrb, self, mrb_data_type_finder<T>::data_type());
                T *s = reinterpret_cast<T*>(p);
                Ret re = make_call(s, func, t, std::index_sequence_for<Args...>{});
                return mrb_nil_value();
            }
        };
        
        // Functions marked const are weird and extremely annoying
        template<constFuncType func>
        struct const_bound_method {
            static mrb_value value(mrb_state *mrb, mrb_value self) {
                std::string format = mrb_param_format_string<Args...>::value();
                std::cout << std::endl << format << std::endl;
                std::tuple<mrb_conversion_helper<Args>...> t;
                fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                void *p =mrb_data_check_get_ptr(mrb, self, mrb_data_type_finder<T>::data_type());
                T *s = reinterpret_cast<T*>(p);
                Ret re = make_const_call(s, func, t, std::index_sequence_for<Args...>{});
                return mrb_float_value(mrb, re);
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
