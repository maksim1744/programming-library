template<typename T>
struct Rational {
    T a, b;
    Rational(T a = 0, T b = 1) : a(a), b(b) { norm(); }

    friend Rational<T>& operator += (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b + n.b * m.a, n.b * m.b); }
    friend Rational<T>  operator +  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res += m; }
    friend Rational<T>& operator -= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b - n.b * m.a, n.b * m.b); }
    friend Rational<T>  operator -  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res -= m; }
    friend Rational<T>  operator -  (const Rational<T> &n)                       { return Rational{-n.a, n.b}; }
    friend Rational<T>& operator *= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.a, n.b * m.b); }
    friend Rational<T>  operator *  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res *= m; }
    friend Rational<T>& operator /= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b, n.b * m.a); }
    friend Rational<T>  operator /  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res /= m; }

    friend bool operator == (const Rational<T> &n, const Rational<T> &m) { return n.a == m.a && n.b == m.b; }
    friend bool operator != (const Rational<T> &n, const Rational<T> &m) { return n.a != m.a || n.b != m.b; }

    void norm() {
        if (b < 0) {
            a = -a;
            b = -b;
        }
        T d = gcd((a < 0 ? -a : a), b);
        a /= d;
        b /= d;
    }
};

template<typename T>
Rational<T> pow(Rational<T> a, ll p) {
    Rational<T> res = 1;
    while (p) {
        if (p & 1) res *= a;
        a *= a;
        p >>= 1;
    }
    return res;
}

template<typename T> ostream& operator << (ostream &o, const Rational<T> &m) { return o << m.a << '/' << m.b; }
template<typename T> istream& operator >> (istream &i,       Rational<T> &m) { T a, b = 1; i >> a; if (i.peek() == '/') i.get(), i >> b; m = Rational<T>(a, b); return i; }
