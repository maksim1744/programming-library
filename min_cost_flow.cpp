using COST_T = long long;
struct mfmc_edge {
    int from, to;
    long long mx;
    long long cost; // for 1 flow
};

pair<ll, COST_T> max_flow_min_cost(vector<vector<pair<int, pair<ll, COST_T>>>> g_main, int s, int t, ll k) {
    int n = g_main.size();
    vector<vector<int>> g(n);
    vector<mfmc_edge> e;
    for (int u = 0; u < n; ++u) {
        for (auto p : g_main[u]) {
            int v = p.first;
            ll w = p.second.first, c = p.second.second;
            g[u].push_back(e.size());
            e.push_back({u, v, w, c});  // !!! may need to change c to c/w !!!
            g[v].push_back(e.size());
            e.push_back({v, u, 0, -c});
        }
    }
    const ll inf = 1e18l * 4;
    vector<COST_T> p(n, inf);
    p[s] = 0;
    vector<int> par(n, -1);
    for (int i = 0; i < n; ++i) {
        bool change = false;
        for (int j = 0; j < e.size(); ++j) {
            auto edge = e[j];
            if (edge.mx > 0 && p[edge.from] != inf) {
                if (p[edge.to] > p[edge.from] + edge.cost) {
                    p[edge.to] = p[edge.from] + edge.cost;
                    par[edge.to] = j;
                    change = true;
                }
            }
        }
        if (!change)
            break;
    }
    pair<ll, COST_T> ans = {0, 0};
    vector<ll> d = p;
    for (int i = 0; i < k; ++i) {
        if (par[t] == -1)
            break;
        int v = t;
        ll flow = inf, cost = 0;
        while (v != s) {
            flow = min(flow, e[par[v]].mx);
            cost += e[par[v]].cost;
            v = e[par[v]].from;
        }
        v = t;
        while (v != s) {
            e[par[v]].mx -= flow;
            e[par[v]^1].mx += flow;
            v = e[par[v]].from;
        }
        ans.first += flow;
        ans.second += cost;
        par.assign(n, -1);
        d.assign(n, inf);
        d[s] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            int v = pq.top().second;
            ll ds = pq.top().first;
            pq.pop();
            if (ds > d[v])
                continue;
            for (auto i : g[v]) {
                if (e[i].mx > 0 && d[e[i].to] > d[v] + e[i].cost + p[e[i].from] - p[e[i].to]) {
                    d[e[i].to] = d[v] + e[i].cost + p[e[i].from] - p[e[i].to];
                    par[e[i].to] = i;
                    pq.push({d[e[i].to], e[i].to});
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (d[v] == inf || p[v] == inf)
                p[v] = inf;
            else
                p[v] += d[v];
        }
    }
    return ans;
}
