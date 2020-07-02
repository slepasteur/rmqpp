#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>
#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"
#include "utils/overloaded.h"
#include "utils/print.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Parsing a field table.")
{

  SUBCASE("Field table.")
  {
    std::array buffer{
      0x00_b, 0x00_b, 0x00_b, 0x13_b,               // Global field list size.
      0x05_b, '$'_b,  'a'_b,  'b'_b,  'c'_b, 'd'_b, // Field 1 name ($abcd)
      't'_b,                                        // Field 1 type (boolean)
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
    REQUIRE(field_table[1].value == rmq::field_value{-65536});
  }

  SUBCASE("Nested field table.")
  {
    std::array buffer{
      0x00_b, 0x00_b, 0x00_b, 0x27_b,               // Global field list size.
      0x05_b, '$'_b,  'a'_b,  'b'_b,  'c'_b, 'd'_b, // Field 1 name ($abcd)
      't'_b,                                        // Field 1 type (boolean)
      0x01_b,                                       // Field 1 value (true)
      0x05_b, '$'_b,  'e'_b,  'f'_b,  'g'_b, 'h'_b, // Field 2 name ($efgh)
      'F'_b,                                        // Field 2 type (field table)
      0x00_b, 0x00_b, 0x00_b, 0x0A_b,               // sub field table size
      0x04_b, '$'_b,  'x'_b,  'y'_b,  'z'_b,        // name ($xyz)
      'i'_b,                                        // long_uint
      0x00_b, 0x00_b, 0x00_b, 0x07_b,               // value (7)
      0x04_b, 'i'_b,  'j'_b,  'k'_b,  'l'_b,        // Field 3 name ($ijkl)
      'I'_b,                                        // Field 3 type (long-int)
      0xFF_b, 0xFF_b, 0x00_b, 0x00_b                // Field 3 value (-65536)
    };
    const auto parsing_result = rmq::field_table_parser(buffer);

    REQUIRE(parsing_result);
    auto field_table = parsing_result->first;

    //print(field_table);

    REQUIRE(field_table.size() == 3);
    REQUIRE(field_table[0].name == "$abcd");
    REQUIRE(field_table[0].value == rmq::field_value{true});
    REQUIRE(field_table[1].name == "$efgh");
    std::visit(
      rmq::overloaded(
        [](const rmq::field_table& sub_table) { REQUIRE(sub_table.size() == 1); },
        [](auto) { FAIL("Invalid field value type."); }),
      field_table[1].value);
    REQUIRE(field_table[2].name == "ijkl");
    REQUIRE(field_table[2].value == rmq::field_value{-65536});
  }
}
