#pragma once

#include <fmt/format.h>

template <>
struct fmt::formatter<std::byte> : formatter<char>
{
  template <typename FormatContext>
  auto format(std::byte b, FormatContext& ctx)
  {
    return formatter<char>::format(static_cast<char>(b), ctx);
  }
};
