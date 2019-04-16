auto find_matching_dinic(vector<vector<int>>& g_main, vector<int>& a) {
    int n = g_main.size();
    vector<vector<int>> g(n + 2);
    vector<int> e;
    vector<bool> can;
    vector<bool> in_a(n, false);
    auto add = [&](int i, int j) {
        g[i].push_back(e.size());
        e.push_back(j);
        can.push_back(true);
        g[j].push_back(e.size());
        e.push_back(i);
        can.push_back(false);
    };
    for (auto u : a) {
        in_a[u] = true;
        for (auto v : g_main[u])
            add(u, v);
    }
    int s = n, t = n + 1;
    for (int u = 0; u < n; ++u) {
        if (in_a[u])
            add(s, u);
        else
            add(u, t);
    }
    n += 2;
    int flow = 0;
    vector<int> layer(n, -5);
    vector<int> q(n + 10);
    int ql, qr;
    vector<int> ptr(n, 0);
    vector<int> to(n);
    for (int i = 0; i < n; ++i) {
        layer.assign(n, -5);
        ptr.assign(n, 0);
        ql = 0;
        qr = 1;
        q[0] = s;
        layer[s] = 0;
        while (ql < qr) {
            int v = q[ql++];
            for (auto k : g[v]) {
                if (can[k] && layer[e[k]] == -5) {
                    layer[e[k]] = layer[v] + 1;
                    q[qr++] = e[k];
                }
            }
        }
        if (layer[t] == -5)
            break;
        int prev_flow = flow;
        while (true) {
            function<bool(int)> go = [&](int v) -> bool {
                if (v == t)
                    return true;
                while (ptr[v] < g[v].size()) {
                    if (layer[v] + 1 == layer[e[g[v][ptr[v]]]] && can[g[v][ptr[v]]]) {
                        int x = go(e[g[v][ptr[v]]]);
                        if (x) {
                            can[g[v][ptr[v]]] = false;
                            can[g[v][ptr[v]]^1] = true;
                            return true;
                        } else {
                            ++ptr[v];
                        }
                    } else {
                        ++ptr[v];
                    }
                }
                return false;
            };
            if (!go(s))
                break;
            ++flow;
        }
        if (flow == prev_flow)
            break;
    }
    // return flow;  // if need only size
    vector<pair<int, int>> res;
    for (auto i : a) {
        for (auto k : g[i]) {
            if (e[k] != s && !can[k])
                res.emplace_back(i, e[k]);
        }
    }
    return res;
}
