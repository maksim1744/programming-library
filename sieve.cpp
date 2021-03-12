struct Sieve {
    vector<bool> isp;
    vector<int> primes;
    vector<short> mn;

    Sieve(int n, bool gen_primes = false, bool gen_mn = false) {
        isp.assign(n + 1, true); isp[0] = isp[1] = false;
        for (int i = 2; i * i <= n; ++i)
            if (isp[i])
                for (int j = i * i; j <= n; j += i)
                    isp[j] = false;

        if (gen_primes)
            for (int i = 2; i <= n; ++i)
                if (isp[i])
                    primes.push_back(i);

        if (gen_mn) {
            mn.resize(n + 1);
            for (int i = 2; i * i <= n; ++i)
                if (isp[i])
                    for (int j = i * i; j <= n; j += i)
                        mn[j] = i;
        }
    }

    bool is_prime(int k) const {
        return isp[k];
    }

    bool operator[](int k) const {
        return isp[k];
    }

    vector<pair<int, int>> get_prime_divs(int k) const {
        static vector<int> v;
        v.clear();
        while (k != 1 && !isp[k]) {
            v.pb(mn[k]);
            k /= mn[k];
        }
        if (k != 1)
            v.pb(k);
        sort(v.begin(), v.end());
        vector<pair<int, int>> res;
        for (int k : v) {
            if (res.empty() || k != res.back().first)
                res.eb(k, 0);
            res.back().second++;
        }
        return res;
    }

    int phi(int n) const {
        auto v = get_prime_divs(n);
        for (auto [p, c] : v)
            n = n / p * (p - 1);
        return n;
    }
};
