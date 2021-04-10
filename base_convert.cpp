// #define BIGINT_USE_FFT
// O(nlog^2)

const int basen = 8;
const int base = pow(10, basen);
vector<vector<uint32_t>> prec10 = {{uint32_t(base)}};
vector<vector<int>> prec2 = {{(1ll << 32) % base, (1ll << 32) / base}};

// with doubles up to a.size() = b.size() = 3e4
#ifdef BIGINT_USE_FFT
vector<uint32_t> _mul_fft_(const vector<uint32_t> &a, const vector<uint32_t> &b) {
    vector<int> ta(a.size() * 2), tb(b.size() * 2);
    for (int i = 0; i < a.size(); ++i) {
        ta[i * 2] = (a[i] & uint16_t(-1));
        ta[i * 2 + 1] = (a[i] >> 16);
    }
    for (int i = 0; i < b.size(); ++i) {
        tb[i * 2] = (b[i] & uint16_t(-1));
        tb[i * 2 + 1] = (b[i] >> 16);
    }
    auto tc = fft::multiply(ta, tb);
    tc.resize(tc.size() + 5, 0);
    for (int i = 0; i + 1 < tc.size(); ++i) {
        tc[i + 1] += (tc[i] >> 16);
        tc[i] &= uint16_t(-1);
    }
    vector<uint32_t> res(tc.size() / 2);
    for (int i = 0; i < res.size(); ++i) {
        res[i] = ((uint32_t(tc[i * 2 + 1]) << 16) ^ uint32_t(tc[i * 2]));
    }
    while (!res.empty() && res.back() == 0)
        res.pop_back();
    return res;
}
#endif

vector<uint32_t> _mul_(const vector<uint32_t> &a, const vector<uint32_t> &b) {
    if (a.empty() || b.empty()) return {};
    #ifdef BIGINT_USE_FFT
    if (min(a.size(), b.size()) > 350)
        return _mul_fft_(a, b);
    #endif
    vector<uint64_t> tmp(a.size() + b.size() + 1, 0);

    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            uint64_t prod = uint64_t(a[i]) * b[j];
            tmp[i + j] += (prod & uint32_t(-1));
            tmp[i + j + 1] += (prod >> 32);
        }
    }

    for (int i = 0; i + 1 < tmp.size(); ++i) {
        tmp[i + 1] += (tmp[i] >> 32);
        tmp[i] &= uint32_t(-1);
    }

    vector<uint32_t> c(tmp.begin(), tmp.end());

    while (!c.empty() && c.back() == 0) c.pop_back();
    return c;
};

// works with doubles with v.size() up to 3e5
vector<uint32_t> convert(const vector<int> &v) {
    auto _add_ = [](vector<uint32_t> &a, const vector<uint32_t> &b) {
        a.resize(max(a.size(), b.size()) + 1, 0);

        uint32_t carry = 0;
        int i;
        for (i = 0; i < b.size(); ++i) {
            a[i] += b[i];
            uint32_t next_carry = (a[i] < b[i]);
            a[i] += carry;
            next_carry |= (carry && a[i] == 0);
            swap(carry, next_carry);
        }

        while (carry) {
            if ((++a[i++]) != 0)
                break;
        }

        while (!a.empty() && a.back() == 0) a.pop_back();
    };

    while ((1 << prec10.size()) < v.size()) {
        prec10.emplace_back(_mul_(prec10.back(), prec10.back()));
    }

    using It = vector<int>::const_iterator;
    function<vector<uint32_t>(It, It, int)> conv = [&](It it1, It it2, int hint) {
        if (it1 + 1 == it2) {
            return vector{uint32_t(*it1)};
        }

        int ln = it2 - it1;
        while ((1 << hint) >= ln) --hint;
        assert(1 << (hint + 1) >= ln);

        auto imid = it1 + (1 << hint);

        auto vl = conv(it1, imid, hint - 1);
        auto vr = conv(imid, it2, hint - 1);

        vr = _mul_(vr, prec10[hint]);
        _add_(vr, vl);

        return vr;
    };

    return conv(v.begin(), v.end(), prec10.size() - 1);
}

// works with doubles up to 3e6 or more
vector<int> convert(const vector<uint32_t> &v) {
    const int basen = 8;
    const int base = pow(10, basen);
    auto _add_ = [&](vector<int> &a, const vector<int> &b) {
        a.resize(max(a.size(), b.size()) + 1, 0);
        for (int i = 0; i < b.size(); ++i)
            a[i] += b[i];
        for (int i = 0; i + 1 < b.size() || a[i] >= base; ++i) {
            if (a[i] >= base) {
                a[i] -= base;
                ++a[i + 1];
            }
        }
        while (!a.empty() && a.back() == 0)
            a.pop_back();
    };

    auto _slow_mult_ = [&](const vector<int> &a, const vector<int> &b) {
        vector<long long> tmp(a.size() + b.size() + 1, 0);
        for (int i = 0; i < a.size(); ++i) {
            for (int j = 0; j < b.size(); ++j) {
                long long prod = 1ll * a[i] * b[j];
                long long div = prod / base;
                tmp[i + j] += prod - base * div;
                tmp[i + j + 1] += div;
            }
        }
        for (int i = 0; i + 1 < tmp.size(); ++i) {
            long long div = tmp[i] / base;
            tmp[i + 1] += div;
            tmp[i] -= div * base;
        }
        while (!tmp.empty() && tmp.back() == 0)
            tmp.pop_back();
        return vector<int>(tmp.begin(), tmp.end());
    };

    #ifdef BIGINT_USE_FFT
    auto _fft_mult_ = [&](const vector<int> &a, const vector<int> &b) {
        vector<int> ta(a.size() * 2), tb(b.size() * 2);
        static_assert(basen % 2 == 0, "basen has to be even");
        const static int M = pow(10, basen / 2);
        for (int i = 0; i < a.size(); ++i) {
            ta[i * 2] = a[i] % M;
            ta[i * 2 + 1] = a[i] / M;
        }
        for (int i = 0; i < b.size(); ++i) {
            tb[i * 2] = b[i] % M;
            tb[i * 2 + 1] = b[i] / M;
        }
        auto tc = fft::multiply(ta, tb);
        tc.resize(tc.size() / 2 * 2 + 10, 0);
        for (int i = 0; i + 1 < tc.size(); ++i) {
            tc[i + 1] += tc[i] / M;
            tc[i] %= M;
        }
        vector<int> res(tc.size() / 2);
        for (int i = 0; i < res.size(); ++i)
            res[i] = tc[i * 2] + tc[i * 2 + 1] * M;
        while (!res.empty() && res.back() == 0)
            res.pop_back();
        return res;
    };
    #endif

    auto _mult_ = [&](const vector<int> &a, const vector<int> &b) {
        #ifdef BIGINT_USE_FFT
        if (min(a.size(), b.size()) > 380) {
            return _fft_mult_(a, b);
        }
        #endif

        return _slow_mult_(a, b);
    };

    while ((1 << prec2.size()) < v.size()) {
        prec2.emplace_back(_mult_(prec2.back(), prec2.back()));
    }

    using It = vector<uint32_t>::const_iterator;
    function<vector<int>(It, It, int)> conv = [&](It it1, It it2, int hint) {
        if (it1 + 1 == it2) {
            return vector{int((*it1) % base), int((*it1) / base)};
        }

        int ln = it2 - it1;
        while ((1 << hint) >= ln) --hint;
        assert(1 << (hint + 1) >= ln);

        auto imid = it1 + (1 << hint);

        auto vl = conv(it1, imid, hint - 1);
        auto vr = conv(imid, it2, hint - 1);

        vr = _mult_(vr, prec2[hint]);
        _add_(vr, vl);

        return vr;
    };

    return conv(v.begin(), v.end(), prec2.size() - 1);
}

int rem(const vector<uint32_t> &v, int mod) {
    ll ans = 0;
    for (int i = (int)v.size() - 1; i >= 0; --i) {
        ans = ((ans << 32) + v[i]) % mod;
    }
    return ans;
}

int rem(const vector<int> &v, int mod) {
    ll ans = 0;
    for (int i = (int)v.size() - 1; i >= 0; --i) {
        ans = ((ans * base) + v[i]) % mod;
    }
    return ans;
}

string to_string(const vector<uint32_t> &v) {
    if (v.empty()) return "0";
    string res;
    for (int i = (int)v.size() - 1; i >= 0; --i) {
        for (int j = 31; j >= 0; --j) {
            char c = '0' + ((v[i] >> j) & 1);
            if (c == '0' && res.empty()) continue;
            res += c;
        }
    }
    return res;
}
