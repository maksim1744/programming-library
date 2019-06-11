ull mul_mod(ull a, ull b, ull mod) {  // mod <= 1<<63, then a%mod + b%mod < 1<<64
    ull a0 = a & 0xffffffff;
    ull a1 = a >> 32;
    ull b0 = b & 0xffffffff;
    ull b1 = b >> 32;
    ull c0 = a0 * b0 % mod;
    ull c1 = (a0 * b1 % mod + a1 * b0 % mod) % mod;
    int t = 32;
    while (t) {
        int k = min(t, __builtin_clzll(c1));
        c1 = (c1 << k) % mod;
        t -= k;
    }
    ull c2 = a1 * b1 % mod;
    t = 64;
    while (t) {
        int k = min(t, __builtin_clzll(c2));
        c2 = (c2 << k) % mod;
        t -= k;
    }
    return ((c0 + c1) % mod + c2) % mod;
}
