vector<vector<int>> dominator_tree(vector<vector<int>> g, int root) {
    int n = g.size();
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }
    swap(p[root], p[0]);
    swap(g[0], g[root]);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            g[i][j] = p[g[i][j]];
        }
    }
    vector<vector<int>> tree(n);
    vector<vector<int>> bucket(n);
    vector<int> sdom(n), dom(n), par(n), label(n), dsu(n);
    vector<vector<int>> gi(n);
    vector<int> arr(n, -1), rev(n);
    int tm = 0;

    function<int(int, int)> ask = [&](int u, int x) {
        if (u == dsu[u]) return x ? -1 : u;
        int v = ask(dsu[u], x + 1);
        if (v < 0) return u;
        if (sdom[label[dsu[u]]] < sdom[label[u]])
            label[u] = label[dsu[u]];
        dsu[u] = v;
        return x ? v : label[u];
    };
    auto un = [&](int u, int v) {
        dsu[v] = u;
    };

    function<void(int)> dfs = [&](int v) {
        arr[v] = tm;
        rev[tm] = v;
        label[tm] = sdom[tm] = dsu[tm] = tm;
        ++tm;
        for (int k : g[v]) {
            if (arr[k] == -1) {
                dfs(k);
                par[arr[k]] = arr[v];
            }
            gi[arr[k]].pb(arr[v]);
        }
    };
    dfs(0);
    assert(tm == n);  // connected

    for (int i = n - 1; i >= 0; --i) {
        for (int j : gi[i]) {
            sdom[i] = min(sdom[i], sdom[ask(j, 0)]);
        }
        if (i != 0) bucket[sdom[i]].pb(i);
        for (int w : bucket[i]) {
            int v = ask(w, 0);
            if (sdom[v] == sdom[w]) dom[w] = sdom[w];
            else dom[w] = v;
        }
        if (i != 0) un(par[i], i);
    }
    for (int i = 1; i < n; ++i) {
        if (dom[i] != sdom[i])
            dom[i] = dom[dom[i]];
        tree[rev[dom[i]]].pb(rev[i]);
        tree[rev[i]].pb(rev[dom[i]]);
    }

    swap(tree[root], tree[0]);
    for (int i = 0; i < tree.size(); ++i) {
        for (int j = 0; j < tree[i].size(); ++j) {
            tree[i][j] = p[tree[i][j]];
        }
    }

    return tree;
}
