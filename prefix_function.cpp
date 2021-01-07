template<typename T>
vector<int> prefix_function(const T &s) {
    int n = s.size();
    vector<int> p(n, 0);
    for (int i = 1; i < n; ++i) {
        p[i] = (s[i] == s[0]);
        int x = i;
        while (x) {
            x = p[x - 1];
            if (s[x] == s[i]) {
                p[i] = x + 1;
                break;
            }
        }
    }
    return p;
}
