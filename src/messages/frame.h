#pragma once

#include <span>

#include "rmq/types.h"

namespace rmq {

enum class FrameType : octet
{
  frame_method = 1,
  frame_header = 2,
  frame_body = 3,
  frame_heartbeat = 8
};

struct Frame
{
  octet type;
  short_uint channel;
  long_uint size;
  std::span<const std::byte> payload;
};

}
