long long mod = 998244353, g = 3, ginv = 332748118, inv2 = (mod + 1) / 2;

long long powMod(long long a, long long n, long long mod = mod) {
    if (n == 1) {
        return a % mod;
    }
    if (n % 2 == 0) {
        long long k = powMod(a, n / 2, mod);
        return k * k % mod;
    } else {
        return powMod(a, n - 1, mod) * a % mod;
    }
}

long long findG(long long mod = mod) {
    vector<long long> d;
    long long n = mod - 1;
    if (!(n & 1)) {
        d.push_back(2);
        while (!(n & 1)) {
            n >>= 1;
        }
    }
    long long p = 3;
    while (n > 1) {
        if (n % p == 0) {
            d.push_back(p);
            while (n % p == 0) {
                n /= p;
            }
        }
        p += 2;
    }
    n = mod - 1;
    for (auto& item : d) {
        item = n / item;
    }
    long long g = 2;
    while (true) {
        bool ok = true;
        for (auto k : d) {
            if (powMod(g, k, mod) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            break;
        }
        ++g;
    }
    return g;
}

vector<long long> ntt(vector<long long>& v, bool inv = false) {
    if (v.size() == 1) {
        return v;
    }
    int n = 1;
    int p2 = 0;
    while (n < v.size()) {
        n <<= 1;
        ++p2;
    }
    v.resize(n, 0);
    assert(mod % n == 1);

    long long w0;
    if (inv) {
        w0 = powMod(ginv, (mod - 1) / n, mod);
    } else {
        w0 = powMod(g, (mod - 1) / n, mod);
    }

    n /= 2;
    int k = 0, n1 = 2 * n;
    while (n1 > 1) {
        n1 >>= 1;
        ++k;
    }

    vector<long long> w(2 * n);
    w[0] = 1;
    w[1] = w0;
    for (int i = 2; i < 2 * n; ++i) {
        if (i >= n) {
            w[i] = mod - w[i - n];
        } else {
            w[i] = w[i / 2] * w[i - i / 2] % mod;
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
    vector<long long> v1(n * 2);
    for (int i = 0; i < 2 * n; ++i) {
        v1[i] = (v[ind[i]]);
    }
    for (int i = k - 1; i >= 0; --i) {
        int cnt = (1 << i);
        int sz = (1 << (k - i));
        int k1 = sz / 2 - 1;
        for (int j = 0; j < cnt; ++j) {
            int k2 = sz * j;
            vector<long long> va(v1.begin() + sz * j, v1.begin() + sz * j + sz / 2);
            vector<long long> vb(v1.begin() + sz * j + sz / 2, v1.begin() + sz * j + sz);
            for (int u = 0; u < sz; ++u) {
                v1[k2 + u] = (va[u & k1] + w[u << i] * vb[u & k1]) % mod;
            }
        }
    }

    if (inv) {
        for (int i = 0; i < n; ++i) {
            v1[i] = v1[i] * mpow(inv2, k) % mod;
        }
    }
    return v1;
}

vector<long long> mul(vector<long long>& a, vector<long long>& b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) {
        n <<= 1;
    }
    a.resize(n, 0);
    b.resize(n, 0);
    vector<long long> ntta = ntt(a), nttb = ntt(b);
    for (int i = 0; i < n; ++i) {
        ntta[i] = ntta[i] * nttb[i] % mod;
    }
    return ntt(ntta, true);
}

vector<long long> sqr(vector<long long>& a) {
    int n = 1;
    while (n < a.size() * 2 - 1) {
        n <<= 1;
    }
    a.resize(n, 0);
    vector<long long> ntta = ntt(a);
    for (int i = 0; i < n; ++i) {
        ntta[i] = ntta[i] * ntta[i] % mod;
    }
    return ntt(ntta, true);
}
