#pragma once

#define REQUIRE_PARSED_EQUAL(parsed, value)                                                                            \
  REQUIRE(parsed);                                                                                                     \
  REQUIRE(parsed->first == value);                                                                                     \
  REQUIRE(parsed->second.empty());

