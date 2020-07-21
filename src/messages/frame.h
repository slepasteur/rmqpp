#pragma once

#include <span>

#include "rmq/types.h"

namespace rmq {

enum FrameType : octet
{
  frame_method = 1,
  frame_header = 2,
  frame_body = 3,
  frame_heartbeat = 8
};

constexpr inline auto FRAME_END = std::byte{0xCE};

struct Frame
{
  octet type;
  short_uint channel;
  long_uint size;
  std::span<const std::byte> payload;
};

}
