#include "sym.hpp"

namespace NM {
    Sym::Sym(const std::string arg) : str(arg) {}
    
    bool Sym::operator==(const Sym &other) const {
        return str == other.str;
    }
    
    bool Sym::operator!=(const Sym &other) const {
        return str != other.str;
    }
    
    Sym::operator std::string() const {
        return str;
    }
}
