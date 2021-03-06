namespace ntt {
// TODO: square

Mint g = 0;  // set to something, or leave at 0 to calculate at runtime, g^(max power of 2 in mod-1) == 1
int maxn = -1;

vector<Mint> buf1;
vector<Mint> buf2;

vector<int> reversed = {0};

void init_g() {
    int phi = (int)Mint(-1);  // mod - 1
    maxn = 1;
    while ((phi & 1) == 0) {
        maxn <<= 1;
        phi >>= 1;
    }

    if (g == 0) {
        while (true) {
            Mint v = pow(g, maxn / 2);
            if (v != 1 && v * v == 1)
                break;
            ++g;
        }
    }

    assert(pow(g, maxn) == 1 && pow(g, maxn / 2) != 1);
}

void update_n(int n) {
    if (maxn == -1)
        init_g();
    assert(n <= maxn);
    assert((n & (n - 1)) == 0);
    int cur = reversed.size();
    if (n <= cur) return;
    reversed.resize(n);
    while (cur < n) {
        for (int i = 0; i < cur; ++i)
            reversed[i] <<= 1;
        for (int i = cur; i < (cur << 1); ++i)
            reversed[i] = reversed[i - cur] ^ 1;
        cur *= 2;
    }
}

void ntt_internal(vector<Mint> &v, int from, int n, bool inv) {
    update_n(n);
    int N = reversed.size();

    int d = __lg(N) - __lg(n);

    for (int i = 1; i < n; ++i)
        if (i < (reversed[i] >> d))
            swap(v[from + i], v[from + (reversed[i] >> d)]);

    for (int ln = 1; ln < n; ln <<= 1) {
        Mint ww;
        ww = pow(g, maxn / (ln * 2));
        if (inv)
            ww = Mint(1) / ww;
        for (int i = 0; i < n; i += (ln << 1)) {
            Mint w = 1;
            for (int j = 0; j < ln; ++j) {
                Mint y = v[from + i + j + ln] * w;
                w *= ww;
                v[from + i + j + ln] = v[from + i + j] - y;
                v[from + i + j]      = v[from + i + j] + y;
            }
        }
    }

    if (inv) {
        Mint ni = Mint(1) / n;
        for (int i = 0; i < n; ++i)
            v[from + i] *= ni;
    }
}

vector<Mint> ntt(const vector<Mint> &v, int n = -1) {
    if (n == -1) {
        n = 1;
        while (n < v.size()) n <<= 1;
    }
    assert(v.size() <= n);
    buf1.assign(n, 0);
    for (int i = 0; i < v.size(); ++i)
        buf1[i] = v[i];
    ntt_internal(buf1, 0, n, false);
    return buf1;
}

vector<Mint> ntti(const vector<Mint> &v) {
    assert(!v.empty() && (v.size() & (v.size() - 1)) == 0);
    buf1.resize(v.size());
    for (int i = 0; i < v.size(); ++i)
        buf1[i] = v[i];
    ntt_internal(buf1, 0, buf1.size(), true);
    return buf1;
}

vector<Mint> multiply(const vector<Mint> &a, const vector<Mint> &b) {
    if (a.empty() || b.empty()) return {};
    int n = 2;
    while (n < a.size() + b.size() - 1) n <<= 1;

    buf1.assign(n, 0);
    for (int i = 0; i < a.size(); ++i)
        buf1[i] = a[i];

    buf2.assign(n, 0);
    for (int i = 0; i < b.size(); ++i)
        buf2[i] = b[i];

    ntt_internal(buf1, 0, n, false);
    ntt_internal(buf2, 0, n, false);

    for (int i = 0; i < n; ++i)
        buf1[i] *= buf2[i];

    ntt_internal(buf1, 0, n, true);

    buf1.resize(a.size() + b.size() - 1);
    return buf1;
}

} // ntt
