#include <cstddef>
#include <vector>

#include "rmq/transport/connection.h"
#include "transport/serialization/serialize.h"

#include <doctest/doctest.h>

std::vector<std::byte> protocol_header()
{
  std::vector<std::byte> protocol_header;
  rmq::serialize(rmq::ProtocolHeader{}, protocol_header);
  return protocol_header;
}

SCENARIO("Connection start")
{
  std::vector<std::byte> buffer;
  auto sender = [&buffer](std::span<std::byte> data) {
    buffer.clear();
    buffer.insert(buffer.end(), data.begin(), data.end());
  };

  GIVEN("A new connection.")
  {
    auto connection = rmq::Connection(sender);

    THEN("The connection sends the protocol header.") { REQUIRE(buffer == protocol_header()); }

    WHEN("Receiving a start message.")
    {}
  }
}
