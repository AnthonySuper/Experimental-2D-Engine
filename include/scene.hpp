#ifndef _SCENE_H
#define _SCENE_H
#include <SDL.h>
#include <stdexcept>
#include <memory>
#include <vector>
#include <actor.hpp>
#include "script_engine.hpp"
#include "window.hpp"

namespace NM {
    class Scene {
        
        class RenderCreationFailedError : std::runtime_error {
        public:
            RenderCreationFailedError() :
            std::runtime_error(std::string("Render creation failed") + SDL_GetError()) {}
        };
        
        Scene(Window *w);
        
        void addActor(std::string behavior_class_name);
        
    private:
        std::vector<Actor> actors;
        
        Window* const window;
        
        SDL_Renderer *render;
    };
}

#endif