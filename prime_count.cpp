// works in <1s for 1e11, 4s for 1e12
ll count_primes(ll n) {
    auto f = [&](ll n) {
        // should be multiplicative (f(ab) = f(a)f(b)),
        // the result will be sum f(p) over all primes
        return 1;
    };
    auto pref = [&](ll n) {
        // should return sum_{i=1..n} f(i)
        return n;
    };

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
    vector<ll> s(v.size());
    for (int i = 0; i < s.size(); ++i)
        s[i] = pref(v[i]) - 1;
    auto geti = [&](ll x) {
        if (x <= sq) return (int)x - 1;
        else         return (int)(v.size() - (n / x));
    };
    for (ll p = 2; p * p <= n; ++p) {
        if (s[p - 1] != s[p - 2]) {
            ll sp = s[p - 2];
            ll p2 = p * p;
            for (int i = (int)v.size() - 1; i >= 0; --i) {
                if (v[i] < p2) {
                    break;
                }
                s[i] -= (s[geti(v[i] / p)] - sp) * f(p);
            }
        }
    }
    return s.back();
}
