struct dinic_edge {
    int from, to;
    long long mx;
};

long long max_flow_dinic(vector<vector<pair<int, long long>>> g_main, int s, int t) {
    int n = g_main.size();
    vector<vector<int>> g(n);
    vector<dinic_edge> e;
    for (int u = 0; u < n; ++u) {
        for (auto pr : g_main[u]) {
            int v = pr.first;
            long long w = pr.second;
            g[u].push_back(e.size());
            e.push_back({u, v, w});
            g[v].push_back(e.size());
            e.push_back({v, u, 0});
        }
    }
    long long flow = 0;
    vector<int> layer(n, -5);
    vector<int> q(n + 10);
    int ql, qr;
    vector<int> ptr(n, 0);
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
                if (e[k].mx > 0 && layer[e[k].to] == -5) {
                    layer[e[k].to] = layer[v] + 1;
                    q[qr++] = e[k].to;
                }
            }
        }
        if (layer[t] == -5)
            break;
        long long prev_flow = flow;
        while (true) {
            function<long long(int, long long)> go = [&](int v, long long now) -> long long {
                if (v == t)
                    return now;
                while (ptr[v] < g[v].size()) {
                    if (layer[v] + 1 == layer[e[g[v][ptr[v]]].to] && e[g[v][ptr[v]]].mx > 0) {
                        long long next = min(now, e[g[v][ptr[v]]].mx);
                        long long x = go(e[g[v][ptr[v]]].to, next);
                        if (x > 0) {
                            e[g[v][ptr[v]]].mx -= x;
                            e[g[v][ptr[v]]^1].mx += x;
                            return x;
                        } else {
                            ++ptr[v];
                        }
                    } else {
                        ++ptr[v];
                    }
                }
                return 0;
            };
            long long el_flow = go(s, 1e9l * 1e9l * 4l);
            if (el_flow == 0)
                break;
            flow += el_flow;
        }
        if (flow == prev_flow)
            break;
    }
    return flow;
}
