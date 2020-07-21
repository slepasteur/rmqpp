#pragma once

#include <fmt/format.h>

template <>
struct fmt::formatter<std::byte> : formatter<uint8_t>
{
  template <typename FormatContext>
  auto format(std::byte b, FormatContext& ctx)
  {
    return formatter<uint8_t>::format(static_cast<uint8_t>(b), ctx);
  }
};
