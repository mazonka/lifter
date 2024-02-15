
#include <sstream>

#include "bigun.h"     // also defines ull_t
#include "../mathut.h"

string Bigun::str() const { return p->str(); }

Bigun::Bigun(unsigned long long x): p(new BigunNative(x)) {}
Bigun::Bigun(const Bigun & a) : Bigun() { *p = *a.p; }
Bigun::Bigun(Bigun && a) noexcept : p(nullptr)  { p.swap(a.p); }
Bigun & Bigun::operator=(const Bigun & a) { *p = *a.p; return *this; }
Bigun & Bigun::operator=(Bigun && a) noexcept { p.swap(a.p); return *this; }


Bigun & Bigun::operator+=(const Bigun & a) { *p += *a.p; return *this; }
Bigun & Bigun::operator-=(const Bigun & a) { *p -= *a.p; return *this; }
Bigun & Bigun::operator*=(const Bigun & a) { *p *= *a.p; return *this; }
Bigun & Bigun::operator/=(const Bigun & a) { *p /= *a.p; return *this; }
Bigun & Bigun::operator%=(const Bigun & a) { *p %= *a.p; return *this; }
Bigun & Bigun::operator|=(const Bigun & a) { *p |= *a.p; return *this; }
Bigun & Bigun::operator^=(const Bigun & a) { *p ^= *a.p; return *this; }
Bigun & Bigun::operator&=(const Bigun & a) { *p &= *a.p; return *this; }

Bigun & Bigun::operator<<=(const Bigun & a) { *p <<= *a.p; return *this; }
Bigun & Bigun::operator>>=(const Bigun & a) { *p >>= *a.p; return *this; }

bool Bigun::operator==(const Bigun & a) const { return *p == *a.p; }
bool Bigun::operator<(const Bigun & a) const { return *p < *a.p; }

Bigun Bigun::operator~() const { *p = ~*p; return *this; }
Bigun Bigun::operator-() const { *p = -*p; return *this; }
Bigun & Bigun::operator--() { --*p; return *this; }
Bigun & Bigun::operator++() { ++*p; return *this; }


Bigun Bigun::powmod(Bigun x, Bigun y) const
{
    Bigun r;
    *r.p = this->p->powmod(*x.p, *y.p);
    return r;
}

Bigun Bigun::addmod(Bigun x, Bigun m) const
{
    Bigun r(*this);
    *r.p += *x.p;
    return r %= m;
}

Bigun Bigun::mulmod(Bigun x, Bigun m) const
{
    Bigun r;
    *r.p = this->p->mulmod(*x.p, *m.p);
    return r;
}

Bigun Bigun::submod(Bigun x, Bigun m) const
{
    Bigun r(*this);
    if ( *p < *x.p ) return r += (m - x);
    return r -= x;
}

Bigun Bigun::invmod(Bigun m) const
{
    Bigun r;
    *r.p = this->p->invmod(*m.p);
    return r;
}

bool Bigun::isZero() const
{
    return this->p->isZero();
}

Bigun::ull_t Bigun::ull() const
{
    return this->p->ull();
}

// ================================================

int mpir_impl() { return 0; }

int Bigun::maxbitsz() { return 8 * sizeof(ull_t) - 1; }

Bigun::Bigun(string s) : Bigun(0)
{
    std::istringstream is(s);
    is >> (*this);
}

std::istream & operator>>(std::istream & is, Bigun & x)
{
    ull_t z;
    is >> z;
    *x.p = z;
    return is;
}

static ull_t mulmod(ull_t a, ull_t b, ull_t m)
{
    if ( b == 0 ) return 0;
    if ( b == 1 ) return a % m;

    auto y = b / 2;
    auto c = a * 2 % m;
    auto z = mulmod(c, y, m);

    if ( !(b % 2) ) return z;

    return (z + a) % m;
}

static ull_t powmod(ull_t x, ull_t p, ull_t m)
{
    if ( p == 0 ) return 1;
    if ( p == 1 ) return mulmod(x, 1, m);
    if ( p == 2 ) return mulmod(x, x, m);

    auto y = powmod(x, p / 2, m);
    auto z = mulmod(y, y, m);

    if ( !(p % 2) ) return z;

    return mulmod(x, z, m);
}

static ull_t invmod(ull_t x, ull_t m)
{
    ///ull_t r;
    ///if ( math::invert<ull_t>(x % m, m, &r) ) return r;
    ///return 0;
    return math::inverse(x % m, m);
}


// ================================================


string BigunNative::str() const { return std::to_string(n); }

BigunNative BigunNative::powmod(BigunNative x, BigunNative m) const
{
    return BigunNative(::powmod(n, x.n, m.n));
}

BigunNative BigunNative::mulmod(BigunNative x, BigunNative m) const
{
    return BigunNative(::mulmod(n, x.n, m.n));
}

BigunNative BigunNative::invmod(BigunNative m) const
{
    return BigunNative(::invmod(n, m.n));
}

bool BigunNative::isZero() const { return n == 0; }

// ================================================

Integer & Integer::operator+=(Integer const & b)
{
    if ( b.s == 0 ) {}
    else if ( s == 0 ) *this = b;
    else if ( s == b.s ) { x += b.x; }
    else if ( s * b.s == -1 )
    {
        if ( x > b.x ) {x -= b.x;}
        else if ( x < b.x ) { x = b.x - x; s = b.s; }
        else *this = Integer(0);
    }

    return *this;
}


Integer & Integer::operator*=(Integer const & b)
{
    x *= b.x;
    s *= b.s;
    return *this;
}

Integer Integer::mulmod_ret(Integer a, const Integer & m) const
{
    Integer t0(0);
    auto y = *this;
    while (y < t0) y += m;
    while (a < t0) a += m;
    a.x = y.x.mulmod(a.x, m.x);
    ///a.s *= s;
    ///a.s = a.x.isZero() ? 0 : a.s;
    a.s = a.x.isZero() ? 0 : 1;
    return a;
}

Integer & Integer::operator/=(Integer const & b)
{
    x /= b.x;
    s *= b.s;
    if (x.isZero()) s = 0;
    return *this;
}

Integer & Integer::operator%=(Integer const & b)
{
    x %= b.x;
    s *= b.s;
    if (x.isZero()) s = 0;
    return *this;
}

bool Integer::operator<(const Integer & a) const
{
    if ( s < a.s ) return true;
    if ( s > a.s ) return false;
    return s < 0 ? a.x < x : x < a.x;
}


Integer & Integer::operator<<=(const Integer & a) { x <<= a.x; return *this; }
Integer & Integer::operator>>=(const Integer & a) { x >>= a.x; return *this; }

Integer & Integer::operator++()
{
    if ( s == 0 ) *this = Integer(1);
    else if ( s < 0 ) --x;
    else ++x;
    return *this;
}

Integer & Integer::operator--()
{
    if ( s < 0 ) ++x;
    else --x;
    if (x.isZero()) s = 0;
    return *this;
}


