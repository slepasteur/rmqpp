#include <string>

#include <parse_it/parser.h>

#include "byte_span_to_string.h"

namespace rmq {

inline auto short_string_parser()
{
  return [](parse_it::parse_input_t input) -> parse_it::parse_result_t<std::string> {
    constexpr auto size_parser = parse_it::uint8_parser();
    auto size = size_parser(input);

    if (!size)
    {
      return std::nullopt;
    }
    auto content_parser = parse_it::n_bytes(size->first);
    return parse_it::fmap(byte_span_to_string, content_parser)(size->second);
  };
}

} // namespace rmq