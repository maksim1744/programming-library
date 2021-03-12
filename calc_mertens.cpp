// calculates mertens function M(n), where
// M(n) = sum_{i=1..n} mobius(n), where
// mobius(n) =
//     1, if n has even number of primes
//    -1, if n has odd number of primes
//     0, if n is divisible by a square
ll calc_mertens(ll n) {
    ll y = n / (ll)pow(n, 0.33);

    vector<int> mu(y + 1, -1); mu[0] = 0; mu[1] = 1;
    {
        vector<bool> isp(y + 1, true); isp[0] = isp[1] = false;
        vector<int> mn(y + 1);
        for (int i = 2; i * i <= y; ++i)
            if (isp[i])
                for (int j = i * i; j <= y; j += i)
                    isp[j] = false, mn[j] = i;
        for (int i = 2; i <= y; ++i)
            if (!isp[i])
                mu[i] = -mu[i / mn[i]];
        for (int i = 2; i * i <= y; ++i)
            if (isp[i])
                for (int j = i * i; j <= y; j += i * i)
                    mu[j] = 0;
    }
    for (int i = 1; i < mu.size(); ++i)
        mu[i] += mu[i - 1];

    vector<ll> v;
    v.reserve((int)sqrt(n) * 2 + 20);
    ll sq;
    {
        ll k = 1;
        for (; k * k <= n; ++k) {
            v.pb(k);
        }
        --k;
        sq = k;
        if (k * k == n) --k;
        for (; k >= 1; --k) {
            v.pb(n / k);
        }
    }
    vector<ll> s(v.size(), 0);
    auto geti = [&](ll x) {
        if (x <= sq) return (int)x - 1;
        else         return (int)(v.size() - (n / x));
    };

    for (int i = 0; i < v.size(); ++i) {
        if (v[i] <= y) {
            s[i] = mu[v[i]];
            continue;
        }
        ll sq = sqrt(v[i]);
        assert(sq * sq <= v[i] && (sq + 1) * (sq + 1) > v[i]);
        ll mid = v[i] / sq;
        s[i] = 1;
        for (ll j = 2; j <= mid; ++j) {
            s[i] -= s[geti(v[i] / j)];
        }
        for (ll k = 1; k < v[i] / mid; ++k) {
            s[i] -= s[geti(k)] * (v[i] / k - v[i] / (k + 1));
        }
    }
    return s.back();
}
