namespace small_vec_ns {

template<typename T, int N>
struct small_vec {
    array<T, N> data;
    int size_ = 0;

    small_vec(int n = 0, const T& value = T()) : size_(n) {
        fill(data.begin(), data.begin() + n, value);
    }
    template<typename It>
    small_vec(It begin, It end) {
        while (begin != end) data[size_++] = *(begin++);
    }
    small_vec(const std::initializer_list<T>& list) : small_vec(list.begin(), list.end()) {}

    void assign(int n, const T& value) {
        size_ = n;
        for (int i = 0; i < n; ++i) data[i] = value;
    }
    void resize(int n, const T& value = T()) {
        if (n > size_) fill(data.begin() + size_, data.begin() + n, value);
        size_ = n;
    }

    int  size () const { return size_;      }
    bool empty() const { return size_ == 0; }

    auto begin () const { return data.begin();         }
    auto end   () const { return data.begin() + size_; }
    auto begin ()       { return data.begin();         }
    auto end   ()       { return data.begin() + size_; }
    auto rbegin() const { return data.rend () - size_; }
    auto rend  () const { return data.rend ();         }
    auto rbegin()       { return data.rend () - size_; }
    auto rend  ()       { return data.rend ();         }

    void push_back(const T&  value) { data[size_++] = value; }
    void push_back(      T&& value) { data[size_++] = std::move(value); }
    template<typename... Args> void emplace_back(Args&&... args) { data[size_++] = T(args...);; }
    void pop_back() { --size_; }

    template<typename It>
    void insert(It pos, T value) {
        while (pos <= end()) { swap(*(pos++), value); }
        ++size_;
    }
    template<typename It>
    void erase(It pos) {
        while (next(pos) < end()) { auto nx = next(pos); swap(*(pos++), *nx); }
        --size_;
    }

          T& operator[] (int i)       { return data[i]; }
    const T& operator[] (int i) const { return data[i]; }

          T& front()       { return data[0];         }
          T& back ()       { return data[size_ - 1]; }
    const T& front() const { return data[0];         }
    const T& back () const { return data[size_ - 1]; }

    bool operator < (const small_vec<T, N>& other) const {
        for (int i = 0; i < min(size_, other.size_); ++i)
            if (data[i] != other.data[i])
                return data[i] < other.data[i];
        return size_ < other.size_;
    }
    bool operator >  (const small_vec<T, N>& other) const { return other < *this; }
    bool operator <= (const small_vec<T, N>& other) const { return !(other < *this); }
    bool operator >= (const small_vec<T, N>& other) const { return !(*this < other); }
    bool operator == (const small_vec<T, N>& other) const {
        if (size_ != other.size_) return false;
        for (int i = 0; i < size_; ++i)
            if (data[i] != other.data[i])
                return false;
        return true;
    }
    bool operator != (const small_vec<T, N>& other) const { return !(*this == other); }

    vector<T> to_vector() const { return vector<T>(begin(), end()); }
};

template<typename T, int N> string _to_string_(const small_vec<T, N>& v) { return _to_string_(v.to_vector()); }
}
using namespace small_vec_ns;
