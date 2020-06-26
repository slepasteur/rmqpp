#include <cstddef>
#include <functional>
#include <span>
#include <variant>

#include "rmq/types.h"

namespace rmq {

namespace details {

struct Init
{};

using State = std::variant<Init>;

} // namespace details

class Connection
{
public:
  using Sender = std::function<void(std::span<std::byte>)>;

public:
  explicit Connection(Sender sender);

  void on_data(std::span<std::byte> data);

private:
  Sender send_;
  details::State state_;
  Buffer send_buffer_{};
};

} // namespace rmq
