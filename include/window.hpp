#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <stdint.h>
#include <stdexcept>
#include <string>
#include "util.hpp"
#include <SDL.h>
#include "script_engine.hpp"
#include "render/gl_include.hpp"


namespace NM {
    /**
     Class representing a game window.
     
     */
    class Window {
        
    public:
        
        /**
         Create a new window with a given title, width, and height
         @param title the title of this window
         @param width the width of this window, in pixels
         @param height the height of this window, in pixels
         */
        Window(std::string title, int width, int height);
        
        /**
         Create a new window with the given title, width, and height,
         at a given position on the user's screen
         @param title the title of this window
         @param width the width of this window, in pixels
         @param height the height of this window, in pixels
         @param x the x coordinate on the user's screen to create this window
         @param y the y coordinate on the user's screen to create this window
         */
        Window(std::string title, int width, int height, int x, int y);
        
        inline SDL_Window* getWindow() { return window; }
        
        inline void swapBuffer() {
            SDL_GL_SwapWindow(window);
        }
        
        inline void clearScreen() {
            glClear(GL_COLOR_BUFFER_BIT);
        }
        
        ScriptEngine scriptEngine;
        
    private:
        
        SDL_Window *window;
        SDL_GLContext ctx;
    };
    
    /**
     An exception thrown when we cannot create a window.
     
     This is a nearly-impossible error to fix, so this is most often used to crash the program
     with relevant debugging information.
     */
    class WindowCreationFailedError : std::runtime_error {
    public:
        WindowCreationFailedError() :
        std::runtime_error(std::string("Window creation failed with message: ") + SDL_GetError()) {};
    };
    
    class OpenGLContextNotObtainedError : std::runtime_error {
    public:
        OpenGLContextNotObtainedError() :
        std::runtime_error(SDL_GetError()) {};
    };
    
}


#endif