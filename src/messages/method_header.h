#pragma once

#include "rmq/types.h"

namespace rmq {

struct MethodHeader
{
  short_uint class_id;
  short_uint method_id;
};

} // namespace rmq
