const long double eps = 1e-12;
const long double pi = 3.14159265358979323;

// when using big integers mod = 10000 works better than 100000 and bigger;
// using Complex = complex<long double>;  // slower than my Complex

namespace fft {
struct Complex {
    long double x, y;

    Complex() : x(0), y(0) {}
    Complex(long double _x, long double _y) : x(_x), y(_y) {}
    Complex(long double _x) : x(_x), y(0) {}
    Complex(const Complex& c) : x(c.x), y(c.y) {}
};

Complex operator - (Complex& a) {
    return Complex(-a.x, -a.y);
}

Complex operator + (Complex& a, Complex&& b) {
    return Complex(a.x + b.x, a.y + b.y);
}

Complex operator * (Complex& a, Complex& b) {
    return Complex(a.x * b.x - a.y * b.y, a.x * b.y + b.x * a.y);
}

template<typename T>
vector<Complex> fft(vector<T>& v, bool good = false) {
    if (!good) {
        int p2 = 1;
        while (p2 < v.size()) {
            p2 <<= 1;
        }
        v.resize(p2);
        for (int i = v.size(); i < p2; ++i) {
            v[i] = 0;
        }
    }
    if (v.size() == 1) {
        return vector<Complex> {Complex(v[0])};
    }
    int n = v.size() / 2;
    vector<Complex> v1(n * 2);
    int k = 0, n1 = 2 * n;
    while (n1 > 1) {
        n1 >>= 1;
        ++k;
    }
    vector<Complex> w(2 * n);
    w[0] = 1;
    w[1] = Complex(cosl(pi / n), sinl(pi / n));
    for (int i = 2; i < 2 * n; ++i) {
        if (i >= n) {
            w[i] = -w[i - n];
        } else {
            w[i] = w[i / 2] * w[i - i / 2];
        }
    }
    vector<int> ind(2 * n, 0);
    ind[1] = 1 << (k - 1);
    int id = 2;
    for (int i = 1; i < k; ++i) {
        int z = (1 << i);
        for (int j = 0; j < z; ++j) {
            ind[id] = (ind[id - z]) + (1 << (k - 1 - i));
            ++id;
        }
    }
    for (int i = 0; i < 2 * n; ++i) {
        v1[i] = Complex(v[ind[i]]);
    }
    for (int i = k - 1; i >= 0; --i) {
        int cnt = (1 << i);
        int sz = (1 << (k - i));
        int k1 = sz / 2 - 1;
        for (int j = 0; j < cnt; ++j) {
            int k2 = sz * j;
            vector<Complex> va(v1.begin() + sz * j, v1.begin() + sz * j + sz / 2);
            vector<Complex> vb(v1.begin() + sz * j + sz / 2, v1.begin() + sz * j + sz);
            for (int u = 0; u < sz; ++u) {
                v1[k2 + u] = va[u & k1] + w[u << i] * vb[u & k1];
            }
        }
    }
    return v1;
}

template<typename T>
vector<T> multiply(vector<T>& a, vector<T>& b) {
    // if (a.size() <= 2 || b.size() <= 2) {
    //     vector<T> c(a.size() + b.size() - 1, 0);
    //     for (int i = 0; i < a.size(); ++i) {
    //         for (int j = 0; j < b.size(); ++j) {
    //             c[i + j] += a[i] * b[j];
    //         }
    //     }
    //     while (c.size() > 0 && c.back() == 0) {
    //         c.pop_back();
    //     }
    //     return c;
    // }
    int k = a.size() + b.size() - 1;
    int p2 = 1;
    while (p2 < k) {
        p2 <<= 1;
    }
    int oldasize = a.size();
    int oldbsize = b.size();
    a.resize(p2);
    for (int i = oldasize; i < p2; ++i) {
        a[i] = 0;
    }
    b.resize(p2);
    for (int i = oldbsize; i < p2; ++i) {
        b[i] = 0;
    }
    int n = p2;
    vector<Complex> va = fft(a, true), vb = fft(b, true);
    for (int i = 0; i < n; ++i) {
        va[i] = va[i] * vb[i];
    }
    vector<Complex> ansC = fft(va, true);
    vector<T> ans(n);
    for (int i = 0; i < n; ++i) {
        if (typeid(T) == typeid(int) || typeid(T) == typeid(long long)) {
            ans[i] = (long long)round(ansC[i].x / (long double)(1.0 * n));  // if T is int
        } else {
            ans[i] = ansC[i].x / (1.0 * n);  // if T is real
            if (ans[i] < eps) {
                ans[i] = 0;
            }
        }
    }
    for (int i = 1; i <= n / 2; ++i) {
        swap(ans[i], ans[n - i]);
    }
    while (ans.size() > 0 && ans.back() == 0) {
        ans.pop_back();
    }
    return ans;
}

template<typename T>
void square(vector<T>& a) {
    int k = a.size() * 2 - 1;
    int p2 = 1;
    while (p2 < k) {
        p2 *= 2;
    }
    int oldasize = a.size();
    a.resize(p2);
    for (int i = oldasize; i < p2; ++i) {
        a[i] = 0;
    }
    int n = p2;
    vector<Complex> va = fft(a, true);
    for (int i = 0; i < n; ++i) {
        va[i] = va[i] * va[i];
    }
    vector<Complex> ansC = fft(va, true);
    for (int i = 0; i < n; ++i) {
        if (typeid(T) == typeid(int) || typeid(T) == typeid(long long)) {
            a[i] = (long long)round(ansC[i].x / (long double)(1.0 * n));  // if T is int
        } else {
            a[i] = ansC[i].x / (1.0 * n);  // if T is real
            if (a[i] < eps) {
                a[i] = 0;
            }
        }
    }
    for (int i = 1; i <= n / 2; ++i) {
        swap(a[i], a[n - i]);
    }
    while (a.size() > 0 && a.back() == 0) {
        a.pop_back();
    }
}
}  // fft
