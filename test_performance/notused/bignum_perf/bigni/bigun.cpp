#include "bigun.h"
#include "../mathut.h" // needs ull_t defined

static Integer mod_mul_rec(Integer a, Integer b, Integer m)
{
    if (b == 0) return 0;
    if (b == 1) return a % m;

    a %= m;
    b %= m;

    auto y = (a << 1);
    auto q = (b >> 1);
    y = mod_mul_rec(y, q, m);
    if (b == (q << 1)) return y;
    return (y + a) % m;
}

static Integer mod_mul_max(Integer a, Integer b, Integer m)
{
    if (a > b) return mod_mul_rec(a, b, m);
    return mod_mul_rec(b, a, m);
}

Integer mod::mul(Integer a, Integer b, Integer m)
{
    if (1) // optimize for faster
        if ( a >> 30 == 0 && b >> 30 == 0 ) return a * b % m;

    auto neg = [&m](Integer x)
    {
        if ( isZero(x) ) return x;
        return m - x;
    };

    if ( isNeg(a) )
    {
        if ( isNeg(b) )
            return mod::mul(-a, -b, m);
        else
            return neg(mod::mul(-a, b, m));
    }
    else if ( isNeg(b) ) return neg(mod::mul(a, -b, m));

    return mod_mul_max(a, b, m);
}


Integer mod::pow(Integer x, Integer p, Integer m)
{
    if ( p == 0 ) return 1;
    if ( p == 1 ) return mod::mul(x, 1, m);
    if ( p == 2 ) return mod::mul(x, x, m);

    auto y = mod::pow(x, p / 2, m);
    auto z = mod_mul_rec(y, y, m);

    if ( !(p % 2) ) return z;

    return mod_mul_max(x, z, m);
}

Integer mod::inv(Integer a, Integer mod)
{
    return math::inverse(a, mod);
}
