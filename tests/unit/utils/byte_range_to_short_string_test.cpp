#include <concepts>

#include <parse_it/utils/byte_litterals.h>

#include "utils/to_short_string.h"

#include <doctest/doctest.h>


TEST_CASE("A long string can be converted to a short string")
{
  using namespace parse_it::byte_litterals;
  auto long_string = rmq::long_string{'a'_b, 'b'_b, 'c'_b};

  auto result = rmq::to_short_string(long_string);

  REQUIRE(result == "abc");
}
