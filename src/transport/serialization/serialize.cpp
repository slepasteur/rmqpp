#include "serialize.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include <parse_it/utils/byte_litterals.h>

namespace rmq {

using namespace parse_it::byte_litterals;

void serialize(const short_string& str, Buffer& buffer)
{
  assert(str.size() <= std::numeric_limits<octet>::max());
  buffer.reserve(buffer.size() + sizeof(octet) + str.size());
  serialize(static_cast<octet>(str.size()), buffer);
  std::transform(str.begin(), str.end(), std::back_inserter(buffer), [](char c) { return static_cast<std::byte>(c); });
}

void serialize(const long_string& str, Buffer& buffer)
{
  assert(str.size() <= std::numeric_limits<long_uint>::max());
  buffer.reserve(buffer.size() + sizeof(long_uint) + str.size());
  serialize(static_cast<long_uint>(str.size()), buffer);
  std::copy(str.begin(), str.end(), std::back_inserter(buffer));
}

void serialize(const Frame& frame, Buffer& buffer)
{
  serialize(frame.type, buffer);
  serialize(frame.channel, buffer);
}

void serialize(ProtocolHeader, Buffer& buffer)
{
  buffer.insert(buffer.end(), {'A'_b, 'M'_b, 'Q'_b, 'P'_b, 0_b, 0_b, 9_b, 1_b});
}

void serialize(const Start&, Buffer&) {}

} // namespace rmq
