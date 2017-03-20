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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetSwapInterval(1);
        ctx = SDL_GL_CreateContext(window);
        if(ctx == nullptr) {
            throw OpenGLContextNotObtainedError();
        }
    }
    
}