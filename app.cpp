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
    NM::bindVector(mrb);
    mrb_load_string(mrb, "a = Vector.new(0,0) \n" \
                    "b = Vector.new(3, 0) \n " \
                    "puts a.absolute_distance(b)");
    
}
