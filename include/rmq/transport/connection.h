#include <cstddef>
#include <functional>
#include <span>
#include <variant>

#include "rmq/types.h"

namespace rmq {

namespace details {

struct Init
{};

struct Started
{};

struct Error
{};

using State = std::variant<Init, Started, Error>;

} // namespace details

class Connection
{
public:
  using Sender = std::function<void(std::span<std::byte>)>;

public:
  explicit Connection(Sender sender);

  /**
   * Method to call when new data is received on the tcp connection.
   * @return a boolean saying if the connection is still valid (true) or if it must be discarded
   * (false).
   * TODO replace with proper error handling.
   */
  [[nodiscard]] bool on_data(std::span<std::byte> data);

private:
  Sender send_;
  details::State state_;
  Buffer send_buffer_{};
};

} // namespace rmq
