#ifndef _NM_ENTITY_REF_HPP
#define _NM_ENTITY_REF_HPP

namespace NM {
    class Entity;
    class World;
    /**
     * This is an abstraction of a Reference to an Entity
     * Currently it gets by id, but this will likely change to a shared_ptr
     * eventually (once I figure out how to write a proper allocator for it)
     */
    class EntityRef {
        
        friend class World;
    public:
        EntityRef(World *w, int id) : world(w), id(id) {}
        
        World * const world;
        const int id;
        // if false, will blindly deref possibly null refs
        static constexpr bool checkNulls = true;
        
        operator bool() const;
        
        bool operator==(const EntityRef& other) const;
        
        explicit operator Entity&();
        
    public:
        Entity& get();
    };
}

#endif
