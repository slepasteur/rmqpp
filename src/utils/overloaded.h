#pragma once

namespace rmq {

// Utility to ease visitation of variant.
// std::visit(overloaded(
//   [](int i){ /*handle int case*/ },
//   [](std::string s){ /*handle string case*/ },
//   [](auto) { /*handle all other cases*/ }
// ), v)
template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}
