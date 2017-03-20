#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "window.hpp"
#include <mruby/variable.h>
#include "polygon.hpp"
#include "window.hpp"
#include "render/gl_program.hpp"
#include <chrono>
#include "world.hpp"
#include "component_ref.hpp"

std::chrono::milliseconds getTime() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}


int main(int argc, char* argv[]) {
    
    NM::World w;
    NM::ComponentRef e = w.create<NM::PhysicsBody>(10, {{0, 0}, {1, 0}, {1, 1}});
    std::cout << e.getUnsafe().getName() << std::endl;
    auto b = e.get<NM::NullComponent>();
    std::cout << b.getName() << std::endl;
    
    
    /*
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw "Something failed";
    }
    NM::Window w("test", 800, 600);
    SDL_Event e;
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    auto p = NM::GLProgram::fromFiles("assets/shaders/test.vert",
                                      "assets/shaders/test.frag");
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 2. Copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. Then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //4. Unbind the VAO
    glBindVertexArray(0);
    p.use();
    auto lastUpdate = getTime();
    auto frameCount = 0;
    
    while(e.type != SDL_QUIT) {
        SDL_PollEvent(&e);
        w.clearScreen();
        auto thisUpdate = getTime();
        if((thisUpdate - lastUpdate).count() > 1000) {
            std::cout << "rendered " << frameCount << "frames" << std::endl;
            frameCount = 0;
            lastUpdate = thisUpdate;
        }
        else {
            frameCount++;
        }
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        w.swapBuffer();
    }
    */
    
}
