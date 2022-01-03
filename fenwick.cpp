template<typename T>
struct fenwick {
    vector<T> tree;
    int n;
    int K;

    fenwick(int n = 0) : n(n) {
        tree.assign(n, 0);
        K = 0;
        while ((1 << K) <= n)
            ++K;
    }

    void add(int i, T k) {
        for (; i < n; i = (i | (i + 1)))
            tree[i] += k;
    }

    T ask(int r) {
        T res = 0;
        for (; r >= 0; r = (r & (r + 1)) - 1)
            res += tree[r];
        return res;
    }

    T ask(int l, int r) {
        if (l > r) return 0;
        return ask(r) - ask(l - 1);
    }

    // find first i such that sum[0..i] >= x
    int lower_bound(T x) {
        int cur = 0;
        T cur_sum = 0;
        for (int k = K - 1; k >= 0; --k) {
            int ind = cur | ((1 << k) - 1);
            if (ind >= n) continue;
            if (cur_sum + tree[ind] >= x) continue;
            cur_sum += tree[ind];
            cur |= (1 << k);
        }
        return cur;
    }
};
