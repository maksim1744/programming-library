const int inf = 2 * 1e9 + 100;

struct item {
    bool is_equal = false;
    int equal = 0;
    int mx;

    item(int mx = -inf): mx(mx) {}

    int ask() {
        return is_equal ? equal : mx;
    }

    auto update(item& a, item& b) {
        if (a.ask() >= b.ask()) {  // return left maximum
            mx = a.ask();
        } else {
            mx = b.ask();
        }
    }
};

item operator + (item a, item b) {
    return item(max(a.ask(), b.ask()));
}

struct segtree {
    vector< item > tree;
    int n;

    segtree(int size) : n(size), tree(size * 4, item()) {}
    template< typename InType >
    segtree(vector< InType >& v) : n(v.size()), tree(v.size() * 4) {
        build(v, 0, 0, n - 1);
    }

    template< typename InType >
    void build(vector< InType >& v, int i, int l, int r) {
        if (l == r) {
            tree[i] = item(v[l]);
            return;
        }
        int m = (l + r) / 2;
        build(v, i * 2 + 1, l, m);
        build(v, i * 2 + 2, m + 1, r);
        tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }

    template< typename InType >
    void set(int l, int r, InType k, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            tree[i].is_equal = true;
            tree[i].equal = k;
            return;
        }
        int m = (vl + vr) / 2;
        if (tree[i].is_equal) {
            tree[i].is_equal = false;
            tree[2 * i + 1].is_equal = true;
            tree[2 * i + 1].equal = tree[i].equal;
            tree[2 * i + 2].is_equal = true;
            tree[2 * i + 2].equal = tree[i].equal;
        }
        if (r <= m) set(l, r, k, 2 * i + 1, vl, m);
        else if (l > m) set(l, r, k, 2 * i + 2, m + 1, vr);
        else {
            set(    l, m, k, 2 * i + 1,    vl,  m);
            set(m + 1, r, k, 2 * i + 2, m + 1, vr);
        }
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template< typename InType >
    void set(int l, int r, InType k) {
        set(l, r, k, 0, 0, n - 1);
    }

    auto ask(int l, int r, int i, int vl, int vr) {
        if (tree[i].is_equal) {
            return tree[i].equal;
        }
        if (l == vl && r == vr) {
            return tree[i].ask();
        }
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask(l, r, 2 * i + 1, vl, m);
        if (l > m)
            return ask(l, r, 2 * i + 2, m + 1, vr);
        return max(ask(l, m, 2 * i + 1, vl, m), ask(m + 1, r, 2 * i + 2, m + 1, vr));
    }

    auto ask(int l, int r) {
        return ask(l, r, 0, 0, n - 1);
    }
};
