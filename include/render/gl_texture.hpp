#ifndef _GL_TEXTURE_H
#define _GL_TEXTURE_H

#include <SDL_image.h>
#include "render/gl_include.hpp"
#include <string>
#include "errors.hpp"
#include <sstream>

namespace NM {
    class GLTexture {
        
    public:
        GLTexture(std::string path);
        void bind();
        inline unsigned int getWidth() const {
            return width;
        }
        
        inline unsigned int getHeight() const {
            return height;
        }
        
        inline GLuint getID() const {
            return id;
        }
    private:
        GLuint id;
        unsigned int width, height;
    };
    
    std::string inspect(const GLTexture &tex);
}
#endif