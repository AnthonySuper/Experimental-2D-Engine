#include <iostream>
#include <SDL.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <chrono>
#include "vector.hpp"

using namespace NM;

RClass *klass;

static mrb_value make_vector(mrb_state *st, mrb_value self) {
    std::cout << std::endl << "Making a vector " << std::endl;
    mrb_value x = mrb_float_value(st, 0.0);
    mrb_value y = mrb_float_value(st, 0.0);
    mrb_get_args(st, "ff", &x, &y);
    Vector *v = new Vector(mrb_float(x), mrb_float(y));
    return mrb_obj_value(Data_Wrap_Struct(st, klass, &NM::Vector::mrb_type, (void *) v));
}



int main() {
    mrb_state *mrb = mrb_open();
    klass = mrb_define_class(mrb, "Vector", mrb->object_class);
    mrb_define_class_method(mrb, klass, "new", make_vector, MRB_ARGS_REQ(2));
    mrb_callable c = &mrb_binder<Vector, double, const Vector&>::const_bound_method<&Vector::absoluteDistance>::value;
    mrb_define_method(mrb, klass, "absolute_distance", c, MRB_ARGS_REQ(1));
    mrb_load_string(mrb, "puts Vector.new(10,11).absolute_distance(Vector.new(10,12))");
    
    if(mrb->exc) {
        std::cout << "Whata surprise, we have an exception" << std::endl;
        std::cout << RSTRING_PTR(mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0)) << std::endl;
    }
    
}
