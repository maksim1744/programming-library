template<typename T, typename F = std::function<T(const T&, const T&)>>
struct SparseTable {
    vector<vector<T>> table;
    vector<int> p2;
    F combine;

    SparseTable(int n, F combine) : combine(combine) {
        while ((1 << table.size()) <= n || table.empty())
            table.emplace_back(n);
    }
    template<typename U>
    SparseTable(const vector<U>& v, F combine) : SparseTable<T>(v.size(), combine) {
        table[0].assign(v.begin(), v.end());
        build();
    }

    void build() {
        p2.resize(table[0].size() + 1);
        for (int i = 2; i < p2.size(); ++i)
            p2[i] = p2[i / 2] + 1;
        for (int i = 1; i < table.size(); ++i) {
            for (int j = 0; j + (1 << i) <= table[i].size(); ++j) {
                table[i][j] = combine(table[i - 1][j], table[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    T ask(int l, int r) {
        int ln = p2[r - l + 1];
        if (r - l + 1 == ln) return table[ln][l];
        return combine(table[ln][l], table[ln][r - (1 << ln) + 1]);
    }
};
