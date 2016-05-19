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
#include <vector>

namespace NM::mrb {
    
    class BadValueConersion : std::logic_error {
    public:
        
        BadValueConersion(std::string &arg) : std::logic_error(arg) {};
        BadValueConersion(const char *w) : std::logic_error(w) {};
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
                                           || std::is_floating_point<T>::value
                                           || std::is_same<std::string, T>::value
                                           || std::is_same<const char*, T>::value);
        };
        
        
    }
    
    template<typename T>
    struct data_type {
        static const mrb_data_type* value() {
            using rt = typename std::remove_reference<typename std::remove_pointer<T>::type>::type;
            static_assert(traits::is_shared_native<rt>::value,
                          "Can't get an MRB data type for a non-MRB object");
            return &(std::remove_reference<rt>::type::mrb_type);
        }
    };
    
    
    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value && ! std::is_same<bool, T>::value, mrb_value>::type to_value(mrb_state *mrb, T i) {
        return mrb_fixnum_value(i);
    }
    
    inline mrb_value to_value(mrb_state *mrb, bool b) {
        return mrb_bool_value(b);
    }
    
    
    template<typename T>
    inline typename std::enable_if<std::is_floating_point<T>::value, mrb_value>::type to_value(mrb_state *mrb, T i) {
        return mrb_float_value(mrb, i);
    }
    
    // No need to template this, since it only works for strings!
    inline mrb_value to_value(mrb_state *mrb, std::string s) {
        return mrb_str_new(mrb, s.c_str(), s.length());
    }
    
    /**
     Convert a user-defined type into an mruby object.
     Note that we can only convert copy-constructable objects at the current moment, and cannot handle pointers at all.
     That is going to change soon, hopefulyl!
     */
    template<typename T>
    typename std::enable_if<traits::is_shared_native<T>::value && std::is_copy_constructible<T>::value, mrb_value>::type to_value(mrb_state *mrb, T obj) {
        const mrb_data_type *type = data_type<T>::value();
        struct RClass* klass = mrb_class_get(mrb, type->struct_name);
        // We create a copy here
        T *n = new T(obj);
        return mrb_obj_value(Data_Wrap_Struct(mrb, klass, type, n));
    }
    
    template<typename T>
    typename std::enable_if<traits::is_shared_native<typename std::remove_pointer<T>::type>::value &&
    std::is_pointer<T>::value, mrb_value>::type to_value
    (mrb_state *mrb, T obj) {
        const mrb_data_type *type = data_type<typename std::remove_pointer<T>::type>::value();
        struct RClass *klass = mrb_class_get(mrb, type->struct_name);
        return mrb_obj_value(Data_Wrap_Struct(mrb, klass, type, obj));
    }
    
    inline std::string native_typename(mrb_value val) {
        return std::string{"Native type: "} + DATA_TYPE(val)->struct_name;
    }
    
    inline std::string data_type_string(mrb_value val) {
        switch(mrb_type(val)) {
            case MRB_TT_TRUE:
            case MRB_TT_FALSE:
                return "bool";
            case MRB_TT_FIXNUM:
                return "fixnum";
            case MRB_TT_STRING:
                return "string";
            case MRB_TT_FLOAT:
                return "float";
            case MRB_TT_EXCEPTION:
                return "exception";
            case MRB_TT_OBJECT:
                return "object";
            case MRB_TT_CPTR:
                return "cptr";
            case MRB_TT_DATA:
                return native_typename(val);
            default:
                return "Something weird";
        }
    }
    
    
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type from_value(mrb_state *mrb, mrb_value val) {
        switch(mrb_type(val)) {
            case MRB_TT_FIXNUM:
                return (T) (mrb_float) mrb_fixnum(val);
                break;
            case MRB_TT_FLOAT:
                return (T) mrb_float(val);
                break;
            default:
                auto q = std::string{"Expected a fixnum/integer, recieved a "} + data_type_string(val);
                throw BadValueConersion{q};
        }
    }
    
    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type from_value(mrb_state *mrb, mrb_value val) {
        switch(mrb_type(val)) {
            case MRB_TT_FLOAT:
                return (T) (mrb_int) (mrb_fixnum(val));
                break;
            case MRB_TT_FIXNUM:
                return (T) (mrb_fixnum(val));
                break;
            default:
                auto q =std::string{"Expected a float, recieved a "} + data_type_string(val);
                throw BadValueConersion("Value given was not a float");
        }
    }
    
    template<typename T>
    typename std::enable_if<traits::is_shared_native<typename std::remove_reference<typename std::remove_pointer<T>::type>::type>::value>::type conversion_check(mrb_state *mrb, mrb_value val) {
        using Ti = typename std::remove_reference<typename std::remove_pointer<T>::type>::type;
        auto type = data_type<T>::value();
        
        if(mrb_type(val) != MRB_TT_DATA) {
            auto q = std::string{"Expected a native type, recieved "} + data_type_string(val);
            throw BadValueConersion{q};
        }
        
        if(DATA_TYPE(val) != type) {
            // Super expensive operation, but this just crashes the engine anyway, so who cares
            auto real_type = data_type_string(val);
            auto expected_type = std::string{DATA_TYPE(val)->struct_name};
            auto q = std::string{"Expected native type "} + expected_type + " got " + real_type;
            throw BadValueConersion{q};
        }
    }
    
    
    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value &&
    traits::is_shared_native<typename std::remove_pointer<T>::type>::value, T>::type
    from_value(mrb_state *mrb, mrb_value val) {
        const mrb_data_type *type = data_type<T>::value();
        conversion_check<T>(mrb, val);
        void *ptr = mrb_data_check_get_ptr(mrb, val, type);
        return static_cast<T>(ptr);
    }
    
    template<typename T>
    typename std::enable_if<(! std::is_pointer<T>::value) &&
    traits::is_shared_native<T>::value, T>::type from_value(mrb_state *mrb, mrb_value val) {
        using ptr = typename std::add_pointer<typename std::remove_reference<T>::type>::type;
        const mrb_data_type *type = data_type<T>::value();
        conversion_check<T>(mrb, val);
        // copy construct a new value
        void *p = mrb_data_get_ptr(mrb, val, type);
        // Kind of unclear what we're doing here.
        // Basically, we cast the void * p to a type of T*
        // We than dereference this ptr to cause the copy constructor to be invoked
        return * static_cast<ptr>(p);
    }
    
    
    
    

    
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
        constexpr static const auto value = 'o';
    };
    
    // f is for float
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        constexpr static const auto value = 'f';
    };
    
    // i is for integer
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        constexpr static auto value = 'i';
    };
    
    // b is for bool
    template<typename T>
    struct param_char<T, typename std::enable_if<std::is_same<bool, T>::value>::type> {
        constexpr static auto value = 'b';
    };
    
    
    
    /**
     Obtain the printf-style format specifier required by mrg_get_args.
     
     We already defined templates to determine the proper character for arbitrary types, so this
     function returns the entire format string. It does so by folding over the types—
     Take the first type, get its format specifier character, add the string that specifies the rest of
     the types.
     */
    template<typename ...Args>
    struct param_format_string {
        static const char value[];
    };
    
    template<typename ...Args>
    const char param_format_string<Args...>::value[] = {(param_char<typename std::remove_reference<Args>::type>::value)..., '\0'};
    
    /**
     Defines a struct that assists in conversion of types from mruby types to C++ types.
     */
    template<typename T, typename U = void>
    struct mrb_conversion_helper {
    };
    
    
    // Specialization for floating point types
    template<typename T>
    struct mrb_conversion_helper<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
        mrb_float d;
        mrb_state *mrb;
        
        operator T() {
            return static_cast<T>(d);
        }
        
        void* to_ptr() {
            return (void *) &d;
        }
        
    };
    
    // Specialization for integral types
    template<typename T>
    struct mrb_conversion_helper<T,
    typename std::enable_if<std::is_integral<T>::value>::type> {
        mrb_int i;
        mrb_state *mrb;
        
        operator T() {
            return static_cast<T>(i);
        }
        
        void* to_ptr() {
            return (void *) &i;
        }
    };
    
    // Specialization for strings
    template<> struct mrb_conversion_helper<std::string> {
        char *ptr;
        mrb_state *mrb;
        
        operator std::string() {
            return ptr;
        }
        
        void* to_ptr() {
            return (void *) &ptr;
        }
    };
    
    /**
     Specialization of user-defined types.
     This works for reference types and value types, pointer types are handled seperately
     */
    template<typename T>
    struct mrb_conversion_helper<T,
    typename std::enable_if<traits::is_shared_native<typename std::remove_reference<T>::type>::value>::type> {
        
        typedef typename std::remove_reference<T>::type contained_type;
        
        mrb_value v;
        mrb_state *mrb;
        
        operator T() {
            // mruby's mrb_data_check_get_ptr function does not raise on error, it simply returns NULL.
            // We want it to throw an exception, which mrb_data_get_ptr does.
            void *ptr = mrb_data_get_ptr(mrb, v, data_type<typename std::remove_reference<T>::type>::value());
            contained_type *t = static_cast<contained_type*>(ptr);
            return *t;
        }
        
        void* to_ptr() {
            return (void *) &v;
        }
    };
    
    template<typename T>
    struct mrb_conversion_helper<T,
    typename std::enable_if< traits::is_shared_native<typename std::remove_pointer<T>::type>::value
    && std::is_pointer<T>::value>::type> {
        mrb_value v;
        mrb_state *mrb;
        
        operator T() {
            void *ptr = mrb_data_get_ptr(mrb, v, data_type<typename std::remove_pointer<T>::type>::value());
            return static_cast<T>(ptr);
        }
        
        void* to_ptr() {
            return &v;
        }
    };
    
    
    
    template<typename T>
    struct translator {
        static_assert(traits::is_shared_native<T>::value,
                      "can only translate shared native values");
        
        static void makeClass(mrb_state *mrb) {
            mrb_define_class(mrb, data_type<T>::value()->struct_name, mrb->object_class);
        }
        
        static struct RClass* getClass(mrb_state *mrb) {
            return mrb_class_get(mrb, data_type<T>::value()->struct_name);
        }
        
        template<typename ...Args>
        struct constructor {
            static void bind(mrb_state *mrb) {
                mrb_func_t  f = &val;
                mrb_define_class_method(mrb,
                                        getClass(mrb), "new", f, MRB_ARGS_REQ(sizeof...(Args)));
            }
            
        private:
            static mrb_value val(mrb_state *mrb, mrb_value self) {
                std::string format = param_format_string<Args...>::value;
                std::tuple<mrb_conversion_helper<Args>...> t;
                translator<T>::fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                translator<T>::fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                T* constructed = make_call(t, std::index_sequence_for<Args...>{});
                return mrb_obj_value(Data_Wrap_Struct(mrb, getClass(mrb), data_type<T>::value(), (void *) constructed));
            }
            
            template<class Tuple, std::size_t... indexes>
            static T* make_call(Tuple &t, std::index_sequence<indexes...>) {
                return new T(std::get<indexes>(t)...);
            }
        };
        
        template<typename Ret, typename ...Args>
        struct method {
            typedef Ret(T::*funcType)(Args...);
            typedef Ret(T::*constFuncType)(Args...) const;
            
            template<funcType func>
            struct binder {
                static void bind(mrb_state *mrb, std::string name) {
                    mrb_func_t f = &method;
                    mrb_define_method(mrb,
                                      getClass(mrb), name.c_str(), f, sizeof...(Args));
                }
                
            private:
                static mrb_value method(mrb_state *mrb, mrb_value self) {
                    std::string format = param_format_string<Args...>::value;
                    std::tuple<mrb_conversion_helper<Args>...> t;
                    fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                    fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                    void *p =mrb_data_check_get_ptr(mrb, self, data_type<T>::value());
                    T *s = reinterpret_cast<T*>(p);
                    Ret re = make_call(s, func, t, std::index_sequence_for<Args...>{});
                    return to_value(mrb, re);
                }
                
                template<class Tuple, std::size_t... indexes>
                static Ret make_call(T *self, funcType f, Tuple &t, std::index_sequence<indexes...>) {
                    return (self->*f)(std::get<indexes>(t)...);
                }
            };
            
            template<constFuncType func>
            struct const_binder {
                static void bind(mrb_state *mrb, std::string name) {
                    mrb_func_t f = &method;
                    mrb_define_method(mrb,
                                      getClass(mrb), name.c_str(), f, sizeof...(Args));
                }
                
            private:
                static mrb_value method(mrb_state *mrb, mrb_value self) {
                    std::string format = param_format_string<Args...>::value;
                    std::cout << std::endl << format << std::endl;
                    std::tuple<mrb_conversion_helper<Args>...> t;
                    fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                    fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                    void *p =mrb_data_get_ptr(mrb, self, data_type<T>::value());
                    T *s = reinterpret_cast<T*>(p);
                    Ret re = make_call(s, func, t, std::index_sequence_for<Args...>{});
                    return to_value(mrb, re);
                }
                
                /**
                 Use parameter pack expansion and an index sequence paramater to call the function
                 */
                template<class Tuple, std::size_t... indexes>
                static Ret make_call(T *self, constFuncType f, Tuple &t, std::index_sequence<indexes...>) {
                    return (self->*f)(std::get<indexes>(t)...);
                }
                
            };
            
        };
        
    protected:
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
            (void) swallow{0, (void (std::get<indexes>(t).mrb = mrb), 0)...};
        }
        
        /**
         Nasty paramater packs shit to properly expand this into varargs. So gross.
         */
        template<class Tuple, std::size_t... indexes>
        static void fill_tuple(std::string format,
                               mrb_state *mrb,
                               Tuple &t,
                               std::index_sequence<indexes...>) {
            mrb_get_args(mrb, format.c_str(), (std::get<indexes>(t).to_ptr())...);
        }
    };
    
}

#endif /* mrb_wrapper_h */
