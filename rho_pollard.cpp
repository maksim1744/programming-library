ull mul_mod(ull a, ull b, ull mod) {
    #ifdef __SIZEOF_INT128__
    return (__int128)a * b % mod;
    #endif
    ll res = a * b - mod * (ull)((long double)1 / mod * a * b);
    if (res < 0) res += mod;
    if (res >= mod) res -= mod;
    assert(0 <= res && res < mod);
    return res;
}

ll get_divisor(ll n) {
    // maybe check divisors <= 1000 to speed up
    auto f = [&](ll x) {
        return (mul_mod(x, x, n) + 1) % n;
    };
    while (true) {
        ll x = rnd(1, n - 1);
        ll y = f(x);
        int cnt = 0;
        while (x % n != y % n && ++cnt < 30000) {
            ll d = __gcd(n, abs(x - y));
            if (d == 0)
                break;
            else if (d != 1)
                return d;
            x = f(x);
            y = f(f(y));
        }
    }
}

ll get_prime_divisor(ll n) {
    ll p = n;
    while (!is_prime(p))
        p = get_divisor(p);
    return p;
}
