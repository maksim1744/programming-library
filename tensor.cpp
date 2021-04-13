namespace tensor_ns {
template<typename T, int N>
struct _Tensor_ {
    vector<T> v;
    vector<size_t> dims;
    vector<size_t> sz;

    _Tensor_(const array<size_t, N> &list, T value = T()) {
        dims = sz = vector<size_t>(list.begin(), list.end());
        dims.push_back(1);
        sz.push_back(1);
        for (int i = (int)sz.size() - 2; i >= 0; --i)
            sz[i] *= sz[i + 1];
        v.assign(sz[0], value);
    }
};

template<typename T, int N, int M = N>
struct Tensor {
    shared_ptr<_Tensor_<T, M>> tensor;
    size_t l;
    size_t dim;

    Tensor(const array<size_t, M> &list, T value = T()) : tensor(new _Tensor_<T, M>(list, value)), l(0), dim(0) {}
    Tensor(shared_ptr<_Tensor_<T, M>> tensor, size_t l, size_t dim) : tensor(tensor), l(l), dim(dim) {}

    Tensor<T, N - 1, M> operator [] (size_t i) {
        return Tensor<T, N - 1, M>(tensor, l + tensor->sz[dim + 1] * i, dim + 1);
    }
    const Tensor<T, N - 1, M> operator [] (size_t i) const {
        return Tensor<T, N - 1, M>(tensor, l + tensor->sz[dim + 1] * i, dim + 1);
    }
    void fill(T value) {
        std::fill(tensor->v.begin() + l, tensor->v.begin() + l + total_size(), value);
    }
    int size() const {
        return tensor->dims[M - N];
    }
    int total_size() const {
        return tensor->sz[M - N];
    }
    auto to_vector() const {
        vector res = {{(*this)[0].to_vector()}};
        for (int i = 1; i < size(); ++i)
            res.push_back((*this)[i].to_vector());
        return res;
    }
};

template<typename T, int M>
struct Tensor<T, 1, M> {
    shared_ptr<_Tensor_<T, M>> tensor;
    size_t l;
    size_t dim;

    Tensor(const array<size_t, M> &list, T value = T()) : tensor(new _Tensor_<T, M>(list, value)), l(0), dim(0) {}
    Tensor(shared_ptr<_Tensor_<T, M>> tensor, size_t l, size_t dim) : tensor(tensor), l(l), dim(dim) {}

    T &operator [] (size_t i) {
        return tensor->v[l + i];
    }
    T &operator [] (size_t i) const {
        return tensor->v[l + i];
    }
    void fill(T value) {
        std::fill(tensor->v.begin() + l, tensor->v.begin() + l + total_size(), value);
    }
    int size() const {
        return tensor->dims[M - 1];
    }
    int total_size() const {
        return tensor->sz[M - 1];
    }
    auto to_vector() const {
        return vector<T>(tensor->v.begin() + l, tensor->v.begin() + l + size());
    }
};

template<typename T, int N, int M>
ostream &operator << (ostream &o, const Tensor<T, N, M> &t) {
    for (int i = 0; i < t.size(); ++i) {
        o << endl;
        o << t[i];
    }
    return o;
}
template<typename T, int M>
ostream &operator << (ostream &o, const Tensor<T, 1, M> &t) {
    auto v = t.to_vector();
    o << "[";
    for (int i = 0; i < v.size(); ++i) {
        if (i) o << ", ";
        o << v[i];
    }
    o << "]";
    return o;
}
template<typename T, int M>
ostream &operator << (ostream &o, const Tensor<T, 2, M> &t) {
    auto v = t.to_vector();
    vector<vector<string>> result(v.size());
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v[i].size(); ++j) {
            stringstream ss;
            ss << v[i][j];
            result[i].push_back(ss.str());
        }
    }
    vector<int> widths;
    for (int i = 0; i < result.size(); ++i) {
        widths.resize(max(widths.size(), result[i].size()), 0);
        for (int j = 0; j < result[i].size(); ++j)
            widths[j] = max(widths[j], (int)result[i][j].size());
    }
    if (widths.empty()) return o;

    const int padding = 12;

    widths.assign(widths.size(), *max_element(widths.begin(), widths.end()));

    int total = accumulate(widths.begin(), widths.end(), 0);
    total += 2 * ((int)widths.size() - 1);
    o << endl;
    for (int i = 0; i < v.size(); ++i) {
        o << string(padding, ' ');
        o << "[";
        int left = total;
        for (int j = 0; j < v[i].size(); ++j) {
            o << setw(widths[j]) << result[i][j];
            if (j + 1 != v[i].size())
                o << ", ";
            left -= widths[j];
            left -= 2;
        }
        if (!v[i].empty())
            left += 2;
        if (v[i].size() != widths.size())
            o << string(left, ' ');
        o << "]";
        if (i + 1 != v.size())
            o << ",\n";
    }
    o << "]]";

    return o;
}
}
using namespace tensor_ns;
