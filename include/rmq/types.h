#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

namespace rmq {

using octet = std::uint8_t;
using short_uint = std::uint16_t;
using long_uint = std::uint32_t;
using long_long_uint = std::uint64_t;
using short_string = std::string;
using long_string = std::vector<std::byte>;

// field table specific types
using short_short_int = std::int8_t;
using short_short_uint = octet;
using short_int = std::int16_t;
using long_int = std::int32_t;
using long_long_int = std::int64_t;
using timestamp = std::chrono::system_clock::time_point;
static_assert(sizeof(std::time_t) == sizeof(std::uint64_t), "Time must be represented as a 64bit value.");

struct unit
{
  auto operator<=>(const unit&) const = default;
};

struct decimal
{
  long_int number;
  octet scale;
  auto operator<=>(const decimal&) const = default;
};
struct field_wrapper;
using field_array = std::vector<field_wrapper>;

struct field;
using field_table = std::vector<field>;

using field_value = std::variant<
  bool, short_short_int, short_short_uint, short_int, short_uint, long_int, long_uint, long_long_int, long_long_uint,
  float, double, decimal, short_string, long_string, field_array, timestamp, field_table, unit>;

struct field_wrapper
{
  field_value data;

  template <typename... Ts>
  field_wrapper(Ts&&... xs)
      : data(std::forward<Ts>(xs)...)
  {}

  auto operator<=>(const field_wrapper&) const = default;
};

struct field
{
  short_string name;
  field_value value;
  auto operator<=>(const field&) const = default;
};

using Buffer = std::vector<std::byte>;

} // namespace rmq
