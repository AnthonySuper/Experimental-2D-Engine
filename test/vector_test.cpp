#include "catch.hpp"
#include "vector.hpp"


TEST_CASE("Vector constructs properly with arguments") {
    NM::Vector p(10, 15);
    REQUIRE(p.x == 10);
    REQUIRE(p.y == 15);
}

TEST_CASE("Vector constructrs properly without arguments") {
    NM::Vector p;
    REQUIRE(p.x == 0);
    REQUIRE(p.y == 0);
}

TEST_CASE("Absolute distance works") {
    
    SECTION("When starting at zero") {
        NM::Vector p(0,0);
        NM::Vector o(1,1);
        REQUIRE(p.absoluteDistance(o) == o.absoluteDistance(p));
        REQUIRE(p.absoluteDistance(o) == Approx(1.41421356237));
    }
    
    SECTION("When starting at nonzero") {
        NM::Vector p(3,3);
        NM::Vector o(5,2);
        REQUIRE(p.absoluteDistance(o) == Approx(2.2360679775));
    }
}

TEST_CASE("Proper unit vectors") {
    NM::Vector p(10, 0);
    REQUIRE(p.unitVector() == NM::Vector(1,0));
}


TEST_CASE("Scalar multiplication") {
    SECTION("When scalar is first") {
        NM::Vector p(4,5);
        NM::Vector res = 10 * p;
        REQUIRE(res.x == 40);
        REQUIRE(res.y == 50);
    }
    
    SECTION("When scalar is second") {
        NM::Vector p(4,5);
        NM::Vector res = p * 10;
        REQUIRE(res.x == 40);
        REQUIRE(res.y == 50);
    }
}

TEST_CASE("Equality comparison") {
    NM::Vector p(4,5);
    NM::Vector q(4,5);
    REQUIRE(q == p);
}

TEST_CASE("Inspection") {
    NM::Vector p(0,0);
    REQUIRE(NM::inspect(p) == "Vector: (0, 0)");
}
