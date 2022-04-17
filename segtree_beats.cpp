namespace segtree_ns {

// supports min=, max=, += on a segment, maintains sum on a segment

const ll inf = 1e18;

struct mod_max { ll v; };
struct mod_min { ll v; };
struct mod_add { ll v; };

struct item {
    ll mn1, mn2, mx1, mx2, sm;
    ll cmn1, cmx1;
    mod_add madd{0};

    template<typename T>
    void init(const T &t, int l, int r) {
        mn1 = mx1 = t;
        mn2 = inf;
        mx2 = -inf;
        cmn1 = cmx1 = r - l + 1;
        sm = t * cmn1;
    }

    void update(const item &first, const item &second, int l, int r) {
        sm = first.sm + second.sm;

        mn1 = min(first.mn1, second.mn1);
        if (first.mn1 < second.mn1) cmn1 = first.cmn1;
        else if (first.mn1 > second.mn1) cmn1 = second.cmn1;
        else cmn1 = first.cmn1 + second.cmn1;

        mx1 = max(first.mx1, second.mx1);
        if (first.mx1 > second.mx1) cmx1 = first.cmx1;
        else if (first.mx1 < second.mx1) cmx1 = second.cmx1;
        else cmx1 = first.cmx1 + second.cmx1;

        mn2 = min(mn1 ==  first.mn1 ?  first.mn2 :  first.mn1,
                  mn1 == second.mn1 ? second.mn2 : second.mn1);
        mx2 = max(mx1 ==  first.mx1 ?  first.mx2 :  first.mx1,
                  mx1 == second.mx1 ? second.mx2 : second.mx1);
    }

    static item merge(const item &first, const item &second, int l, int r) {
        item res;
        res.update(first, second, l, r);  // careful with different lengths
        return res;
    }

    // return true if modification can be applied
    bool modify(const mod_add &md, int l, int r) {
        ll m = md.v;
        sm += m * (r - l + 1);
        mn1 += m;
        mn2 += m;
        mx1 += m;
        mx2 += m;
        madd.v += m;
        return true;
    }

    // return true if modification can be applied
    bool modify(const mod_min &md, int l, int r) {
        ll m = md.v;

        if (m >= mx1)
            return true;

        if (m <= mn1) {
            init(m, l, r);
            return true;
        }
        if (m > mx2) {
            sm += cmx1 * (m - mx1);
            if (mx1 == mn2)
                mn2 = m;
            mx1 = m;
            return true;
        }
        return false;
    }

    // return true if modification can be applied
    bool modify(const mod_max &md, int l, int r) {
        ll m = md.v;

        if (m <= mn1)
            return true;

        if (m >= mx1) {
            init(m, l, r);
            return true;
        }
        if (m < mn2) {
            sm += cmn1 * (m - mn1);
            if (mn1 == mx2)
                mx2 = m;
            mn1 = m;
            return true;
        }
        return false;
    }

    void push(item &first, item &second, int l, int r) {
        int m = (l + r) / 2;
        if (mn1 == mx1) {
            first.init(mn1, l, m);
            second.init(mn1, m + 1, r);
        } else {
            first.modify(madd, l, m);
            first.modify(mod_min{mx1}, l, m);
            first.modify(mod_max{mn1}, l, m);
            second.modify(madd, m + 1, r);
            second.modify(mod_min{mx1}, m + 1, r);
            second.modify(mod_max{mn1}, m + 1, r);
        }
        madd.v = 0;
    }
};

string to_string(const item &i) {
    stringstream ss;
    ss << "[" << "]";
    return ss.str();
}
ostream& operator << (ostream &o, const item &i) {
    return o << to_string(i);
}

struct segtree {
    vector<item> tree;
    int n = 1;

    segtree(int n = 1) : n(n) {
        tree.resize(1 << (__lg(max(1, n - 1)) + 2));
    }

    template<typename T>
    segtree(const vector<T>& v) {
        build(v);
    }

    template<typename T>
    void build(const vector<T> &v, int i, int l, int r) {
        if (l == r) {
            tree[i].init(v[l], l, r);
            return;
        }
        int m = (l + r) >> 1;
        build(v, i * 2 + 1, l, m);
        build(v, i * 2 + 2, m + 1, r);
        tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], l, r);
    }

    template<typename T>
    void build(const vector<T> &v) {
        n = v.size();
        tree.assign(1 << (__lg(max(1, n - 1)) + 2), item());
        build(v, 0, 0, n - 1);
    }

    item ask(int l, int r, int i, int vl, int vr) {
        if (vl != vr) {
            tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
        }
        if (l == vl && r == vr) {
            return tree[i];
        }
        int m = (vl + vr) >> 1;
        if (r <= m) {
            return ask(l, r, i * 2 + 1, vl, m);
        } else if (m < l) {
            return ask(l, r, i * 2 + 2, m + 1, vr);
        } else {
            return item::merge(ask(l, m, i * 2 + 1, vl, m), ask(m + 1, r, i * 2 + 2, m + 1, vr), l, r);
        }
    }

    item ask(int l, int r) {
        l = max(l, 0); r = min(r, n - 1);
        if (l > r) return item();
        return ask(l, r, 0, 0, n - 1);
    }

    template<typename T>
    void set(int ind, const T &t) {
        static array<pair<int, int>, 30> st;
        int l = 0, r = n - 1, i = 0;
        int ptr = -1;
        while (l != r) {
            if (l != r) {
                tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], l, r);
            }
            st[++ptr] = {l, r};
            int m = (l + r) >> 1;
            if (ind <= m) {
                i = i * 2 + 1;
                r = m;
            } else {
                i = i * 2 + 2;
                l = m + 1;
            }
        }
        tree[i].init(t, l, r);
        while (i != 0) {
            i = (i - 1) / 2;
            tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], st[ptr].first, st[ptr].second);
            --ptr;
        }
    }

    template<typename Modifier>
    void modify(int l, int r, const Modifier &modifier, int i, int vl, int vr) {
        if (vl != vr) {
            tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
        }
        if (l == vl && r == vr && tree[i].modify(modifier, vl, vr)) {
            return;
        }
        int m = (vl + vr) >> 1;
        if (r <= m) {
            modify(l, r, modifier, i * 2 + 1, vl, m);
        } else if (m < l) {
            modify(l, r, modifier, i * 2 + 2, m + 1, vr);
        } else {
            modify(l, m, modifier, i * 2 + 1, vl, m);
            modify(m + 1, r, modifier, i * 2 + 2, m + 1, vr);
        }
        tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
    }

    template<typename Modifier>
    void modify(int l, int r, const Modifier &modifier) {
        l = max(l, 0); r = min(r, n - 1);
        if (l > r) return;
        modify(l, r, modifier, 0, 0, n - 1);
    }
};
}
using segtree_ns::segtree, segtree_ns::mod_min, segtree_ns::mod_max, segtree_ns::mod_add;
