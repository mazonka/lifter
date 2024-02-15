template <class T>
T egcdT_rec(const T& a, const T& b, T& s0, T& t0, T s1, T t1, T s2, T t2)
{
    auto c = a % b;
    if (c == T(0)) return b;
    auto q = (a - c) / b; // exact division
    s0 = s2 - s1 * q;
    t0 = t2 - t1 * q;
    return egcdT_rec(b, c, s0, t0, s0, t0, s1, t1);
}

template <class T>
T egcdT(const T& a, const T& b, T& s0, T& t0)
{
    T s1(0), t1(1), s2(1), t2(0);
    return egcdT_rec(a, b, s0, t0, s1, t1, s2, t2);
}
