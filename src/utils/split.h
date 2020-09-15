#pragma once

#include <algorithm>
#include <span>
#include <vector>

#include "rmq/types.h"

namespace rmq {

inline std::vector<std::span<const std::byte>> split_long_string(const rmq::long_string& data, std::byte delimiter)
{
  std::vector<std::span<const std::byte>> result;
  auto begin = data.cbegin();
  rmq::long_string::const_iterator end;

  while ((end = std::find(begin, data.cend(), delimiter)) != data.cend())
  {
    result.push_back(std::span(begin, end));
    begin = std::next(end);
  }

  result.emplace_back(begin, end);

  return result;
}

} // namespace rmq
