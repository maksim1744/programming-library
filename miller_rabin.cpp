bool is_prime(ull n) {
    if (n < 2)
        return false;
    for (int i = 2; i < min(n, (ull)1000); ++i)
        if (n % i == 0)
            return false;
    if (n <= 1000 * 1000)
        return true;
    int s = 0;
    ull d = n - 1;
    while (d % 2 == 0) {
        ++s;
        d >>= 1;
    }
    int k = 70;  // ~ log n
    for (int ii = 0; ii < k; ++ii) {
        ll a = rnd(2, n - 2);
        ll x = mpow(a, d, n);
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
