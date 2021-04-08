const int basen = 9;
const int base = pow(10, basen);

// uncomment to multiply large numbers with fft
// basen should be even, at most 8
// with doubles in fft and basen = 8 works up to Big.v.size() = 1e6 (but fails for 1e6 + 5e4)
// #define BIGINT_USE_FFT

// division works in n^2 * log(base), where n = Big.v.size()

struct Big {
    vector<int> v;
    bool minus = false;

    Big() {}
    Big(long long k) {
        if (k < 0) {
            minus = true;
            k = -k;
        }
        while (k) {
            v.push_back(k % base);
            k /= base;
        }
    }
    Big(string s) {
        if (s[0] == '-') {
            s.erase(s.begin());
            minus = true;
        }
        reverse(s.begin(), s.end());
        while (s.size() % basen != 0)
            s.push_back('0');
        reverse(s.begin(), s.end());
        for (int i = 0; i < s.size(); i += basen)
            v.push_back(stoi(s.substr(i, basen)));
        reverse(v.begin(), v.end());
        norm();
    }

    Big &operator += (const Big &other) {
        if (minus == other.minus) {
            _add_(v, other.v);
        } else {
            if (_comp_(other.v, v)) {
                _sub_(v, other.v);
            } else {
                _sub2_(v, other.v);
                minus ^= 1;
            }
        }
        norm();
        return *this;
    }
    Big operator + (const Big &other) const {
        auto res = *this;
        return res += other;
    }

    Big operator - () const {
        Big res = *this;
        if (!v.empty()) res.minus ^= 1;
        return res;
    }
    Big &operator -= (const Big &other) {
        return *this += -other;
    }
    Big operator - (const Big &other) const {
        auto res = *this;
        return res -= other;
    }

    Big operator * (const Big &other) const {
        if (v.empty() || other.v.empty()) return 0;
        Big res;
        res.v = _mult_(v, other.v);
        res.minus = minus ^ other.minus;
        return res;
    }
    Big &operator *= (const Big &other) {
        return *this = *this * other;
    }

    Big operator / (const Big &other) const {
        Big res;
        res.v = _div_(v, other.v).first;
        res.minus = minus ^ other.minus;
        res.norm();
        return res;
    }
    Big &operator /= (const Big &other) {
        return *this = *this / other;
    }

    Big operator % (const Big &other) const {
        Big res;
        res.v = _div_(v, other.v).second;
        res.minus = minus ^ other.minus;
        res.norm();
        return res;
    }
    Big &operator %= (const Big &other) {
        return *this = *this % other;
    }

    int operator % (int m) const {
        long long p = 1;
        long long res = 0;
        for (int k : v) {
            res += k * p % m;
            p = p * base % m;
        }
        return res % m;
    }

    void norm() {
        while (!v.empty() && v.back() == 0)
            v.pop_back();
        if (v.empty())
            minus = false;
    }

    bool operator < (const Big &other) const {
        if (minus != other.minus) return minus;
        if (minus) return _comp_(other.v, v);
        else return _comp_(v, other.v);
    }
    bool operator > (const Big &other) const {
        return other < *this;
    }
    bool operator <= (const Big &other) const {
        return !(other < *this);
    }
    bool operator >= (const Big &other) const {
        return !(*this < other);
    }

    bool operator == (const Big &other) const {
        return minus == other.minus && v == other.v;
    }
    bool operator != (const Big &other) const {
        return !(*this == other);
    }

  private:
    static void _sub_(vector<int> &a, const vector<int> &b) {
        a.resize(max(a.size(), b.size()) + 1, 0);
        for (int i = 0; i < b.size(); ++i)
            a[i] -= b[i];
        for (int i = 0; i + 1 < a.size(); ++i) {
            if (a[i] < 0) {
                a[i] += base;
                --a[i + 1];
            }
        }
        assert(a.back() >= 0);
        while (!a.empty() && a.back() == 0)
            a.pop_back();
    }

    static void _sub2_(vector<int> &a, const vector<int> &b) {
        a.resize(max(a.size(), b.size()) + 1, 0);
        for (int i = 0; i < a.size(); ++i)
            a[i] = (i < b.size() ? b[i] : 0) - a[i];
        for (int i = 0; i + 1 < a.size(); ++i) {
            if (a[i] < 0) {
                a[i] += base;
                --a[i + 1];
            }
        }
        assert(a.back() >= 0);
        while (!a.empty() && a.back() == 0)
            a.pop_back();
    }

    static void _add_(vector<int> &a, const vector<int> &b) {
        a.resize(max(a.size(), b.size()) + 1, 0);
        for (int i = 0; i < b.size(); ++i)
            a[i] += b[i];
        for (int i = 0; i + 1 < a.size(); ++i) {
            if (a[i] >= base) {
                a[i] -= base;
                ++a[i + 1];
            }
        }
        while (!a.empty() && a.back() == 0)
            a.pop_back();
    }

    static bool _comp_(const vector<int> &a, const vector<int> &b) {
        if (a.size() != b.size())
            return a.size() < b.size();
        for (int i = (int)a.size() - 1; i >= 0; --i)
            if (a[i] != b[i])
                return a[i] < b[i];
        return false;
    }

    static vector<int> _mult_(const vector<int> &a, const vector<int> &b) {
        #ifdef BIGINT_USE_FFT
        // tested on a.v.size() = 1e6, b.v.size() = C, fft is better on C > ~500 : https://ideone.com/kSYLd8
        // if a.v.size() = b.v.size() = C, it's 380 : https://ideone.com/MJTo1Y
        if (min(a.size(), b.size()) > 380) {
            return _fft_mult_(a, b);
        }
        #endif

        return _slow_mult_(a, b);
    }

    static vector<int> _slow_mult_(const vector<int> &a, const vector<int> &b) {
        vector<long long> tmp(a.size() + b.size() + 1, 0);
        for (int i = 0; i < a.size(); ++i) {
            for (int j = 0; j < b.size(); ++j) {
                long long prod = 1ll * a[i] * b[j];
                long long div = prod / base;
                tmp[i + j] += prod - base * div;
                tmp[i + j + 1] += div;
            }
        }
        for (int i = 0; i + 1 < tmp.size(); ++i) {
            long long div = tmp[i] / base;
            tmp[i + 1] += div;
            tmp[i] -= div * base;
        }
        while (!tmp.empty() && tmp.back() == 0)
            tmp.pop_back();
        return vector<int>(tmp.begin(), tmp.end());
    }

    #ifdef BIGINT_USE_FFT
    static vector<int> _fft_mult_(const vector<int> &a, const vector<int> &b) {
        vector<int> ta(a.size() * 2), tb(b.size() * 2);
        static_assert(basen % 2 == 0, "basen has to be even");
        const static int M = pow(10, basen / 2);
        for (int i = 0; i < a.size(); ++i) {
            ta[i * 2] = a[i] % M;
            ta[i * 2 + 1] = a[i] / M;
        }
        for (int i = 0; i < b.size(); ++i) {
            tb[i * 2] = b[i] % M;
            tb[i * 2 + 1] = b[i] / M;
        }
        auto tc = fft::multiply(ta, tb);
        tc.resize(tc.size() / 2 * 2 + 10, 0);
        for (int i = 0; i + 1 < tc.size(); ++i) {
            tc[i + 1] += tc[i] / M;
            tc[i] %= M;
        }
        vector<int> res(tc.size() / 2);
        for (int i = 0; i < res.size(); ++i)
            res[i] = tc[i * 2] + tc[i * 2 + 1] * M;
        while (!res.empty() && res.back() == 0)
            res.pop_back();
        return res;
    }
    #endif

    static pair<vector<int>, vector<int>> _div_(vector<int> a, vector<int> b) {
        if (a.size() < b.size()) {
            return {{}, a};
        }
        vector<int> res;
        vector<int> c, c2;
        for (int i = (int)a.size() - b.size(); i >= 0; --i) {
            c.resize(b.size() + i);
            for (int j = 0; j < b.size(); ++j) {
                c[i + j] = b[j];
            }
            int L = 0, R = base;
            while (R - L > 1) {
                int C = (L + R) / 2;
                c2 = _mult_(c, {C});
                if (_comp_(a, c2)) {
                    R = C;
                } else {
                    L = C;
                }
            }
            c = _mult_(c, {L});
            _sub_(a, c);
            res.push_back(L);
        }
        reverse(res.begin(), res.end());
        return {res, a};
    }
};

string to_string(const Big &b) {
    if (b.v.empty()) return "0";
    string res;
    for (int i = (int)b.v.size() - 1; i >= 0; --i) {
        string t = to_string(b.v[i]);
        if (!res.empty())
            t = string(basen - t.size(), '0') + t;
        res += t;
    }
    if (b.minus)
        res.insert(res.begin(), '-');
    return res;
}

ostream &operator << (ostream &o, const Big &b) {
    return o << to_string(b);
};

istream &operator >> (istream &i, Big &b) {
    string s;
    i >> s;
    b = Big(s);
    return i;
}

Big gcd(Big a, Big b) {
    while (b != 0) {
        a %= b;
        swap(a, b);
    }
    return a;
}
