#include <cstddef>
#include <vector>

#include <parse_it/utils/byte_litterals.h>

#include "helpers/require_parsed_equal.h"
#include "rmq/messages/protocol_header.h"
#include "transport/parsing/parse.h"
#include "transport/serialization/serialize.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Serializing the protocol header.")
{
  rmq::Buffer buffer{};
  rmq::serialize(rmq::ProtocolHeader{}, buffer);

  REQUIRE(buffer == rmq::Buffer{'A'_b, 'M'_b, 'Q'_b, 'P'_b, 0_b, 0_b, 9_b, 1_b});
}

TEST_CASE("Serializing the Start message.")
{
  rmq::Buffer buffer{};
  auto start = rmq::Start{};
  rmq::serialize(start, buffer);
  auto parsed = rmq::parse_start(buffer);
  REQUIRE_PARSED_EQUAL(parsed, start);
}

TEST_CASE("Serializing the Start OK message.")
{
  rmq::Buffer buffer{};
  auto start_ok = rmq::StartOk{};
  rmq::serialize(start_ok, buffer);
  auto parsed = rmq::parse_start_ok(buffer);
  REQUIRE_PARSED_EQUAL(parsed, start_ok);
}
