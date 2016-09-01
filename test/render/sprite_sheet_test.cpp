#include <catch.hpp>
#include "render/sprite_sheet.hpp"


using namespace NM;

TEST_CASE("it breaks on nonexistant files") {
    REQUIRE_THROWS_AS(new NM::Spritesheet("does_not_exist",
                                          10,
                                          10), BadFileError);
}