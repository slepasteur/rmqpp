#include <array>
#include <ctime>

#include <fmt/format.h>
#include <parse_it/utils/byte_litterals.h>

#include "transport/parsing/data_field_parser.h"
#include "transport/parsing/parse.h"
#include "utils/overloaded.h"
#include "utils/print.h"

#include <doctest/doctest.h>

using namespace parse_it::byte_litterals;

TEST_CASE("Parsing Start message.")
{
  std::array buffer{
    0X1_b,  0X0_b,  0X0_b,  0X0_b,  0X0_b,  0X1_b,  0XF9_b, 0X0_b,  0XA_b,  0X0_b,  0XA_b,  0X0_b,  0X9_b,  0X0_b,
    0X0_b,  0X1_b,  0XD4_b, 0XC_b,  0X63_b, 0X61_b, 0X70_b, 0X61_b, 0X62_b, 0X69_b, 0X6C_b, 0X69_b, 0X74_b, 0X69_b,
    0X65_b, 0X73_b, 0X46_b, 0X0_b,  0X0_b,  0X0_b,  0XC7_b, 0X12_b, 0X70_b, 0X75_b, 0X62_b, 0X6C_b, 0X69_b, 0X73_b,
    0X68_b, 0X65_b, 0X72_b, 0X5F_b, 0X63_b, 0X6F_b, 0X6E_b, 0X66_b, 0X69_b, 0X72_b, 0X6D_b, 0X73_b, 0X74_b, 0X1_b,
    0X1A_b, 0X65_b, 0X78_b, 0X63_b, 0X68_b, 0X61_b, 0X6E_b, 0X67_b, 0X65_b, 0X5F_b, 0X65_b, 0X78_b, 0X63_b, 0X68_b,
    0X61_b, 0X6E_b, 0X67_b, 0X65_b, 0X5F_b, 0X62_b, 0X69_b, 0X6E_b, 0X64_b, 0X69_b, 0X6E_b, 0X67_b, 0X73_b, 0X74_b,
    0X1_b,  0XA_b,  0X62_b, 0X61_b, 0X73_b, 0X69_b, 0X63_b, 0X2E_b, 0X6E_b, 0X61_b, 0X63_b, 0X6B_b, 0X74_b, 0X1_b,
    0X16_b, 0X63_b, 0X6F_b, 0X6E_b, 0X73_b, 0X75_b, 0X6D_b, 0X65_b, 0X72_b, 0X5F_b, 0X63_b, 0X61_b, 0X6E_b, 0X63_b,
    0X65_b, 0X6C_b, 0X5F_b, 0X6E_b, 0X6F_b, 0X74_b, 0X69_b, 0X66_b, 0X79_b, 0X74_b, 0X1_b,  0X12_b, 0X63_b, 0X6F_b,
    0X6E_b, 0X6E_b, 0X65_b, 0X63_b, 0X74_b, 0X69_b, 0X6F_b, 0X6E_b, 0X2E_b, 0X62_b, 0X6C_b, 0X6F_b, 0X63_b, 0X6B_b,
    0X65_b, 0X64_b, 0X74_b, 0X1_b,  0X13_b, 0X63_b, 0X6F_b, 0X6E_b, 0X73_b, 0X75_b, 0X6D_b, 0X65_b, 0X72_b, 0X5F_b,
    0X70_b, 0X72_b, 0X69_b, 0X6F_b, 0X72_b, 0X69_b, 0X74_b, 0X69_b, 0X65_b, 0X73_b, 0X74_b, 0X1_b,  0X1C_b, 0X61_b,
    0X75_b, 0X74_b, 0X68_b, 0X65_b, 0X6E_b, 0X74_b, 0X69_b, 0X63_b, 0X61_b, 0X74_b, 0X69_b, 0X6F_b, 0X6E_b, 0X5F_b,
    0X66_b, 0X61_b, 0X69_b, 0X6C_b, 0X75_b, 0X72_b, 0X65_b, 0X5F_b, 0X63_b, 0X6C_b, 0X6F_b, 0X73_b, 0X65_b, 0X74_b,
    0X1_b,  0X10_b, 0X70_b, 0X65_b, 0X72_b, 0X5F_b, 0X63_b, 0X6F_b, 0X6E_b, 0X73_b, 0X75_b, 0X6D_b, 0X65_b, 0X72_b,
    0X5F_b, 0X71_b, 0X6F_b, 0X73_b, 0X74_b, 0X1_b,  0XF_b,  0X64_b, 0X69_b, 0X72_b, 0X65_b, 0X63_b, 0X74_b, 0X5F_b,
    0X72_b, 0X65_b, 0X70_b, 0X6C_b, 0X79_b, 0X5F_b, 0X74_b, 0X6F_b, 0X74_b, 0X1_b,  0XC_b,  0X63_b, 0X6C_b, 0X75_b,
    0X73_b, 0X74_b, 0X65_b, 0X72_b, 0X5F_b, 0X6E_b, 0X61_b, 0X6D_b, 0X65_b, 0X53_b, 0X0_b,  0X0_b,  0X0_b,  0X13_b,
    0X72_b, 0X61_b, 0X62_b, 0X62_b, 0X69_b, 0X74_b, 0X40_b, 0X30_b, 0X38_b, 0X30_b, 0X30_b, 0X62_b, 0X30_b, 0X31_b,
    0X32_b, 0X62_b, 0X66_b, 0X31_b, 0X66_b, 0X9_b,  0X63_b, 0X6F_b, 0X70_b, 0X79_b, 0X72_b, 0X69_b, 0X67_b, 0X68_b,
    0X74_b, 0X53_b, 0X0_b,  0X0_b,  0X0_b,  0X37_b, 0X43_b, 0X6F_b, 0X70_b, 0X79_b, 0X72_b, 0X69_b, 0X67_b, 0X68_b,
    0X74_b, 0X20_b, 0X28_b, 0X63_b, 0X29_b, 0X20_b, 0X32_b, 0X30_b, 0X30_b, 0X37_b, 0X2D_b, 0X32_b, 0X30_b, 0X32_b,
    0X30_b, 0X20_b, 0X56_b, 0X4D_b, 0X77_b, 0X61_b, 0X72_b, 0X65_b, 0X2C_b, 0X20_b, 0X49_b, 0X6E_b, 0X63_b, 0X2E_b,
    0X20_b, 0X6F_b, 0X72_b, 0X20_b, 0X69_b, 0X74_b, 0X73_b, 0X20_b, 0X61_b, 0X66_b, 0X66_b, 0X69_b, 0X6C_b, 0X69_b,
    0X61_b, 0X74_b, 0X65_b, 0X73_b, 0X2E_b, 0XB_b,  0X69_b, 0X6E_b, 0X66_b, 0X6F_b, 0X72_b, 0X6D_b, 0X61_b, 0X74_b,
    0X69_b, 0X6F_b, 0X6E_b, 0X53_b, 0X0_b,  0X0_b,  0X0_b,  0X39_b, 0X4C_b, 0X69_b, 0X63_b, 0X65_b, 0X6E_b, 0X73_b,
    0X65_b, 0X64_b, 0X20_b, 0X75_b, 0X6E_b, 0X64_b, 0X65_b, 0X72_b, 0X20_b, 0X74_b, 0X68_b, 0X65_b, 0X20_b, 0X4D_b,
    0X50_b, 0X4C_b, 0X20_b, 0X31_b, 0X2E_b, 0X31_b, 0X2E_b, 0X20_b, 0X57_b, 0X65_b, 0X62_b, 0X73_b, 0X69_b, 0X74_b,
    0X65_b, 0X3A_b, 0X20_b, 0X68_b, 0X74_b, 0X74_b, 0X70_b, 0X73_b, 0X3A_b, 0X2F_b, 0X2F_b, 0X72_b, 0X61_b, 0X62_b,
    0X62_b, 0X69_b, 0X74_b, 0X6D_b, 0X71_b, 0X2E_b, 0X63_b, 0X6F_b, 0X6D_b, 0X8_b,  0X70_b, 0X6C_b, 0X61_b, 0X74_b,
    0X66_b, 0X6F_b, 0X72_b, 0X6D_b, 0X53_b, 0X0_b,  0X0_b,  0X0_b,  0X11_b, 0X45_b, 0X72_b, 0X6C_b, 0X61_b, 0X6E_b,
    0X67_b, 0X2F_b, 0X4F_b, 0X54_b, 0X50_b, 0X20_b, 0X32_b, 0X33_b, 0X2E_b, 0X30_b, 0X2E_b, 0X32_b, 0X7_b,  0X70_b,
    0X72_b, 0X6F_b, 0X64_b, 0X75_b, 0X63_b, 0X74_b, 0X53_b, 0X0_b,  0X0_b,  0X0_b,  0X8_b,  0X52_b, 0X61_b, 0X62_b,
    0X62_b, 0X69_b, 0X74_b, 0X4D_b, 0X51_b, 0X7_b,  0X76_b, 0X65_b, 0X72_b, 0X73_b, 0X69_b, 0X6F_b, 0X6E_b, 0X53_b,
    0X0_b,  0X0_b,  0X0_b,  0X5_b,  0X33_b, 0X2E_b, 0X38_b, 0X2E_b, 0X35_b, 0X0_b,  0X0_b,  0X0_b,  0XE_b,  0X41_b,
    0X4D_b, 0X51_b, 0X50_b, 0X4C_b, 0X41_b, 0X49_b, 0X4E_b, 0X20_b, 0X50_b, 0X4C_b, 0X41_b, 0X49_b, 0X4E_b, 0X0_b,
    0X0_b,  0X0_b,  0X5_b,  0X65_b, 0X6E_b, 0X5F_b, 0X55_b, 0X53_b, 0XCE_b};

  auto start = rmq::parse_start(buffer);

  REQUIRE(start);

  //print(start->first);
  REQUIRE(start->first.version_major == 0);
  REQUIRE(start->first.version_minor == 9);
}
