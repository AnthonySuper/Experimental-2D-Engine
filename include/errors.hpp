#ifndef _ERRORS_HPP
#define _ERRORS_HPP

#include <stdexcept>

// File containing generalized error subclasses, used in multiple places.


namespace NM {
    class FileNotFoundError : public std::runtime_error {
    public:
        FileNotFoundError(std::string path) :
        std::runtime_error("Could not find file" + path) {}
    };
    
    class BadFileError : public std::runtime_error {
    public:
        BadFileError(std::string path) :
        std::runtime_error("Problem reading file \"" + path +"\"") {}
    };
}
#endif