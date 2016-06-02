//
//  mrb_wrapper.hpp
//  nmtt
//
//  Created by Anthony Super on 5/16/16.


#pragma once
#ifndef mrb_wrapper_h
#define mrb_wrapper_h

#include <mruby.h>
#include <mruby/data.h>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

/**
 @brief Functions, templtes, and other goodies that let us use mruby with native types

 @discussion
 There's a lot of template metaprogramming going on in this, and it may be slightly overwhelming.
 The important thing here is to remain calm. Put on some relaxing music.
 I know I had to when writing this stuff.
 */
namespace NM { namespace mrb {

    #define autobinder(func) binder<decltype(func), &func>
    
    /**
     @breif Exception for a bad conversion between an mrb type and a native type
     */
    class BadValueConversion : std::logic_error {
    public:

        BadValueConversion(std::string &arg) : std::logic_error(arg) {};
        BadValueConversion(const char *w) : std::logic_error(w) {};
    };


    /**
     @brief function that mruby can use in method definitions and the like
     */
    typedef mrb_value (*callable)(mrb_state*, mrb_value);


    /**
     @brief template to create a destructor function for some type
     @see is_data_type_struct
     */

    template<typename T>
    void destructor_value(mrb_state *mrb, void *self) {
        T* type = reinterpret_cast<T*>(self);
        delete type;
    }
    
    /**
     @brief define a destructor which does nothing
     
     Used in situations where we don't want mruby to manage our memory for
     a given class. At some point we should use GC proection instead, but the
     mruby documentation on that is really bad, so we use this hack for now.
     */
    inline void destructor_noop(mrb_state *mrb, void *self) {
        // Do nothing
    }

    /**
     @brief type traits relating to shared data between C++ and Mruby
     */
    namespace traits {
        /**
         @brief helper to determine if something is a "const struct mrb_data_type" or not
         */
        template<typename T>
        struct is_data_type_struct {
            constexpr static bool value = std::is_same<const struct mrb_data_type, T>::value;
        };

        /**
         @brief Helper variable template for is_data_type_struct.
         */
        template<typename T>
        constexpr bool is_data_type_struct_v = is_data_type_struct<T>::value;


        /**
         @brief determine if a user-defined object can be shared with MRB

         @discussion

         This determines if some user-defined type can be shared with MRB.
         To determine sharability in the general case, you can use is_converable,
         which determines if a given type is sharable in any case.

         A type is sharable with MRB if it has a static member of type "const struct mrb_data_type".
         This struct will determine the type's Ruby name, as well as a deleter function
         (which should almost always be a specialization of the `destructor_value` templte).
         */
        template<typename T, typename Enable = void>
        struct is_shared_native {
            constexpr static bool value = false;
        };

        /**
         @cond 0
         Turning off documentation here for a second.
         */
        template<typename T>
        struct is_shared_native <T,
        typename std::enable_if<is_data_type_struct_v<decltype(T::mrb_type)>>::type> {
            constexpr static bool value = true;
        };
        /**
         @endcond
         */

        /**
         @brief Helper variable template for is_shared_native.
         */
        template<typename T>
        constexpr bool is_shared_native_v = is_shared_native<T>::value;

        /**
         Type trait which determines if this is any type we can share with MRB, including primitive types
         such as mrb_float, mrb_bool, and so on.
         */
        template<typename T>
        struct is_convertable {
            constexpr static bool value = (is_shared_native_v<T>
                                           || std::is_integral<T>::value
                                           || std::is_same<bool, T>::value
                                           || std::is_floating_point<T>::value
                                           || std::is_same<std::string, T>::value
                                           || std::is_same<const char*, T>::value);
        };

        /**
         @brief Helper variable template for is_convertable.
         */
        template<typename T>
        constexpr bool is_convertable_v = is_convertable<T>::value;
    }

    /**
     @brief get the mrb_data-type struct used to share this type with mruby
     @see NM::mrb::traits::is_shared_native<T>
     */
    template<typename T, typename rt = std::remove_reference_t<std::remove_pointer_t<T>>>
    struct data_type {
        static_assert(traits::is_shared_native_v<rt>,
                      "Can't get an MRB data type for a non-MRB object");

        constexpr static const mrb_data_type* value = &(rt::mrb_type);
    };


    template<typename T>
    inline typename std::enable_if<std::is_integral<T>::value, mrb_value>::type to_value(mrb_state *mrb, T i) {
        return mrb_fixnum_value(i);
    }

    template<>
    inline mrb_value to_value<bool>(mrb_state *mrb, bool b) {
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
    typename std::enable_if<traits::is_shared_native_v<T> && std::is_copy_constructible<T>::value, mrb_value>::type to_value(mrb_state *mrb, T obj) {
        const mrb_data_type *type = data_type<T>::value;
        struct RClass* klass = mrb_class_get(mrb, type->struct_name);
        // We create a copy here
        T *n = new T(obj);
        return mrb_obj_value(Data_Wrap_Struct(mrb, klass, type, n));
    }

    template<typename T>
    typename std::enable_if<traits::is_shared_native_v<typename std::remove_pointer<T>::type> &&
    std::is_pointer<T>::value, mrb_value>::type to_value
    (mrb_state *mrb, T obj) {
        const mrb_data_type *type = data_type<T>::value;
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
                throw BadValueConversion{q};
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
                throw BadValueConversion("Value given was not a float");
        }
    }

    template<>
    inline bool from_value<bool>(mrb_state *mrb, mrb_value val) {
        switch (mrb_type(val)) {
            case MRB_TT_TRUE:
            case MRB_TT_FALSE:
                return mrb_bool(val);

            default:
                auto q = std::string{"Expected a bool, received a "} + data_type_string(val);
                throw BadValueConversion{q};
        }
    }

    template<typename T>
    typename std::enable_if<traits::is_shared_native_v<typename std::remove_reference<typename std::remove_pointer<T>::type>::type>>::type conversion_check(mrb_state *mrb, mrb_value val) {
        auto type = data_type<T>::value;

        if(mrb_type(val) != MRB_TT_DATA) {
            auto q = std::string{"Expected a native type, recieved "} + data_type_string(val);
            throw BadValueConversion{q};
        }

        if(DATA_TYPE(val) != type) {
            // Super expensive operation, but this just crashes the engine anyway, so who cares
            auto real_type = data_type_string(val);
            auto expected_type = std::string{DATA_TYPE(val)->struct_name};
            auto q = std::string{"Expected native type "} + expected_type + " got " + real_type;
            throw BadValueConversion{q};
        }
    }


    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value &&
    traits::is_shared_native_v<typename std::remove_pointer<T>::type>, T>::type
    from_value(mrb_state *mrb, mrb_value val) {
        const mrb_data_type *type = data_type<T>::value;
        conversion_check<T>(mrb, val);
        void *ptr = mrb_data_check_get_ptr(mrb, val, type);
        return static_cast<T>(ptr);
    }

    template<typename T>
    typename std::enable_if<(! std::is_pointer<T>::value) &&
    traits::is_shared_native_v<T>, T>::type from_value(mrb_state *mrb, mrb_value val) {
        using ptr = typename std::add_pointer<typename std::remove_reference<T>::type>::type;
        const mrb_data_type *type = data_type<T>::value;
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
    struct param_char<T, typename std::enable_if<traits::is_shared_native_v<T>>::type> {
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
    template<>
    struct param_char<bool> {
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

    template<typename ...Args>
    constexpr auto param_format_string_v = param_format_string<Args...>::value;

    /**
     Defines a struct that assists in conversion of types from mruby types to C++ types.
     */
    template<typename T, typename U = void>
    struct conversion_helper {
    };


    // Specialization for floating point types
    template<typename T>
    struct conversion_helper<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
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
    struct conversion_helper<T,
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
    template<> struct conversion_helper<std::string> {
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
    struct conversion_helper<T,
    typename std::enable_if<traits::is_shared_native_v<typename std::remove_reference<T>::type>>::type> {

        typedef typename std::remove_reference<T>::type contained_type;

        mrb_value v;
        mrb_state *mrb;

        operator T() {
            // mruby's mrb_data_check_get_ptr function does not raise on error, it simply returns NULL.
            // We want it to throw an exception, which mrb_data_get_ptr does.
            void *ptr = mrb_data_get_ptr(mrb, v, data_type<T>::value);
            contained_type *t = static_cast<contained_type*>(ptr);
            return *t;
        }

        void* to_ptr() {
            return (void *) &v;
        }
    };

    template<typename T>
    struct conversion_helper<T,
    typename std::enable_if< traits::is_shared_native_v<typename std::remove_pointer<T>::type>
    && std::is_pointer<T>::value>::type> {
        mrb_value v;
        mrb_state *mrb;

        operator T() {
            void *ptr = mrb_data_get_ptr(mrb, v, data_type<T>::value);
            return static_cast<T>(ptr);
        }

        void* to_ptr() {
            return &v;
        }
    };


    /**
     @brief template to help us bind native types to Ruby.

     @discussion

     This template class uses a variety of increasingly horrifying metaprogramming techniques to
     provide a clean(ish) user interface to share C++ classes to Ruby.

     Let's say I have a type Vector, which has some methods.
     I can write a "binder function" which takes in an `mrb_state*`.
     I want to make that function bind a lot of methods into `mrb_state` so I can use my Vector type from ruby.
     To do so, I can do the following:
     @code
     using t = translator<Vector>;
     t::makeClass(mrb); // pass in the mrb_state and create a new class
     // bind a constructor that takes two doubles, and calls the C++ constructor with the same parameters
     // this lets us do Vector.new(a, b) in ruby to get a new native vector type we can use!
     t::constructor<double, double>::bind(mrb);
     // bind some methods that return a double and take no arguments
     using doubleRet = t::method<double>;
     // bind the `getX` method as `.x` into mruby
     // we use const_binder because this is a const function
     doubleRet::const_binder<&Vector::getX>::bind(mrb, "x");
     doubleRet::const_binder<&Vector::getY>::bind(mrb, "x");
     // Bind methods that take a double and return a double
     // the ::method sub-struct takes in the return type of the function to be bound,
     // then a list of arguments to that function
     // void return types do not work at the moment, I shall fix that soon.
     using setters = t::method<double, double>;
     // Now, if we have a vector `v` in ruby, we can do `v.x = 100` and it will update on our native object!
     setters::binder<&Vector::setX>::bind(mrb, "x=");
     setters::binder<&Vector::setY>::bind(mrb, "y=");
     // This is where things get really cool
     // We can bind in native types ruby knows about, which will automatically be converted when passed to our
     // C++ code. This lets us use all our C++ code from Ruby transparently, with no effort!
     // In this case, we define methods that return a double and have one paramter of type `const Vector&`.
     using doubleVec = t::method<double, const Vector&>;
     doubleVec::const_binder<&Vector::absoluteDistance>::bind(mrb, "absolute_distance");
     doubleVec::const_binder<&Vector::dotProduct>::bind(mrb, "dot");
     @endcode
     */
    template<typename T>
    struct translator {
        static_assert(traits::is_shared_native_v<T>,
                      "can only translate shared native values");

        static void makeClass(mrb_state *mrb) {
            mrb_define_class(mrb, data_type<T>::value->struct_name, mrb->object_class);
        }

        static struct RClass* getClass(mrb_state *mrb) {
            return mrb_class_get(mrb, data_type<T>::value->struct_name);
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
                std::string format = param_format_string_v<Args...>;
                std::tuple<conversion_helper<Args>...> t;
                translator<T>::fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                translator<T>::fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                T* constructed = make_call(t, std::index_sequence_for<Args...>{});
                return mrb_obj_value(Data_Wrap_Struct(mrb, getClass(mrb), data_type<T>::value, (void *) constructed));
            }

            template<class Tuple, std::size_t... indexes>
            static T* make_call(Tuple &t, std::index_sequence<indexes...>) {
                return new T(std::get<indexes>(t)...);
            }
        };

        template<typename Ret, typename ...Args>
        struct method {
        private:
            using funcType = Ret(T::*)(Args...);
            using constFuncType = Ret(T::*)(Args...) const;

            template<typename FT, FT func>
            struct binder_helper {
                static void bind(mrb_state *mrb, std::string name) {
                    mrb_func_t f = &method;
                    mrb_define_method(mrb,
                                      getClass(mrb), name.c_str(), f, sizeof...(Args));
                }

            private:
                static mrb_value method(mrb_state *mrb, mrb_value self) {
                    std::string format = param_format_string_v<Args...>;
                    std::tuple<conversion_helper<Args>...> t;
                    fill_tuple(format, mrb, t, std::index_sequence_for<Args...>{});
                    fill_mrb_values(mrb, t, std::index_sequence_for<Args...>{});
                    void *p = mrb_data_get_ptr(mrb, self, data_type<T>::value);
                    T *s = reinterpret_cast<T*>(p);
                    Ret re = make_call(s, func, t, std::index_sequence_for<Args...>{});
                    return to_value(mrb, re);
                }

                template<class Tuple, std::size_t... indexes>
                static Ret make_call(T *self, FT f, Tuple &t, std::index_sequence<indexes...>) {
                    return (self->*f)(std::get<indexes>(t)...);
                }
            };

        public:
            template<funcType func>
            using binder = binder_helper<funcType, func>;

            template<constFuncType func>
            using const_binder = binder_helper<constFuncType, func>;
            
        };

    protected:
        /**
         Given a tuple of mrb_conversion_helpers, sets each one of their mrb members to the current mrb_state.
         We need this to do some value conversions.

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

} } /* end namespace */

#endif /* mrb_wrapper_h */
