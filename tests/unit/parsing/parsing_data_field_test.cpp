#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>
#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"
#include "utils/overloaded.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Parsing uint8_t (octet) type.")
{
  std::array buffer{0x12_b};
  const auto parsing_result = rmq::octet_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12);
}

TEST_CASE("Parsing uint16_t (short integer) type.")
{
  std::array buffer{0x12_b, 0x34_b};
  const auto parsing_result = rmq::short_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x1234);
}

TEST_CASE("Parsing uint32_t (long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b};
  const auto parsing_result = rmq::long_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12345678);
}

TEST_CASE("Parsing uint64_t (long long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b, 0x91_b, 0x23_b, 0x45_b, 0x67_b};
  const auto parsing_result = rmq::long_long_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x1234567891234567ull);
}

TEST_CASE("Parsing a short string.")
{
  std::array buffer{0x04_b, 'a'_b, 'b'_b, 'c'_b, 'd'_b};
  const auto parsing_result = rmq::short_string_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result->first == std::string{"abcd"});
}

TEST_CASE("Parsing a long string.")
{
  std::array buffer{0x00_b, 0x00_b, 0x00_b, 0x04_b, 'a'_b, 'b'_b, 'c'_b, 'd'_b};
  const auto parsing_result = rmq::long_string_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result->first == std::vector{'a'_b, 'b'_b, 'c'_b, 'd'_b});
}

