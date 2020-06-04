#include "rmq/types.h"

static_assert(std::numeric_limits<float>::is_iec559, "Float must use IEEE 754 representation.");
static_assert(std::numeric_limits<double>::is_iec559, "Double must use IEEE 754 representation.");
