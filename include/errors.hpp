#ifndef _ERRORS_HPP
#define _ERRORS_HPP

#include <stdexcept>

// File containing generalized error subclasses, used in multiple places.


namespace NM {
    class FileNotFoundError : std::runtime_error {
        
    public:
        FileNotFoundError(std::string path) : std::runtime_error("Could not find file" + path) {}
    };
}
#endif