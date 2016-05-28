#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "script_engine.hpp"
#include "vector.hpp"

int main() {
    NM::ScriptEngine e;
    NM::Vector::bindMRB(e.getMRB());
    e.runString("v = Vector.new(10,11)\n" \
                "puts v.x\n" \
                "puts v.y\n");
}
