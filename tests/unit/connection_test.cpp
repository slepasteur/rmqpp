#include <cstddef>
#include <vector>

#include "rmq/transport/connection.h"
#include "transport/parsing/parse.h"
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
  rmq::Buffer msg_buffer;
  std::vector<std::byte> send_buffer;
  auto sender = [&send_buffer](std::span<std::byte> data) {
    send_buffer.clear();
    send_buffer.insert(send_buffer.end(), data.begin(), data.end());
  };

  GIVEN("A new connection.")
  {
    auto connection = rmq::Connection(sender);
    auto on_message = [&msg_buffer, &connection](const auto& message) {
      msg_buffer.clear();
      rmq::serialize(message, msg_buffer);
      return connection.on_data(msg_buffer);
    };

    THEN("The connection sends the protocol header.") { REQUIRE(send_buffer == protocol_header()); }

    WHEN("Receiving a start message.")
    {
      REQUIRE(on_message(rmq::Start{}));

      THEN("A start ok message is sent.")
      {
        auto start_ok = rmq::parse_start_ok(send_buffer);
        REQUIRE(start_ok);
      }
    }
  }
}
