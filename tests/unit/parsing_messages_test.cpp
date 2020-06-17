#include <array>

#include "transport/parsing/parse.h"

#include <doctest/doctest.h>

TEST_CASE("Parsing Start message.")
{
  std::array<std::byte, 0> buffer{};

  auto start = rmq::parse_start(buffer);

  REQUIRE(start);
}
