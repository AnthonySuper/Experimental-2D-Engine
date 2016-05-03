#include "catch.hpp"
#include "util.hpp"

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(1 == NM::Util::factorial(1));
    REQUIRE(6 == NM::Util::factorial(3));
    REQUIRE(3628800 == NM::Util::factorial(10));
}