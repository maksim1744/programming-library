// O(n*log^2)
vector<int> build_suf_array(string s) {
    bool add_dollar = true;
    if (add_dollar)
        s += '$';
    int n = s.size();
    vector<pair<int, int>> cur;
    for (int i = 0; i < s.size(); ++i) {
        cur.eb(i, 0);
    }
    sort(cur.begin(), cur.end(), [&](pair<int, int> a, pair<int, int> b) {
        return s[a.first] < s[b.first];
    });
    for (int i = 1; i < s.size(); ++i) {
        cur[i].second = cur[i - 1].second + (s[cur[i].first] != s[cur[i - 1].first]);
    }
    for (int k = 1; k <= n; k *= 2) {
        vector<pair<int, int>> ind(n);
        for (int i = 0; i < n; ++i) {
            ind[cur[i].first] = mp(i, cur[i].second);
        }
        auto comp = [&](int i, int j) {
            if (ind[i].second != ind[j].second) {
                return ind[i].second < ind[j].second ? -1 : 1;
            }
            int i1 = (i + k) % n;
            int j1 = (j + k) % n;
            if (ind[i1].second == ind[j1].second) return 0;
            if (ind[i1].second < ind[j1].second) return -1;
            if (ind[i1].second > ind[j1].second) return 1;
        };
        sort(cur.begin(), cur.end(), [&](pair<int, int> a, pair<int, int> b) {
            return comp(a.first, b.first) == -1;
        });
        cur[0].second = 0;
        for (int i = 1; i < cur.size(); ++i) {
            cur[i].second = cur[i - 1].second + (comp(cur[i - 1].first, cur[i].first) != 0);
        }
    }
    vector<int> res;
    for (auto [ind, group] : cur) {
        res.pb(ind);
    }
    if (add_dollar)
        res.erase(res.begin());

    return res;
}
