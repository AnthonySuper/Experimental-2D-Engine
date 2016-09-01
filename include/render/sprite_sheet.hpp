#ifndef _SPRITESHEET_HPP
#define _SPRITESHEET_HPP
#include "render/gl_texture.hpp"
#include <string>
#include <stdexcept>


namespace NM {
    class Spritesheet {
    private:
        GLTexture tex;
        
    public:
        const unsigned int spriteHeight;
        const unsigned int spriteWidth;
        const unsigned int sheetWidth;
        const unsigned int sheetHeight;
        
        Spritesheet(std::string path,
                    unsigned int width,
                    unsigned int height);
        
        inline void setTileIndex(unsigned int in) {
            tileIndex = in;
        }
        
        class MisalignedSheetError : public std::runtime_error {
        public:
            MisalignedSheetError() :
            runtime_error("Sprite size misaligned to sheet size")
            {}
        };
        
        
    private:
        unsigned int tileIndex;

    };
}
#endif