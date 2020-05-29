#include <string>

#include <parse_it/parser.h>

#include "byte_span_to_string.h"

namespace rmq {

inline constexpr auto octet_parser = parse_it::integral_parser<std::uint8_t>();
inline constexpr auto short_int_parser = parse_it::integral_parser<std::uint16_t>();
inline constexpr auto long_int_parser = parse_it::integral_parser<std::uint32_t>();
inline constexpr auto long_long_int_parser = parse_it::integral_parser<std::uint64_t>();
inline constexpr auto short_string_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<std::string> {
  auto size = octet_parser(input);

  if (!size)
  {
    return std::nullopt;
  }
  auto content_parser = parse_it::n_bytes(size->first);
  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
};
inline constexpr auto long_string_parser = [](parse_it::parse_input_t input) -> parse_it::parse_result_t<std::string> {
  auto size = long_int_parser(input);

  if (!size)
  {
    return std::nullopt;
  }
  auto content_parser = parse_it::n_bytes(size->first);
  return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
};

} // namespace rmq