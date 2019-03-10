bool matching_dfs(int v, vector< vector< int > >& g, vector< int >& from, vector< bool >& u) {
    u[v] = true;
    for (auto k : g[v]) {
        if (from[k] == -1 || (!u[from[k]] && matching_dfs(from[k], g, from, u))) {
            from[k] = v;
            return true;
        }
    }
    return false;
}

vector< pair< int, int > > matching(vector< vector< int > >& g, vector< int > a, vector< int > b = {}) {
    if (b.size() + a.size() != g.size()) {
        vector< bool > has(g.size(), false);
        for (auto v : a) {
            has[v] = true;
        }
        for (int i = 0; i < g.size(); ++i) {
            if (!has[i]) {
                b.push_back(i);
            }
        }
    }
    if (a.size() > b.size()) swap(a, b);
    vector< bool > u(g.size(), false);
    vector< int > from(g.size(), -1);
    for (auto v : a) {
        if (matching_dfs(v, g, from, u)) {
        }
        u.assign(g.size(), false);
    }
    vector< pair< int, int > > result;
    for (int i = 0; i < from.size(); ++i) {
        if (from[i] != -1) result.emplace_back(from[i], i);
    }
    return result;
}
