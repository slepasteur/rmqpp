#include <string>

#include <parse_it/parser.h>

#include "byte_span_to_string.h"
#include "rmq/types.h"

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
 * decimal-value = scale long-int
 * scale = OCTET ; number of decimal digit
 */
inline constexpr auto decimal_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<decimal> {
  return parse_it::combine(
    [](octet scale, long_int number) { return decimal{.number = number, .scale = scale}; }, octet_parser,
    long_int_parser)(input);
};

/**
 * field-value
 */
inline constexpr auto field_value_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field_value> {
  using namespace parse_it::byte_litterals;
  constexpr auto byte_to_bool = [](auto b) { return field_value(b != 0_b); };

  auto type = parse_it::any_byte()(input);
  if (!type)
  {
    return std::nullopt;
  }

  switch (type->first)
  {
  case 't'_b:
    return bool_field_value_parser(type->second);
  case 'b'_b:
    return short_short_int_parser(type->second);
  case 'B'_b:
    return short_short_uint_parser(type->second);
  case 'U'_b:
    return short_int_parser(type->second);
  case 'u'_b:
    return short_uint_parser(type->second);
  case 'I'_b:
    return long_int_parser(type->second);
  case 'i'_b:
    return long_uint_parser(type->second);
  case 'L'_b:
    return long_long_int_parser(type->second);
  case 'l'_b:
    return long_long_uint_parser(type->second);
  case 'f'_b:
    return float_parser(type->second);
  case 'd'_b:
    return double_parser(type->second);
  case 'D'_b:
    return decimal_parser(type->second);
  case 's'_b:
    return short_string_parser(type->second);
  }

  return std::nullopt;
};

/**
 * field-value-pair = field-name field-value
 * field-name = short-string
 */
//inline constexpr auto field_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field> {
//  par
//  auto name = short_string_parser(input);
//
//  if (!name)
//  {
//    return std::nullopt;
//  }
//  auto content_parser = parse_it::n_bytes(size->first);
//  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
//};

//inline constexpr auto field_table_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field_table> {
//  auto size = long_uint_parser(input);
//
//  if (!size)
//  {
//    return std::nullopt;
//  }
//  auto content_parser = parse_it::n_bytes(size->first);
//  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
//};

} // namespace rmq
