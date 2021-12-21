template<typename T>
struct Rational {
    T a, b;
    Rational(T a = 0, T b = 1) : a(a), b(b) { norm(); }

    T floor() const { T res = a / b; if (a < 0 && a != b * res) --res; return res; }
    T ceil()  const { T res = a / b; if (a > 0 && a != b * res) ++res; return res; }

    friend Rational<T>& operator += (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b + n.b * m.a, n.b * m.b); }
    friend Rational<T>  operator +  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res += m; }
    friend Rational<T>& operator -= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b - n.b * m.a, n.b * m.b); }
    friend Rational<T>  operator -  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res -= m; }
    friend Rational<T>  operator -  (const Rational<T> &n)                       { return Rational{-n.a, n.b}; }
    friend Rational<T>& operator *= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.a, n.b * m.b); }
    friend Rational<T>  operator *  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res *= m; }
    friend Rational<T>& operator /= (      Rational<T> &n, const Rational<T> &m) { return n = Rational(n.a * m.b, n.b * m.a); }
    friend Rational<T>  operator /  (const Rational<T> &n, const Rational<T> &m) { auto res = n; return res /= m; }

    // doesn't overflow
    friend bool operator <  (Rational<T> n, Rational<T> m) {
        while (true) {
            T a = n.floor(), b = m.floor(); if (a != b) return a < b;
            n -= a; m -= b;
            if (n.b == m.b) return (n.a < m.a);
            if (n.a == 0) return true; else if (m.a == 0) return false;
            swap(n, m); swap(n.a, n.b); swap(m.a, m.b);
        }
        return false;
    }
    // maybe faster?
    // friend bool operator <  (const Rational<T> &n, const Rational<T> &m) { return n.a * m.b < n.b * m.a; }
    friend bool operator <= (const Rational<T> &n, const Rational<T> &m) { return !(m < n); }
    friend bool operator >  (const Rational<T> &n, const Rational<T> &m) { return   m < n ; }
    friend bool operator >= (const Rational<T> &n, const Rational<T> &m) { return !(n < m); }

    friend bool operator == (const Rational<T> &n, const Rational<T> &m) { return n.a == m.a && n.b == m.b; }
    friend bool operator != (const Rational<T> &n, const Rational<T> &m) { return n.a != m.a || n.b != m.b; }

    static T gcd(T a, T b) {
        while (b != 0) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

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
