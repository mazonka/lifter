#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <string>

using std::string;

using ull_t = unsigned long long;

using Integer = long long;

inline bool isNeg(Integer a) { return a < 0; }
inline bool isZero(Integer a) { return a == 0; }
inline double todbl(Integer a) { return double(a); }

namespace mod
{

Integer mul(Integer a, Integer b, Integer m);
Integer pow(Integer a, Integer b, Integer m);
Integer inv(Integer a, Integer m);

} // mod

