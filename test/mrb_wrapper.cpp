#include "catch.hpp"
#include "mrb_wrapper.hpp"
#include "vector.hpp"

using namespace NM::mrb;

TEST_CASE("param_char", "[mrb_wrapper]") {
	SECTION("primitive types") {
		REQUIRE((param_char<float>::value == 'f'));
		REQUIRE((param_char<int>::value == 'i'));
		REQUIRE((param_char<bool>::value == 'b'));
	}

	SECTION("shared natives") {
		REQUIRE((param_char<NM::Vector>::value == 'o'));
	}
}
