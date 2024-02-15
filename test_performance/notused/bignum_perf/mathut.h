#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "bigun.h"

using std::string;

const int TESTNPRIMES = 3;


namespace math
{

using ull_t = unsigned long long;
using sll_t = signed long long;

bool isPrime(Integer x);

template<class T> inline T gcdT(T a, T b)
{
    if ( b == T(0) ) return a;
    a %= b;
    return math::gcdT(b, a);
}

int egcd(const int & a, const int & b, int & s, int & t);
Integer egcd(const Integer & a, const Integer & b, Integer & s, Integer & t);
Integer inverse(const Integer & a, const Integer & mod);
ull_t inverse(const ull_t & a, const ull_t & mod);

Integer factor_ferma(Integer x);
Integer factor_root(Integer x);
Integer factor_pollard_pm1(Integer x);
Integer factor_pollard_rho(Integer x);

std::vector<Integer> factors(Integer x, int alg);
// alg 0:root, 1:ferma, 2:pollardPm1, 3:pollardRho

std::vector<Integer> factors(Integer x); // automatic selection

// find max n such that (n+1)^2 > x
// newton root square integer
template <class T> T sqrtnT(T n)
{
    const T t2 {2}, t4 {4};

    if ( n < t2 ) return n;
    if ( n < t4 ) return (n + 1) / 2;

    T x = n, y = n;
    while ( y > t2 ) { x >>= 1; y >>= 2; }

    // x>1
    while ( x * x < n ) x += (x >> 1);

    for (;;)
    {
        auto z = (x + n / x) / 2;
        if ( z >= x ) break;
        x = z;
    }

    return x;
}

inline Integer sqrtn_newton(Integer x) { return sqrtnT<Integer>(x); }
inline Integer sqrtn(Integer x) { return sqrtn_newton(x); }
inline bool isSqrtn(Integer x) { auto s = sqrtn(x); return s * s == x; }
Integer pow2(int d);
Integer pow10(int d);
int log2floor(Integer n);
int log2sharp(Integer n);
void reduce_sub_emp(Integer & a, const Integer & m);

} //math


template <class T>
inline void operator+=(std::vector<T> & a, const std::vector<T> & b)
{ a.insert(a.end(), b.begin(), b.end()); }

template <class T>
inline void operator+=(std::vector<T> & v, const T & b) { v.emplace_back(b); }

