#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>

#include "transport/parsing/data_field_parser.h"

#include <doctest/doctest.h>

constexpr std::byte operator"" _b(unsigned long long n)
{
  return static_cast<std::byte>(n);
}

constexpr std::byte operator"" _b(char c)
{
  return static_cast<std::byte>(c);
}

TEST_CASE("Parsing uint8_t (octet) type.")
{
  std::array buffer{0x12_b};
  constexpr auto parser = parse_it::uint8_parser();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12);
}

TEST_CASE("Parsing uint16_t (short integer) type.")
{
  std::array buffer{0x12_b, 0x34_b};
  constexpr auto parser = parse_it::uint16_parser();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x3412);
}

TEST_CASE("Parsing uint32_t (long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b};
  constexpr auto parser = parse_it::uint32_parser();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x78563412);
}

TEST_CASE("Parsing uint64_t (long long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b, 0x91_b, 0x23_b, 0x45_b, 0x67_b};
  constexpr auto parser = parse_it::uint64_parser();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x6745239178563412ull);
}

TEST_CASE("Parsing a short string.")
{
  std::array buffer{0x04_b, 'a'_b, 'b'_b, 'c'_b, 'd'_b};
  auto parser = rmq::short_string_parser();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result->first == std::string{"abcd"});
}
