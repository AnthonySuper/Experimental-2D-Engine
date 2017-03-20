#ifndef _NMTT_SYM_HPP
#define _NMTT_SYM_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace NM {
    
    /**
     * A Sym is basically a "fast string" used for messages.
     * Right now it's not fast---it's just a string.
     * In the future, this will change, and we will intern
     * the string in some way, probably using a hash code or
     * btree.
     * These strings are immutable.
     */
    class Sym {
        const std::string str;
        
    public:
        Sym(const std::string arg);
        bool operator==(const Sym& other) const;
        bool operator!=(const Sym& other) const;
        
        /**
         * Obtain the original string this sym is constructed
         * from. Note that, in the future, this will likely
         * be much slower than an equality comparison.
         * As such, it should mostly be used for debugging.
         */
        explicit operator std::string() const;
    };
    
}

#endif
