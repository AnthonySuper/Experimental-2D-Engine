#include "render/sprite_sheet.hpp"

namespace NM {

    Spritesheet::Spritesheet(std::string path,
                             unsigned int height,
                             unsigned int width)
    : spriteHeight(height), spriteWidth(width),
    sheetHeight(tex.getHeight()),
    sheetWidth(tex.getWidth()),
    tex(path),
    tileIndex(0)
    {
        if(sheetWidth % spriteWidth != 0 ||
           sheetHeight % spriteHeight != 0) {
            throw MisalignedSheetError();
        }
    }
    
}