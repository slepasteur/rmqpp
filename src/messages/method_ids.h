#pragma once

#include "rmq/types.h"

namespace rmq {

enum ConnectionMethodId : short_uint
{
  START = 10,
  START_OK = 11,
  SECURE = 20,
  SECURE_OK = 21,
  TUNE = 30,
  TUNE_OK = 31,
  OPEN = 40,
  OPEN_OK = 41,
  CLOSE = 50,
  CLOSE_OK = 51
};
}
