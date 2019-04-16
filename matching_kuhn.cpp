bool matching_dfs(int v, vector<vector<int>>& g, vector<int>& from, vector<bool>& u) {
    u[v] = true;
    for (auto k : g[v]) {
        if (from[k] == -1 || (!u[from[k]] && matching_dfs(from[k], g, from, u))) {
            from[k] = v;
            return true;
        }
    }
    return false;
}

vector<pair<int, int>> find_matching(vector<vector<int>>& g, vector<int> a) {
    bool rev = false;
    if (2 * a.size() > g.size()) {
        rev = true;
        vector<bool> has(g.size(), false);
        for (auto v : a)
            has[v] = true;
        a.clear();
        a.reserve(g.size() - a.size());
        for (int i = 0; i < g.size(); ++i)
            if (!has[i])
                a.push_back(i);
    }
    vector<bool> u(g.size(), false);
    vector<int> from(g.size(), -1);
    for (auto v : a)
        if (matching_dfs(v, g, from, u))
            u.assign(g.size(), false);
    vector<pair<int, int>> result;
    for (int v = 0; v < from.size(); ++v) {
        if (from[v] != -1) {
            if (rev) result.emplace_back(v, from[v]);
            else result.emplace_back(from[v], v);
        }
    }
    return result;
}

bool dominanting_set_dfs(int v, vector<vector<int>>& g, vector<int>& with, vector<bool>& u,
                                                                           vector<bool>& take) {
    u[v] = true;
    for (auto k : g[v]) {
        take[k] = true;
        if (!u[with[k]])
            dominanting_set_dfs(with[k], g, with, u, take);
    }
}

vector<int> find_dominanting_set(vector<vector<int>>& g, vector<int>& a) {
    auto matching = find_matching(g, a);
    vector<int> with(g.size(), -1);
    for (auto edge : matching) {
        with[edge.first] = edge.second;
        with[edge.second] = edge.first;
    }
    vector<bool> u(g.size(), false);
    vector<bool> take(g.size(), false);
    for (auto v : a)
        if (with[v] == -1)
            dominanting_set_dfs(v, g, with, u, take);
    for (auto e : matching)
        if (!take[e.second])
            take[e.first] = true;
    vector<int> result(matching.size());
    for (int i = 0; i < matching.size(); ++i) {
        if (take[matching[i].second]) result[i] = matching[i].second;
        else result[i] = matching[i].first;
    }
    return result;
}

vector<int> find_independent_set(vector<vector<int>>& g, vector<int>& a) {
    vector<int> result;
    vector<bool> u(g.size(), false);
    for (auto v : find_dominanting_set(g, a))
        u[v] = true;
    for (int i = 0; i < g.size(); ++i)
        if (!u[i])
            result.push_back(i);
    return result;
}
