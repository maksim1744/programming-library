// usage: auto c = fwht::fwht(a, b, fwht::AND);
// just transformation: fwht::fwht(a, inv, fwht::AND);
namespace fwht {

enum Type {
    XOR,
    NXOR,
    AND,
    OR,
};

template<typename T>
void fwht(vector<T> &v, bool inv = false, Type tp = XOR) {
    assert((v.size() & (v.size() - 1)) == 0);
    for (int step = 1; step < v.size(); step <<= 1) {
        for (int i = 0; i < v.size(); i += step * 2) {
            for (int j = 0; j < step; ++j) {
                T a = v[i + j], b = v[i + j + step];
                if (tp == Type::XOR) {
                    v[i + j] = a + b;
                    v[i + j + step] = a - b;
                } else if (tp == Type::NXOR) {
                    v[i + j] = -a + b;
                    v[i + j + step] = a + b;
                } else if (tp == Type::AND) {
                    v[i + j] = (inv ? b - a : b);
                    v[i + j + step] = (inv ? a : a + b);
                } else if (tp == Type::OR) {
                    v[i + j] = (inv ? b : a + b);
                    v[i + j + step] = (inv ? a - b : a);
                }
            }
        }
    }
    if (inv && (tp == Type::XOR || tp == Type::NXOR)) {
        T val = 1;
        for (int j = 1; j < v.size(); j <<= 1)
            val *= 2;
        for (int i = 0; i < v.size(); ++i) {
            v[i] /= val;
        }
    }
}

template<typename T>
vector<T> fwht(vector<T> a, vector<T> b, Type tp = XOR) {
    fwht(a, false, tp);
    fwht(b, false, tp);
    for (int i = 0; i < a.size(); ++i)
        a[i] *= b[i];
    fwht(a, true, tp);
    return a;
}

};
