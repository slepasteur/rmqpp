#pragma once

#include <cstddef>

namespace rmq {
constexpr auto BOOL_TYPE = static_cast<std::byte>('t');
constexpr auto SHORT_SHORT_INT_TYPE = static_cast<std::byte>('b');
constexpr auto SHORT_SHORT_UINT_TYPE = static_cast<std::byte>('B');
constexpr auto SHORT_INT_TYPE = static_cast<std::byte>('U');
constexpr auto SHORT_UINT_TYPE = static_cast<std::byte>('u');
constexpr auto LONG_INT_TYPE = static_cast<std::byte>('I');
constexpr auto LONG_UINT_TYPE = static_cast<std::byte>('i');
constexpr auto LONG_LONG_INT_TYPE = static_cast<std::byte>('L');
constexpr auto LONG_LONG_UINT_TYPE = static_cast<std::byte>('l');
constexpr auto FLOAT_TYPE = static_cast<std::byte>('f');
constexpr auto DOUBLE_TYPE = static_cast<std::byte>('d');
constexpr auto DECIMAL_TYPE = static_cast<std::byte>('D');
constexpr auto SHORT_STRING_TYPE = static_cast<std::byte>('s');
constexpr auto LONG_STRING_TYPE = static_cast<std::byte>('S');
constexpr auto FIELD_ARRAY_TYPE = static_cast<std::byte>('A');
constexpr auto TIMESTAMP_TYPE = static_cast<std::byte>('T');
constexpr auto FIELD_TABLE_TYPE = static_cast<std::byte>('F');
constexpr auto UNIT_TYPE = static_cast<std::byte>('V');
} // namespace rmq
