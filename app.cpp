#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include <SDL_ttf.h>
#include "window.hpp"
#include "polygon.hpp"
#include "render/sprite_sheet.hpp"
#include "render/gl_program.hpp"

int main(int argc, char* argv[]) {
    std::cout << argv[0] << std::endl;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Couldn't init!";
    }
    SDL_Window *dispWindow;
    dispWindow = SDL_CreateWindow("TEST",
                     SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED,
                     600,
                     600,
                     SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GLContext ctx = SDL_GL_CreateContext(dispWindow);
    GLfloat vertices_position[] = {
        0.0, 0.0,
        0.5, 0.0,
        0.5, 0.5,
           
        0.0, 0.0,
        0.0, 0.5,
        -0.5, 0.5,
           
        0.0, 0.0,
        -0.5, 0.0,
        -0.5, -0.5,
           
        0.0, 0.0,
        0.0, -0.5,
        0.5, -0.5
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices_position),
                 vertices_position,
                 GL_STATIC_DRAW);
    NM::GLProgram p = NM::GLProgram::fromFiles("assets/shaders/test.frag", "assets/shaders/test.vert");
    p.use();
    SDL_GL_SetSwapInterval(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  SDL_GL_SetSwapInterval(1);
    GLint position_attr = glGetAttribLocation(p.getId(), "position");
   
    glVertexAttribPointer(position_attr,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);
    glEnableVertexAttribArray(position_attr);
    SDL_Event e;
    glClearColor(0, 0, 100, 1);
    while(true) {
        SDL_PollEvent(&e);
        if(e.type == SDL_QUIT) {
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT);
      
        glBindVertexArray(vbo);
        glDrawArrays(GL_TRIANGLES, 0, 100);
        SDL_GL_SwapWindow(dispWindow);
    }
    
    
    
}