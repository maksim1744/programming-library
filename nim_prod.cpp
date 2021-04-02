constexpr struct bit_prod_t {
    array<array<uint64_t, 64>, 64> v = {};
    constexpr bit_prod_t() {
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if (!(i & j)) {
                    v[i][j] = uint64_t(1) << (i | j);
                } else {
                    // square(2^2^a) = 2^2^a ^ 2^(2^a-1)
                    int a = (i & j) & -(i & j);
                    v[i][j] = v[i ^ a][j] ^ v[(i ^ a) | (a - 1)][(j ^ a) | (i & (a - 1))];
                }
            }
        }
    }

    const array<uint64_t, 64> &operator [] (int i) const {
        return v[i];
    }
} bit_prod;

uint64_t nim_prod(uint64_t a, uint64_t b) {
    uint64_t res = 0;
    for (int i = 0; (i < 64) && (a >> i); ++i)
        if ((a >> i) & 1)
            for (int j = 0; (j < 64) && (b >> j); ++j)
                if ((b >> j) & 1)
                    res ^= bit_prod[i][j];
    return res;
}
