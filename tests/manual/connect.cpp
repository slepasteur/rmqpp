#include <iostream>
#include <span>

#include <boost/asio.hpp>
#include <fmt/format.h>

#include "rmq/messages/protocol_header.h"
#include "transport/serialization/serialize.h"

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

    fmt::print("Sending protocol header...\n");
    rmq::Buffer buffer{};
    rmq::serialize(rmq::ProtocolHeader{}, buffer);
    boost::asio::write(socket, boost::asio::buffer(buffer), error);
    if (error == boost::asio::error::eof)
    {
      fmt::print("Connection closed.\n");
      return 0;
    }
    else if (error)
    {
      fmt::print("Error: {}\n", error.message());
      return 1;
    }
    fmt::print("Protocol header sent!\n");

    std::array<std::byte, 2048> buf;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);

    if (error == boost::asio::error::eof)
    {
      fmt::print("Connection closed.\n");
      return 0;
    }
    else if (error)
    {
      fmt::print("Error: {}\n", error.message());
      return 1;
    }

    auto received = std::span(buf.begin(), len);
    fmt::print("Received\n\n [{:#X}_b]\n\n", fmt::join(received, "_b, "));
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  }
