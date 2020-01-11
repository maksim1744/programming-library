vector<pair<int, int>> edmonds(vector<vector<int>>& g) {
    int n = g.size();
    vector<int> match(n, -1);
    vector<int> p(n);
    vector<bool> u(n);
    vector<bool> ulca(n);
    vector<int> q(n + 5, -1);
    vector<int> base(n);
    vector<bool> blossom(n);
    int ql = 0, qr = 0;

    auto lca = [&](int a, int b) {
        ulca.assign(n, false);
        while (true) {
            a = base[a];
            ulca[a] = true;
            if (match[a] == -1)
                break;
            a = p[match[a]];
        }
        while (true) {
            b = base[b];
            if (ulca[b]) return b;
            b = p[match[b]];
        }
        return -1;
    };

    auto mark_path = [&](int v, int b, int k) {
        while (base[v] != b) {
            blossom[base[v]] = true;
            blossom[base[match[v]]] = true;
            p[v] = k;
            k = match[v];
            v = p[match[v]];
        }
    };

    auto find_path = [&](int root) {
        p.assign(n, -1);
        u.assign(n, false);
        for (int i = 0; i < n; ++i)
            base[i] = i;
        ql = qr = 0;
        q[qr++] = root;
        u[root] = true;
        while (ql < qr) {
            int v = q[ql++];
            showt(v, base);
            show(match);
            show(p);
            for (auto k : g[v]) {
                show(k);
                if (base[v] == base[k] || match[v] == k) {
                    continue;
                } else if (k == root || match[k] != -1 && p[match[k]] != -1) {
                    blossom.assign(n, false);
                    int b = lca(v, k);
                    mark_path(v, b, k);
                    mark_path(k, b, v);
                    for (int i = 0; i < n; ++i) {
                        if (blossom[base[i]]) {
                            base[i] = b;
                            if (!u[i]) {
                                u[i] = true;
                                q[qr++] = i;
                            }
                        }
                    }
                } else if (p[k] == -1) {
                    p[k] = v;
                    if (match[k] == -1)
                        return k;
                    k = match[k];
                    u[k] = true;
                    q[qr++] = k;
                }
            }
        }
        return -1;
    };

    for (int i = 0; i < n; ++i) {
        if (match[i] == -1) {
            showt(i, match);
            int v = find_path(i);
            show(v);
            show(p, match);
            while (v != -1) {
                match[v] = p[v];
                int k = match[p[v]];
                match[p[v]] = v;
                v = k;
            }
        }
    }
    show(match);
    vector<pair<int, int>> ans;
    for (int i = 0; i < n; ++i)
        if (match[i] > i)
            ans.eb(i, match[i]);
    show(ans);
    return ans;
}
