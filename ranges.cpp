namespace ranges {
// waiting for C++ 20

template<typename I> struct _range {
    I first, last;
    explicit _range(const I &first, const I &last) : first(first), last(last) {}

    I  begin() const { return first; }
    I    end() const { return  last; }

    auto rbegin() const { return make_reverse_iterator(last ); }
    auto   rend() const { return make_reverse_iterator(first); }

    int   size() const { return distance(first, last); }
    bool empty() const { return first == last; }
};
template<typename I> _range<I> range(const I &first, const I &last) { return _range<I>(first, last); }
template<typename C> auto range(C &c) { return _range(c.begin(), c.end()); }

template<typename I> _range<reverse_iterator<I>> reversed(const _range<I> &r) { return range(make_reverse_iterator(r.end()), make_reverse_iterator(r.begin())); }
// not const reference to replace UB in reversed(f()) with CE
template<typename T> auto reversed(T &v) { return range(v.rbegin(), v.rend()); }

// comparison operators
template<typename I1, typename I2>
bool operator < (const _range<I1> &a, const _range<I2> &b) {
    auto ita = a.begin(); auto itb = b.begin();
    for (; ita != a.end() && itb != b.end(); ++ita, ++itb)
        if (*ita != *itb)
            return *ita < *itb;
    return ita == a.end() && itb != b.end();
}
template<typename I1, typename I2> bool operator >  (const _range<I1> &a, const _range<I2> &b) { return   b < a;  }
template<typename I1, typename I2> bool operator <= (const _range<I1> &a, const _range<I2> &b) { return !(a > b); }
template<typename I1, typename I2> bool operator >= (const _range<I1> &a, const _range<I2> &b) { return !(a < b); }

// equality operators, O(1) for random_access_iterators with different range lengths
template<typename I1, typename I2>
bool operator_eq_brute(const _range<I1> &a, const _range<I2> &b) {
    auto ita = a.begin(); auto itb = b.begin();
    for (; ita != a.end() && itb != b.end(); ++ita, ++itb)
        if (*ita != *itb)
            return false;
    return ita == a.end() && itb == b.end();
}
template<typename I1, typename I2,
         typename enable_if<is_base_of<random_access_iterator_tag, typename iterator_traits<I1>::iterator_category>::value &&
                            is_base_of<random_access_iterator_tag, typename iterator_traits<I2>::iterator_category>::value>::type* = nullptr>
bool operator == (const _range<I1> &a, const _range<I2> &b) {
    if (a.size() != b.size()) return false;
    return operator_eq_brute(a, b);
}
template<typename I1, typename I2,
         typename enable_if<!is_base_of<random_access_iterator_tag, typename iterator_traits<I1>::iterator_category>::value ||
                            !is_base_of<random_access_iterator_tag, typename iterator_traits<I2>::iterator_category>::value>::type* = nullptr>
bool operator == (const _range<I1> &a, const _range<I2> &b) {
    return operator_eq_brute(a, b);
}
template<typename I1, typename I2> bool operator != (const _range<I1> &a, const _range<I2> &b) { return !(a == b); }

#define unrange(r) (r).first, (r).last

}
using namespace ranges;
