// O(n*log)
vector<int> build_suf_array(string s, bool add_dollar = true) {
    if (add_dollar)
        s += '$';
    int n = s.size();
    vector<pair<int, int>> cur(s.size());
    for (int i = 0; i < s.size(); ++i) {
        cur[i] = make_pair(i, 0);
    }
    sort(cur.begin(), cur.end(), [&](const pair<int, int> &a, const pair<int, int> &b) {
        return s[a.first] < s[b.first];
    });
    for (int i = 1; i < s.size(); ++i) {
        cur[i].second = cur[i - 1].second + (s[cur[i].first] != s[cur[i - 1].first]);
    }
    vector<int> ind(n);
    vector<int> count(n);
    vector<pair<int, int>> tmp(n);
    for (int k = 1; k <= n; k *= 2) {
        for (int i = 0; i < n; ++i) {
            ind[cur[i].first] = cur[i].second;
        }
        for (int i = 0; i < n; ++i) {
            cur[i].first = (cur[i].first - k + n) % n;
            cur[i].second = ind[cur[i].first];
        }
        count.assign(count.size(), 0);
        for (int i = 0; i < n; ++i) {
            count[cur[i].second]++;
        }
        for (int i = 1; i < n; ++i)
            count[i] += count[i - 1];
        for (int i = 0; i < n; ++i)
            --count[i];
        for (int i = n - 1; i >= 0; --i) {
            tmp[count[cur[i].second]--] = make_pair(cur[i].first, 0);
        }
        swap(cur, tmp);
        for (int i = 1; i < n; ++i) {
            cur[i].second = cur[i - 1].second;
            if (ind[cur[i].first] != ind[cur[i - 1].first] || ind[(cur[i].first + k) % n] != ind[(cur[i - 1].first + k) % n])
                ++cur[i].second;
        }
    }
    vector<int> res(n);
    for (int i = 0; i < cur.size(); ++i) {
        res[i] = cur[i].first;
    }

    if (add_dollar)
        res.erase(res.begin());

    return res;
}
