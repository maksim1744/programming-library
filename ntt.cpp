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
    vector<long long> v1(n / 2), v2(n / 2);
    for (int i = 0; i < n / 2; ++i) {
        v1[i] = v[2 * i];
        v2[i] = v[2 * i + 1];
    }
    vector<long long> ntt1 = ntt(v1, inv), ntt2 = ntt(v2, inv);
    long long w, x = 1;
    if (inv) {
        w = powMod(ginv, (mod - 1) / n, mod);
    } else {
        w = powMod(g, (mod - 1) / n, mod);
    }
    vector<long long> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = (ntt1[i % (n / 2)] + x * ntt2[i % (n / 2)]) % mod;
        x = (x * w) % mod;
    }
    if (inv) {
        // long long ninv = powMod(inv2, p2, mod);
        for (int i = 0; i < n; ++i) {
            ans[i] = ans[i] * inv2 % mod;
        }
    }
    return ans;
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
