#pragma once

#include "rmq/messages/protocol_header.h"
#include "rmq/types.h"

namespace rmq {

void serialize(ProtocolHeader, Buffer& buffer);
}
