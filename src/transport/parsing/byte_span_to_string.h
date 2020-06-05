#pragma once

#include <cstddef>
#include <span>
#include <string>

namespace rmq {

std::string byte_span_to_string(std::span<const std::byte> bytes);

}