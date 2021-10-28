mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
long long rnd (long long l, long long r) { return (long long)(rng() % (r - l + 1)) + l; }
long long rnd (long long r)       { return rng() % r; }
long long rnd ()           { return rng(); }
long double rndf()           { return (long double)rng() / (long double)ULLONG_MAX; }
long double rndf(long double l, long double r) { return rndf() * (r - l) + l; }

pair<int, int> gen_dif_pair(int l, int r) {
    while (true) {
        int u = rnd(l, r), v = rnd(l, r);
        if (u != v)
            return {u, v};
    }
}

pair<int, int> gen_lr(int l, int r, bool allow_same = true) {
    while (true) {
        int u = rnd(l, r), v = rnd(l, r);
        if (u == v && !allow_same) continue;
        if (u > v) swap(u, v);
        return {u, v};
    }
}

vector<pair<int, int>> gen_tree(int n) {
    struct DSU {
        vector<int> rk;
        vector<int> p;
        int n;

        DSU(int n = 1) : n(n) {
            reset(n);
        }

        void reset(int n) {
            p.resize(n);
            iota(p.begin(), p.end(), 0);
            rk.assign(n, 1);
        }

        int par(int v) {
            return v == p[v] ? v : p[v] = par(p[v]);
        }

        bool un(int u, int v) {
            u = par(u);
            v = par(v);
            if (u == v) return false;
            if (rk[u] > rk[v]) swap(u, v);
            p[u] = v;
            rk[v] += rk[u];
            return true;
        }
    };

    DSU d(n);
    vector<pair<int, int>> res;
    int comps = n;
    while (comps > 1) {
        auto [u, v] = gen_dif_pair(0, n - 1);
        if (d.un(u, v)) {
            res.emplace_back(u, v);
            --comps;
        }
    }
    return res;
}

vector<pair<int, int>> gen_graph(int n, int m, bool connected = false) {
    set<pair<int, int>> has;
    vector<pair<int, int>> res;
    if (connected) {
        res = gen_tree(n);
        for (auto& [u, v] : res)
            if (u > v)
                swap(u, v);
        has.insert(res.begin(), res.end());
    }
    while (res.size() < m) {
        auto p = gen_lr(0, n - 1, false);
        if (has.count(p)) continue;
        has.insert(p);
        res.push_back(p);
    }
    shuffle(res.begin(), res.end(), rng);
    for (auto& [u, v] : res)
        if (rnd(2))
            swap(u, v);
    return res;
}

template<typename T = int>
vector<T> gen_array(int n, T l, T r) {
    vector<T> res(n);
    for (int i = 0; i < n; ++i)
        res[i] = rnd(l, r);
    return res;
}
