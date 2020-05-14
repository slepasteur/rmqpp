#include <array>
#include <cstddef>
#include <span>

#include <doctest/doctest.h>

constexpr std::byte operator"" _b(unsigned long long n)
{
  return static_cast<std::byte>(n);
}

TEST_CASE("Parsing uint8_t (octet) type.")
{
  [[maybe_unused]] std::array buffer{0x12_b};
}
