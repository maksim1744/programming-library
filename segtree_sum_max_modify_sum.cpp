struct item {
    int mx = 0;
    int sm = 0;  // sum on [l; r]
    int ind = 0;  // index of maximum
    int l, r;
    int mod_sm = 0;  // modify sum, not sum on [l; r] !!!

    item() {}
    item(int value, int ind = 0): sm(value), mx(value), ind(ind), l(ind), r(ind) {}

    int ask_mx() {
        return mx + mod_sm;
    }
    int ask_sm() {
        return sm + mod_sm * (r - l + 1);
    }

    auto update(item& a, item& b) {
        if (a.ask_mx() >= b.ask_mx()) {  // return left maximum
            mx = a.ask_mx();
            ind = a.ind;
        } else {
            mx = b.ask_mx();
            ind = b.ind;
        }
        sm = a.ask_sm() + b.ask_sm();
        l = a.l; r = b.r;
        return *this;
    }
};

item operator + (item a, item b) {
    return item().update(a, b);
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
            tree[i] = item(v[l], l);
            return;
        }
        int m = (l + r) / 2;
        build(v, i * 2 + 1, l, m);
        build(v, i * 2 + 2, m + 1, r);
        tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }

    template< typename InType >
    void set(int ind, InType k, int i, int vl, int vr) {
        if (vl == vr) {
            tree[i].sm = k - tree[i].mod_sm;
            tree[i].mx = k - tree[i].mod_sm;
            return;
        }
        int m = (vl + vr) / 2;
        if (ind <= m) set(ind, k - tree[i].mod_sm, 2 * i + 1, vl, m);
        else set(ind, k - tree[i].mod_sm, 2 * i + 2, m + 1, vr);
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template< typename InType >
    void set(int ind, InType k) {
        set(ind, k, 0, 0, n - 1);
    }

    template< typename InType >
    void add(int l, int r, InType k, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            tree[i].mod_sm += k;
            return;
        }
        int m = (vl + vr) / 2;
        if (r <= m) add(l, r, k, 2 * i + 1, vl, m);
        else if (l > m) add(l, r, k, 2 * i + 2, m + 1, vr);
        else {
            add(    l, m, k, 2 * i + 1,    vl,  m);
            add(m + 1, r, k, 2 * i + 2, m + 1, vr);
        }
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template< typename InType >
    void add(int l, int r, InType k) {
        add(l, r, k, 0, 0, n - 1);
    }

    auto ask_sm(int l, int r, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            return tree[i].ask_sm();
        }
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask_sm(l, r, 2 * i + 1, vl, m) + tree[i].mod_sm * (r - l + 1);
        if (l > m)
            return ask_sm(l, r, 2 * i + 2, m + 1, vr) + tree[i].mod_sm * (r - l + 1);
        return ask_sm(l, m, 2 * i + 1, vl, m) + ask_sm(m + 1, r, 2 * i + 2, m + 1, vr) +
            tree[i].mod_sm * (r - l + 1);
    }

    auto ask_sm(int l, int r) {
        return ask_sm(l, r, 0, 0, n - 1);
    }

    auto ask_mx(int l, int r, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            return tree[i].ask_mx();
        }
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask_mx(l, r, 2 * i + 1, vl, m) + tree[i].mod_sm;
        if (l > m)
            return ask_mx(l, r, 2 * i + 2, m + 1, vr) + tree[i].mod_sm;
        return max(ask_mx(l, m, 2 * i + 1, vl, m), ask_mx(m + 1, r, 2 * i + 2, m + 1, vr)) + tree[i].mod_sm;
    }

    auto ask_mx(int l, int r) {
        return ask_mx(l, r, 0, 0, n - 1);
    }

    auto ask_item(int l, int r, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            auto it = tree[i];
            it.mx = it.ask_mx();
            it.sm = it.ask_sm();
            it.mod_sm = 0;
            return it;
        }
        int m = (vl + vr) / 2;
        item it;
        if (r <= m)
            it = ask_item(l, r, 2 * i + 1, vl, m);
        else if (l > m)
            it = ask_item(l, r, 2 * i + 2, m + 1, vr);
        else
            it = ask_item(l, m, 2 * i + 1, vl, m) + ask_item(m + 1, r, 2 * i + 2, m + 1, vr);
        it.mx += tree[i].mod_sm;
        it.sm += tree[i].mod_sm * (r - l + 1);
        return it;
    }

    auto ask_item(int l, int r) {
        return ask_item(l, r, 0, 0, n - 1);
    }
};
