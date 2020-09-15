#include <iterator>
#include <ranges>

#include <fmt/format.h>
#include <parse_it/utils/byte_litterals.h>

#include "utils/format_bytes.h"
#include "utils/split.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;


namespace {

void require_expected_split(
  const std::vector<std::span<const std::byte>>& split, const std::vector<rmq::long_string>& expected)
{
  REQUIRE(split.size() == expected.size());
  for (std::size_t i = 0; i < expected.size(); ++i)
  {
    auto msg = fmt::format("{:02x} == {:02x}", fmt::join(split[i], ","), fmt::join(expected[i], ","));
    INFO(msg);
    REQUIRE(std::ranges::equal(split[i], expected[i]));
  }
}

} // namespace

TEST_CASE("Split long string on space returns vector of span representing splits.")
{
  const auto source = rmq::long_string{'a'_b, 'b'_b, ' '_b, 'c'_b};

  const auto split_view = rmq::split_long_string(source, ' '_b);

  const auto expected = std::vector{rmq::long_string{'a'_b, 'b'_b}, rmq::long_string{'c'_b}};
  require_expected_split(split_view, expected);
}

TEST_CASE("Split empty string returns a vector of one empty element.")
{
  const auto source = rmq::long_string{};

  const auto split_view = rmq::split_long_string(source, ' '_b);

  REQUIRE(split_view.size() == 1);
  REQUIRE(split_view[0].empty());
}

TEST_CASE(
  "Split long string with consecutive spaces returns vector of span representing splits including empty splits.")
{
  const auto source = rmq::long_string{'a'_b, 'b'_b, ' '_b, ' '_b, ' '_b, 'c'_b};

  const auto split_view = rmq::split_long_string(source, ' '_b);

  const auto expected =
    std::vector{rmq::long_string{'a'_b, 'b'_b}, rmq::long_string{}, rmq::long_string{}, rmq::long_string{'c'_b}};
  require_expected_split(split_view, expected);
}
