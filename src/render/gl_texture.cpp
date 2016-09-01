#include "render/gl_texture.hpp"

namespace NM {
    GLTexture::GLTexture(std::string path) {
        SDL_Surface *surf = IMG_Load(path.c_str());
        GLuint myId;
        if(surf == nullptr) {
            throw BadFileError(path);
        }
        glGenTextures(1, &myId);
        id = myId;
        
        int mode = GL_RGB;
        if(surf->format->BytesPerPixel == 4) {
            mode = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, id);
        width  = surf->w;
        height = surf->h;
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     mode,
                     surf->w,
                     surf->h,
                     0,
                     mode,
                     GL_UNSIGNED_BYTE,
                     surf->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void GLTexture::bind() {
        glBindTexture(GL_TEXTURE_2D, id);
    }
}