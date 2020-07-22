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

StartOk start_response(const Start&)
{
  auto client_properties = field_table{
    field{.name = short_string{"product"}, .value = short_string{"rmqpp"}},
    field{.name = short_string{"version"}, .value = short_string{"0.1"}},
    field{
      .name = short_string{"information"},
      .value = short_string{"RabbitMQ client library for c++. Website: https://github.com/slepasteur/rmqpp"}}};

  //start.mechanisms

  //return StartOk { .client_properties = std::move(client_properties); }
  return StartOk{};
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

bool Connection::on_data(std::span<std::byte> data)
{
  using namespace rmq::details;
  state_ = std::visit(
    overloaded(
      [this, data](details::Init) -> State {
        auto start_parse = parse_start(data);
        if (!start_parse)
        {
          fmt::print("Failed to parse Start message.\n");
          return Error{};
        }

        const auto& start = start_parse->first;
        fmt::print("Broker version {}.{}\n", start.version_major, start.version_minor);

        fmt::print("Sending start ok...\n");
        auto start_ok = start_response(start);
        send_message(start_ok, send_, send_buffer_);
        fmt::print("Start ok sent!\n");
        return Started{};
      },
      [](Started s) -> State {
        // TODO
        return s;
      },
      [](Error e) -> State { return e; }),
    state_);

  return !std::holds_alternative<Error>(state_);
}

}
