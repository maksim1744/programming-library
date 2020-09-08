template<typename T>
struct fenwick2d {
    vector<vector<T>> tree;
    int n, m;

    fenwick2d(int n = 0, int m = 0) : n(n), m(m) {
        tree.assign(n, vector<T>(m, 0));
    }

    void add(int ii1, int ii2, T k) {
        for (int i1 = ii1; i1 < n; i1 = (i1 | (i1 + 1)))
            for (int i2 = ii2; i2 < m; i2 = (i2 | (i2 + 1)))
                tree[i1][i2] += k;
    }

    T ask(int rr1, int rr2) {
        T res = 0;
        for (int r1 = rr1; r1 >= 0; r1 = (r1 & (r1 + 1)) - 1)
            for (int r2 = rr2; r2 >= 0; r2 = (r2 & (r2 + 1)) - 1)
                res += tree[r1][r2];
        return res;
    }

    T ask(int l1, int r1, int l2, int r2) {
        if (l1 > r1 || l2 > r2) return 0;
        return ask(r1, r2) - ask(l1 - 1, r2) - ask(r1, l2 - 1) + ask(l1 - 1, l2 - 1);
    }
};
