#include <array>
#include <cstddef>
#include <span>

#include <parse_it/parser.h>
#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

TEST_CASE("Parsing uint8_t (octet) type.")
{
  std::array buffer{0x12_b};
  const auto parsing_result = rmq::octet_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12);
}

TEST_CASE("Parsing uint16_t (short integer) type.")
{
  std::array buffer{0x12_b, 0x34_b};
  const auto parsing_result = rmq::short_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x1234);
}

TEST_CASE("Parsing uint32_t (long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b};
  const auto parsing_result = rmq::long_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x12345678);
}

TEST_CASE("Parsing uint64_t (long long integer) type.")
{
  std::array buffer{0x12_b, 0x34_b, 0x56_b, 0x78_b, 0x91_b, 0x23_b, 0x45_b, 0x67_b};
  const auto parsing_result = rmq::long_long_uint_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result.value().first == 0x1234567891234567ull);
}

TEST_CASE("Parsing a short string.")
{
  std::array buffer{0x04_b, 'a'_b, 'b'_b, 'c'_b, 'd'_b};
  const auto parsing_result = rmq::short_string_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result->first == std::string{"abcd"});
}

TEST_CASE("Parsing a long string.")
{
  std::array buffer{0x00_b, 0x00_b, 0x00_b, 0x04_b, 'a'_b, 'b'_b, 'c'_b, 'd'_b};
  const auto parsing_result = rmq::long_string_parser(buffer);

  REQUIRE(parsing_result);
  REQUIRE(parsing_result->first == std::vector{'a'_b, 'b'_b, 'c'_b, 'd'_b});
}

TEST_CASE("Parsing field values.")
{
  SUBCASE("Boolean field value true.")
  {
    std::array buffer{
      't'_b, // type (boolean)
      0x1_b  // value (true)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(overloaded([](bool b) { REQUIRE(b); }, [](auto) { FAIL("Invalid field value type."); }), field_value);
  }

  SUBCASE("Boolean field value false.")
  {
    std::array buffer{
      't'_b, // type (boolean)
      0x0_b  // value (true)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(overloaded([](bool b) { REQUIRE(!b); }, [](auto) { FAIL("Invalid field value type."); }), field_value);
  }

  SUBCASE("Short short int field value.")
  {
    std::array buffer{
      'b'_b, // type (short-short-int)
      0xFF_b // value (-1)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::short_short_int i) { REQUIRE(i == -1); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Short short uint field value.")
  {
    std::array buffer{
      'B'_b, // type (short-short-uint)
      0xFF_b // value (255)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::short_short_uint i) { REQUIRE(i == 255); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Short int field value.")
  {
    std::array buffer{
      'U'_b,         // type (short-int)
      0xFF_b, 0x00_b // value (-256)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::short_int i) { REQUIRE(i == -256); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Short uint field value.")
  {
    std::array buffer{
      'u'_b,         // type (short-uint)
      0xFF_b, 0x00_b // value (65280)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::short_uint i) { REQUIRE(i == 65280); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Long int field value.")
  {
    std::array buffer{
      'I'_b,                         // type (long-int)
      0xFF_b, 0xFF_b, 0x00_b, 0x00_b // value (-65536)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::long_int i) { REQUIRE(i == -65536); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Long uint field value.")
  {
    std::array buffer{
      'i'_b,                         // type (long-uint)
      0x00_b, 0x00_b, 0xFF_b, 0xFF_b // value (65535)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::long_uint i) { REQUIRE(i == 65535); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Long long int field value.")
  {
    std::array buffer{
      'L'_b,                         // type (long-long-int)
      0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xFF_b, 0xFF_b // value (65535)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::long_long_int i) { REQUIRE(i == 65535); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Long long uint field value.")
  {
    std::array buffer{
      'l'_b,                         // type (long-long-uint)
      0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0xFF_b, 0xFF_b // value (65535)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::long_long_uint i) { REQUIRE(i == 65535); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }


  SUBCASE("Float field value.")
  {
    std::array buffer{
      'f'_b,                         // type (float)
      0x3F_b, 0x80_b, 0x00_b, 0x00_b // value (1.0)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](float f) { REQUIRE(f == 1.0f); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Double field value.")
  {
    std::array buffer{
      'd'_b,                         // type (double)
      0x40_b, 0x08_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b, 0x00_b // value (3.0)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](double d) { REQUIRE(d == 3.0); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Decimal field value.")
  {
    std::array buffer{
      'D'_b,                         // type (decimal)
      0x04_b, // scale
      0xFF_b, 0xFF_b, 0x00_b, 0x00_b // number (-65536)
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::decimal d) { REQUIRE(d.scale == 4); REQUIRE(d.number == -65536); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);
  }

  SUBCASE("Short string field value.")
  {
    std::array buffer{
      's'_b,                         // type (short string)
      0x04_b, // string size
      'a'_b, 'b'_b, 'c'_b, 'd'_b // "abcd"
    };
    const auto parsing_result = rmq::field_value_parser(buffer);

    REQUIRE(parsing_result);
    auto field_value = parsing_result->first;
    std::visit(
      overloaded([](rmq::short_string s) { REQUIRE(s == "abcd"); }, [](auto) { FAIL("Invalid field value type."); }),
      field_value);

  }
}

TEST_CASE("Parsing a field table.")
{

  SUBCASE("Field table.")
  {
    //  std::array buffer{
    //    //
    //    0x00_b, 0x00_b, 0x00_b, 0x08_b,               // Global field list size.
    //    0x05_b, '$'_b,  'a'_b,  'b'_b,  'c'_b, 'd'_b, // Field 1 name ($abcd)
    //    't'_b,                                        //Field 1 type (boolean)
    //    0x01_b                                        // Field 1 value (true)
    //  };
    //  const auto parsing_result = rmq::field_table_parser(buffer);
    //
    //  REQUIRE(parsing_result);
    //  auto field_table = parsing_result->first;
    //  REQUIRE(field_table)
  }
}
