#ifndef _ERRORS_HPP
#define _ERRORS_HPP

#include <stdexcept>

namespace NM {
    class FileNotFoundError : std::runtime_error {
        
    public:
        FileNotFoundError(std::string path) : std::runtime_error("Could not find file" + path) {}
    };
}
#endif