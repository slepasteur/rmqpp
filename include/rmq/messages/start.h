#pragma once

#include "rmq/types.h"

namespace rmq {

struct Start
{
  octet version_major;
  octet version_minor;
  field_table server_properties;
  long_string mechanisms;
  long_string locales;
};

}
