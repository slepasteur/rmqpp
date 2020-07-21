#include <parse_it/utils/byte_litterals.h>

#include "helpers/require_parsed_equal.h"
#include "transport/parsing/data_field_parser.h"
#include "transport/serialization/serialize.h"
#include "utils/format_bytes.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;


namespace {

auto serialize_and_parse(const rmq::field_value& value)
{
  rmq::Buffer buffer;
  rmq::serialize(value, buffer);
  auto serialized = fmt::format("{:x}", fmt::join(buffer, ", "));
  return rmq::field_value_parser(buffer);
}

} // namespace

TEST_CASE("Serializing rmq types.")
{
  rmq::Buffer buffer;

  SUBCASE("Octet")
  {
    rmq::serialize(rmq::octet{12}, buffer);
    REQUIRE(buffer == rmq::Buffer{12_b});
  }

  SUBCASE("Short uint")
  {
    rmq::serialize(rmq::short_uint{0x1234}, buffer);
    REQUIRE(buffer == rmq::Buffer{0x12_b, 0x34_b});
  }

  SUBCASE("Long int")
  {
    rmq::serialize(rmq::long_int{0x12345678}, buffer);
    REQUIRE(buffer == rmq::Buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b});
  }

  SUBCASE("Short string")
  {
    rmq::short_string str{"adbc"};
    rmq::serialize(str, buffer);
    const auto parsed = rmq::short_string_parser(buffer);

    REQUIRE_PARSED_EQUAL(parsed, str);
  }

  SUBCASE("Long string")
  {
    rmq::long_string str{'a'_b, 'b'_b, 'c'_b, 'd'_b, 'e'_b};
    rmq::serialize(str, buffer);
    const auto parsed = rmq::long_string_parser(buffer);

    REQUIRE_PARSED_EQUAL(parsed, str);
  }

  SUBCASE("Decimal")
  {
    rmq::decimal dec{.number = 123, .scale = 2};
    rmq::serialize(dec, buffer);
    const auto parsed = rmq::decimal_parser(buffer);

    REQUIRE_PARSED_EQUAL(parsed, dec);
  }

  SUBCASE("Field values.")
  {
    SUBCASE("bool")
    {
      rmq::field_value value{true};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("short_short_int")
    {
      rmq::field_value value{rmq::short_short_int(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("short_short_uint")
    {
      rmq::field_value value{rmq::short_short_uint(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("short_int")
    {
      rmq::field_value value{rmq::short_int(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("short_uint")
    {
      rmq::field_value value{rmq::short_uint(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("long_int")
    {
      rmq::field_value value{12};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("long_uint")
    {
      rmq::field_value value{rmq::long_uint(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("long_long_int")
    {
      rmq::field_value value{rmq::long_long_int(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("long_long_uint")
    {
      rmq::field_value value{rmq::long_long_uint(12)};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("float")
    {
      rmq::field_value value{12.34f};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("double")
    {
      rmq::field_value value{12.34};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("timestamp")
    {
      // Internal representation of timestamp has a greater precision has the one that is
      // serialized. We therefore need to get now() to second precision before using it.
      auto now_in_seconds = rmq::timestamp{
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())};
      rmq::field_value value{now_in_seconds};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("decimal")
    {
      rmq::field_value value{rmq::decimal{.number = 12, .scale = 3}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("short_string")
    {
      rmq::field_value value{rmq::short_string{"short string!"}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("long_string")
    {
      using namespace parse_it::byte_litterals;
      rmq::field_value value{rmq::long_string{'l'_b, 'o'_b, 'n'_b, 'g'_b}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("field_array")
    {
      rmq::field_value value{rmq::field_array{{12}, {35.56}, {false}}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("field_table")
    {
      rmq::field_value value{rmq::field_table{
        {.name = "long_int", .value = 12}, {.name = "double", .value = 35.56}, {.name = "bool", .value = true}}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }

    SUBCASE("unit")
    {
      rmq::field_value value{rmq::unit{}};
      auto parsed = serialize_and_parse(value);
      REQUIRE_PARSED_EQUAL(parsed, value);
    }
  }
}

