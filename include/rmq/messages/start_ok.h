#pragma once

#include "rmq/types.h"

namespace rmq {

struct StartOk
{
  field_table client_properties;
  short_string mechanism;
  long_string response;
  short_string locale;
};

} // namespace rmq
