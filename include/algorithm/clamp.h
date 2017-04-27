#pragma once

#include "algorithm_api.h"

namespace muk {

  // same as std::clamp in C++17
  // implementation from http://en.cppreference.com/w/cpp/algorithm/clamp
  template<class T>
  constexpr const T& clamp(const T& v, const T& lo, const T& hi)
  {
    return clamp(v, lo, hi, std::less<>());
  }

  // same as std::clamp in C++17
  // implementation from http://en.cppreference.com/w/cpp/algorithm/clamp
  template<class T, class Compare>
  constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
  {
    return assert(!comp(hi, lo)),
      comp(v, lo) ? lo : comp(hi, v) ? hi : v;
  }

}