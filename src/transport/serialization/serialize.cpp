#include "serialize.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include <parse_it/utils/byte_litterals.h>

#include "transport/field_value_types.h"
#include "utils/overloaded.h"

namespace rmq {

using namespace parse_it::byte_litterals;

void serialize(unit, Buffer&) {}

namespace {

void serialize_field_value_type(const field_value& value, Buffer& buffer)
{
  std::visit(
    overloaded{
      [&buffer](bool) { buffer.push_back(BOOL_TYPE); },                         //
      [&buffer](short_short_int) { buffer.push_back(SHORT_SHORT_INT_TYPE); },   //
      [&buffer](short_short_uint) { buffer.push_back(SHORT_SHORT_UINT_TYPE); }, //
      [&buffer](short_int) { buffer.push_back(SHORT_INT_TYPE); },               //
      [&buffer](short_uint) { buffer.push_back(SHORT_UINT_TYPE); },             //
      [&buffer](long_int) { buffer.push_back(LONG_INT_TYPE); },                 //
      [&buffer](long_uint) { buffer.push_back(LONG_UINT_TYPE); },               //
      [&buffer](long_long_int) { buffer.push_back(LONG_LONG_INT_TYPE); },       //
      [&buffer](long_long_uint) { buffer.push_back(LONG_LONG_UINT_TYPE); },     //
      [&buffer](float) { buffer.push_back(FLOAT_TYPE); },                       //
      [&buffer](double) { buffer.push_back(DOUBLE_TYPE); },                     //
      [&buffer](timestamp) { buffer.push_back(TIMESTAMP_TYPE); },               //
      [&buffer](decimal) { buffer.push_back(DECIMAL_TYPE); },                   //
      [&buffer](short_string) { buffer.push_back(SHORT_STRING_TYPE); },         //
      [&buffer](long_string) { buffer.push_back(LONG_STRING_TYPE); },           //
      [&buffer](field_array) { buffer.push_back(FIELD_ARRAY_TYPE); },           //
      [&buffer](field_table) { buffer.push_back(FIELD_TABLE_TYPE); },           //
      [&buffer](unit) { buffer.push_back(UNIT_TYPE); },                         //
      [](auto) {}                                                               //
    },
    value);
}

void serialize_field_value_value(const field_value& value, Buffer& buffer)
{
  std::visit(
    overloaded{
      [&buffer](const auto& x) { serialize(x, buffer); }, //
    },
    value);
}

} // namespace

void serialize(const short_string& str, Buffer& buffer)
{
  assert(str.size() <= std::numeric_limits<octet>::max());
  buffer.reserve(buffer.size() + sizeof(octet) + str.size());
  serialize(static_cast<octet>(str.size()), buffer);
  std::transform(str.begin(), str.end(), std::back_inserter(buffer), [](char c) { return static_cast<std::byte>(c); });
}

void serialize(const long_string& str, Buffer& buffer)
{
  assert(str.size() <= std::numeric_limits<long_uint>::max());
  buffer.reserve(buffer.size() + sizeof(long_uint) + str.size());
  serialize(static_cast<long_uint>(str.size()), buffer);
  std::copy(str.begin(), str.end(), std::back_inserter(buffer));
}

void serialize(const decimal& dec, Buffer& buffer)
{
  serialize(dec.scale, buffer);
  serialize(dec.number, buffer);
}

void serialize(const timestamp& t, Buffer& buffer)
{
  serialize(std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count(), buffer);
}


void serialize(const field_value& value, Buffer& buffer)
{
  serialize_field_value_type(value, buffer);
  serialize_field_value_value(value, buffer);
}

void serialize(const field_array& array, Buffer& buffer)
{
  serialize(rmq::long_int(array.size()), buffer);
  for (const auto& wrapper : array)
  {
    serialize(wrapper.data, buffer);
  }
}

void serialize(const field& f, Buffer& buffer)
{
  serialize(f.name, buffer);
  serialize(f.value, buffer);
}

void serialize(const field_table& table, Buffer& buffer)
{
  Buffer table_buffer;
  for (const auto& field : table)
  {
    serialize(field, table_buffer);
  }

  buffer.reserve(buffer.size() + sizeof(long_uint) + table_buffer.size());
  serialize(long_uint(table_buffer.size()), buffer);
  std::copy(table_buffer.begin(), table_buffer.end(), std::back_inserter(buffer));
}

void serialize(const Frame& frame, Buffer& buffer)
{
  serialize(frame.type, buffer);
  serialize(frame.channel, buffer);
}

void serialize(ProtocolHeader, Buffer& buffer)
{
  buffer.insert(buffer.end(), {'A'_b, 'M'_b, 'Q'_b, 'P'_b, 0_b, 0_b, 9_b, 1_b});
}

void serialize(const Start&, Buffer&) {}

} // namespace rmq
