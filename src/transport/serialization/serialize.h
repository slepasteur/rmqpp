#pragma once

#include <bit>

#include <parse_it/utils/arithmetic.h>

#include "messages/frame.h"
#include "rmq/messages/protocol_header.h"
#include "rmq/messages/start.h"
#include "rmq/messages/start_ok.h"
#include "rmq/types.h"

namespace rmq {

template <parse_it::arithmetic T, std::endian FROM_ENDIAN = std::endian::big>
constexpr inline auto serialize(T value, Buffer& buffer)
{
  static_assert(
    std::endian::native == std::endian::little || std::endian::native == std::endian::big,
    "Only little en big endian platforms are supported.");
  constexpr auto size = sizeof(T);

  auto begin = reinterpret_cast<std::byte*>(&value);
  auto end = std::next(begin, size);
  if constexpr (FROM_ENDIAN == std::endian::native)
  {
    std::copy(begin, end, std::back_inserter(buffer));
  }
  else
  {
    std::reverse_copy(begin, end, std::back_inserter(buffer));
  }
}

void serialize(const short_string& str, Buffer& buffer);
void serialize(const long_string& str, Buffer& buffer);

void serialize(ProtocolHeader, Buffer& buffer);
void serialize(const Frame& frame, Buffer& buffer);
void serialize(const Start&, Buffer& buffer);
}
