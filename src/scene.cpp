#include "scene.hpp"

namespace NM {
    Scene::Scene(std::shared_ptr<Window> w) : window(w) {
        render = SDL_CreateRenderer(w->getWindow(), -1, SDL_RENDERER_ACCELERATED);
        if(render == nullptr) {
            throw RenderCreationFailedError();
        }
    }
}