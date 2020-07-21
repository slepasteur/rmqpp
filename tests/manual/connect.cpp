#include <iostream>
#include <span>

#include <boost/asio.hpp>
#include <fmt/format.h>

#include "rmq/messages/protocol_header.h"
#include "rmq/transport/connection.h"
#include "transport/parsing/parse.h"
#include "transport/serialization/serialize.h"
#include "utils/overloaded.h"

using boost::asio::ip::tcp;

template <>
struct fmt::formatter<std::byte> : fmt::formatter<uint8_t>
{
  template <typename FormatContext>
  auto format(std::byte b, FormatContext& ctx)
  {
    return formatter<uint8_t>::format(std::to_integer<uint8_t>(b), ctx);
  }
};

void handle_error(boost::system::error_code error)
{
  if (error == boost::asio::error::eof)
  {
    fmt::print("Connection closed.\n");
    exit(0);
  }
  else if (error)
  {
    fmt::print("Error: {}\n", error.message());
    exit(1);
  }
}

void send_data(tcp::socket& socket, std::span<std::byte> buffer)
{
  boost::system::error_code error;
  boost::asio::write(socket, boost::asio::buffer(buffer.data(), buffer.size()), error);
  handle_error(error);
}

std::span<std::byte> read_data(tcp::socket& socket, std::span<std::byte> buffer)
{
  boost::system::error_code error;
  size_t len = socket.read_some(boost::asio::buffer(buffer.data(), buffer.size()), error);
  handle_error(error);
  return buffer.first(len);
}


int main()
{
  try
  {
    boost::asio::io_context io_context;
    boost::system::error_code error;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("localhost", "5672");

    fmt::print("Connecting...\n");
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    fmt::print("Connected!\n");

    auto connection = rmq::Connection([&socket](std::span<std::byte> buffer) { send_data(socket, buffer); });

    std::array<std::byte, 2048> buf;
    auto received = read_data(socket, buf);
    if (!connection.on_data(received))
    {
      fmt::print("Error while processing received data.");
    }
    //fmt::print("Received\n\n [{:#X}_b]\n\n", fmt::join(received, "_b, "));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  }
