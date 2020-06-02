#include <string>

#include <parse_it/parser.h>

#include "byte_span_to_string.h"
#include "rmq/types.h"

namespace rmq {

/**
 * OCTET
 */
inline constexpr auto octet_parser = parse_it::integral_parser<octet>();

/**
 * short-uint = 2*OCTET
 */
inline constexpr auto short_uint_parser = parse_it::integral_parser<short_uint>();

/**
 * long-uint = 4*OCTET
 */
inline constexpr auto long_uint_parser = parse_it::integral_parser<long_uint>();

/**
 * long-long-uint = 8*OCTET
 */
inline constexpr auto long_long_uint_parser = parse_it::integral_parser<long_long_uint>();

/**
 * short-string = OCTET *string-char ; length + content
 * string-char = %x01 .. %xFF
 */
inline constexpr auto short_string_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<short_string> {
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
  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
};

inline constexpr auto field_value_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<field_value> {
  using namespace parse_it::byte_litterals;
  constexpr auto byte_to_bool = [](auto b){ return field_value(b != 0_b); };

  auto type = parse_it::any_byte()(input);
  if (!type)
  {
    return std::nullopt;
  }

  switch (type->first)
  {
  case 't'_b:
    return parse_it::fmap(byte_to_bool, parse_it::any_byte())(type->second);
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