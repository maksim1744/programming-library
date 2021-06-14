namespace static_pointer_ns {

// pointer to vector of nodes instead of just heap
//   - takes only sizeof(int) memory, instead of sizeof(size_t)
//   - has the same api as raw pointers, so can be used after one "replace all" action
//   - does not break with wrong aliasing (like __int128 with fast_allocator.cpp)
//   - only takes memory for nodes, so temporary vectors do not take space like with fast_allocator.cpp
//   - supports operator ostream and casting to int (-1 is nullptr), which makes debugging easier (can be used as node id)

// usage:
// example for struct S is in the bottom
// better performace with .reserve() in main
// create new nodes with static new_node() function
//   is is possible to create using operator new, but it has some unnecessary overhead
// does not support operator delete

template<typename T, vector<T>& v>
struct static_pointer {
    int ind = -1;
    static_pointer() {}
    static_pointer(int ind) : ind(ind) {}
    static_pointer(T *ptr) {
        v.push_back(*ptr);
        delete ptr;
        ind = v.size() - 1;
    }
    static_pointer(nullptr_t p) : ind(-1) {}

    template<typename... Args>
    static static_pointer<T, v> new_node(Args... args) {
        v.emplace_back(args...);
        return v.size() - 1;
    }

    T* operator -> () const {
        return &v[ind];
    }

    T& operator * () {
        return v[ind];
    }

    operator bool () const {
        return ind != -1;
    }
    operator int () const {
        return ind;
    }

    bool operator == (nullptr_t p) const {
        return ind == -1;
    }
    bool operator == (int k) const {
        return ind == k;
    }
    bool operator != (nullptr_t p) const {
        return ind != -1;
    }
    bool operator != (int k) const {
        return ind != k;
    }
};

template<typename T, vector<T>& v>
ostream &operator << (ostream &o, const static_pointer<T, v> &ptr) {
    return o << (int)ptr;
}

}
using namespace static_pointer_ns;

// struct S;
// vector<S> nodes;
// using s_ptr = static_pointer<S, nodes>;
