#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "window.hpp"
#include "polygon.hpp"
#include "collision_result.hpp"

int main() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Couldn't init!";
    }
}