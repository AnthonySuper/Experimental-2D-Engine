#include "catch.hpp"
#include "polygon.hpp"
#include <iostream>

TEST_CASE("Collisions with squares"){
    NM::Polygon g{{0,0}, {3,0}, {3,3}, {0,3}};
    SECTION("Positive collision") {
        NM::Polygon s{{2,2},{4,2}, {4,4}, {2,4}};
        REQUIRE(g.preciseCollision(s));
        REQUIRE(s.preciseCollision(g));
    }
    
    SECTION("Negative collision") {
        NM::Polygon q{{4,4}, {6,4}, {6,6}, {4,6}};
        auto res = q.preciseCollision(g);
        REQUIRE(!q.preciseCollision(g));
        REQUIRE(!g.preciseCollision(q));
    }
    
    SECTION("Collision with a triangle") {
        NM::Polygon tri{{2,2}, {-1, 4}, {4, 4}};
        REQUIRE(tri.preciseCollision(g));
        REQUIRE(g.preciseCollision(tri));
    }
}


TEST_CASE("Centroids") {
    SECTION("With squares") {
        NM::Polygon square{{0,0}, {0, 4}, {4,4}, {4,0}};
        REQUIRE(NM::Point(2,2) == square.getCentroid());
    }
    
    SECTION("With triangles") {
        NM::Polygon tri{{1,1}, {0,0}, {0,1}};
        REQUIRE(tri.getCentroid().x == Approx(1.0/3.0));
        REQUIRE(tri.getCentroid().y == Approx(2.0/3.0));
    }
}


TEST_CASE("Collisions with hexagons") {
    
    NM::Polygon hex{{1,2},{2,1},{3,1},{4,2},{3,3},{2,3}};
    SECTION("Positive collision") {
        NM::Polygon other{{3,2},{5,2},{5,4},{3,5},{3,4}};
        REQUIRE(hex.preciseCollision(other));
        REQUIRE(other.preciseCollision(hex));
    }
    
    SECTION("Negative collision") {
        NM::Polygon other{{8,1},{9,2},{8,3},{7,1}};
        REQUIRE(!hex.preciseCollision(other));
        REQUIRE(!other.preciseCollision(hex));
    }
    
}

TEST_CASE("Moving after a collision") {
    NM::Polygon tri{{1,5}, {5,5}, {3,1}};
    NM::Polygon tri2{{3,3},{5,1},{5,2}};
    REQUIRE(tri.preciseCollision(tri2));
}