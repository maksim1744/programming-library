const int inf = 2 * 1e9 + 100;

struct item {
    bool is_equal = false;
    int equal = 0;
    int mx;
    int sm = 0;
    int mod_sm = 0;
    int l = 0, r = 0;

    item(int value = -inf, int l = -1, int r = -1): mx(value), l(l), r(r), sm(value == -inf ? 0 : value) {}

    auto ask_mx() {
        return is_equal ? equal : mx;
    }

    auto ask_sm() {
        return is_equal ? equal * (r - l + 1) : sm;
    }

    auto update(item& a, item& b) {
        equal = 0;
        is_equal = false;
        mod_sm = 0;
        if (a.ask_mx() >= b.ask_mx()) {  // return left maximum
            mx = a.ask_mx();
        } else {
            mx = b.ask_mx();
        }
        sm = a.ask_sm() + b.ask_sm();
        l = a.l; r = b.r;
        return *this;
    }
};

string to_string(item t) {
    return "[is_eq = " + to_string(t.is_equal) + ", equal = " + to_string(t.equal) + ", mx = " + to_string(t.mx) +
        ", sm = " + to_string(t.sm) + ", mod_sm = " + to_string(t.mod_sm) +
        ", (l,r) = (" + to_string(t.l) + "," + to_string(t.r) + ")]";
}

item operator + (item a, item b) {
    return item().update(a, b);
}

struct segtree {
    vector<item> tree;
    int n;

    segtree(int size) : n(size), tree(size * 4, item()) {}
    template<typename InType>
    segtree(vector<InType>& v) : n(v.size()), tree(v.size() * 4) {
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

    void push(int i, int l, int r) {
        if (l == r)
            return;
        if (tree[i].is_equal) {
            tree[2 * i + 1].equal = tree[i].equal;
            tree[2 * i + 1].is_equal = true;

            tree[2 * i + 2].equal = tree[i].equal;
            tree[2 * i + 2].is_equal = true;
        } else {
            if (tree[2 * i + 1].is_equal) {
                tree[2 * i + 1].equal += tree[i].mod_sm;
            } else {
                tree[2 * i + 1].mod_sm += tree[i].mod_sm;
                tree[2 * i + 1].mx += tree[i].mod_sm;
                tree[2 * i + 1].sm += tree[i].mod_sm * (tree[2 * i + 1].r - tree[2 * i + 1].l + 1);
            }

            if (tree[2 * i + 2].is_equal) {
                tree[2 * i + 2].equal += tree[i].mod_sm;
            } else {
                tree[2 * i + 2].mod_sm += tree[i].mod_sm;
                tree[2 * i + 2].mx += tree[i].mod_sm;
                tree[2 * i + 2].sm += tree[i].mod_sm * (tree[2 * i + 2].r - tree[2 * i + 2].l + 1);
            }
        }
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template<typename InType>
    void set(int l, int r, InType k, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            tree[i].is_equal = true;
            tree[i].equal = k;
            return;
        }
        int m = (vl + vr) / 2;
        push(i, vl, vr);
        if (r <= m) set(l, r, k, 2 * i + 1, vl, m);
        else if (l > m) set(l, r, k, 2 * i + 2, m + 1, vr);
        else {
            set(    l, m, k, 2 * i + 1,    vl,  m);
            set(m + 1, r, k, 2 * i + 2, m + 1, vr);
        }
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template<typename InType>
    void set(int l, int r, InType k) {
        if (l > r)
            return;
        set(l, r, k, 0, 0, n - 1);
    }

    template<typename InType>
    void add(int l, int r, InType k, int i, int vl, int vr) {
        if (l == vl && r == vr) {
            if (tree[i].is_equal) {
                tree[i].equal += k;
            } else {
                tree[i].mod_sm += k;
                tree[i].sm += k * (tree[i].r - tree[i].l + 1);
                tree[i].mx += k;
            }
            return;
        }
        int m = (vl + vr) / 2;
        push(i, vl, vr);
        if (r <= m) add(l, r, k, 2 * i + 1, vl, m);
        else if (l > m) add(l, r, k, 2 * i + 2, m + 1, vr);
        else {
            add(    l, m, k, 2 * i + 1,    vl,  m);
            add(m + 1, r, k, 2 * i + 2, m + 1, vr);
        }
        tree[i].update(tree[2 * i + 1], tree[2 * i + 2]);
    }

    template<typename InType>
    void add(int l, int r, InType k) {
        if (l > r)
            return;
        add(l, r, k, 0, 0, n - 1);
    }

    auto ask_mx(int l, int r, int i, int vl, int vr) {
        if (tree[i].is_equal) {
            return tree[i].equal;
        }
        if (l == vl && r == vr) {
            return tree[i].ask_mx();
        }
        push(i, vl, vr);
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask_mx(l, r, 2 * i + 1, vl, m);
        if (l > m)
            return ask_mx(l, r, 2 * i + 2, m + 1, vr);
        return max(ask_mx(l, m, 2 * i + 1, vl, m), ask_mx(m + 1, r, 2 * i + 2, m + 1, vr));
    }

    auto ask_mx(int l, int r) {
        if (l > r)
            return -inf;
        return ask_mx(l, r, 0, 0, n - 1);
    }

    auto ask_sm(int l, int r, int i, int vl, int vr) {
        if (tree[i].is_equal) {
            return tree[i].equal * (r - l + 1);
        }
        if (l == vl && r == vr) {
            return tree[i].ask_sm();
        }
        push(i, vl, vr);
        int m = (vl + vr) / 2;
        if (r <= m)
            return ask_sm(l, r, 2 * i + 1, vl, m);
        if (l > m)
            return ask_sm(l, r, 2 * i + 2, m + 1, vr);
        return ask_sm(l, m, 2 * i + 1, vl, m) + ask_sm(m + 1, r, 2 * i + 2, m + 1, vr);
    }

    auto ask_sm(int l, int r) {
        if (l > r)
            return 0;
        return ask_sm(l, r, 0, 0, n - 1);
    }
};
