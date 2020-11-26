const long long base = 1e9;
const int basen = 9;

struct Big {
    vector<long long> v;
    bool minus = false;

    Big(long long n = 0) : minus(n < 0) {
        n = abs(n);
        while (n != 0) {
            v.pb(n % base);
            n /= base;
        }
        if (v.empty()) {
            v.pb(0);
        }
    }

    void norm() {
        if (v.size() == 0)
            v = {0};
        else
            while (v.size() > 1 && v.back() == 0)
                v.pop_back();
        if (v.size() == 1 && v[0] == 0)
            minus = false;
    }

    void assign(string& s) {
        v.clear();
        reverse(s.begin(), s.end());
        if (s.back() == '-') {
            s.pop_back();
            minus = true;
        }
        while (s.size() % basen != 0)
            s.push_back('0');
        for (int i = 0; i < s.size() / basen; ++i) {
            long long k = 0;
            long long p10 = 1;
            for (int j = 0; j < basen; ++j) {
                k += p10 * (s[i * basen + j] - '0');
                p10 *= 10;
            }
            v.push_back(k);
        }
    }

    auto size() const {
        return v.size();
    }

    auto& operator[] (int i) const {
        return v[i];
    }

    auto& operator[] (int i) {
        return v[i];
    }

    auto& back() {
        return v.back();
    }

    void resize(int k) {
        v.resize(k);
    }

    auto mul10(int k = 1) {
        if (k / basen > 0) {
            v.resize(v.size() + k / basen, 0);
            for (int i = (int)v.size() - k / basen - 1; i >= 0; --i) {
                v[i + k / basen] = v[i];
            }
            for (int i = 0; i < k / basen; ++i) {
                v[i] = 0;
            }
            k %= basen;
        }
        long long p10 = 1;
        for (int i = 0; i < k; ++i) {
            p10 *= 10;
        }
        for (int i = 0; i < v.size(); ++i) {
            v[i] *= p10;
        }
        long long rem = 0;
        for (int i = 0; i < v.size(); ++i) {
            v[i] += rem;
            rem = v[i] / base;
            v[i] %= base;
        }
        if (rem != 0)
            v.push_back(rem);
        return *this;
    }

    auto div10(int k = 1) {
        if (k % basen != 0) {
            mul10(basen - k % basen);
            k += basen - k % basen;
        }
        if (k / basen >= v.size()) {
            v = {0};
            return *this;
        }
        for (int i = k / basen; i < v.size(); ++i) {
            v[i - k / basen] = v[i];
        }
        for (int i = (int)v.size() - k / basen; i < v.size(); ++i) {
            v[i] = 0;
        }
        norm();
        return *this;
    }

    void add10(int k = 1) {
        while (k / basen >= v.size())
            v.push_back(0);
        v.push_back(0);
        int p10 = 1;
        for (int i = 0; i < k % basen; ++i) {
            p10 *= 10;
        }
        v[k / basen] += p10;
        int i = k / basen;
        long long rem = 0;
        while (v[i] >= base || rem > 0) {
            rem = v[i] / base;
            v[i] %= base;
            ++i;
        }
        norm();
    }

};

istream& operator >> (istream& in, Big& b) {
    string s;
    cin >> s;
    b.assign(s);
    return in;
}

bool operator == (Big a, Big b) {
    return a.minus == b.minus && a.v == b.v;
}

bool operator == (Big& a, long long k) {
    return (a == Big(k));
}

bool operator != (Big a, Big b) {
    return !(a == b);
}

bool operator < (Big a, Big b) {
    if (a.minus != b.minus)
        return a.minus;
    bool minus = a.minus;
    if (a.size() != b.size())
        return (a.size() < b.size())^minus;
    for (int i = (int)a.size() - 1; i >= 0; --i)
        if (a[i] != b[i])
            return (a[i] < b[i])^minus;
    return false;
}

bool abs_comp(const Big& a, const Big& b) {
    if (a.size() != b.size())
        return (a.size() < b.size());
    for (int i = (int)a.size() - 1; i >= 0; --i)
        if (a[i] != b[i])
            return (a[i] < b[i]);
    return false;
}

Big operator - (Big a) {
    a.minus = !a.minus;
    return a;
}

Big operator - (Big a, Big b);
Big operator + (Big a, Big b) {
    if (a.minus == b.minus) {
        Big c;
        c.resize(max(a.size(), b.size()) + 3);
        for (int i = 0; i < a.size(); ++i) {
            c[i] += a[i];
        }
        for (int i = 0; i < b.size(); ++i) {
            c[i] += b[i];
        }
        for (int i = 0; i < c.size(); ++i) {
            if (c[i] >= base) {
                c[i] -= base;
                c[i + 1]++;
            }
        }
        c.minus = a.minus;
        c.norm();
        return c;
    } else {
        if (a.minus) {
            return b - (-a);
        } else {
            return a - (-b);
        }
    }
}

Big operator - (Big a, Big b) {
    if (a.minus == b.minus) {
        bool minus = false;
        if (abs_comp(a, b)) {
            swap(a, b);
            minus = true;
        }
        for (int i = 0; i < b.size(); ++i) {
            a[i] -= b[i];
            if (a[i] < 0) {
                a[i] += base;
                a[i + 1]--;
            }
        }
        a.minus ^= minus;
        a.norm();
        return a;
    } else {
        if (a.minus) {
            return -((-a) + b);
        } else {
            return a + (-b);
        }
    }
}

Big operator * (Big a, Big b) {
    Big c;
    c.resize(a.size() + b.size() + 3);
    c.minus = a.minus ^ b.minus;
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            c[i + j] += a[i] * b[j];
            if (c[i + j] >= base) {
                c[i + j + 1] += c[i + j] / base;
                c[i + j] %= base;
            }
        }
    }
    for (int i = 0; i < c.size(); ++i) {
        if (c[i] >= base) {
            c[i + 1] += c[i] / base;
            c[i] %= base;
        }
    }
    c.norm();
    return c;
}

string to_string(Big a) {
    string s = a.minus ? "-" : "";
    s += to_string(a.v.back());
    for (int i = (int)a.size() - 2; i >= 0; --i) {
        string s1 = to_string(a[i]);
        if (s1.size() % basen != 0) {
            for (int j = 0; j < basen - (s1.size() % basen); ++j) {
                s.push_back('0');
            }
        }
        s += s1;
    }
    return s;
}

pair<Big, Big> div(Big a, Big b) {
    bool minus = a.minus ^ b.minus;
    a.minus = false;
    b.minus = false;
    Big c;
    if (a < b) {
        return {c, a};
    }
    int l = 0, r = a.size() * basen + 10;
    while (r - l > 1) {
        int m = (l + r) / 2;
        b.mul10(m);
        if (a < b) {
            r = m;
        } else {
            l = m;
        }
        b.div10(m);
    }
    int shift = l;
    while (!(a < b) && shift >= 0) {
        b.mul10(shift);
        while (!(a < b)) {
            a = a - b;
            c.add10(shift);
        }
        b.div10(shift);
        --shift;
    }
    c.minus = a.minus^b.minus;
    if (c == 0)
        c.minus = false;
    return {c, a};
}

Big operator / (Big a, Big b) {
    return div(a, b).first;
}

Big gcd(Big a, Big b) {
    if (b < a)
        swap(a, b);
    if (a == 0)
        return b;
    b = div(b, a).second;
    return gcd(b, a);
}

struct Rat {
    Big a, b;

    void norm() {
        if (a == 0) {
            a.minus = false;
            b = Big(1);
        } else {
            if (b < 0) {
                a.minus = !a.minus;
                b.minus = !b.minus;
            }
            bool minus = a.minus;
            a.minus = false;
            Big d = gcd(a, b);
            a = a / d;
            b = b / d;
            a.minus = minus;
        }
    }

    Rat(Big a = Big(0), Big b = Big(1)) : a(a), b(b) {
        norm();
    }
};

Rat operator - (Rat a) {
    if (a.a == 0)
        return a;
    a.a.minus = !a.a.minus;
    return a;
}
Rat operator + (Rat m, Rat n) {
    return Rat(m.a * n.b + m.b * n.a, m.b * n.b);
}
Rat operator - (Rat m, Rat n) {
    return Rat(m.a * n.b - m.b * n.a, m.b * n.b);
}
Rat operator * (Rat m, Rat n) {
    return Rat(m.a * n.a, m.b * n.b);
}
Rat operator / (Rat m, Rat n) {
    return Rat(m.a * n.b, n.a * m.b);
}

bool operator == (Rat a, Rat b) {
    return a.a == b.a && a.b == b.b;
}
bool operator != (Rat a, Rat b) {
    return !(a == b);
}
bool operator < (Rat a, Rat b) {
    return a.a * b.b < b.a *a.b;
}
bool operator > (Rat a, Rat b) {
    return b < a;
}
bool operator <= (Rat a, Rat b) {
    return !(b < a);
}
bool operator >= (Rat a, Rat b) {
    return !(a < b);
}

string to_string(Rat r) {
    return to_string(r.a) + "/" + to_string(r.b);
}
