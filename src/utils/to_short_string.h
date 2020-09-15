#pragma once

#include <concepts>

#include "rmq/types.h"

namespace rmq {

/**
 * Converts a range of bytes to a short string (i.e. range of char).
 */
rmq::short_string to_short_string(std::ranges::sized_range auto byte_range) //
  requires std::same_as<std::remove_cvref_t<decltype(*begin(byte_range))>, std::byte>
{
  rmq::short_string result;
  result.reserve(std::ranges::size(byte_range));
  std::transform(std::begin(byte_range), std::end(byte_range), std::back_inserter(result), [](std::byte b) {
    return static_cast<char>(b);
  });
  return result;
}
}
