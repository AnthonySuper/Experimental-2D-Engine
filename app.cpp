#include <iostream>
#include <SDL.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>
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
    Vector vec(100,1);
    mrb_state *mrb = mrb_open();
    klass = mrb_define_class(mrb, "Vector", mrb->object_class);
    mrb_define_class_method(mrb, klass, "new", make_vector, MRB_ARGS_REQ(2));
    NM::mrb::callable dist = mrb::binder<Vector, double, const Vector&>::const_bound_method<&NM::Vector::absoluteDistance>::method;
    NM::mrb::callable add = mrb::binder<Vector, Vector, const Vector&>::const_bound_method<&NM::Vector::add>::method;
    NM::mrb::callable getX = mrb::binder<Vector, double>::const_bound_method<&NM::Vector::getX>::method;
    mrb_define_method(mrb, klass, "+", add, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, klass, "absolute_distance", dist, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, klass, "x", getX, MRB_ARGS_NONE());
    mrb_sym v = mrb_intern_cstr(mrb, "$v");
    mrb_gv_set(mrb, v, mrb::to_value(mrb, vec));
    mrb_load_string(mrb, "puts $v.x");
}
