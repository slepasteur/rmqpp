#include "parse.h"

#include "data_field_parser.h"

namespace rmq {

auto parse_start(std::span<std::byte> buffer) -> parse_it::parse_result_t<Start>
{
  return parse_it::combine(
    [](octet major, octet minor, field_table properties, long_string mechanisms, long_string locales) {
      return Start{
        .version_major = major,
        .version_minor = minor,
        .server_properties = properties,
        .mechanisms = mechanisms,
        .locales = locales};
    },
    octet_parser, octet_parser, field_table_parser, long_string_parser, long_string_parser)(buffer);
}

} // namespace rmq
