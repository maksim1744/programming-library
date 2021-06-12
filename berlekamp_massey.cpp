// n = size of recurrence = size of base (initial terms)
// m = size of sequence given to find_recurrence()

// find_recurrence() works in O(mn)
// nth(ind) works in O(n^2*log(ind)) or in O(n*logn*log(ind)) if ntt is enabled

// it is possible to use nth() with precalculated recurrence,
// use set_recurrence and set_base for that

// rec should satisfy a[i] = sum_{j=0..rec.size()-1} a[i-j-1] * rec[j]

// #define BERLEKAMP_USE_NTT

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

    #ifdef BERLEKAMP_USE_NTT
    T nth_fast(long long ind) const {
        if (ind < base.size())
            return base[ind];

        // remove this assert is your mod is different but works fine with ntt
        static_assert(T::mod() == 998244353, "make sure your mod is correct");

        assert(base.size() == rec.size());

        vector<T> cur_poly = {0, 1};
        vector<T> res = {1};

        vector<Mint> rec_inv, rec_with_1;
        int fsize = rec.size() + 1;
        int N = 1;
        {
            while (N < (rec.size() + 1) * 4)
                N *= 2;

            rec_with_1 = rec;
            rec_with_1.insert(rec_with_1.begin(), -1);
            for (auto &k : rec_with_1)
                k = -k;

            rec_inv = rec_with_1;
            reverse(rec_with_1.begin(), rec_with_1.end());
            rec_inv = poly_inv(rec_inv, rec_inv.size());

            rec_inv = ntt::ntt(rec_inv, N);
            rec_with_1 = ntt::ntt(rec_with_1, N / 2);
        }

        while (ind) {
            int cur_poly_sz = cur_poly.size();
            cur_poly = ntt::ntt(cur_poly, N / 2);
            if (ind & 1) {
                int res_sz = res.size();
                res = ntt::ntt(res, N / 2);
                for (int i = 0; i < N / 2; ++i)
                    res[i] *= cur_poly[i];
                res = ntt::ntti(res);
                res.resize(cur_poly_sz + res_sz - 1);
                poly_mod_fast(res, rec_with_1, rec_inv, fsize, N);
            }
            for (int i = 0; i < N / 2; ++i)
                cur_poly[i] *= cur_poly[i];
            cur_poly = ntt::ntti(cur_poly);
            cur_poly.resize(cur_poly_sz * 2 - 1);
            poly_mod_fast(cur_poly, rec_with_1, rec_inv, fsize, N);
            ind >>= 1;
        }

        T answer = 0;
        for (int i = 0; i < res.size(); ++i)
            answer += base[i] * res[i];
        return answer;
    }
    #endif

    T nth_slow(long long ind) const {
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

    T nth(long long ind) const {
        #ifdef BERLEKAMP_USE_NTT
        if (rec.size() > 150)
            return nth_fast(ind);
        #endif

        return nth_slow(ind);
    }

  private:
    vector<T> poly_mul(const vector<T> &a, const vector<T> &b) const {
        vector<T> res(a.size() + b.size() - 1);
        for (int i = 0; i < a.size(); ++i)
            for (int j = 0; j < b.size(); ++j)
                res[i + j] += a[i] * b[j];
        return res;
    }

    #ifdef BERLEKAMP_USE_NTT
    void poly_mod_fast(vector<T> &a, const vector<T> &f, const vector<T> &fi, int fsize, int N) const {
        if (a.size() < fsize) return;

        reverse(a.begin(), a.end());
        auto d = a;
        d = ntt::ntt(d, N);
        for (int i = 0; i < N; ++i)
            d[i] *= fi[i];
        d = ntt::ntti(d);

        if (d.size() > a.size() - fsize + 1)
            d.resize(a.size() - fsize + 1);

        reverse(d.begin(), d.end());
        reverse(a.begin(), a.end());

        d = ntt::ntt(d, N / 2);
        for (int i = 0; i < N / 2; ++i)
            d[i] *= f[i];
        d = ntt::ntti(d);

        for (int i = 0; i < min(a.size(), d.size()); ++i)
            a[i] -= d[i];

        a.resize(fsize - 1);
    }

    vector<T> poly_inv(const vector<T> &v, int k = -1) const {
        if (k == -1) k = v.size();
        assert(v[0] != 0);
        vector<T> res = {T(1) / v[0]};
        int cur_size = 1;
        vector<T> v_pref;
        while (res.size() < k) {
            cur_size *= 2;
            while (v_pref.size() < v.size() && v_pref.size() < cur_size)
                v_pref.pb(v[v_pref.size()]);
            auto tmp = ntt::multiply(v_pref, res);
            tmp[0] -= 2;
            if (tmp.size() > cur_size)
                tmp.resize(cur_size);
            for (auto &k : tmp)
                k = -k;
            res = ntt::multiply(res, tmp);
            if (res.size() > cur_size)
                res.resize(cur_size);
        }
        return res;
    }
    #endif

    void poly_mod(vector<T> &a, const vector<T> &f) const {
        while (a.size() > f.size()) {
            T k = a.back();
            a.pop_back();
            if (k != 0) {
                for (int i = 0; i < f.size(); ++i)
                    a[a.size() - i - 1] += f[i] * k;
            }
        }
    }
};
