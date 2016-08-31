#include "window.hpp"

namespace NM {
    
    Window::Window(std::string title,
                   int width,
                   int height) : Window(title,
                                        width,
                                        height,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED) {};
    
    Window::Window(std::string title, int width, int height, int x, int y) {
        
        window = SDL_CreateWindow(title.c_str(),
                                  x,
                                  y,
                                  width,
                                  height,
                                  SDL_WINDOW_OPENGL);
        if(window == nullptr) {
            throw WindowCreationFailedError();
        }
        ctx = SDL_GL_CreateContext(window);
        if(ctx == nullptr) {
            throw OpenGLContextNotObtainedError();
        }
    }
    
}