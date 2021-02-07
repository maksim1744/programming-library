namespace fwht {

// H is initial matrix, G is inverse, div if 1/det(H)^2
// string c1c2c3c4 in comment means that f(0, 0) = c1, f(0, 1) = c2, f(1, 0) = c3, f(1, 1) = c4
// max value sum(a) * sum(b) * div^log_2(len(a)), use int128 if necessary

struct fwhd_info {
    array<array<int, 2>, 2> H;
    array<array<int, 2>, 2> G;
    const int div;
};

// this amount of brackets is not a joke: https://stackoverflow.com/a/12844625
const fwhd_info F0110 = {{{{{ 1,  1}}, {{ 1, -1}}}}, {{{{ 1,  1}}, {{ 1, -1}}}}, 2};  // 0110, xor
const fwhd_info F1001 = {{{{{-1,  1}}, {{ 1,  1}}}}, {{{{-1,  1}}, {{ 1,  1}}}}, 2};  // 1001, !xor
const fwhd_info F0001 = {{{{{ 0,  1}}, {{ 1,  1}}}}, {{{{-1,  1}}, {{ 1,  0}}}}, 1};  // 0001, and
const fwhd_info F0111 = {{{{{ 1,  1}}, {{ 1,  0}}}}, {{{{ 0,  1}}, {{ 1, -1}}}}, 1};  // 0111, or

const auto [H, G, div] = F0110;

template<typename T>
void fwht(vector<T> &v, bool inv = false, const array<array<int, 2>, 2> &H = H, const array<array<int, 2>, 2> &G = G, const int div = div) {
    assert((v.size() & (v.size() - 1)) == 0);
    for (int step = 1; step < v.size(); step <<= 1) {
        for (int i = 0; i < v.size(); i += step * 2) {
            for (int j = 0; j < step; ++j) {
                T a = v[i + j], b = v[i + j + step];
                if (inv) {
                    v[i + j]        = a * G[0][0] + b * G[0][1];
                    v[i + j + step] = a * G[1][0] + b * G[1][1];
                } else {
                    v[i + j]        = a * H[0][0] + b * H[0][1];
                    v[i + j + step] = a * H[1][0] + b * H[1][1];
                }
            }
        }
    }
    if (inv && div != 1) {
        T val = 1;
        for (int j = 1; j < v.size(); j <<= 1)
            val *= div;
        for (int i = 0; i < v.size(); ++i) {
            v[i] /= val;
        }
    }
}

template<typename T>
vector<T> fwht(vector<T> a, vector<T> b, const array<array<int, 2>, 2> &H = H, const array<array<int, 2>, 2> &G = G, const int div = div) {
    fwht(a, false, H, G, div);
    fwht(b, false, H, G, div);
    for (int i = 0; i < a.size(); ++i)
        a[i] *= b[i];
    fwht(a, true, H, G, div);
    return a;
}

};
