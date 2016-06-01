#ifndef _SCENE_H
#define _SCENE_H
#include <SDL.h>
#include <stdexcept>
#include <memory>
#include "window.hpp"

namespace NM {
    class Scene {
        
        class RenderCreationFailedError : std::runtime_error {
        public:
            RenderCreationFailedError() :
            std::runtime_error(std::string("Render creation failed") + SDL_GetError()) {}
        };
        
        Scene(std::shared_ptr<Window> window);
        
    private:
        const std::shared_ptr<Window> window;
        SDL_Renderer *render;
    };
}
#endif