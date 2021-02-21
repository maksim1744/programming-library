// returns vector of n vertices on a line and vector of n-1 edges between them
template<typename T, typename OP = std::less<T>>
pair<vector<int>, vector<T>> tree_to_line_save_max(vector<pair<T, pair<int, int>>> edges) {
    sort(edges.begin(), edges.end(), [&](const auto &a, const auto &b) {
        return OP()(a.first, b.first);
    });
    int n = edges.size() + 1;
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    auto left = p, right = p;
    vector<int> rk(n, 1);

    vector<vector<pair<int, T>>> near(n);

    function<int(int)> par = [&](int v) {
        return v == p[v] ? v : p[v] = par(p[v]);
    };

    auto un = [&](int u, int v) {
        u = par(u);
        v = par(v);
        if (u == v) return;
        if (rk[u] > rk[v]) swap(u, v);
        p[u] = v;
        rk[v] += rk[u];
    };

    for (auto [w, uv] : edges) {
        auto [u, v] = uv;
        u = par(u);
        v = par(v);
        assert(u != v);
        near[right[u]].emplace_back(left[v], w);
        near[left[v]].emplace_back(right[u], w);
        un(u, v);
        int k = par(u);
        left[k] = left[u];
        right[k] = right[v];
    }

    int x = par(0);
    assert(rk[x] == n);
    x = left[x];

    vector<int> vert = {x};
    vector<T> w;
    if (n == 1)
        return {vert, w};
    assert(near[x].size() == 1);

    int prev = x;
    w.push_back(near[x][0].second);
    x = near[x][0].first;
    vert.push_back(x);
    for (int i = 0; i < n - 2; ++i) {
        int ind = 0;
        if (near[x][ind].first == prev) ind = 1;
        w.push_back(near[x][ind].second);
        prev = x;
        x = near[x][ind].first;
        vert.push_back(x);
    }

    return {vert, w};
}
