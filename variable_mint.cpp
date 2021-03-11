struct VarModular {
    using value_type = int;
    static value_type mod;
    value_type value;

    VarModular(ll k = 0) : value(norm(k)) {}

    VarModular& operator += (const VarModular& m)       { value += m.value; if (value >= mod) value -= mod; return *this; }
    VarModular  operator +  (const VarModular& m) const { VarModular r = *this; return r += m; }

    VarModular& operator -= (const VarModular& m)       { value -= m.value; if (value < 0)    value += mod; return *this; }
    VarModular  operator -  (const VarModular& m) const { VarModular r = *this; return r -= m; }
    VarModular  operator -                     () const { return VarModular(-value); }

    VarModular& operator *= (const VarModular& m)       { value = value * 1ll * m.value % mod; return *this; }
    VarModular  operator *  (const VarModular& m) const { VarModular r = *this; return r *= m; }

    VarModular& operator /= (const VarModular& m)       { return *this *= m.inv(); }
    VarModular  operator /  (const VarModular& m) const { VarModular r = *this; return r /= m; }

    VarModular& operator ++                    ()       { return *this += 1; }
    VarModular& operator --                    ()       { return *this -= 1; }
    VarModular  operator ++                 (int)       { VarModular r = *this; *this += 1; return r; }
    VarModular  operator --                 (int)       { VarModular r = *this; *this -= 1; return r; }

    bool        operator == (const VarModular& m) const { return value == m.value; }
    bool        operator != (const VarModular& m) const { return value != m.value; }

    explicit    operator       int() const { return value; }
    explicit    operator      bool() const { return value; }
    explicit    operator long long() const { return value; }

    value_type norm(ll k) {
        if (!(-mod <= k && k < mod)) k %= mod;
        if (k < 0) k += mod;
        return k;
    }

    VarModular inv() const {
        value_type a = value, b = mod, x = 0, y = 1;
        while (a != 0) { value_type k = b / a; b -= k * a; x -= k * y; swap(a, b); swap(x, y); }
        return VarModular(x);
    }
};
VarModular pow(VarModular m, ll p) {
    VarModular r(1);
    while (p) {
        if (p & 1) r *= m;
        m *= m;
        p >>= 1;
    }
    return r;
}
VarModular::value_type VarModular::mod = 1'000'000'007;
// use "VarModular::mod = [your value]" later

ostream& operator << (ostream& o, const VarModular &m) { return o << m.value; }
istream& operator >> (istream& i,       VarModular &m) { ll k; i >> k; m.value = m.norm(k); return i; }
string   to_string(const VarModular& m) { return to_string(m.value); }

using Mint = VarModular;
// using Mint = long double;

vector<Mint> f, fi;
void init_C(int n) {
    f.assign(n, 1); fi.assign(n, 1);
    for (int i = 2; i < n; ++i) f[i] = f[i - 1] * i;
    fi.back() = Mint(1) / f.back();
    for (int i = n - 2; i >= 0; --i) fi[i] = fi[i + 1] * (i + 1);
}
Mint C(int n, int k) {
    if (k < 0 || k > n) return 0;
    else return f[n] * fi[k] * fi[n - k];
}
