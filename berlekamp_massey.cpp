// n = size of recurrence = size of base (initial terms)
// m = size of sequence given to find_recurrence()

// find_recurrence() works in O(mn)
// nth(ind) works O(n^2 log(ind))

// it is possible to use nth() with precalculated recurrence,
// use set_recurrence and set_base for that

// rec should satisfy a[i] = sum_{j=0..rec.size()-1} a[i-j-1] * rec[j]

template<typename T>
struct BerlekampMassey {
    vector<T> rec;
    vector<T> base;

    BerlekampMassey() {}

    // this can be used with initializer_list
    void set_recurrence(const vector<T> &v) {
        rec = v;
    }
    template<typename U>
    void set_recurrence(const vector<U> &v) {
        rec.assign(v.begin(), v.end());
    }

    void set_base(const vector<T> &v) {
        base = v;
    }
    template<typename U>
    void set_base(const vector<U> &v) {
        base.assign(v.begin(), v.end());
    }

    void find_recurrence(const vector<T> &v) {
        rec.clear();
        int last_change = -1;
        vector<T> last_s = {};
        T last_delta;
        for (int i = 0; i < v.size(); ++i) {
            T val = 0;
            for (int j = 0; j < rec.size(); ++j)
                val += rec[j] * v[i - 1 - j];
            if (val == v[i]) continue;

            if (last_change == -1) {
                last_change = i;
                rec.assign(i + 1, 0);
                last_delta = v[i];
                continue;
            }

            auto tmp = ((int)last_s.size() - last_change > (int)rec.size() - i ? rec : last_s);

            last_s.push_back(-1);
            int rec_size = rec.size();
            rec.resize(max(rec.size(), last_s.size() + i - last_change - 1));

            T mult = (val - v[i]) / last_delta;
            for (auto &k : last_s)
                k *= mult;

            for (int j = 0; j < last_s.size(); ++j)
                rec[j + i - last_change - 1] += last_s[last_s.size() - j - 1];

            if ((int)last_s.size() - last_change - 1 > rec_size - i) {
                reverse(tmp.begin(), tmp.end());
                last_s = tmp;
                last_change = i;
                last_delta = v[i] - val;
            } else {
                last_s = tmp;
            }
        }
        base = vector<T>(v.begin(), v.begin() + rec.size());
    }
    template<typename U>
    void find_recurrence(const vector<U> &v) {
        find_recurrence(vector<T>(v.begin(), v.end()));
    }

    T nth(long long ind) const {
        if (ind < base.size())
            return base[ind];

        assert(base.size() == rec.size());

        vector<T> cur_poly = {0, 1};
        vector<T> res = {1};
        while (ind) {
            if (ind & 1) {
                res = poly_mul(res, cur_poly);
                poly_mod(res, rec);
            }
            cur_poly = poly_mul(cur_poly, cur_poly);
            poly_mod(cur_poly, rec);
            ind >>= 1;
        }

        T answer = 0;
        for (int i = 0; i < res.size(); ++i)
            answer += base[i] * res[i];
        return answer;
    }

  private:
    vector<T> poly_mul(const vector<T> &a, const vector<T> &b) const {
        vector<T> res(a.size() + b.size() - 1);
        for (int i = 0; i < a.size(); ++i)
            for (int j = 0; j < b.size(); ++j)
                res[i + j] += a[i] * b[j];
        return res;
    }

    vector<T> poly_mod(vector<T> &a, const vector<T> &f) const {
        while (a.size() > f.size()) {
            T k = a.back();
            a.pop_back();
            if (k != 0) {
                for (int i = 0; i < f.size(); ++i)
                    a[a.size() - i - 1] += f[i] * k;
            }
        }
        return a;
    }
};
