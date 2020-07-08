#pragma once

#include "rmq/types.h"

namespace rmq {

enum ClassId : short_uint
{
  CONNECTION = 10,
  CHANNEL = 20,
  EXCHANGE = 40,
  QUEUE = 50,
  BASIC = 60,
  TX = 90
};
}
