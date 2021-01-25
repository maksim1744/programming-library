struct item {
    int value = 0;
    int l = 0;
    int r = 0;
    int mod_sm = 0;  // modify sum, not sum on [l; r] !!!

    item() {}
    item(int value, int l = 0, int r = 0): value(value), l(l), r(r) {}

    int ask() {
        return value + mod_sm * (r - l + 1);
    }
};

item operator + (item a, item b) {
    return item(a.ask() + b.ask(), a.l, b.r);
}

struct segtree {
    vector<item> tree;
    int n;

    segtree(int size) : segtree(vector<int>(size, 0)) {}
    template<typename InType>
    segtree(vector<InType> v) : n(v.size()), tree(v.size() * 4) {
        build(v, 0, 0, n - 1);
    }

    template<typename InType>
    void build(vector<InType>& v, int i, int l, int r) {
        if (l == r) {
            tree[i] = item(v[l], l, l);
            return;
        }
        int m = (l + r) / 2;
        build(v, i * 2 + 1, l, m);
        build(v, i * 2 + 2, m + 1, r);
        tree[i] = tree[2 * i + 1] + tree[2 * i + 2];
    }

    template<typename InType>
    void set(int ind, InType k, int i, int vl, int vr) {
        if (vl == vr) {
            tree[i].value = k - tree[i].mod_sm;
            return;
        }
        int m = (vl + vr) / 2;
        if (ind <= m) set(ind, k - tree[i].mod_sm, 2 * i + 1, vl, m);
        else set(ind, k - tree[i].mod_sm, 2 * i + 2, m + 1, vr);
        tree[i].value = (tree[2 * i + 1] + tree[2 * i + 2]).value;
    }

    template<typename InType>
    void set(int ind, InType k) {
        set(ind, k, 0, 0, n - 1);
    }

    template<typename InType>
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
        tree[i].value = (tree[2 * i + 1] + tree[2 * i + 2]).value;
    }

    template<typename InType>
    void add(int l, int r, InType k) {
        add(l, r, k, 0, 0, n - 1);
    }

    auto ask(int l, int r, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            return tree[i].ask();
        }
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask(l, r, 2 * i + 1, vl, m) + tree[i].mod_sm * (r - l + 1);
        if (l > m)
            return ask(l, r, 2 * i + 2, m + 1, vr) + tree[i].mod_sm * (r - l + 1);
        return ask(l, m, 2 * i + 1, vl, m) + ask(m + 1, r, 2 * i + 2, m + 1, vr) + tree[i].mod_sm * (r - l + 1);
    }

    auto ask(int l, int r) {
        return ask(l, r, 0, 0, n - 1);
    }
};
