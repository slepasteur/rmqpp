#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"
#include "transport/serialization/serialize.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

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

    REQUIRE(parsed);
    REQUIRE(parsed->first == str);
  }

  SUBCASE("Long string")
  {
    rmq::long_string str{'a'_b, 'b'_b, 'c'_b, 'd'_b, 'e'_b};
    rmq::serialize(str, buffer);
    const auto parsed = rmq::long_string_parser(buffer);

    REQUIRE(parsed);
    REQUIRE(parsed->first == str);
  }
}

