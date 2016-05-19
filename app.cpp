#include <iostream>
#include <SDL.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <chrono>
#include "vector.hpp"
#include "mrb_wrapper.hpp"

int main() {
    mrb_state *mrb = mrb_open();
    NM::Vector::bindMRB(mrb);
    mrb_value v = mrb_load_string(mrb, "a = Vector.new(0,0) \n" \
                    "puts a.inspect \n" \
                    "a");
    NM::Vector *vec = NM::mrb::from_value<NM::Vector*>(mrb, v);
    std::cout << "Got a ptr " << vec << std::endl;
}
