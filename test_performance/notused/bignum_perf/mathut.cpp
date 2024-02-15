#include <algorithm>

#include "err.h"

#include "mathut.h"
#include "egcd.inc"

// modulus, base, odd factor of n-1, parity level
static bool miller_rabin(const Integer n, const Integer a, const Integer d, int s)
{
    Integer t1(1), tn(n - 1);
    auto b = mod::pow(a, d, n);
    if (b == t1) return true;

    for (int i = 0; i < s; i++)
    {
        if (b == tn) return true;
        else if (b == t1) return false;
        b = mod::mul(b, b, n);
    }
    return false;
}

static bool miller_rabin(const Integer n, const std::vector<Integer> & v)
{
    // find s and d
    int s = 0;
    Integer d;
    {
        auto a = n - 1;
        while (1)
        {
            auto b = a / 2;
            if ( b * 2 != a ) { d = a; break; }
            ++s;
            a = b;
        }
    }

    for (const auto & a : v)
    {
        if (!miller_rabin(n, a, d, s)) return false;
    }

    return true;
}

static bool isPrime(const Integer n, const std::vector<Integer> & v)
{
    // check Fermat condition
    for (const auto & i : v)
    {
        if (i == n) return true;
        if (i != mod::pow(i, n, n)) return false;
    }

    // check divisibility
    for (const auto & i : v)
    {
        if ( ::isZero(n % i) ) return false;
    }

    if (n == Integer(3)) return true;

    // Lucas test
    if (0)
    {
        Integer t1(1);
        auto u = math::factors(n - 1);

        auto it = std::unique(u.begin(), u.end());
        u.resize( std::distance(u.begin(), it) );

        for (const auto & i : v)
        {
            bool notprime = false;
            for (const auto & q : u)
            {
                if ( mod::pow(i, (n - 1) / q, n) == t1 )
                {
                    notprime = true;
                    break;
                }
            }
            if ( !notprime ) return true;
        }
    }

    if ( !miller_rabin(n, v) ) return false;

    return true; // probably prime
}

static std::vector<Integer> genPrimes()
{
    std::vector<Integer> v = {Integer(2)};

    while ( v.size() < TESTNPRIMES )
    {
        auto x = v.back();

        if ( v.size() == 1 ) x = Integer(3);
        else x += Integer(2);

        while ( !isPrime(x, v) ) x += Integer(2);
        v.push_back(x);
    }

    return v;
}

bool math::isPrime(Integer x)
{
    static std::vector<Integer> vprimes = genPrimes();
    return ::isPrime(x, vprimes);
}

// using 2*3, can yet be improved by using 2*3*5 etc
Integer math::factor_root(Integer n)
{
    if ( n < 4 ) return Integer { 1 };

    if (::isZero(n % 2)) return Integer { 2 };
    if (::isZero(n % 3)) return Integer { 3 };

    if (isPrime(n)) return Integer { 1 };

    auto sq = sqrtn(n);

    for ( Integer i(5); i <= sq; )
    {
        if ( ::isZero( n % i ) ) return i; i += 2;
        if ( ::isZero( n % i ) ) return i; i += 4;
    }

    return Integer { 1 };
}

Integer math::factor_ferma(Integer n)
{
    if (n < 4) return Integer { 1 };

    if (::isZero(n % 2)) return Integer { 2 };

    if (isPrime(n)) return Integer { 1 };

    Integer y(0);

    while ( !isSqrtn(n + y * y) ) ++y;

    return sqrtn(n + y * y) - y;
}


Integer math::factor_pollard_pm1(Integer n)
{
    if (n < 4) return Integer { 1 };

    if (::isZero(n % 2)) return Integer { 2 };

    if (isPrime(n)) return Integer { 1 };

    Integer aa(2), a(2), i(0), d(n);

    while ( d == 1 || d == n )
    {
        ++i;
        a = mod::pow(a, i, n);
        d = gcdT(a - 1, n);
        if ( a == 1 )
        {
            ++aa;
            a = aa;
            i = 0;
        }
    }

    if (d < 0) nevers("Negative result");

    return d;
}

Integer math::factor_pollard_rho(Integer n)
{
    if (n < 4) return Integer { 1 };
    if (::isZero(n % 2)) return Integer { 2 };
    if (isPrime(n)) return Integer { 1 };

    Integer t1(1), t2(2), c;

    auto g = [&](Integer z)
    {
        return mod::mul(z, z, n) + c;
    };

    for (c = t2; c < n; ++c)
    {
        auto x = t2;
        auto y = t2;
        auto d = t1;
        while (d == t1)
        {
            x = g(x);
            y = g(g(y));
            auto a = x - y; if (isNeg(a)) a = -a;
            d = gcdT<Integer>(a, n);
        }
        if (d != n) return d;
    }

    return t1;
}

std::vector<Integer> math::factors(Integer n, int alg)
{
    auto ff = alg == 0 ? math::factor_root
              : alg == 1 ? math::factor_ferma
              : alg == 2 ? math::factor_pollard_pm1
              : alg == 3 ? math::factor_pollard_rho
              : math::factor_root;

    Integer t4(4), t2(2), t1(1), t0(0);
    if ( n < t4 ) return {n};

    std::vector<Integer> v;
    while ( n != t1 )
    {
        auto p = ff(n);
        if ( p == t1 ) { v.push_back(n); break; }

        v += factors(p, alg);

        if (0) // sanity check
        {
            auto w = n / p;
            if (w * p != n) never;
        }

        n /= p;
    }

    std::sort(v.begin(), v.end());
    return v;
}

std::vector<Integer> math::factors(Integer n)
{
    // make automatic selection
    return factors(n, 0); // This proves the best for random numbers <64bits
}

Integer math::pow2(int d)
{
    Integer n(2);
    while (--d) n <<= 1;
    return n;
}

Integer math::pow10(int d)
{
    Integer n(1);
    while (--d) n *= Integer(10);
    return n;
}

int math::log2sharp(Integer n)
{
    if (isNeg(n)) nevers("negative");
    if (n == 1) return 0;
    int d = 0;
    while (1)
    {
        auto k = (n >> 1);
        if (isZero(k)) break;
        if ((k << 1) != n) nevers("not pow2");
        n = k;
        ++d;
    }
    return d;
}

void math::reduce_sub_emp(Integer & a, const Integer & m)
{
    auto m2 = (m - 1) / 2;
    auto n2 = m2 + 1 - m;

    while (a > m2) a -= m;
    while (a < n2) a += m;
}

int math::egcd(const int & a, const int & b, int & s, int & t)
{
    return egcdT<int>(a, b, s, t);
}

Integer math::egcd(const Integer & a, const Integer & b, Integer & s, Integer & t)
{
    return egcdT<Integer>(a, b, s, t);
}

Integer math::inverse(const Integer & a, const Integer & mod)
{
    Integer s, t;
    auto gcd = egcdT<Integer>(a, mod, s, t);
    if (gcd * gcd != Integer(1)) nevers("no inverse (Integer)");
    while (s < 0) s += mod;
    s /= gcd;
    return s % mod;
}

math::ull_t math::inverse(const ull_t & a, const ull_t & mod)
{
    sll_t s, t;
    auto gcd = egcdT<sll_t>(a, mod, s, t);
    if (gcd * gcd != 1ull ) nevers("no inverse (ull)");
    while (s < 0) s += mod;
    s /= gcd;
    return s % mod;
}
