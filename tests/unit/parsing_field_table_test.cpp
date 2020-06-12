#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>
#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"
#include "utils/overloaded.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Parsing a field table.")
{

  SUBCASE("Field table.")
  {
    std::array buffer{
      0x00_b, 0x00_b, 0x00_b, 0x13_b,               // Global field list size.
      0x05_b, '$'_b,  'a'_b,  'b'_b,  'c'_b, 'd'_b, // Field 1 name ($abcd)
      't'_b,                                        //Field 1 type (boolean)
      0x01_b,                                       // Field 1 value (true)
      0x05_b, '$'_b,  'e'_b,  'f'_b,  'g'_b, 'h'_b, // Field 1 name ($abcd)
      'I'_b,                                        // type (long-int)
      0xFF_b, 0xFF_b, 0x00_b, 0x00_b                // value (-65536)
    };
    const auto parsing_result = rmq::field_table_parser(buffer);

    REQUIRE(parsing_result);
    auto field_table = parsing_result->first;
    REQUIRE(field_table.size() == 2);
    REQUIRE(field_table[0].name == "$abcd");
    REQUIRE(field_table[0].value == rmq::field_value{true});
    REQUIRE(field_table[1].name == "$efgh");
    REQUIRE(field_table[1].value == rmq::field_value{rmq::long_int(-65536)});
  }
}
