namespace grid_iterator_ns {
const array<pair<int, int>, 8> dirs = {{{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}}};
struct GridIterator {
    int n, m;
    GridIterator(int n, int m) : n(n), m(m) {}

    template<size_t N>
    struct NeighbourIteratorContainer {
        int i, j, n, m;
        NeighbourIteratorContainer(int i, int j, int n, int m) : i(i), j(j), n(n), m(m) {}

        struct NeighbourIterator {
            int cur;
            int i, j, n, m;
            NeighbourIterator(int cur, int i, int j, int n, int m) : cur(cur), i(i), j(j), n(n), m(m) {
                skip_to_first_allowed();
            }

            void skip_to_first_allowed() {
                while (cur < N &&
                    (i + dirs[cur].first  < 0 || i + dirs[cur].first  >= n ||
                     j + dirs[cur].second < 0 || j + dirs[cur].second >= m)) {
                    ++cur;
                }
            }

            NeighbourIterator& operator ++ () {
                ++cur;
                skip_to_first_allowed();
                return *this;
            }

            pair<int, int> operator * () const { return {i + dirs[cur].first, j + dirs[cur].second}; }

            bool operator == (const NeighbourIterator& other) const { return cur == other.cur; }
        };

        auto begin() const { return NeighbourIterator(0, i, j, n, m); }
        auto end()   const { return NeighbourIterator(N, i, j, n, m); }

        auto collect() const {
            vector<pair<int, int>> result;
            for (auto it = begin(); it != end(); ++it) result.push_back(*it);
            return result;
        }
    };

    template<size_t N>
    auto iterate(int i, int j) const {
        static_assert(N == 4 || N == 8, "you can remove this, but make sure you understand what you are doing");
        return NeighbourIteratorContainer<N>(i, j, n, m);
    }
};
}
using grid_iterator_ns::GridIterator;
