#include "collision_result.hpp"

namespace NM {
    CollisionResult::CollisionResult() : hasResult(false),
        mtv() {
        
    }
    
    CollisionResult::CollisionResult(NM::Vector v) : hasResult(true),
        mtv(v) {
    }
    
    std::string inspect(const CollisionResult &r) {
        std::stringstream ss;
        if(r) {
            ss << "CollisionResult " << NM::inspect(r.mtv);
        }
        else {
            ss << "CollisionResult (no collision)";
        }
        return ss.str();
    }
}
