#include "serialize.h"

#include <parse_it/utils/byte_litterals.h>

namespace rmq {

using namespace parse_it::byte_litterals;

void serialize(ProtocolHeader, Buffer& buffer)
{
  buffer.insert(buffer.end(), {'A'_b, 'M'_b, 'Q'_b, 'P'_b, 0_b, 0_b, 9_b, 1_b});
}

} // namespace rmq
