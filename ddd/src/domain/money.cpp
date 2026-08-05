#pragma once
#include <cstdio>
#include <sstream>

#include "../domain/money.h"

namespace ddd {

std::string Money::format() const {
  std::int64_t c = cents_;
  bool neg = c < 0;
  std::int64_t abs = neg ? -c : c;
  std::int64_t yuan = abs / 100;
  std::int64_t fen = abs % 100;
  char buf[32];
  std::snprintf(buf, sizeof(buf), "%lld.%02lld", (long long)yuan, (long long)fen);
  std::string s(buf);
  return neg ? "-" + s : s;
}

}  // namespace ddd