namespace fft {
// TODO: square

using dbl = double;  // works for max value (max(a)*max(b)*n) up to 1e14 (multiply_mod with n up to 1e5)
// using dbl = long double;  // works for max value (max(a)*max(b)*n) up to 1e17
const dbl PI = acosl(-1.0l);

struct Complex {
    dbl x, y;
    Complex(dbl x = 0, dbl y = 0) : x(x), y(y) {}
    Complex conj() const {
        return Complex(x, -y);
    }
};

Complex  operator *  (const Complex &a, const Complex &b) { return Complex(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
void     operator /= (      Complex &a, int n)            { a.x /= n; a.y /= n; }
Complex  operator /  (const Complex &a, int n)            { return Complex(a.x / n, a.y / n); }
Complex  operator +  (const Complex &a, const Complex &b) { return Complex(a.x + b.x, a.y + b.y); }
Complex  operator -  (const Complex &a, const Complex &b) { return Complex(a.x - b.x, a.y - b.y); }

string   to_string   (            const Complex &x) { return (string)"(" + std::to_string(x.x) + ", " + std::to_string(x.y) + ")"; };
ostream& operator << (ostream &o, const Complex &x) { return o << to_string(x); }

vector<Complex> buf1;
vector<Complex> buf2;

vector<Complex> w = {1, 1};
vector<int> reversed = {0};

void update_n(int n) {
    assert((n & (n - 1)) == 0);
    int cur = reversed.size();
    if (n <= cur) return;
    reversed.resize(n);
    w.resize(n + 1);
    while (cur < n) {
        for (int i = 0; i < cur; ++i)
            reversed[i] <<= 1;
        for (int i = cur; i < (cur << 1); ++i)
            reversed[i] = reversed[i - cur] ^ 1;
        for (int i = (cur << 1) - 2; i > 0; i -= 2)
            w[i] = w[i / 2];
        for (int i = 1; i < (cur << 1); i += 2)
            w[i] = Complex(cos(PI * i / cur), sin(PI * i / cur));
        cur *= 2;
    }
    w.back() = 1;
}

void fft_internal(vector<Complex> &v, int from, int n, bool inv) {
    update_n(n);
    int N = reversed.size();

    int d = __lg(N) - __lg(n);

    for (int i = 1; i < n; ++i)
        if (i < (reversed[i] >> d))
            swap(v[from + i], v[from + (reversed[i] >> d)]);

    for (int ln = 1; ln < n; ln <<= 1) {
        int step = (inv ? -N : N) / (ln * 2);
        for (int i = 0; i < n; i += (ln << 1)) {
            int ind = (inv ? N : 0);
            for (int j = 0; j < ln; ++j) {
                Complex y = v[from + i + j + ln] * w[ind];
                ind += step;
                v[from + i + j + ln] = v[from + i + j] - y;
                v[from + i + j]      = v[from + i + j] + y;
            }
        }
    }

    if (inv)
        for (int i = 0; i < n; ++i)
            v[from + i] /= n;
}

vector<Complex> fft(const vector<int> &v, int n = -1) {
    if (n == -1) {
        n = 1;
        while (n < v.size()) n <<= 1;
    }
    assert(v.size() <= n);
    buf1.assign(n, {0, 0});
    for (int i = 0; i < v.size(); ++i)
        buf1[i].x = v[i];
    fft_internal(buf1, 0, n, false);
    return vector<Complex>(buf1.begin(), buf1.end());
}

vector<long long> fft(const vector<Complex> &v) {
    assert(!v.empty() && (v.size() & (v.size() - 1)) == 0);
    buf1.resize(v.size());
    for (int i = 0; i < v.size(); ++i)
        buf1[i] = v[i];
    fft_internal(buf1, 0, buf1.size(), true);
    vector<long long> result(v.size());
    for (int i = 0; i < result.size(); ++i)
        result[i] = llround(buf1[i].x);
    return result;
}

vector<long long> multiply(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    int n = 2;
    while (n < a.size() + b.size() - 1) n <<= 1;

    buf1.assign(n, {0, 0});

    for (int i = 0; i < a.size(); ++i)
        buf1[i].x = a[i];
    for (int i = 0; i < b.size(); ++i)
        buf1[i].y = b[i];

    fft_internal(buf1, 0, n, false);

    for (int i = 0; i <= (n >> 1); ++i) {
        // a --fft--> a1 + a2*i
        // b --fft--> b1 + b2*i
        // fact: FFT(a)[k] = FFT(a)[n - k].conj()
        // using this we can get formulas for FFT(a) and FFT(b) from FFT(a+bi)

        int j = (n - i) & (n - 1);

        auto v = (buf1[i] + buf1[j].conj()) * (buf1[i] - buf1[j].conj()) / 4;
        swap(v.x, v.y);

        buf1[i] = v.conj();
        buf1[j] = v;
    }

    fft_internal(buf1, 0, n, true);

    vector<long long> result(a.size() + b.size() - 1);
    for (int i = 0; i < result.size(); ++i)
        result[i] = llround(buf1[i].x);
    return result;
}

vector<int> multiply_mod(const vector<int> &a, const vector<int> &b, int mod) {
    if (a.empty() || b.empty()) return {};
    int n = 2;
    while (n < a.size() + b.size() - 1) n <<= 1;

    buf1.assign(n * 2, {0, 0});
    for (int i = 0; i < a.size(); ++i) {
        buf1[i].x = a[i] & ((1 << 15) - 1);
        buf1[i].y = a[i] >> 15;
    }
    buf2.assign(n * 2, {0, 0});
    for (int i = 0; i < b.size(); ++i) {
        buf2[i].x = b[i] & ((1 << 15) - 1);
        buf2[i].y = b[i] >> 15;
    }

    fft_internal(buf1, 0, n, false);
    fft_internal(buf2, 0, n, false);

    for (int i = 0; i <= (n >> 1); ++i) {
        int j = (n - i) & (n - 1);

        Complex as = (buf1[i] + buf1[j].conj()) / 2;
        Complex bs = (buf2[i] + buf2[j].conj()) / 2;
        Complex al = (buf1[i] - buf1[j].conj()) / 2;
        Complex bl = (buf2[i] - buf2[j].conj()) / 2;

        Complex asbs = as * bs;
        Complex albs = al * bs;
        Complex asbl = as * bl; swap(asbl.x, asbl.y);
        Complex albl = al * bl; swap(albl.x, albl.y);

        buf1[i] = asbs + albl.conj();
        buf1[j] = asbs.conj() - albl;

        buf2[i] = asbl.conj() + albs;
        buf2[j] = asbl - albs.conj();
    }

    fft_internal(buf1, 0, n, true);
    fft_internal(buf2, 0, n, true);

    vector<int> result(a.size() + b.size() - 1);
    for (int i = 0; i < result.size(); ++i) {
        long long asbs = llround(buf1[i].x);
        long long albl = llround(buf1[i].y);
        long long asbl = llround(buf2[i].x);
        long long albs = llround(buf2[i].y);
        result[i] = (((albl % mod) << 30) + (((asbl + albs) % mod) << 15) + asbs) % mod;
    }
    return result;
}

} // fft
