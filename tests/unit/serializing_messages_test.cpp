#include <cstddef>
#include <vector>

#include <parse_it/utils/byte_litterals.h>

#include "rmq/messages/protocol_header.h"
#include "transport/serialization/serialize.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Serializing the protocol header.")
{
  rmq::Buffer buffer{};
  rmq::serialize(rmq::ProtocolHeader{}, buffer);

  REQUIRE(buffer == rmq::Buffer{'A'_b, 'M'_b, 'Q'_b, 'P'_b, 0_b, 0_b, 9_b, 1_b});
}
