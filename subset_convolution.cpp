// res[i] = sum_{j = submask(i)} a[i] * b[i ^ j]
template<typename T>
vector<T> subset_convolution(const vector<T> &a, const vector<T> &b) {
    int n = a.size();
    assert((n & (n - 1)) == 0);
    assert(a.size() == b.size());
    static vector<vector<T>> buf1;
    static vector<vector<T>> buf2;
    static vector<T> buf3;

    buf1.assign(__lg(n) + 1, vector<T>(n, 0));
    buf2.assign(__lg(n) + 1, vector<T>(n, 0));

    for (int i = 0; i < n; ++i) {
        int cnt = __builtin_popcount(i);
        buf1[cnt][i] = a[i];
        buf2[cnt][i] = b[i];
    }

    auto submask_sum = [&](vector<T> &v) {
        for (int b = 0; (1 << b) < n; ++b)
            for (int i = 0; i < n; ++i)
                if (((i >> b) & 1) == 0)
                    v[i ^ (1 << b)] += v[i];
    };

    for (int i = 0; i < buf1.size(); ++i) {
        submask_sum(buf1[i]);
        submask_sum(buf2[i]);
    }

    vector<T> res(n, 0);

    for (int i = 0; i < buf1.size(); ++i) {
        buf3.assign(n, 0);

        for (int j = 0; j <= i; ++j)
            for (int k = 0; k < n; ++k)
                buf3[k] += buf1[j][k] * buf2[i - j][k];

        for (int b = 0; (1 << b) < n; ++b)
            for (int i = 0; i < n; ++i)
                if (((i >> b) & 1) == 0)
                    buf3[i ^ (1 << b)] -= buf3[i];

        for (int j = 0; j < n; ++j)
            if (__builtin_popcount(j) == i)
                res[j] = buf3[j];
    }

    return res;
}
