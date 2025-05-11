template<typename T>
struct LiChao {
    static const T INF = std::numeric_limits<T>::max() / 2 - 5;
    int n;
    vector<pair<T, T>> tree;

    LiChao(int _n) : n(max(_n, 1)) {
        int nn = (1 << __lg(n)) * 4;
        tree.assign(nn, {0, -INF});
    }
    
    void add(T k, T b) {
        add_inner({k, b}, 0, 0, n - 1);
    }

    void add_inner(pair<T, T> line, int i, int l, int r) {
        while (l <= r) {
            T curl = tree[i].first * l + tree[i].second;
            T curr = tree[i].first * r + tree[i].second;
            T mel = line.first * l + line.second;
            T mer = line.first * r + line.second;
            if (curl >= mel && curr >= mer) {
                break;
            }
            if (curl <= mel && curr <= mer) {
                tree[i] = line;
                break;
            }
            assert(line.first != tree[i].first);
            int m = (l + r) / 2;
            T cur = tree[i].first * m + tree[i].second;
            T me = line.first * m + line.second;
            if (me > cur) {
                swap(line, tree[i]);
            }
            if ((me <= cur && mer > curr) || (me > cur && mer < curr)) {
                l = m + 1;
                i = i * 2 + 2;
            } else {
                r = m;
                i = i * 2 + 1;
            }
        }
    }

    void add_segment(T k, T b, int l, int r) {
        l = max(l, 0);
        r = min(r, n - 1);
        add_segment_inner({k, b}, l, r, 0, 0, n - 1);
    }

    void add_segment_inner(pair<T, T> line, int l, int r, int i, int vl, int vr) {
        if (l > vr || r < vl) {
            return;
        }
        if (l <= vl && vr <= r) {
            add_inner(line, i, vl, vr);
            return;
        }
        int m = (vl + vr) / 2;
        add_segment_inner(line, l, r, i * 2 + 1, vl, m);
        add_segment_inner(line, l, r, i * 2 + 2, m + 1, vr);
    }

    T ask(int x) {
        T ans = -INF;
        int i = 0;
        int l = 0;
        int r = n - 1;
        while (true) {
            int m = (l + r) / 2;
            ans = max(ans, tree[i].first * x + tree[i].second);
            if (l == r) break;
            if (x <= m) {
                r = m;
                i = i * 2 + 1;
            } else {
                l = m + 1;
                i = i * 2 + 2;
            }
        }
        return ans;
    }
};
