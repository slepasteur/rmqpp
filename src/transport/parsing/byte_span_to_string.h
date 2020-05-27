#pragma once

#include <cstddef>
#include <string>
#include <span>

namespace rmq {

std::string byte_span_to_string(std::span<const std::byte> bytes);

}