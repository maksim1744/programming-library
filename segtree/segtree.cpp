using T = int;

struct segtree {
    vector<T> tree;
    int n;

    segtree(int size) : n(size), tree(size * 4, T()) {}
    segtree(vector<T>& v) : n(v.size()), tree(v.size() * 4) {
        build(v, 0, 0, n - 1);
    }

    void build(vector<T>& v, int i, int l, int r) {
        if (l == r) {
            tree[i] = v[l];
            return;
        }
        int m = (l + r) / 2;
        build(v, i * 2 + 1, l, m);
        build(v, i * 2 + 2, m + 1, r);
        tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }

    void set(int ind, T k, int i, int l, int r) {
        while (l < r) {
            int m = (l + r) / 2;
            if (ind <= m) {
                r = m;
                i = 2 * i + 1;
            } else {
                l = m + 1;
                i = 2 * i + 2;
            }
        }
        tree[i] = k;
        while (i != 0) {
            i = (i - 1) / 2;
            tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
        }
    }

    void set(int ind, T k) {
        set(ind, k, 0, 0, n - 1);
    }

    T ask(int l, int r, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            return tree[i];
        }
        int m = (vl + vr) / 2;
        if (r <= m) {
            return ask(l, r, 2 * i + 1, vl, m);
        }
        if (l > m) {
            return ask(l, r, 2 * i + 2, m + 1, vr);
        }
        return ask(l, m, 2 * i + 1, vl, m) + ask(m + 1, r, 2 * i + 2, m + 1, vr);
    }

    T ask(int l, int r) {
        return ask(l, r, 0, 0, n - 1);
    }
};
