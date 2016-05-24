#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include "vector.hpp"
#include "mrb_wrapper.hpp"

int main() {
    mrb_state *mrb = mrb_open();
    NM::Vector::bindMRB(mrb);
    mrb_value v = mrb_load_string(mrb, "a = Vector.new(0,0) \n" \
                    "b = Vector.new(10, 11) \n" \
                    "puts a.x \n" \
                    "puts b.x \n" \
                    "puts \"Absolute distance: #{a.absolute_distance(b)}\" \n ");
}
