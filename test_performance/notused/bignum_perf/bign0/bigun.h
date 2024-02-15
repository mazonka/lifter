#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <string>
#include <memory>

using std::string;

using ull_t = unsigned long long;

int mpir_impl(); // 0 or 1

struct BigunNative;

class Bigun;
std::istream & operator>>(std::istream & is, Bigun & x);
std::ostream & operator<<(std::ostream & os, const Bigun & x);

class Bigun
{
        std::shared_ptr<BigunNative> p;

    protected:

        using ull_t = unsigned long long;

    public:
        explicit Bigun( ull_t = 0 );
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

        Bigun & operator|=(int a) { return *this |= Bigun(ull_t(a)); }
        Bigun & operator<<=(int a) { return *this <<= Bigun(ull_t(a)); }
        Bigun & operator>>=(int a) { return *this >>= Bigun(ull_t(a)); }
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

        friend Bigun operator+(ull_t a, const Bigun & b) { return b + Bigun(a); }
        friend Bigun operator-(ull_t a, const Bigun & b) { return b - Bigun(a); }
        friend Bigun operator*(ull_t a, const Bigun & b) { return b * Bigun(a); }
        friend Bigun operator+(const Bigun & b, ull_t a) { return b + Bigun(a); }
        friend Bigun operator-(const Bigun & b, ull_t a) { return b - Bigun(a); }
        friend Bigun operator*(const Bigun & b, ull_t a) { return b * Bigun(a); }
        friend Bigun operator/(const Bigun & b, ull_t a) { return b / Bigun(a); }
        friend Bigun operator%(const Bigun & b, ull_t a) { return b % Bigun(a); }

        ull_t ull() const;
        double dbl() const { return double(ull()); }

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
    using ull_t = unsigned long long;
    ull_t n;
    BigunNative(unsigned long long x = 0): n(x) {}

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
    BigunNative & operator<<=(const BigunNative & a) { n <<= a.n; return *this; }
    BigunNative & operator>>=(const BigunNative & a) { n >>= a.n; return *this; }

    BigunNative & operator++() { ++n; return *this; }
    BigunNative & operator--() { --n; return *this; }
    BigunNative operator~() const { return ~n; }
    BigunNative operator-() const { return 0ull - n; }

    bool operator==(const BigunNative & a) const { return n == a.n; }
    bool operator<(const BigunNative & a) const { return n < a.n; }

    ull_t ull() const { return n; }
};


class Integer
{
        Bigun x;
        int s;
    public:
        explicit Integer( int a = 0 ) : Integer((long long)a)  {}
        explicit Integer( ull_t a ) : x(a), s(1) {}
        explicit Integer( signed long long a ) : x(0), s(0)
        {
            if (a > 0) { x = Bigun(ull_t(a)); s = 1; }
            else if ( a < 0 ) { s = -1; x = Bigun(ull_t(-a)); }
        }

        Integer operator+(const Integer & a) const { Integer r(*this); return r += a; }
        Integer operator-(const Integer & a) const { Integer r(*this); return r -= a; }
        Integer operator*(const Integer & a) const { Integer r(*this); return r *= a; }
        Integer operator/(const Integer & a) const { Integer r(*this); return r /= a; }
        Integer operator%(const Integer & a) const { Integer r(*this); return r %= a; }

        Integer & operator+=(const Integer & a);
        Integer & operator-=(const Integer & a) { return *this += -a; }
        Integer & operator*=(const Integer & a);
        Integer & operator/=(const Integer & a);
        Integer & operator%=(const Integer & a);

        Integer & operator+=(int a) { return *this += Integer {a}; }

        Integer & operator&=(const Integer & a);
        Integer & operator|=(const Integer & a);
        Integer & operator^=(const Integer & a);
        Integer & operator<<=(const Integer & a);
        Integer & operator>>=(const Integer & a);
        Integer & operator|=(int a) { return *this |= Integer(ull_t(a)); }
        Integer & operator<<=(int a) { return *this <<= Integer(ull_t(a)); }
        Integer & operator>>=(int a) { return *this >>= Integer(ull_t(a)); }
        Integer operator<<(int a) const { Integer r(*this); return r <<= a; }
        Integer operator>>(int a) const
        {
            Integer r(*this);
            r >>= a;
            if (r.x.isZero()) r.s = 0;
            return r;
        }


        bool operator!() const { return x.isZero(); }
        bool operator==(const Integer & a) const { return x == a.x && s == a.s; }
        bool operator!=(const Integer & a) const { return !(*this == a); }
        bool operator<(const Integer & a) const;
        bool operator>=(const Integer & a) const { return !(*this < a); }
        bool operator>(const Integer & a) const { return a < *this; }
        bool operator<=(const Integer & a) const { return !(a < *this); }
        bool operator&&(const Integer & a) const { return !!*this && !!a; }
        bool operator||(const Integer & a) const { return !!*this || !!a; }

        bool operator<(int a) const { return *this < Integer(a); }
        bool operator==(int a) const { return *this == Integer(a); }
        bool operator!=(int a) const { return *this != Integer(a); }
        Integer & operator=(int a) { return *this = Integer(a); }

        bool isNeg() const { return s == -1; }
        bool isZero() const { return s == 0; }
        Bigun abs() const { return x; }
        int sign() const { return s; }
        double dbl() const { return isZero() ? 0 : isNeg() ? -x.dbl() : x.dbl(); }

        friend Integer operator+(int a, const Integer & b) { return b + Integer(a); }
        friend Integer operator-(int a, const Integer & b) { return b - Integer(a); }
        friend Integer operator*(int a, const Integer & b) { return b * Integer(a); }
        friend Integer operator+(const Integer & b, int a) { return b + Integer(a); }
        friend Integer operator-(const Integer & b, int a) { return b - Integer(a); }
        friend Integer operator*(const Integer & b, int a) { return b * Integer(a); }
        friend Integer operator/(const Integer & b, int a) { return b / Integer(a); }
        friend Integer operator%(const Integer & b, int a) { return b % Integer(a); }

        Integer & operator++();
        Integer & operator--();
        Integer operator++(int) { Integer x(*this); ++*this; return x; }
        Integer operator--(int) { Integer x(*this); --*this; return x; }
        Integer operator-() const { Integer r(*this); r.s = -s; return r; }
        Integer operator+() const { return *this; }

        Integer mulmod_ret(Integer a, const Integer & m) const;
        Integer powmod_ret(Integer a, const Integer & m) const { a.x = x.powmod(a.x, m.x); return a; }
        Integer invmod_ret(const Integer & m) const { return Integer(x.invmod(m.x).ull()); }
};

inline std::ostream & operator<<(std::ostream & os, const Integer & x)
{ return os << (x.isNeg() ? "-" : "") << x.abs().str(); }
//{ return os << (x.isNeg() ? "-" : "") << x.abs().str() <<':'<< x.sign(); }


inline bool isNeg(Integer a) { return a.isNeg(); }
inline bool isZero(Integer a) { return a.isZero(); }
void divABRQ(const Integer & a, const Integer & b, Integer * r, Integer * q);

inline double todbl(Integer a) { return a.dbl(); }

namespace mod
{

inline Integer mul(const Integer & a, const Integer & b, const Integer & m) { return a.mulmod_ret(b, m); }
inline Integer pow(const Integer & a, const Integer & b, const Integer & m) { return a.powmod_ret(b, m); }
inline Integer inv(Integer a, Integer m) { return a.invmod_ret(m); }

} // mod
