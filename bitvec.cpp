struct bitvec {
    using value_type = uint64_t;
    const static int word_len = __builtin_popcountll(~(value_type)0);

    vector<value_type> data;
    value_type last_mask;
    int n;

    bitvec(int n = 1, value_type initial_value = 0) : n(n) {
        data.assign((n + word_len - 1) / word_len, 0);
        data[0] = initial_value;
        last_mask = (~(value_type)0) >> (data.size() * word_len - n);
    }

    int size() const {
        return n;
    }

    int count() const {
        int result = 0;
        for (value_type v : data)
            result += __builtin_popcountll(v);
        return result;
    }

    struct bit_reference {
        value_type *val;
        int bit;

        bit_reference(value_type &val, int bit) : val(&val), bit(bit) {}

        bit_reference &operator = (value_type v) {
            (*val) = ((*val) & ~((value_type)1 << bit));
            (*val) ^= ((v & 1) << bit);
            return *this;
        }

        operator bool() const {
            return ((*val) >> bit) & 1;
        }
    };

    bit_reference operator[](size_t ind) {
        return bit_reference(data[ind / word_len], ind % word_len);
    }

    bitvec &operator &= (const bitvec &other) {
        for (int i = 0; i < data.size(); ++i)
            data[i] &= other.data[i];
        return *this;
    }
    bitvec operator & (const bitvec &other) const {
        bitvec res = *this;
        return res &= other;
    }

    bitvec &operator ^= (const bitvec &other) {
        for (int i = 0; i < data.size(); ++i)
            data[i] ^= other.data[i];
        data.back() &= last_mask;
        return *this;
    }
    bitvec operator ^ (const bitvec &other) const {
        bitvec res = *this;
        return res ^= other;
    }

    bitvec &operator |= (const bitvec &other) {
        for (int i = 0; i < data.size(); ++i)
            data[i] |= other.data[i];
        data.back() &= last_mask;
        return *this;
    }
    bitvec operator | (const bitvec &other) const {
        bitvec res = *this;
        return res |= other;
    }

    bitvec &operator >>= (int value) {
        if (value == 0) return *this;
        if (value < 0) return *this >>= (-value);

        int whole = value / word_len;
        if (whole) {
            for (int i = whole; i < data.size(); ++i) {
                data[i - whole] = data[i];
            }
            for (int i = (int)data.size() - whole; i < data.size(); ++i)
                data[i] = 0;
        }

        int rem = value % word_len;
        if (rem) {
            for (int i = 0; i < data.size(); ++i) {
                if (i != 0) data[i - 1] |= (data[i] << (word_len - rem));
                data[i] >>= rem;
            }
        }

        return *this;
    }
    bitvec operator >> (int value) const {
        auto res = *this;
        return res >>= value;
    }

    bitvec &operator <<= (int value) {
        if (value == 0) return *this;
        if (value < 0) return *this >>= (-value);

        int whole = value / word_len;
        if (whole) {
            for (int i = (int)data.size() - 1; i >= whole; --i) {
                data[i] = data[i - whole];
            }
            for (int i = 0; i < whole; ++i)
                data[i] = 0;
        }

        int rem = value % word_len;
        if (rem) {
            for (int i = (int)data.size() - 1; i >= 0; --i) {
                if (i + 1 != data.size()) data[i + 1] |= (data[i] >> (word_len - rem));
                data[i] <<= rem;
            }
        }

        data.back() &= last_mask;

        return *this;
    }
    bitvec operator << (int value) const {
        auto res = *this;
        return res <<= value;
    }
};
