#include "vector.hpp"
#include "mrb_wrapper.hpp"

namespace NM {
    
    const struct mrb_data_type Vector::mrb_type =  {"Vector", &NM::mrb::destructor_value<Vector>};
    
    void Vector::bindMRB(mrb_state *mrb) {
        using namespace NM;
        using t = mrb::translator<Vector>;
        t::makeClass(mrb);
        t::constructor<double, double>::bind(mrb);
        using doubleRet = t::method<double>;
        doubleRet::const_binder<&Vector::getX>::bind(mrb, "x");
        doubleRet::const_binder<&Vector::getY>::bind(mrb, "y");
        using setters = t::method<double, double>;
        setters::binder<&Vector::setX>::bind(mrb, "x=");
        setters::binder<&Vector::setY>::bind(mrb, "y=");
        using doubleVec = t::method<double, const Vector&>;
        doubleVec::const_binder<&Vector::absoluteDistance>::bind(mrb, "absolute_distance");
        doubleVec::const_binder<&Vector::dotProduct>::bind(mrb, "dot");
        
    }
    
    
}

