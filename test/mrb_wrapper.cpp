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

TEST_CASE("param_format_string", "[mrb_wrapper]") {
	REQUIRE((std::string("fibo") == param_format_string<float, int, bool, NM::Vector>::value));
	REQUIRE((std::string("") == param_format_string<>::value));
}

TEST_CASE("to_value", "[mrb_wrapper]") {
    mrb_state *mrb = mrb_open();

	SECTION("positive integer") {
		mrb_value v = to_value(mrb, 1);
		REQUIRE(data_type_string(v) == "fixnum");
		REQUIRE(from_value<int>(mrb, v) == 1);
	}

	SECTION("negative integer") {
		mrb_value v = to_value(mrb, -15);
		REQUIRE(data_type_string(v) == "fixnum");
		REQUIRE(from_value<int>(mrb, v) == -15);
	}

	SECTION("positive short") {
		mrb_value v = to_value(mrb, static_cast<short>(123));
		REQUIRE(data_type_string(v) == "fixnum");
		REQUIRE(from_value<short>(mrb, v) == 123);
	}

	SECTION("negative short") {
		mrb_value v = to_value(mrb, static_cast<short>(-123));
		REQUIRE(data_type_string(v) == "fixnum");
		REQUIRE(from_value<short>(mrb, v) == -123);
	}

	SECTION("bool") {
		mrb_value v = to_value(mrb, true);
		REQUIRE(data_type_string(v) == "bool");
		REQUIRE(from_value<bool>(mrb, v) == true);

		v = to_value(mrb, false);
		REQUIRE(data_type_string(v) == "bool");
		REQUIRE(from_value<bool>(mrb, v) == false);
	}
}
