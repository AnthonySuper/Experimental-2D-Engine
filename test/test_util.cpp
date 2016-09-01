#include "catch.hpp"
#include "util.hpp"
#include <unistd.h>
#include <iostream>

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(1 == NM::Util::factorial(1));
    REQUIRE(6 == NM::Util::factorial(3));
    REQUIRE(3628800 == NM::Util::factorial(10));
}

TEST_CASE("Files are read", "[readFile]") {
    auto q = NM::Util::readFile("./test_files/read.txt");
    REQUIRE(q[0] == 'a');
    REQUIRE(q[1] == 'b');
    REQUIRE(q[2] == 'c');
    REQUIRE(q[3] == 'd');
    REQUIRE(q.size() == 5);
}