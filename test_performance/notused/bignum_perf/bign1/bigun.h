#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <string>
#include <memory>

// including MPIR
#include "mpir.h"
#include "mpirxx.h"
#include "gmp-impl.h"

using std::string;

using ull_t = unsigned long long;
using sll_t = signed long long;

int mpir_impl(); // 0 or 1

struct BigunNative;

class Bigun;
std::istream & operator>>(std::istream & is, Bigun & x);
std::ostream & operator<<(std::ostream & os, const Bigun & x);

class Bigun
{
        std::shared_ptr<BigunNative> p;

    protected:

    public:
        explicit Bigun( sll_t = 0 );
        explicit Bigun( ull_t x );
        explicit Bigun( int x ): Bigun(sll_t(x)) {}
        explicit Bigun( unsigned x ): Bigun(ull_t(x)) {}
        Bigun(string x);

        Bigun(const Bigun & a);
        Bigun & operator=(const Bigun & a);

        Bigun(Bigun && a) noexcept;
        Bigun & operator=(Bigun && a) noexcept;

        static int maxbitsz();

        string str() const;
        string sth() const; // hex

        Bigun powmod(Bigun x, Bigun m) const;
        Bigun addmod(Bigun x, Bigun m) const;
        Bigun submod(Bigun x, Bigun m) const;
        Bigun mulmod(Bigun x, Bigun m) const;
        Bigun invmod(Bigun m) const;

        Bigun powmod(unsigned x, Bigun m) const { return powmod(Bigun(x), m); }

        bool isZero() const;
        explicit operator bool() { return !isZero(); }

        friend std::istream & operator>>(std::istream & is, Bigun & x);
        friend std::ostream & operator<<(std::ostream & os, const Bigun & x);

        Bigun & operator+=(const Bigun & a);
        Bigun & operator-=(const Bigun & a);
        Bigun & operator*=(const Bigun & a);
        Bigun & operator/=(const Bigun & a);
        Bigun & operator%=(const Bigun & a);
        Bigun & operator&=(const Bigun & a);
        Bigun & operator|=(const Bigun & a);
        Bigun & operator^=(const Bigun & a);
        Bigun & operator<<=(const Bigun & a);
        Bigun & operator>>=(const Bigun & a);

        Bigun & operator+=(int a) { return *this += Bigun(a); }

        Bigun & operator|=(int a) { return *this |= Bigun(sll_t(a)); }
        Bigun & operator<<=(int a) { return *this <<= Bigun(sll_t(a)); }
        Bigun & operator>>=(int a) { return *this >>= Bigun(sll_t(a)); }
        Bigun operator<<(int a) const { Bigun r(*this); return r <<= a; }
        Bigun operator>>(int a) const { Bigun r(*this); return r >>= a; }

        Bigun & operator++();
        Bigun & operator--();
        Bigun operator++(int) { Bigun x(*this); ++*this; return x; }
        Bigun operator--(int) { Bigun x(*this); --*this; return x; }
        Bigun operator-() const;
        Bigun operator+() const { return *this; }

        Bigun operator+(const Bigun & a) const { Bigun r(*this); return r += a; }
        Bigun operator-(const Bigun & a) const { Bigun r(*this); return r -= a; }
        Bigun operator*(const Bigun & a) const { Bigun r(*this); return r *= a; }
        Bigun operator/(const Bigun & a) const { Bigun r(*this); return r /= a; }
        Bigun operator%(const Bigun & a) const { Bigun r(*this); return r %= a; }
        Bigun operator&(const Bigun & a) const { Bigun r(*this); return r &= a; }
        Bigun operator|(const Bigun & a) const { Bigun r(*this); return r |= a; }
        Bigun operator^(const Bigun & a) const { Bigun r(*this); return r ^= a; }
        Bigun operator<<(const Bigun & a) const { Bigun r(*this); return r <<= a; }
        Bigun operator>>(const Bigun & a) const { Bigun r(*this); return r >>= a; }
        Bigun operator~() const;
        bool operator!() const { return isZero(); }
        bool operator==(const Bigun & a) const;
        bool operator!=(const Bigun & a) const { return !(*this == a); }
        bool operator<(const Bigun & a) const;
        bool operator>=(const Bigun & a) const { return !(*this < a); }
        bool operator>(const Bigun & a) const { return a < *this; }
        bool operator<=(const Bigun & a) const { return !(a < *this); }
        bool operator&&(const Bigun & a) const { return !!*this && !!a; }
        bool operator||(const Bigun & a) const { return !!*this || !!a; }

        bool operator<(int a) const { return *this < Bigun(a); }
        bool operator==(int a) const { return *this == Bigun(a); }
        bool operator!=(int a) const { return *this != Bigun(a); }
        Bigun & operator=(int a) { return *this = Bigun(a); }

        friend Bigun operator+(sll_t a, const Bigun & b) { return b + Bigun(a); }
        friend Bigun operator-(sll_t a, const Bigun & b) { return b - Bigun(a); }
        friend Bigun operator*(sll_t a, const Bigun & b) { return b * Bigun(a); }
        friend Bigun operator+(const Bigun & b, sll_t a) { return b + Bigun(a); }
        friend Bigun operator-(const Bigun & b, sll_t a) { return b - Bigun(a); }
        friend Bigun operator*(const Bigun & b, sll_t a) { return b * Bigun(a); }
        friend Bigun operator/(const Bigun & b, sll_t a) { return b / Bigun(a); }
        friend Bigun operator%(const Bigun & b, sll_t a) { return b % Bigun(a); }

        sll_t sll() const;
	double dbl() const;

        // bit access
        struct BitVal
        {
            bool v; // 0 or 1
            bool operator!() const { return !v; }
        };

        struct BitRef
        {
            Bigun * p = nullptr;
            int i;

            BitVal val() const;
            void setbit(BitVal b);

            BitRef operator=(BitVal b) { setbit(b); return *this; }
            BitRef operator=(int x) { setbit(BitVal {!!x}); return *this; }
            BitRef operator=(const BitRef & b) { setbit(b.val()); return *this; }

            bool operator!() const { return !val(); }
        };
        BitRef operator[](int i) { return BitRef {this, i}; }
        BitVal operator()(int i) const { Bigun t(*this); return t[i].val(); }
        friend Bigun operator*(BitVal a, const Bigun & b) { return (!a) ? Bigun(0) : b; }
        friend Bigun operator*(const BitRef & a, const Bigun & b) { return a.val() * b; }
};


inline std::ostream & operator<<(std::ostream & os, const Bigun & x) { return os << x.str(); }


struct BigunNative
{
    mpz_class n;
    BigunNative( unsigned long long x = 0): n(x) {}
    BigunNative( signed long long x): n(x) {}
    BigunNative( mpz_class a ): n(a) {}

    string str() const;
    BigunNative powmod(BigunNative x, BigunNative m) const;
    BigunNative mulmod(BigunNative x, BigunNative m) const;
    BigunNative invmod(BigunNative m) const;

    bool isZero() const;

    BigunNative & operator+=(const BigunNative & a) { n += a.n; return *this; }
    BigunNative & operator-=(const BigunNative & a) { n -= a.n; return *this; }
    BigunNative & operator*=(const BigunNative & a) { n *= a.n; return *this; }
    BigunNative & operator/=(const BigunNative & a) { n /= a.n; return *this; }
    BigunNative & operator%=(const BigunNative & a) { n %= a.n; return *this; }
    BigunNative & operator&=(const BigunNative & a) { n &= a.n; return *this; }
    BigunNative & operator|=(const BigunNative & a) { n |= a.n; return *this; }
    BigunNative & operator^=(const BigunNative & a) { n ^= a.n; return *this; }
    BigunNative & operator<<=(const BigunNative & a);
    BigunNative & operator>>=(const BigunNative & a);

    BigunNative & operator++() { ++n; return *this; }
    BigunNative & operator--() { --n; return *this; }
    BigunNative operator~() const { return BigunNative(~n); }
    BigunNative operator-() const { return BigunNative(-n); }

    bool operator==(const BigunNative & a) const { return n == a.n; }
    bool operator<(const BigunNative & a) const { return n < a.n; }

    unsigned long long ull() const { return n.get_ui(); }
    double dbl() const { return n.get_d(); }
};


using Integer = Bigun;

inline bool isNeg(Integer a) { return a < Integer(0); }
inline bool isZero(Integer a) { return a.isZero(); }
inline double todbl(Integer a) { return a.dbl(); }

namespace mod
{

inline Integer mul(const Integer & a, const Integer & b, const Integer & m) { return a.mulmod(b, m); }
inline Integer pow(const Integer & a, const Integer & b, const Integer & m) { return a.powmod(b, m); }
inline Integer inv(Integer a, Integer m) { return a.invmod(m); }

} // mod
