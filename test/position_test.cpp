#include "catch.hpp"
#include "position.hpp"

TEST_CASE("Position constructs properly with arguments") {
    NM::Position p(10, 15);
    REQUIRE(p.x == 10);
    REQUIRE(p.y == 15);
}

TEST_CASE("Position constructrs properly without arguments") {
    NM::Position p;
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 0);
}