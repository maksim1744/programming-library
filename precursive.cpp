template<typename T>
struct PRecursive {
    // n = size of recurrence = size of base (initial terms)
    // m = size of sequence given to find_recurrence()
    // k = max_degtree given to find_recurrence()

    // find_recurrence() works in O(m*k^2*n^3)
    // nth(ind) works in O(ind*k*n)

    // it is possible to use nth() with precalculated recurrence,
    // use set_recurrence and set_base for that

    // rec should satisfy a[i] =  (sum_{j=0..rec.size()-1} a[i-j-1] * sum_{k=0..rec[j+1].size()-1} rec[j+1][k]*i^k)
    //                          / (sum_{k=0..rec[0].size()-1} rec[0][k]*i^k)

    // pass validate = x to use last x elements of a sequence for validation
    // use print() to print recurrence relation in a readable form

    vector<vector<T>> rec;
    vector<T> base;
    vector<T> cache;

    PRecursive() {}

    // this can be used with initializer_list
    void set_recurrence(const vector<vector<T>> &v) {
        rec = v;
    }
    template<typename U>
    void set_recurrence(const vector<vector<U>> &v) {
        rec.clear();
        for (const auto& l : v)
            rec.emplace_back(l.begin(), l.end());
    }

    void set_base(const vector<T> &v) {
        base = v;
    }
    template<typename U>
    void set_base(const vector<U> &v) {
        base.assign(v.begin(), v.end());
    }

    void find_recurrence(const vector<T>& v, int max_degree, int validate = 0) {
        // just gaussian elimination
        for (int sz = 0;; ++sz) {
            vector<vector<Mint>> A;
            for (int i = sz; i + validate < v.size(); ++i) {
                vector<Mint> row((sz + 1) * (max_degree + 1) + 1);
                row.back() = 0;
                for (int j = 0; j <= sz; ++j) {
                    T pw = 1;
                    for (int k = 0; k <= max_degree; ++k) {
                        row[k * (sz + 1) + j] = v[i - j] * pw * (j == 0 ? -1 : 1);
                        pw *= i;
                    }
                }
                row.erase(row.begin()); // some element in denominator has to be 1, otherwise all 0-s is a solution
                row.back() = v[i];
                A.push_back(std::move(row));
            }

            assert(!A.empty());
            int last = 0;
            for (int col = 0; last < A.size() && col + 1 < A[0].size(); ++col) {
                if (A[last][col] == 0) {
                    for (int j = last + 1; j < A.size(); ++j) {
                        if (A[j][col] != 0) {
                            swap(A[j], A[last]);
                            break;
                        }
                    }
                }
                if (A[last][col] == 0) continue;
                for (int j = last + 1; j < A.size(); ++j) {
                    if (A[j][col] == 0) continue;
                    T coef = A[j][col] / A[last][col];
                    for (int k = col; k < A[0].size(); ++k) {
                        A[j][k] -= A[last][k] * coef;
                    }
                }
                ++last;
            }

            bool has_solution = true;
            for (int i = 0; i < A.size(); ++i) {
                if (A[i].back() != 0) has_solution = false;
                for (int j = 0; j + 1 < A[i].size(); ++j)
                    if (A[i][j] != 0)
                        has_solution = true;
                if (!has_solution) break;
            }
            if (!has_solution) continue;
            vector<T> solution(A[0].size() - 1);
            for (int i = (int)A.size() - 1; i >= 0; --i) {
                int j = 0;
                while (j < A[i].size() && A[i][j] == 0) ++j;
                if (j == A[i].size()) continue;
                T res = A[i].back();
                for (int k = j + 1; k + 1 < A[i].size(); ++k)
                    res -= A[i][k] * solution[k];
                solution[j] = res / A[i][j];
            }
            solution.insert(solution.begin(), 1);
            rec.resize(sz + 1);
            for (int i = 0; i <= sz; ++i) {
                rec[i].resize(max_degree + 1);
                for (int j = 0; j <= max_degree; ++j) {
                    rec[i][j] = solution[j * (sz + 1) + i];
                }
            }

            // remove largest degrees with zeros
            while (true) {
                bool has_non_zero = false;
                for (const auto& r : rec)
                    if (r.back() != 0)
                        has_non_zero = true;
                if (has_non_zero) break;
                for (auto& r : rec)
                    r.pop_back();
            }

            base.assign(v.begin(), v.begin() + sz);
            cache = base;
            for (int i = (int)v.size() - validate; i < v.size(); ++i) {
                T correct = v[i];
                T calculated = nth(i);
                if (correct != calculated) {
                    cerr << "validation failed at index " << i << ": correct is " << correct << ", calculated is " << calculated << endl;
                    print();
                    assert(correct == calculated);
                }
            }
            break;
        }
    }
    template<typename U>
    void find_recurrence(const vector<U> &v, int max_degree, int validate = 0) {
        find_recurrence(vector<T>(v.begin(), v.end()), max_degree);
    }

    T nth(int ind) {
        while (ind >= cache.size()) {
            T res = 0;
            for (int i = 1; i < rec.size(); ++i) {
                T pw = 1;
                for (int j = 0; j < rec[i].size(); ++j) {
                    res += cache[cache.size() - i] * rec[i][j] * pw;
                    pw *= T(cache.size());
                }
            }
            T den = 0;
            T pw = 1;
            for (int j = 0; j < rec[0].size(); ++j) {
                den += rec[0][j] * pw;
                pw *= T(cache.size());
            }
            cache.push_back(res / den);
        }
        return cache[ind];
    }

    void print() const {
        string recs = "";
        auto get_poly = [&](int i) -> string {
            stringstream poly;
            for (int j = 0; j < rec[i].size(); ++j) {
                if (rec[i][j] != 0) {
                    if (!poly.str().empty()) poly << '+';
                    poly << rec[i][j];
                    if (j) {
                        poly << "n";
                        if (j > 1)
                            poly << '^' << j;
                    }
                }
            }
            return poly.str();
        };
        for (int i = 1; i < rec.size(); ++i) {
            string polys = get_poly(i);
            if (polys.empty()) continue;
            polys = "(" + polys + ")*a[n-" + to_string(i) + "]";
            if (!recs.empty()) recs += " + ";
            recs += polys;
        }
        if (recs.empty()) recs = "0";
        cerr << '(' << get_poly(0) << ")*a[n] = " << recs << '\n';
        cerr << "base: [";
        for (int i = 0; i < base.size(); ++i) {
            if (i) cerr << ", ";
            cerr << base[i];
        }
        cerr << "]" << endl;
    }
};
