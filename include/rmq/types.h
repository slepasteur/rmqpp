#pragma once

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
struct decimal
{
  long_int number;
  octet scale;
};
struct field_wrapper;
using field_array = std::vector<field_wrapper>;

using field_value = std::variant<
  bool, short_short_int, short_short_uint, short_int, short_uint, long_int, long_uint, long_long_int, long_long_uint,
  float, double, decimal, short_string, long_string, field_array>;

struct field_wrapper
{
  field_value data;

  template <typename... Ts>
  field_wrapper(Ts&&... xs)
      : data(std::forward<Ts>(xs)...)
  {}
};

struct field
{
  short_string name;
  field_value value;
};
using field_table = std::vector<field>;

} // namespace rmq
