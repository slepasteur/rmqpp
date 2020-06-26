#include "rmq/transport/connection.h"

#include <fmt/format.h>

#include "transport/parsing/parse.h"
#include "transport/serialization/serialize.h"
#include "utils/overloaded.h"

namespace rmq {

namespace {

using Sender = Connection::Sender;

template <typename T>
void serialize_to_buffer(const T& message, Buffer& send_buffer)
{
  send_buffer.clear();
  serialize(message, send_buffer);
}

template <typename T>
void send_message(const T& message, Sender& send, Buffer& send_buffer)
{
  serialize_to_buffer(message, send_buffer);
  send(send_buffer);
}

} // namespace

Connection::Connection(Sender sender)
    : send_(std::move(sender))
    , state_(details::Init{})
{
  fmt::print("Sending protocol header...\n");
  send_message(rmq::ProtocolHeader{}, send_, send_buffer_);
  fmt::print("Protocol header sent!\n");
}

void Connection::on_data(std::span<std::byte> data)
{
  std::visit(
    overloaded([data](details::Init) {
      auto start_parse = parse_start(data);
      if (!start_parse)
      {
        fmt::print("Failed to parse Start message.\n");
        exit(1);
      }

      const auto& start = start_parse->first;
      fmt::print("Broker version {}.{}\n", start.version_major, start.version_minor);

      //        fmt::print("Sending start ok...\n");
      //        send_message(rmq::StartOk{}, send_, send_buffer_);
      //        fmt::print("Start ok sent!\n");
    }),
    state_);
}

}
