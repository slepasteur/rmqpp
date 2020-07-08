#pragma once

#include <parse_it/parser_types.h>

#include "rmq/messages/start.h"
#include "rmq/messages/start_ok.h"

namespace rmq {

auto parse_start(std::span<std::byte> buffer) -> parse_it::parse_result_t<Start>;
auto parse_start_ok(std::span<std::byte> buffer) -> parse_it::parse_result_t<StartOk>;

} // namespace rmq
