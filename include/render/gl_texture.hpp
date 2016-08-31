#ifndef _GL_TEXTURE_H
#define _GL_TEXTURE_H

#include <SDL_image.h>
#include <OpenGL/gl3.h>
#include <string>


namespace NM {
    class GLTexture {
        
    public:
        GLTexture(std::string path);
        void bind();
        
    private:
        GLuint id;
        unsigned int width, height;
    };
}
#endif