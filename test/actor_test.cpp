#include "catch.hpp"
#include "actor.hpp"

TEST_CASE("Actor player visibilty") {
    NM::Actor a;
    a.visibilityMask[10] = true;
    a.visibilityMask[11] = true;
    SECTION("Allows seeing players") {
        NM::Player p;
        p.bitmask[11] = true;
        REQUIRE(a.visibleBy(p) == true);
    }
    SECTION("Disallows nonseeing players") {
        NM::Player p;
        REQUIRE(a.visibleBy(p) == false);
    }
}

TEST_CASE("Actor player controllability") {
    NM::Actor a;
    a.visibilityMask[11] = true;
    
    SECTION("Allows controlling players") {
        NM::Player p;
        p.bitmask[11] = true;
        REQUIRE(a.visibleBy(p) == true);
    }
    
    SECTION("Disallows noncontrolling players") {
        NM::Player p;
        REQUIRE(a.visibleBy(p) == false);
    }
}