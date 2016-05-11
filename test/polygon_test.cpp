#include "catch.hpp"
#include "polygon.hpp"

TEST_CASE("Collisions with squares"){
    NM::Polygon g{{0,0}, {3,0}, {3,3}, {0,3}};
    SECTION("Positive collision") {
        NM::Polygon s{{2,2},{4,2}, {4,4}, {2,4}};
        REQUIRE(g.preciseCollision(s) == true);
        REQUIRE(s.preciseCollision(g) == true);
    }
    
    SECTION("Negative collision") {
        NM::Polygon q{{4,4}, {6,4}, {6,6}, {4,6}};
        REQUIRE(q.preciseCollision(g) == false);
        REQUIRE(g.preciseCollision(q) == false);
    }
}

TEST_CASE("Collisions with hexagons") {
    NM::Polygon hex{{1,2},{2,1},{3,1},{4,2},{3,3},{2,3}};
    SECTION("Positive collision") {
        NM::Polygon other{{3,2},{5,2},{5,4},{3,5},{3,4}};
        REQUIRE(hex.preciseCollision(other) == true);
        REQUIRE(other.preciseCollision(hex) == true);
    }
    
    SECTION("Negative collision") {
        NM::Polygon other{{8,1},{9,2},{8,3},{7,1}};
        REQUIRE(hex.preciseCollision(other) == false);
        REQUIRE(other.preciseCollision(hex) == false);
    }
}