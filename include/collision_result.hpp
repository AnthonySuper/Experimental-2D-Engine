#ifndef _COLLISION_RESULT_H
#define _COLLISION_RESULT_H

#include "vector.hpp"
#include <sstream>

namespace NM {
    /**
     Model the result of some collision, including if the collision was successful.
     
     @remarks This class has a user-defined conversion to bool, so you may check if the collision
     was successful simply by using `if(collision);`
     */
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