#include "scene.hpp"

namespace NM {
    Scene::Scene(Window *w) : window(w) {
        // this should never happen
        if(window == nullptr) {
            throw std::runtime_error("Null window passed to scene");
        }
        render = SDL_CreateRenderer(w->getWindow(), -1, SDL_RENDERER_ACCELERATED);
        if(render == nullptr) {
            throw RenderCreationFailedError();
        }
    }
    
    void Scene::addActor(std::string behavior_class_name) {
        actors.emplace_back(window->scriptEngine.getMRB(), behavior_class_name);
    }
}