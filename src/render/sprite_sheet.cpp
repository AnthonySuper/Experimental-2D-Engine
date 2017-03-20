#include "render/sprite_sheet.hpp"

namespace NM {

    SpriteSheet::SpriteSheet(std::string path,
                             unsigned int height,
                             unsigned int width)
    : tex(path),
    spriteHeight(height),
    spriteWidth(width),
    sheetWidth(tex.getWidth()),
    sheetHeight(tex.getHeight())
    {
        if(sheetWidth % spriteWidth != 0 ||
           sheetHeight % spriteHeight != 0) {
            throw MisalignedSheetError();
        }
    }
    
    const struct mrb_data_type SpriteSheet::mrb_type = {"SpriteSheet",
        &NM::mrb::destructor_value<SpriteSheet>};
    
    void SpriteSheet::bindMRB(mrb_state *mrb) {
        using namespace NM::mrb;
        using t = translator<SpriteSheet>;
        t::makeClass(mrb);
        t::constructor<std::string, int, int>::bind(mrb);
    }
    
    
}