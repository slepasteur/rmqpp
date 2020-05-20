#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>

#include <doctest/doctest.h>

constexpr std::byte operator"" _b(unsigned long long n)
{
  return static_cast<std::byte>(n);
}

TEST_CASE("Parsing uint8_t (octet) type.")
{
  std::array buffer{0x12_b};
  constexpr auto parser = parse_it::any_byte();
  const auto parsing_result = parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12_b);
}
