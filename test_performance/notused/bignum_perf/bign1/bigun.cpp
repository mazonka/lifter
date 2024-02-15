#include <sstream>

#include "bigun.h"     // also defines ull_t

using ull_t = unsigned long long;

string Bigun::str() const { return p->str(); }

Bigun::Bigun(sll_t x): p(new BigunNative(x)) {}
Bigun::Bigun(ull_t x): p(new BigunNative(x)) {}
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

Bigun Bigun::operator~() const { Bigun r(*this); *r.p = ~*r.p; return r; }
Bigun Bigun::operator-() const { Bigun r(*this); *r.p = -*r.p; return r; }
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

// ===============================================



int mpir_impl() { return 1; }

int Bigun::maxbitsz() { return 10000000; } // enough

Bigun::Bigun(string s) : Bigun(0)
{
    p->n = s;
}

std::istream & operator>>(std::istream & is, Bigun & x)
{
    string s;
    is >> s;
    x.p->n = s;
    return is;
}

BigunNative & BigunNative::operator<<=(const BigunNative & a)
{
    unsigned long c = a.n.get_ui();
    while (c--) n = n * 2;
    return *this;
}

BigunNative & BigunNative::operator>>=(const BigunNative & a)
{
    unsigned long c = a.n.get_ui();
    while (c--) n = n / 2;
    return *this;
}


static inline mpz_class mulmod(mpz_class a, mpz_class b, mpz_class m)
{
    return a * b % m;
}

static mpz_class powmod(const mpz_class & x, const mpz_class & p, const mpz_class & m)
{
    if ( p == 0 ) return 1;
    if ( p == 1 ) return mulmod(x, 1, m);
    if ( p == 2 ) return mulmod(x, x, m);

    auto y = powmod(x, p / 2, m);
    auto z = mulmod(y, y, m);

    if ( (p % 2) == 0 ) return z;

    return mulmod(x, z, m);
}

static inline mpz_class invmod(mpz_class a, mpz_class m)
{
    mpz_class rop;
    if ( mpz_invert (rop.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t())  ) return rop;
    return mpz_class(0);
}

string BigunNative::str() const { return n.get_str(); }

BigunNative BigunNative::powmod(BigunNative x, BigunNative m) const
{
    auto z = ::powmod(n, x.n, m.n);
    return BigunNative(z);
}

BigunNative BigunNative::mulmod(BigunNative x, BigunNative m) const
{
    return BigunNative(::mulmod(n, x.n, m.n));
}

BigunNative BigunNative::invmod(BigunNative m) const
{
    return BigunNative(::invmod(n, m.n));
}

bool BigunNative::isZero() const
{
    return 0 == mpz_cmp_ui(n.get_mpz_t(), 0);
}

string Bigun::sth() const
{
    static const char ch[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    std::vector<char> stk;

    Bigun x(*this);

    while (!x.isZero())
    {
        auto k = x % 16;
        int q = 0;
        for ( int j = 0; j < 4; j++ ) { int p = 1 << j; q |= (k & Bigun(p)).isZero() ? 0 : p; }
        stk.push_back(ch[q]);
        x >>= 4;
    }

    if ( stk.empty() ) return "0";

    string r;
    for ( auto i = stk.size(); i > 0; ) r += stk[--i];
    return r;
}

sll_t Bigun::sll() const
{
    return (sll_t)this->p->ull();
}

double Bigun::dbl() const
{
    return this->p->dbl();
}
