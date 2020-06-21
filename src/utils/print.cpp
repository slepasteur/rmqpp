#include "print.h"

#include "rmq/messages/start.h"
#include "utils/overloaded.h"

namespace rmq {

void print_indent(int indent)
{
  for (int i = 0; i < indent; ++i)
  {
    fmt::print(" ");
  }
}

void print(timestamp t, int indent)
{
  print_indent(indent);
  auto time = std::chrono::system_clock::to_time_t(t);
  fmt::print("{}\n", std::ctime(&time));
}

void print(decimal d, int indent)
{
  print_indent(indent);
  fmt::print("{}*10e-{}\n", d.number, d.scale);
}

void print(long_string s, int indent)
{
  print_indent(indent);
  std::string str;
  str.reserve(s.size());
  std::transform(s.begin(), s.end(), std::back_inserter(str), [](std::byte b) { return std::to_integer<char>(b); });
  fmt::print("{}\n", str);
}

void print(unit, int indent)
{
  print_indent(indent);
  fmt::print("{{}}\n");
}

void print(const field_wrapper& field, int indent)
{
  print(field.data, indent);
}

void print(const field_value& value, int indent)
{
  std::visit(overloaded{[indent](auto f) { print(f, indent); }}, value);
}

void print(const field_array& array, int indent)
{
  for (const auto& f : array)
  {
    print_indent(indent);
    fmt::print(" - ");
    print(f);
    //    fmt::print("\n");
  }
}

void print(const field_table& table, int indent)
{
  for (const auto& f : table)
  {
    print_indent(indent);
    fmt::print("{}:\n", f.name);
    print(f.value, indent + 2);
  }
}

void print(const Start& start)
{
  fmt::print("Start:\n  major: {}\n  minor: {}\n", start.version_major, start.version_minor);
  print(start.server_properties);
}
} // namespace rmq
