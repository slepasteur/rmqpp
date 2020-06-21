#include "parse.h"

#include <fmt/format.h>
#include <parse_it/utils/byte_litterals.h>

#include "data_field_parser.h"
#include "messages/frame.h"
#include "messages/method_header.h"

using namespace parse_it::byte_litterals;

namespace rmq {

namespace {

auto parse_frame(std::span<std::byte> buffer) -> parse_it::parse_result_t<Frame>
{
  auto type = octet_parser(buffer);
  if (!type)
  {
    return std::nullopt;
  }

  auto channel = short_uint_parser(type->second);
  if (!channel)
  {
    return std::nullopt;
  }

  auto size = long_uint_parser(channel->second);
  if (!size)
  {
    return std::nullopt;
  }

  auto payload = parse_it::n_bytes(size->first)(size->second);
  if (!payload)
  {
    return std::nullopt;
  }

  auto frame_end = parse_it::one_byte(0xCE_b)(payload->second);
  if (!frame_end)
  {
    return std::nullopt;
  }

  auto frame = Frame{.type = type->first, .channel = channel->first, .size = size->first, .payload = payload->first};
  return parse_it::parse_result_t<Frame>({std::move(frame), frame_end->second});
}

constexpr auto parse_method_header = parse_it::combine(
  [](short_uint class_id, short_uint method_id) { return MethodHeader{.class_id = class_id, .method_id = method_id}; },
  short_uint_parser, short_uint_parser);
}

auto parse_start(std::span<std::byte> buffer) -> parse_it::parse_result_t<Start>
{
  const auto frame = parse_frame(buffer);
  if (!frame)
  {
    return std::nullopt;
  }

  auto header = parse_method_header(frame->first.payload);
  if (!header)
  {
    return std::nullopt;
  }

  if (header->first.class_id != 10 || header->first.method_id != 10)
  {
    return std::nullopt;
  }

  auto start = parse_it::combine(
    [](octet major, octet minor, field_table properties, long_string mechanisms, long_string locales) {
      return Start{
        .version_major = major,
        .version_minor = minor,
        .server_properties = properties,
        .mechanisms = mechanisms,
        .locales = locales};
    },
    octet_parser, octet_parser, field_table_parser, long_string_parser, long_string_parser)(header->second);
  if (!start)
  {
    return std::nullopt;
  }

  return parse_it::parse_result_t<Start>({start->first, frame->second});
}

} // namespace rmq
