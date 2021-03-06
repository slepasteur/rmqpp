#pragma once

#include <ctime>
#include <string>

#include <parse_it/parser.h>
#include <parse_it/utils/byte_litterals.h>

#include "byte_span_to_string.h"
#include "rmq/types.h"
#include "transport/field_value_types.h"

namespace rmq {

/**
 * OCTET
 */
inline constexpr auto octet_parser = parse_it::arithmetic_parser<octet>();

/**
 * short-uint = 2*OCTET
 */
inline constexpr auto short_uint_parser = parse_it::arithmetic_parser<short_uint>();

/**
 * long-uint = 4*OCTET
 */
inline constexpr auto long_uint_parser = parse_it::arithmetic_parser<long_uint>();

/**
 * long-long-uint = 8*OCTET
 */
inline constexpr auto long_long_uint_parser = parse_it::arithmetic_parser<long_long_uint>();

/**
 * short-string = OCTET *string-char ; length + content
 * string-char = %x01 .. %xFF
 */
inline constexpr auto short_string_parser =
  [](parse_it::parse_input_t input) -> parse_it::parse_result_t<short_string> {
  auto size = octet_parser(input);

  if (!size)
  {
    return std::nullopt;
  }
  auto content_parser = parse_it::n_bytes(size->first);
  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
};

/**
 * long-string = long-uint *OCTET ; length + content
 */
inline constexpr auto long_string_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<long_string> {
  auto size = long_uint_parser(input);

  if (!size)
  {
    return std::nullopt;
  }
  auto content_parser = parse_it::n_bytes(size->first);
  return parse_it::fmap([](auto bytes) { return std::vector(bytes.begin(), bytes.end()); }, content_parser)(
    size->second);
};

/**
 * boolean = OCTET ; 0 = FALSE, else TRUE
 */
inline constexpr auto bool_field_value_parser = parse_it::arithmetic_parser<bool>();

/**
 * short-short-int = OCTET
 */
inline constexpr auto short_short_int_parser = parse_it::arithmetic_parser<short_short_int>();

/**
 * short-short-uint = OCTET
 */
inline constexpr auto short_short_uint_parser = parse_it::arithmetic_parser<short_short_uint>();

/**
 * short-int = 2*OCTET
 */
inline constexpr auto short_int_parser = parse_it::arithmetic_parser<short_int>();

/**
 * long-int = 4*OCTET
 */
inline constexpr auto long_int_parser = parse_it::arithmetic_parser<long_int>();

/**
 * long-long-int = 8*OCTET
 */
inline constexpr auto long_long_int_parser = parse_it::arithmetic_parser<long_long_int>();

/**
 * float = 4*OCTET ; IEEE-754
 */
inline constexpr auto float_parser = parse_it::arithmetic_parser<float>();

/**
 * double = 8*OCTET ; rfc1832 XDR double 
 */
inline constexpr auto double_parser = parse_it::arithmetic_parser<double>();

/**
 * 64-bit POSIX time_t
 */
inline constexpr auto timestamp_parser = parse_it::fmap(
  [](std::time_t t) { return std::chrono::system_clock::from_time_t(t); }, parse_it::arithmetic_parser<std::time_t>());

/**
 * No field
 */
inline constexpr auto unit_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<unit> {
  return parse_it::parse_result_t<unit>({unit{}, input});
};

/**
 * decimal-value = scale long-int
 * scale = OCTET ; number of decimal digit
 */
inline constexpr auto decimal_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<decimal> {
  return parse_it::combine(
    [](octet scale, long_int number) { return decimal{.number = number, .scale = scale}; }, octet_parser,
    long_int_parser)(input);
};

inline auto field_value_parser(parse_it::parse_input_t input) -> parse_it::parse_result_t<field_value>;

inline auto field_table_parser(parse_it::parse_input_t input) -> parse_it::parse_result_t<field_table>;

/**
 * field-array = long-int *field-value ; array of values
 */
inline constexpr auto field_array_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field_array> {
  auto size = long_int_parser(input);

  if (!size)
  {
    return std::nullopt;
  }

  field_array array;
  auto buffer = size->second;
  for (int i = 0; i < size->first; ++i)
  {
    auto field = field_value_parser(buffer);
    if (!field)
    {
      return std::nullopt;
    }
    buffer = field->second;
    array.push_back(field->first);
  }
  return parse_it::parse_result_t<field_array>({array, buffer});
};

/**
 * field-value
 */
inline auto field_value_parser(parse_it::parse_input_t input) -> parse_it::parse_result_t<field_value>
{
  using namespace parse_it::byte_litterals;

  auto type = parse_it::any_byte()(input);
  if (!type)
  {
    return std::nullopt;
  }

  switch (type->first)
  {
  case BOOL_TYPE:
    return bool_field_value_parser(type->second);
  case SHORT_SHORT_INT_TYPE:
    return short_short_int_parser(type->second);
  case SHORT_SHORT_UINT_TYPE:
    return short_short_uint_parser(type->second);
  case SHORT_INT_TYPE:
    return short_int_parser(type->second);
  case SHORT_UINT_TYPE:
    return short_uint_parser(type->second);
  case LONG_INT_TYPE:
    return long_int_parser(type->second);
  case LONG_UINT_TYPE:
    return long_uint_parser(type->second);
  case LONG_LONG_INT_TYPE:
    return long_long_int_parser(type->second);
  case LONG_LONG_UINT_TYPE:
    return long_long_uint_parser(type->second);
  case FLOAT_TYPE:
    return float_parser(type->second);
  case DOUBLE_TYPE:
    return double_parser(type->second);
  case DECIMAL_TYPE:
    return decimal_parser(type->second);
  case SHORT_STRING_TYPE:
    return short_string_parser(type->second);
  case LONG_STRING_TYPE:
    return long_string_parser(type->second);
  case FIELD_ARRAY_TYPE:
    return field_array_parser(type->second);
  case TIMESTAMP_TYPE:
    return timestamp_parser(type->second);
  case FIELD_TABLE_TYPE:
    return field_table_parser(type->second);
  case UNIT_TYPE:
    return unit_parser(type->second);
  }

    return std::nullopt;
}

/**
 * field-value-pair = field-name field-value
 * field-name = short-string
 */
inline constexpr auto field_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field> {
  return parse_it::combine(
    [](short_string name, field_value value) { return field{.name = name, .value = value}; }, short_string_parser,
    field_value_parser)(input);
};

/**
 * field-table = long-uint *field-value-pair
 */
inline auto field_table_parser(parse_it::parse_input_t input) -> parse_it::parse_result_t<field_table>
{
  auto size = long_uint_parser(input);

  if (!size)
  {
    return std::nullopt;
  }

  auto content = parse_it::n_bytes(size->first)(size->second);

  if (!content)
  {
    return std::nullopt;
  }

  auto result = parse_it::many(field_parser, field_table{}, [](field_table&& t, field f) {
    t.push_back(std::move(f));
    return std::move(t);
  })(content->first);

  // Field table must consume the whole content buffer.
  if (!result || result->second.size() != 0)
  {
    return std::nullopt;
  }

  return parse_it::parse_result_t<field_table>({std::move(result->first), content->second});
}

} // namespace rmq
