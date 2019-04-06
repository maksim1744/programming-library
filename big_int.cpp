const long long base = 1e9;
const int basen = 9;

struct Big {
    vector<long long> v;

    Big() : v{0} {}

    void norm() {
        if (v.size() == 0)
            v = {0};
        else
            while (v.size() > 1 && v.back() == 0)
                v.pop_back();
    }

    void assign(string& s) {
        v.clear();
        reverse(s.begin(), s.end());
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

bool operator == (Big& a, Big& b) {
    return a.v == b.v;
}

bool operator == (Big& a, int k) {
    return (a.size() == 1 && a[0] == k);
}

bool operator < (const Big& a, const Big& b) {
    if (a.size() != b.size())
        return a.size() < b.size();
    for (int i = (int)a.size() - 1; i >= 0; --i)
        if (a[i] != b[i])
            return a[i] < b[i];
    return false;
}

Big& operator -= (Big& a, Big& b) {
    assert(!(a < b));
    int rem = 0;
    for (int i = 0; i < b.size(); ++i) {
        a[i] -= rem;
        a[i] -= b[i];
        rem = 0;
        while (a[i] < 0) {
            a[i] += base;
            ++rem;
        }
    }
    int i = b.size();
    while (rem != 0) {
        a[i] -= rem;
        rem = 0;
        while (a[i] < 0) {
            a[i] += base;
            ++rem;
        }
        ++i;
    }
    a.norm();
    return a;
}

string to_string(Big a) {
    string s = to_string(a.v.back());
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
    assert(!(b == 0));
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
            a -= b;
            c.add10(shift);
        }
        b.div10(shift);
        --shift;
    }
    return {c, a};
}

Big gcd(Big a, Big b) {
    if (b < a)
        swap(a, b);
    if (a == 0)
        return b;
    b = div(b, a).second;
    return gcd(b, a);
}
