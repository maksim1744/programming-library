namespace segtree {

// This implementation is disgusting, but it seems to work and do it faster than previous version.

template<typename Item>
Item tree_merge(const Item& a, const Item& b) {
    Item i;
    i.update(a, b);
    return i;
}

template<typename Item, bool lazy>
struct Pusher {};

template<typename Item>
struct Pusher<Item, false> {
    void push(const vector<Item>&, int, int, int) {}
    Item ask_on_segment(const vector<Item>& tree, int n, int l, int r) {
        l |= n;
        r |= n;
        Item resl, resr;
        while (l <= r) {
            if (l & 1) {
                resl = tree_merge(resl, tree[l]);
                ++l;
            }
            if (!(r & 1)) {
                resr = tree_merge(tree[r], resr);
                --r;
            }
            l >>= 1;
            r >>= 1;
        }
        return tree_merge(resl, resr);
    }
    void push_point(const vector<Item>&, int, int) {}

    template<typename P>
    int lower_bound(const vector<Item>& tree, int n, int l, P p) {
        Item cur;
        if (p(cur)) return l - 1;
        l |= n;
        int r = n | (n - 1);
        // carefully go up
        while (true) {
            if (p(tree_merge(cur, tree[l]))) {
                break;
            }
            if (l == r) return n;
            if (l & 1) {
                cur = tree_merge(cur, tree[l]);
                ++l;
            }
            l >>= 1;
            r >>= 1;
        }

        // usual descent from l
        while (l < n) {
            if (p(tree_merge(cur, tree[l * 2]))) {
                l = l * 2;
            } else {
                cur = tree_merge(cur, tree[l * 2]);
                l = l * 2 + 1;
            }
        }
        return (l ^ n);
    }

    template<typename P>
    int lower_bound_rev(const vector<Item>& tree, int n, int r, P p) {
        Item cur;
        if (p(cur)) return r + 1;
        r |= n;
        int l = n;
        // carefully go up
        while (true) {
            if (p(tree_merge(tree[r], cur))) {
                break;
            }
            if (l == r) return -1;
            if (!(r & 1)) {
                cur = tree_merge(tree[r], cur);
                --r;
            }
            l >>= 1;
            r >>= 1;
        }

        // usual descent from r
        while (r < n) {
            if (p(tree_merge(tree[r * 2 + 1], cur))) {
                r = r * 2 + 1;
            } else {
                cur = tree_merge(tree[r * 2 + 1], cur);
                r = r * 2;
            }
        }
        return (r ^ n);
    }
};

template<typename Item>
struct Pusher<Item, true> {
    void push(vector<Item>& tree, int ind, int l, int r) {
        tree[ind].push(tree[ind * 2], tree[ind * 2 + 1], l, r);
    }

    Item ask_on_segment(vector<Item>& tree, int n, int l, int r) {
        int vl = 0, vr = n - 1;
        int i = 1;
        Item result;
        while (vl != vr) {
            int m = (vl + vr) / 2;
            if (l > m) {
                push(tree, i, vl, vr);
                i = i * 2 + 1;
                vl = m + 1;
            } else if (r <= m) {
                push(tree, i, vl, vr);
                i = i * 2;
                vr = m;
            } else {
                break;
            }
        }
        if (l == vl && r == vr) {
            return tree[i];
        }
        push(tree, i, vl, vr);
        // left
        {
            int ind = i * 2;
            int L = vl, R = (vl + vr) / 2;
            while (l != L) {
                int m = (L + R) / 2;
                push(tree, ind, L, R);
                if (l <= m) {
                    result = tree_merge(tree[ind * 2 + 1], result);
                    ind *= 2;
                    R = m;
                } else {
                    ind = ind * 2 + 1;
                    L = m + 1;
                }
            }
            result = tree_merge(tree[ind], result);
        }
        // right
        {
            int ind = i * 2 + 1;
            int L = (vl + vr) / 2 + 1, R = vr;
            while (r != R) {
                int m = (L + R) / 2;
                push(tree, ind, L, R);
                if (r > m) {
                    result = tree_merge(result, tree[ind * 2]);
                    ind = ind * 2 + 1;
                    L = m + 1;
                } else {
                    ind = ind * 2;
                    R = m;
                }
            }
            result = tree_merge(result, tree[ind]);
        }
        return result;
    }

    void push_point(vector<Item>& tree, int n, int ind) {
        int l = 0, r = n - 1;
        int i = 1;
        while (l != r) {
            push(tree, i, l, r);
            int m = (l + r) / 2;
            if (ind <= m) {
                r = m;
                i *= 2;
            } else {
                l = m + 1;
                i = i * 2 + 1;
            }
        }
    }

    template<typename P>
    pair<int, Item> _lower_bound(vector<Item>& tree, int l, P p, Item cur, int i, int vl, int vr) {
        if (vl == vr) {
            if (p(tree_merge(cur, tree[i]))) {
                return {vl, tree[i]};
            } else {
                return {vl + 1, tree[i]};
            }
        }

        push(tree, i, vl, vr);
        int m = (vl + vr) / 2;
        if (l > m) {
            return _lower_bound(tree, l, p, cur, i * 2 + 1, m + 1, vr);
        } else if (l <= vl) {
            if (!p(tree_merge(cur, tree[i]))) {
                return {vr + 1, tree_merge(cur, tree[i])};
            }
            if (p(tree_merge(cur, tree[i * 2]))) {
                return _lower_bound(tree, l, p, cur, i * 2, vl, m);
            } else {
                return _lower_bound(tree, l, p, tree_merge(cur, tree[i * 2]), i * 2 + 1, m + 1, vr);
            }
        } else {
            auto [ind, it] = _lower_bound(tree, l, p, cur, i * 2, vl, m);
            if (ind <= m) return {ind, it};
            return _lower_bound(tree, l, p, it, i * 2 + 1, m + 1, vr);
        }
    }

    template<typename P>
    int lower_bound(vector<Item>& tree, int n, int l, P p) {
        Item cur;
        if (p(cur)) return l - 1;
        return _lower_bound(tree, l, p, cur, 1, 0, n - 1).first;
    }

    template<typename P>
    pair<int, Item> _lower_bound_rev(vector<Item>& tree, int r, P p, Item cur, int i, int vl, int vr) {
        if (vl == vr) {
            if (p(tree_merge(tree[i], cur))) {
                return {vl, tree[i]};
            } else {
                return {vl - 1, tree[i]};
            }
        }

        push(tree, i, vl, vr);
        int m = (vl + vr) / 2;
        if (r <= m) {
            return _lower_bound_rev(tree, r, p, cur, i * 2, vl, m);
        } else if (r >= vr) {
            if (!p(tree_merge(tree[i], cur))) {
                return {vl - 1, tree_merge(cur, tree[i])};
            }
            if (p(tree_merge(tree[i * 2 + 1], cur))) {
                return _lower_bound_rev(tree, r, p, cur, i * 2 + 1, m + 1, vr);
            } else {
                return _lower_bound_rev(tree, r, p, tree_merge(tree[i * 2 + 1], cur), i * 2, vl, m);
            }
        } else {
            auto [ind, it] = _lower_bound_rev(tree, r, p, cur, i * 2 + 1, m + 1, vr);
            if (ind > m) return {ind, it};
            return _lower_bound_rev(tree, r, p, it, i * 2, vl, m);
        }
    }

    template<typename P>
    int lower_bound_rev(vector<Item>& tree, int n, int r, P p) {
        Item cur;
        if (p(cur)) return r + 1;
        return _lower_bound_rev(tree, r, p, cur, 1, 0, n - 1).first;
    }
};

template<typename Item, bool lazy = false>
struct Segtree {
    vector<Item> tree;
    Pusher<Item, lazy> pusher;
    int n;
    int n0;

    Segtree(int n = 0) {
        build(n);
    }

    template<typename U>
    Segtree(const vector<U>& v) {
        build(v);
    }

    void build(int n) {
        this->n0 = n;
        while (n & (n - 1)) ++n;
        this->n = n;
        tree.assign(n * 2, {});
    }

    template<typename U>
    void build(const vector<U>& v) {
        build(v.size());
        for (int i = 0; i < v.size(); ++i) {
            tree[n | i].init(v[i], i);
        }
        build();
    }

    void build() {
        for (int i = n - 1; i >= 1; --i) {
            tree[i].update(tree[i * 2], tree[i * 2 + 1]);
        }
    }

    void push(int ind, int l, int r) {
        pusher.push(tree, ind, l, r);
    }

    template<typename T>
    void set(int ind, const T& t) {
        pusher.push_point(tree, n, ind);
        ind |= n;
        tree[ind].init(t, ind ^ n);
        ind >>= 1;
        while (ind) {
            tree[ind].update(tree[ind * 2], tree[ind * 2 + 1]);
            ind >>= 1;
        }
    }

    template<typename T>
    void update(int ind, const T& t) {
        pusher.push_point(tree, n, ind);
        ind |= n;
        tree[ind].update(t, ind ^ n);
        ind >>= 1;
        while (ind) {
            tree[ind].update(tree[ind * 2], tree[ind * 2 + 1]);
            ind >>= 1;
        }
    }

    Item& ith(int ind) {
        static_assert(!lazy, "don't use this method with lazy propagation, unless you're sure you need it");
        return tree[ind | n];
    }

    const Item& root() const {
        return tree[1];
    }

    Item ask(int l, int r) {
        l = max(l, 0);
        r = min(r, n - 1);
        if (l > r) return {};
        return pusher.ask_on_segment(tree, n, l, r);
    }

    template<typename T>
    void modify(int l, int r, const T& t) {
        static_assert(lazy, "lazy must be set to true to use this function");
        l = max(l, 0);
        r = min(r, n - 1);
        if (l > r) return;
        int vl = 0, vr = n - 1;
        int i = 1;
        while (vl != vr) {
            int m = (vl + vr) / 2;
            if (l > m) {
                push(i, vl, vr);
                i = i * 2 + 1;
                vl = m + 1;
            } else if (r <= m) {
                push(i, vl, vr);
                i = i * 2;
                vr = m;
            } else {
                break;
            }
        }
        if (l == vl && r == vr) {
            tree[i].modify(t, l, r);
        } else {
            push(i, vl, vr);
            // left
            {
                int ind = i * 2;
                int L = vl, R = (vl + vr) / 2;
                while (l != L) {
                    int m = (L + R) / 2;
                    push(ind, L, R);
                    if (l <= m) {
                        tree[ind * 2 + 1].modify(t, m + 1, R);
                        ind *= 2;
                        R = m;
                    } else {
                        ind = ind * 2 + 1;
                        L = m + 1;
                    }
                }
                tree[ind].modify(t, L, R);
                ind >>= 1;
                while (ind != i) {
                    tree[ind].update(tree[ind * 2], tree[ind * 2 + 1]);
                    ind >>= 1;
                }
            }
            // right
            {
                int ind = i * 2 + 1;
                int L = (vl + vr) / 2 + 1, R = vr;
                while (r != R) {
                    int m = (L + R) / 2;
                    push(ind, L, R);
                    if (r > m) {
                        tree[ind * 2].modify(t, L, m);
                        ind = ind * 2 + 1;
                        L = m + 1;
                    } else {
                        ind = ind * 2;
                        R = m;
                    }
                }
                tree[ind].modify(t, L, R);
                ind >>= 1;
                while (ind != i) {
                    tree[ind].update(tree[ind * 2], tree[ind * 2 + 1]);
                    ind >>= 1;
                }
            }
            tree[i].update(tree[i * 2], tree[i * 2 + 1]);
        }
        i >>= 1;
        while (i) {
            tree[i].update(tree[i * 2], tree[i * 2 + 1]);
            i >>= 1;
        }
    }

    // first index r such that p(tree.ask(l, r)) == true
    // if p() is true for empty item, return l-1
    // if p() is never true, returns n
    template<typename P>
    int lower_bound(int l, P p) {
        l = max(l, 0);
        if (l >= n0) return n0;
        return min(n0, pusher.lower_bound(tree, n, l, p));
    }

    // similarly to lower_bound, returns first (largest) l such that p(tree.ask(l, r)) == true
    template<typename P>
    int lower_bound_rev(int r, P p) {
        r = min(r, n0 - 1);
        if (r < 0) return -1;
        return pusher.lower_bound_rev(tree, n, r, p);
    }
};

}
using segtree::Segtree;

struct Item {
    template<typename T>
    void init(const T& t, int ind) {
    }

    void update(const Item& a, const Item& b) {
    }

    //// similar to init, but more convenient for doing a[i] += x, implement only if needed
    // template<typename T>
    // void update(const T& t, int ind) {}

    //// apply here, save for children
    // template<typename T>
    // void modify(const T& m, int l, int r) {}

    // void push(Item& a, Item& b, int l, int r) {
    //     int m = (l + r) / 2;
    //     a.modify(mod, l, m);
    //     b.modify(mod, m + 1, r);
    //     // reset mod
    // }
};
