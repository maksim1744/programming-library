struct bit_prod_t {
    // B=8 requires 32MB, B=4 requires 0.5 MB, time and space complexity of precalc is 4^B * 64^2 / B^2
    const static int B = 8;
    const static int BM = (uint64_t(1) << B) - 1;
    array<array<array<array<uint64_t, 1 << B>, 64 / B>, 1 << B>, 64 / B> v = {};
    bit_prod_t() {
        array<array<uint64_t, 64>, 64> bp = {};
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if (!(i & j)) {
                    bp[i][j] = uint64_t(1) << (i | j);
                } else {
                    // square(2^2^a) = 2^2^a ^ 2^(2^a-1)
                    int a = (i & j) & -(i & j);
                    bp[i][j] = bp[i ^ a][j] ^ bp[(i ^ a) | (a - 1)][(j ^ a) | (i & (a - 1))];
                }
            }
        }
        array<uint64_t, 1 << B> lg = {};
        for (int i = 0; i < B; ++i)
            lg[1 << i] = i;
        for (uint64_t b1 = 0; b1 < 64 / B; ++b1) {
            for (uint64_t i = 0; i < (1 << B); ++i) {
                for (uint64_t b2 = 0; b2 < 64 / B; ++b2) {
                    for (uint64_t j = 0; j < (1 << B); ++j) {
                        if (i == 0 || j == 0) {
                            v[b1][i][b2][j] = 0;
                            continue;
                        }
                        if ((j & (j - 1)) == 0) {
                            if ((i & (i - 1)) == 0) {
                                v[b1][i][b2][j] = bp[lg[i] + b1 * B][lg[j] + b2 * B];
                            } else {
                                uint64_t a = i & -i;
                                v[b1][i][b2][j] = v[b1][i ^ a][b2][j] ^ v[b1][a][b2][j];
                            }
                        } else {
                            uint64_t a = j & -j;
                            v[b1][i][b2][j] = v[b1][i][b2][j ^ a] ^ v[b1][i][b2][a];
                        }
                    }
                }
            }
        }
    }

    const auto &operator [] (int i) const {
        return v[i];
    }
} bit_prod;

uint64_t nim_prod(uint64_t a, uint64_t b) {
    uint64_t res = 0;
    for (int i = 0, ib = 0; (ib < 64) && (a >> ib); ++i, ib += bit_prod_t::B)
        for (int j = 0, jb = 0; (jb < 64) && (b >> jb); ++j, jb += bit_prod_t::B)
            res ^= bit_prod[i][(a >> ib) & bit_prod_t::BM][j][(b >> jb) & bit_prod_t::BM];
    return res;
}
