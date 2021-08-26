namespace var_mint_ns {
struct VarModular {
    using value_type = int;
  private:
    static value_type P;
  public:
    value_type value;

    VarModular(long long k = 0) : value(norm(k)) {}

    friend VarModular& operator += (      VarModular& n, const VarModular& m) { n.value += m.value; if (n.value >= P) n.value -= P; return n; }
    friend VarModular  operator +  (const VarModular& n, const VarModular& m) { VarModular r = n; return r += m; }

    friend VarModular& operator -= (      VarModular& n, const VarModular& m) { n.value -= m.value; if (n.value < 0)    n.value += P; return n; }
    friend VarModular  operator -  (const VarModular& n, const VarModular& m) { VarModular r = n; return r -= m; }
    friend VarModular  operator -  (const VarModular& n)                      { return VarModular(-n.value); }

    friend VarModular& operator *= (      VarModular& n, const VarModular& m) { n.value = reduce(n.value * 1ll * m.value); return n; }
    friend VarModular  operator *  (const VarModular& n, const VarModular& m) { VarModular r = n; return r *= m; }

    friend VarModular& operator /= (      VarModular& n, const VarModular& m) { return n *= m.inv(); }
    friend VarModular  operator /  (const VarModular& n, const VarModular& m) { VarModular r = n; return r /= m; }

    VarModular& operator ++ (   ) { return *this += 1; }
    VarModular& operator -- (   ) { return *this -= 1; }
    VarModular  operator ++ (int) { VarModular r = *this; *this += 1; return r; }
    VarModular  operator -- (int) { VarModular r = *this; *this -= 1; return r; }

    friend bool operator == (const VarModular& n, const VarModular& m) { return n.value == m.value; }
    friend bool operator != (const VarModular& n, const VarModular& m) { return n.value != m.value; }

    explicit    operator       int() const { return value; }
    explicit    operator      bool() const { return value; }
    explicit    operator long long() const { return value; }

    static value_type           mod()      { return     P; }

    value_type norm(long long k) {
        if (!(-P <= k && k < P)) k %= P;
        if (k < 0) k += P;
        return k;
    }

    VarModular inv() const {
        value_type a = value, b = P, x = 0, y = 1;
        while (a != 0) { value_type k = b / a; b -= k * a; x -= k * y; swap(a, b); swap(x, y); }
        return VarModular(x);
    }

  private:
    static uint64_t m;
  public:
    static void set_mod(value_type mod) {
        m = (__uint128_t(1) << 64) / mod;
        P = mod;
    }

    static value_type reduce(uint64_t a) {
        uint64_t q = ((__uint128_t(m) * a) >> 64);
        a -= q * P;
        if (a >= P)
            a -= P;
        return a;
    }
};
uint64_t VarModular::m = 0;
VarModular pow(VarModular m, long long p) {
    VarModular r(1);
    while (p) {
        if (p & 1) r *= m;
        m *= m;
        p >>= 1;
    }
    return r;
}
VarModular::value_type VarModular::P;
// use "VarModular::set_mod([your value])" later

ostream& operator << (ostream& o, const VarModular& m) { return o << m.value; }
istream& operator >> (istream& i,       VarModular& m) { long long k; i >> k; m.value = m.norm(k); return i; }
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
}
using namespace var_mint_ns;
