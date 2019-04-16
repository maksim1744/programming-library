void compress(vector<int>& v, int from = 0) {
    set<int> s;
    for (auto k : v) {
        s.insert(k);
    }
    vector<int> vs(s.begin(), s.end());
    for (auto& k : v) {
        k = from + distance(vs.begin(), lower_bound(vs.begin(), vs.end(), k));
    }
}
