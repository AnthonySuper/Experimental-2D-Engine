#include "catch.hpp"
#include "player.hpp"

TEST_CASE("Construction") {
    SECTION("Default constructor sets nothing") {
        NM::Player p;
        REQUIRE(p.bitmask.any() == false);
    }
}