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
    queue<int> q;
    vector<int> ptr(n, 0);
    vector<int> to(n);
    for (int i = 0; i < n; ++i) {
        layer.assign(n, -5);
        ptr.assign(n, 0);
        q.push(s);
        layer[s] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto k : g[v]) {
                if (e[k].mx > 0 && layer[e[k].to] == -5) {
                    layer[e[k].to] = layer[v] + 1;
                    q.push(e[k].to);
                }
            }
        }
        if (layer[t] == -5)
            break;
        long long prev_flow = flow;
        while (true) {
            function<bool(int)> go = [&](int v) {
                if (v == t)
                    return true;
                while (ptr[v] < g[v].size()) {
                    if (layer[v] + 1 == layer[e[g[v][ptr[v]]].to] && e[g[v][ptr[v]]].mx > 0 && go(e[g[v][ptr[v]]].to)) {
                        to[v] = ptr[v];
                        return true;
                    } else {
                        ++ptr[v];
                    }
                }
                return false;
            };
            if (!go(s))
                break;
            int v = s;
            long long mn = 1e9l;
            while (v != t) {
                mn = min(mn, e[g[v][to[v]]].mx);
                v = e[g[v][to[v]]].to;
            }
            v = s;
            while (v != t) {
                e[g[v][to[v]]].mx -= mn;
                e[g[v][to[v]]^1].mx += mn;
                v = e[g[v][to[v]]].to;
            }
            flow += mn;
        }
        if (flow == prev_flow)
            break;
    }
    return flow;
}
