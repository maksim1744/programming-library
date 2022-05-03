struct Sieve {
    struct PrimeIterator {
        int prime;
        int count;
        int num;
        const Sieve& sieve;

        PrimeIterator(int num, const Sieve& sieve) : num(num), sieve(sieve) {
            step();
        }

        void step() {
            prime = sieve.mn[num];
            count = 0;
            while (num != 1 && sieve.mn[num] == prime) {
                num /= sieve.mn[num];
                ++count;
            }
        }

        bool operator == (const PrimeIterator& other) const {
            return tie(prime, count, num) == tie(other.prime, other.count, other.num);
        }
        bool operator != (const PrimeIterator& other) const {
            return !(*this == other);
        }

        PrimeIterator& operator ++ () {
            step();
            return *this;
        }

        pair<int, int> operator * () const {
            return {prime, count};
        }
    };

    struct PrimeIteratorContainer {
        int num;
        const Sieve& sieve;
        using iterator = PrimeIterator;

        PrimeIteratorContainer(int num, const Sieve& sieve) : num(num), sieve(sieve) {}

        PrimeIterator begin() const {
            return PrimeIterator(num, sieve);
        }

        PrimeIterator end() const {
            return PrimeIterator(1, sieve);
        }

        vector<pair<int, int>> collect() const {
            vector<pair<int, int>> result;
            for (auto p : *this)
                result.push_back(p);
            return result;
        }
    };

    template<typename U, typename V>
    struct DoubleIterator {
        U u;
        V v;
        U::iterator uit;
        V::iterator vit;
        int prime;
        int count;

        DoubleIterator(const U& u, const V& v, U::iterator uit, V::iterator vit) : u(u), v(v), uit(uit), vit(vit) {
            tie(prime, count) = this->operator*();
        }

        DoubleIterator& operator ++ () {
            if (uit == u.end()) {
                ++vit;
            } else if (vit == v.end()) {
                ++uit;
            } else if ((*uit).first < (*vit).first) {
                ++uit;
            } else if ((*uit).first > (*vit).first) {
                ++vit;
            } else {
                ++uit;
                ++vit;
            }
            return *this;
        }

        bool operator == (const DoubleIterator& other) const {
            return tie(uit, vit) == tie(other.uit, other.vit);
        }
        bool operator != (const DoubleIterator& other) const {
            return !(*this == other);
        }

        pair<int, int> operator * () const {
            if (uit == u.end()) return *vit;
            if (vit == v.end()) return *uit;
            if ((*uit).first < (*vit).first) return *uit;
            if ((*uit).first > (*vit).first) return *vit;
            return {(*uit).first, (*uit).second + (*vit).second};
        }
    };

    template<typename U, typename V>
    struct DoubleIteratorContainer {
        using iterator = DoubleIterator<U, V>;
        U u;
        V v;

        DoubleIteratorContainer(const U& u, const V& v) : u(u), v(v) {}

        iterator begin() const {
            return iterator(u, v, u.begin(), v.begin());
        }

        iterator end() const {
            return iterator(u, v, u.end(), v.end());
        }

        vector<pair<int, int>> collect() const {
            vector<pair<int, int>> result;
            for (auto p : *this)
                result.push_back(p);
            return result;
        }
    };

    vector<bool> isp;
    vector<int> primes;
    vector<int> mn;

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
            mn.assign(n + 1, -1);
            for (int i = 2; i <= n; ++i) {
                if (isp[i]) {
                    mn[i] = i;
                    if ((ll)i * i <= n)
                        for (int j = i * i; j <= n; j += i)
                            if (mn[j] == -1)
                                mn[j] = i;
                }
            }
        }
    }

    bool is_prime(int k) const {
        return isp[k];
    }

    bool operator[](int k) const {
        return isp[k];
    }

    // can be used as vector<pair<int, int>>, as in function phi() below
    // except doesn't create a vector to avoid heap allocations
    // can be transformed into vector<pair<int, int>> with .collect()
    auto get_prime_divs(int p) const {
        return PrimeIteratorContainer(p, *this);
    }

    // if you want to get prime divs for n = a * b * c where max(a, b, c) < size(sieve), then call get_prime_divs(a, b, c)
    template<typename... Args>
    auto get_prime_divs(int p, Args... args) const {
        return DoubleIteratorContainer(PrimeIteratorContainer(p, *this), get_prime_divs(args...));
    }

    int phi(int n) const {
        auto v = get_prime_divs(n);
        for (auto [p, c] : v)
            n = n / p * (p - 1);
        return n;
    }
};
