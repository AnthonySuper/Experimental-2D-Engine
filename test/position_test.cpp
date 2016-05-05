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

TEST_CASE("Absolute distance works") {
    
    SECTION("When starting at zero") {
        NM::Position p(0,0);
        NM::Position o(1,1);
        REQUIRE(p.absoluteDistance(o) == o.absoluteDistance(p));
        REQUIRE(p.absoluteDistance(o) == Approx(1.41421356237));
    }
    
    SECTION("When starting at nonzero") {
        NM::Position p(3,3);
        NM::Position o(5,2);
        REQUIRE(p.absoluteDistance(o) == Approx(2.2360679775));
    }
}

TEST_CASE("Scalar multiplication") {
    SECTION("When scalar is first") {
        NM::Position p(4,5);
        NM::Position res = 10 * p;
        REQUIRE(res.x == 40);
        REQUIRE(res.y == 50);
    }
    
    SECTION("When scalar is second") {
        NM::Position p(4,5);
        NM::Position res = p * 10;
        REQUIRE(res.x == 40);
        REQUIRE(res.y == 50);
    }
}

TEST_CASE("Equality comparison") {
    NM::Position p(4,5);
    NM::Position q(4,5);
    REQUIRE(q == p);
}