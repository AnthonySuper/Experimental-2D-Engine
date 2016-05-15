#ifndef _COLLISION_RESULT_H
#define _COLLISION_RESULT_H

#include "vector.hpp"
#include <sstream>

namespace NM {
    class CollisionResult {
    public:
        
        CollisionResult();
        
        CollisionResult(NM::Vector minNormal);
        
        const bool hasResult;
        
        /**
          The minimum translation vector
         */
        const Vector mtv;
        
        operator bool() const { return hasResult; }
    };
    
    std::string inspect(const CollisionResult &r);
}
#endif