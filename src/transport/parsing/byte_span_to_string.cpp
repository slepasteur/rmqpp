#include "byte_span_to_string.h"

#include <algorithm>

namespace rmq {

std::string byte_span_to_string(std::span<const std::byte> bytes)
{
  std::string result;
  result.reserve(bytes.size());
  std::transform(
    bytes.begin(), bytes.end(), std::back_inserter(result), [](std::byte b) { return std::to_integer<char>(b); });
  return result;
}

}