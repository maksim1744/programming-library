// calculates mertens function M(n), where
// M(n) = sum_{i=1..n} mobius(n), where
// mobius(n) =
//     1, if n has even number of primes
//    -1, if n has odd number of primes
//     0, if n is divisible by a square
ll calc_mertens(ll n) {
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
    s[0] = 1;
    for (int i = 1; i < v.size(); ++i) {
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
