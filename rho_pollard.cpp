ll get_divisor(ll n) {
    for (int i = 2; i < 1000; ++i)
        if (n % i == 0)
            return i;
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
    while (!is_prime(p)) {
        p = get_divisor(p);
    }
    show(p);
    return p;
}
