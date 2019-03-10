long long fpow(long long a, long long p) {
    long long res = 1;
    while (p) {
        if (p & 1) res = res * a;
        p >>= 1;
        a = a * a;
    }
    return res;
}
