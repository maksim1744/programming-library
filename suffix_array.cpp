namespace suffix_array {

// linear suffix array
// works in O(n + sigma)
// for strings call suffix_array(s, ALPHA_SMALL | ALPHA_LARGE) for example,
//     or just suffix_array(s)
// for vectors call suffix_array(v, *max_element(v.begin(), v.end())), v[i] has to be at least 1 for all i

// call lcp(string/vector, suffix_array) for lcp

const int ALPHA_SMALL = 1;
const int ALPHA_LARGE = 2;
const int DIGITS      = 4;

vector<int> buf1, buf2, buf3;

vector<int> suffix_array(vector<int> s, int k) {
    if (s.size() <= 4) {
        vector<int> res(s.size()); iota(res.begin(), res.end(), 0);
        sort(res.begin(), res.end(), [&](int i1, int i2) {
            if (i1 == i2) return false;
            while (i1 < s.size() && i2 < s.size() && s[i1] == s[i2]) {
                ++i1; ++i2;
            }
            if (i1 == s.size() || i2 == s.size()) return i1 > i2;
            return s[i1] < s[i2];
        });
        return res;
    }
    int init_size = s.size();
    s.resize(s.size() + 3, 0);

    vector<int> triple_inds;
    for (int i = 0; i + 3 <= s.size(); ++i)
        if (i % 3 != 0)
            triple_inds.push_back(i);

    // sort triples
    for (int it = 2; it >= 0; --it) {
        buf1.assign(k + 1, 0);
        for (int i : triple_inds)
            buf1[s[i + it]]++;
        for (int i = 1; i < buf1.size(); ++i)
            buf1[i] += buf1[i - 1];
        buf2.resize(triple_inds.size());
        for (int i = (int)triple_inds.size() - 1; i >= 0; --i)
            buf2[--buf1[s[triple_inds[i] + it]]] = triple_inds[i];
        swap(triple_inds, buf2);
    }

    // assign equivalency classes to triples
    buf2.resize(triple_inds.size());
    buf2[0] = 1;
    for (int i = 1; i < triple_inds.size(); ++i) {
        if (s[triple_inds[i]] == s[triple_inds[i - 1]] &&
            s[triple_inds[i] + 1] == s[triple_inds[i - 1] + 1] &&
            s[triple_inds[i] + 2] == s[triple_inds[i - 1] + 2]) {
            buf2[i] = buf2[i - 1];
        } else {
            buf2[i] = buf2[i - 1] + 1;
        }
    }

    // recursively sort triples if there are equal
    if (buf2.back() != buf2.size()) {
        int last = buf2.back();
        buf1.resize(s.size());
        for (int i = 0; i < triple_inds.size(); ++i)
            buf1[triple_inds[i]] = buf2[i];
        buf2.clear();
        for (int i = 1; i + 3 <= s.size(); i += 3)
            buf2.push_back(buf1[i]);
        int n1 = buf2.size();
        for (int i = 2; i + 3 <= s.size(); i += 3)
            buf2.push_back(buf1[i]);
        triple_inds = suffix_array(buf2, last);
        for (int &i : triple_inds) {
            if (i * 3 + 1 + 3 <= s.size()) i = i * 3 + 1;
            else i = (i - n1) * 3 + 2;
        }
    }

    buf2.clear();
    if (s.size() % 3 == 0)
        buf2.push_back(s.size() - 3);
    for (int i : triple_inds)
        if (i % 3 == 1)
            buf2.push_back(i - 1);

    // sort suffixes with i % 3 = 0
    {
        buf1.assign(k + 1, 0);
        for (int i : buf2)
            buf1[s[i]]++;
        for (int i = 1; i < buf1.size(); ++i)
            buf1[i] += buf1[i - 1];

        buf3.resize(buf2.size());
        for (int i = (int)buf2.size() - 1; i >= 0; --i)
            buf3[--buf1[s[buf2[i]]]] = buf2[i];

        swap(buf2, buf3);
    }

    buf1.assign(s.size(), 0);
    for (int i = 0; i < triple_inds.size(); ++i)
        buf1[triple_inds[i]] = i + 1;

    // compare suffixes with i1 % 3 != 0 and i2 % 3 == 0
    auto cmp = [&](int i1, int i2) {
        if (s[i1] != s[i2]) return s[i1] < s[i2];
        ++i1; ++i2;
        if (i1 % 3 != 0) return buf1[i1] < buf1[i2];
        if (s[i1] != s[i2]) return s[i1] < s[i2];
        ++i1; ++i2;
        return buf1[i1] < buf1[i2];
    };

    // merge suf12 and suf0
    buf3.clear();
    auto &res = buf3;
    res.reserve(s.size());
    int i1 = 0, i2 = 0;
    while (i1 < triple_inds.size() || i2 < buf2.size()) {
        bool choose1;
        if (i1 == triple_inds.size()) choose1 = false;
        else if (i2 == buf2.size()) choose1 = true;
        else if (cmp(triple_inds[i1], buf2[i2])) choose1 = true;
        else choose1 = false;

        if (choose1) {
            if (triple_inds[i1] < init_size)
                res.push_back(triple_inds[i1]);
            ++i1;
        } else {
            if (buf2[i2] < init_size)
                res.push_back(buf2[i2]);
            ++i2;
        }
    }

    return res;
}

vector<int> suffix_array(const string &s, int mask = 0) {
    if (mask == 0) {
        array<int, 256> has; has.fill(0);
        for (unsigned char c : s) {
            has[c] = 1;
        }
        int last = 0;
        for (int i = 0; i < 256; ++i) {
            if (has[i]) {
                ++last;
                has[i] = last;
            }
        }

        vector<int> v(s.size());
        for (int i = 0; i < v.size(); ++i) {
            v[i] = has[(unsigned char)s[i]];
        }
        return suffix_array(v, last);
    } else {
        int digit_start = 0;
        int alpha_large_start = (mask & DIGITS) ? 10 : 0;
        int alpha_small_start = (mask & ALPHA_LARGE) ? alpha_large_start + 26 : alpha_large_start;

        vector<int> v(s.size());
        for (int i = 0; i < s.size(); ++i) {
            if      ('0' <= s[i] && s[i] <= '9') v[i] = s[i] - '0' + digit_start + 1;
            else if ('A' <= s[i] && s[i] <= 'Z') v[i] = s[i] - 'A' + alpha_large_start + 1;
            else                                 v[i] = s[i] - 'a' + alpha_small_start + 1;
        }
        return suffix_array(v, alpha_small_start + ((mask & ALPHA_SMALL) ? 26 : 0));
    }
}

template<typename T>
vector<int> lcp(const T &s, const vector<int> &suf_array) {
    int n = s.size();
    vector<int> lcp(n - 1);
    vector<int> ind(n);
    for (int i = 0; i < n; ++i) {
        ind[suf_array[i]] = i;
    }
    int last = 1;
    for (int i = 0; i < n; ++i) {
        last = max(last - 1, 0);
        int i_cur = i;
        if (ind[i_cur] != 0) {
            int i_prev = suf_array[ind[i_cur] - 1];
            while (i_cur + last < s.size() && i_prev + last < s.size() && s[i_cur + last] == s[i_prev + last])
                ++last;
            lcp[ind[i_cur] - 1] = last;
        }
    }
    return lcp;
}

}
