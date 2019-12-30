// copy mul_mod from rho_pollard
ull mpow_long(ull a, ull p, ull mod) {
    ull res = 1;
    while (p) {
        if (p & 1) res = mul_mod(res, a, mod);
        p >>= 1;
        a = mul_mod(a, a, mod);
    }
    return res;
}

bool is_prime(ull n) {
    if (n < 2)
        return false;
    // maybe check divisors <= 1000 to speed up
    int s = 0;
    ull d = n - 1;
    while (d % 2 == 0) {
        ++s;
        d >>= 1;
    }
    int k = 70;  // log n
    for (int ii = 0; ii < k; ++ii) {
        ull a = rnd(2, n - 2);
        ull x = mpow_long(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int i = 0; i < s - 1; ++i) {
            x = mul_mod(x, x, n);
            if (x == 1)
                return false;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}
