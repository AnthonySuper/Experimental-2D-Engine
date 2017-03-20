#ifndef _SPRITESHEET_HPP
#define _SPRITESHEET_HPP
#include "render/gl_texture.hpp"
#include "mrb_wrapper.hpp"
#include <string>
#include <stdexcept>


namespace NM {
    class SpriteSheet {
    private:
        GLTexture tex;
        
    public:
        const unsigned int spriteHeight;
        const unsigned int spriteWidth;
        const unsigned int sheetWidth;
        const unsigned int sheetHeight;
        
        SpriteSheet(std::string path,
                    unsigned int width,
                    unsigned int height);
        
        class MisalignedSheetError : public std::runtime_error {
        public:
            MisalignedSheetError() :
            runtime_error("Sprite size misaligned to sheet size")
            {}
        };
        
        const static struct mrb_data_type mrb_type;
        
        void bindMRB(mrb_state *mrb);

    };
}
#endif