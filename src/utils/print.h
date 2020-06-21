#pragma once

#include "fmt/format.h"
#include "rmq/types.h"

namespace rmq {

struct Start;

void print_indent(int indent);
void print(timestamp t, int indent = 0);
void print(decimal d, int indent = 0);
void print(long_string s, int indent = 0);
void print(unit, int indent = 0);
void print(const field_wrapper& field, int indent = 0);
void print(const field_value& value, int indent = 0);
void print(const field_array& array, int indent = 0);
void print(const field_table& table, int indent = 0);
void print(const Start& start);

template <typename T>
void print(const T& data, int indent = 0)
{
  print_indent(indent);
  fmt::print("{}\n", data);
}

} // namespace rmq
