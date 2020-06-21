#pragma once

#include "rmq/types.h"

namespace rmq {

struct Frame
{
  octet type;
  short_uint channel;
  long_uint size;
  std::span<const std::byte> payload;
};

}
