// use std::greater to make max_queue
// to use min_stack2, set needed array size in min_stack2 and change
//    [min_stack s1, s2;] to [min_stack2 s1, s2;]
template<typename T, typename Comp = std::less<T>>
struct min_queue {
    struct min_stack {
        vector<pair<T, T>> v;

        void push(const T &x) {
            if (v.empty()) {
                v.eb(x, x);
                return;
            }
            const T &was = v.back().first;
            v.eb((Comp()(x, was) ? x : was), x);
        }

        void pop() {
            v.pop_back();
        }

        T extract() {
            auto res = v.back().second;
            v.pop_back();
            return res;
        }

        T get_min() const {
            return v.back().first;
        }

        T top() const {
            return v.back().second;
        }

        bool empty() const {
            return v.empty();
        }

        int size() const {
            return v.size();
        }

        void clear() {
            v.clear();
        }
    };
    struct min_stack2 {
        array<pair<T, T>, 12345> v;
        int iv = 0;

        void push(const T &x) {
            if (iv == 0) {
                v[iv++] = make_pair(x, x);
                return;
            }
            const T &was = v[iv - 1].first;
            v[iv++] = make_pair((Comp()(x, was) ? x : was), x);
        }

        void pop() {
            --iv;
        }

        T extract() {
            auto res = v[iv - 1].second;
            --iv;
            return res;
        }

        T get_min() const {
            return v[iv - 1].first;
        }

        T top() const {
            return v[iv - 1].second;
        }

        bool empty() const {
            return iv == 0;
        }

        int size() const {
            return iv;
        }

        void clear() {
            iv = 0;
        }
    };

    min_stack s1, s2;
    void push(const T &x) {
        s2.push(x);
    }

    void pop() {
        if (s1.empty()) {
            while (!s2.empty())
                s1.push(s2.extract());
        }
        s1.pop();
    }

    T front() {
        if (s1.empty()) {
            while (!s2.empty())
                s1.push(s2.extract());
        }
        return s1.top();
    }

    T get_min() const {
        if (s1.empty()) return s2.get_min();
        else if (s2.empty()) return s1.get_min();
        T a = s1.get_min();
        T b = s2.get_min();
        return (Comp()(a, b) ? a : b);
    }

    bool empty() const {
        return s1.empty() && s2.empty();
    }

    int size() const {
        return s1.size() + s2.size();
    }

    void clear() {
        s1.clear();
        s2.clear();
    }
};

template<typename T, typename U>
ostream &operator << (ostream &o, const min_queue<T, U> &q) {
    o << "[";
    bool first = true;
    for (int i = (int)q.s1.size() - 1; i >= 0; --i) {
        if (!first)
            o << ", ";
        o << q.s1.v[i].second;
        first = false;
    }
    o << " | ";
    for (int i = 0; i < q.s2.size(); ++i) {
        if (!first)
            o << ", ";
        o << q.s2.v[i].second;
        first = false;
    }
    return o;
}
