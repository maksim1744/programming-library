#include "macro_map.h"

#define debug if (true)

vector<string> _split_names_(const string &s) {
    string brackets;
    vector<string> res;
    string token;
    size_t ind = 0;
    while (ind < s.size()) {
        if (s[ind] == ',' && brackets.empty()) {
            res.push_back(token);
            token.clear();
            ++ind;
            while (ind < s.size() && s[ind] == ' ') ++ind;
            continue;
        }

        token.push_back(s[ind]);
        if (s[ind] == '[' || s[ind] == '{' || s[ind] == '(')
            brackets.push_back(s[ind]);
        else if (!brackets.empty() && ((brackets.back() == '[' && s[ind] == ']') ||
                                       (brackets.back() == '(' && s[ind] == ')') ||
                                       (brackets.back() == '{' && s[ind] == '}')))
            brackets.pop_back();
        ++ind;
    }
    res.push_back(token);
    return res;
}

template<typename T, typename _ = void>
struct is_container : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container<
    T,
    std::conditional_t<
        false,
        is_container_helper<
            decltype(std::declval<T>().begin()),
            decltype(std::declval<T>().end())
            >,
        void
        >
    > : public std::true_type {};


template<typename T>
string _to_string_(T v);


template<typename T, typename U = void>
struct printer {
static string _to_string_(T v) {
    stringstream ss;
    ss << v;
    return ss.str();
}
};

template<typename T>
struct printer<T, typename enable_if<is_container<T>::value && !is_same<string, T>::value>::type> {
static string _to_string_(T v) {
    string res = "[";
    for (auto item : v) {
        res += ::_to_string_(item);
        res += ", ";
    }
    if (res.size() > 2)
        res.pop_back(), res.pop_back();
    res += "]";
    return res;
}
};

template<typename U>
struct printer<queue<U>> {
static string _to_string_(queue<U> v) {
    string res = "[";
    while (!v.empty()) {
        auto x = v.front();
        v.pop();
        if (res.size() != 1)
            res += ", ";
        res += ::_to_string_(x);
    }
    res += "]";
    return res;
}
};

template<typename A, typename B, typename C>
struct printer<priority_queue<A, B, C>> {
static string _to_string_(priority_queue<A, B, C> v) {
    string res = "[";
    while (!v.empty()) {
        auto x = v.top();
        v.pop();
        if (res.size() != 1)
            res += ", ";
        res += ::_to_string_(x);
    }
    res += "]";
    return res;
}
};

template<typename U, typename V>
struct printer<pair<U, V>> {
static string _to_string_(pair<U, V> v) {
    return "(" + ::_to_string_(v.first) + ", " + ::_to_string_(v.second) + ")";
}
};

template<>
struct printer<string> {
static string _to_string_(string v) {
    return "\"" + v + "\"";
}
};

template<>
struct printer<__uint128_t> {
static string _to_string_(__uint128_t v) {
    if (v == 0) return "0";
    string result;
    while (v) {
        result.push_back(v % 10 + '0');
        v /= 10;
    }
    reverse(result.begin(), result.end());
    return result;
}
};

template<>
struct printer<__int128> {
static string _to_string_(__int128 v) {
    __uint128_t k = v;
    if (k == (((__uint128_t)1) << 127)) return "-170141183460469231731687303715884105728";
    string result;
    if (v < 0) {
        result += "-";
        v *= -1;
    }
    result += ::_to_string_((__uint128_t)v);
    return result;
}
};

template<typename T>
string _to_string_(T x) {
    return printer<T>::_to_string_(x);
}

size_t _indent_ = 0;
const string _indent_string_ = "|  ";

void _show_indent_() {
    #ifdef SHOW_COLORS
    cerr << "\e[1;30m";
    #endif
    for (size_t i = 0; i < _indent_; ++i) {
        cerr << _indent_string_;
    }
    #ifdef SHOW_COLORS
    cerr << "\e[m";
    #endif
}

template<typename T>
bool _show_unique_associative_container_(string name, T m) {
    if (m.empty()) return false;
    cerr << " [";
    int padding = 8;
    name += ": [";
    padding += name.size();
    cerr << name;
    vector<string> keys, values;
    int key_width = 0, value_width = 0;
    for (auto [a, b] : m) {
        keys.push_back(_to_string_(a));
        values.push_back(_to_string_(b));
        key_width = max(key_width, (int)keys.back().size());
        value_width = max(value_width, (int)values.back().size());
    }
    for (int i = 0; i < keys.size(); ++i) {
        if (i != 0) {
            _show_indent_();
            cerr << string(padding, ' ');
        }
        cerr << std::left << setw(key_width) << keys[i] << ": " << values[i];
        if (i + 1 != keys.size())
            cerr << ",\n";
    }
    cerr << "]]";
    return true;
}

template<typename T, typename U, typename C>
bool _show_unique_(string name, map<T, U, C> m) {
    return _show_unique_associative_container_(name, m);
}
template<typename T, typename U, typename H>
bool _show_unique_(string name, unordered_map<T, U, H> m) {
    return _show_unique_associative_container_(name, m);
}

template<typename T>
bool _show_unique_(string name, vector<vector<T>> v) {
    if (v.empty()) {
        return false;
    }
    vector<vector<string>> result(v.size());
    for (int i = 0; i < v.size(); ++i)
        for (int j = 0; j < v[i].size(); ++j)
            result[i].push_back(_to_string_(v[i][j]));
    vector<int> widths;
    for (int i = 0; i < result.size(); ++i) {
        widths.resize(max(widths.size(), result[i].size()), 0);
        for (int j = 0; j < result[i].size(); ++j)
            widths[j] = max(widths[j], (int)result[i][j].size());
    }
    if (widths.empty()) return false;

    cerr << std::right << " [";
    int padding = 8;
    name += ": [";
    padding += name.size();
    cerr << name;

    widths.assign(widths.size(), *max_element(widths.begin(), widths.end()));

    int total = accumulate(widths.begin(), widths.end(), 0);
    total += 2 * ((int)widths.size() - 1);
    for (int i = 0; i < v.size(); ++i) {
        if (i != 0) {
            _show_indent_();
            cerr << string(padding, ' ');
        }
        cerr << "[";
        int left = total;
        for (int j = 0; j < v[i].size(); ++j) {
            cerr << setw(widths[j]) << result[i][j];
            if (j + 1 != v[i].size())
                cerr << ", ";
            left -= widths[j];
            left -= 2;
        }
        if (!v[i].empty())
            left += 2;
        if (v[i].size() != widths.size())
            cerr << string(left, ' ');
        cerr << "]";
        if (i + 1 != v.size())
            cerr << ",\n";
    }
    cerr << "]]";

    return true;
}

template<typename T>
bool _show_unique_(__attribute__((unused)) string name, __attribute__((unused)) T x) { return false; }

void _show_(__attribute__((unused)) int vals, __attribute__((unused)) vector<string>::const_iterator it) {}

template<typename T, typename... Args>
void _show_(int vals, vector<string>::const_iterator it, T arg, Args... args) {
    if (vals == 1)
        if (_show_unique_(*it, arg))
            return;
    if ((it->front() == '"' || it->front() == '\'') && (it->back() == '"' || it->back() == '\''))
        cerr << " [" << *it << "]";
    else
        cerr << " [" << *it << ": " << _to_string_(arg) << "]";
    ++it;
    _show_(vals, it, args...);
}

void _show_line_number_(int num) {
    _show_indent_();
    cerr << std::right;
    #ifdef SHOW_COLORS
    cerr << "\e[1;34m";
    #endif
    cerr << "[" << setw(3) << num << "] ";
    #ifdef SHOW_COLORS
    cerr << "\e[m";
    #endif
}

#define shows { \
    _show_line_number_(__LINE__); \
    cerr << " " << string(80, '-') << endl; \
}

#define show(args...) { \
    _show_line_number_(__LINE__); \
    auto _names_ = _split_names_(#args); \
    _show_(_names_.size(), _names_.begin(), args); \
    cerr << endl; \
}

auto _start_time_ = clock();
auto _last_time_ = _start_time_;
void _mclock_(string msg = "") {
    cerr << ' ';
    #ifdef SHOW_COLORS
    cerr << "\e[1;31m";
    #endif
    stringstream ss;
    ss.precision(3);

    ss.str("");
    double time1 = (double)(clock() - _last_time_) / CLOCKS_PER_SEC;
    ss << fixed << time1;
    string s1 = ss.str();
    if (s1.size() < 7)
        s1 = string(7 - s1.size(), ' ') + s1;

    ss.str("");
    double time2 = (double)(clock() - _start_time_) / CLOCKS_PER_SEC;
    ss << fixed << time2;
    string s2 = ss.str();
    if (s2.size() < 7)
        s2 = string(7 - s2.size(), ' ') + s2;
    cerr << fixed << "[ " << "last " << s1 << " | " << "total " << s2 << " ] " << msg << endl;
    _last_time_ = clock();
    #ifdef SHOW_COLORS
    cerr << "\e[m";
    #endif
}

#define mclock { \
    _show_line_number_(__LINE__); \
    _mclock_(); \
}

template<typename T>
struct _dbgf_struct_ {
    size_t line_n;
    const string& f_name;
    T f;
    bool first = true;

    _dbgf_struct_(size_t line_n, const string& f_name, T f) : line_n(line_n), f_name(f_name), f(std::move(f)) {}

    void print_args() const {
        cerr << ")";
    }

    template<typename A, typename... Args>
    void print_args(const A& a, Args... args) {
        if (!first)
            cerr << ", ";
        first = false;
        cerr << _to_string_(a);
        print_args(args...);
    }

    template<typename... Args>
    auto operator()(Args... args) {
        first = true;
        _show_line_number_(line_n);
        cerr << ' ' << f_name << "(";
        print_args(args...);
        cerr << endl;

        ++_indent_;
        auto result = f(args...);
        --_indent_;

        first = true;
        _show_line_number_(line_n);
        cerr << ' ' << f_name << "(";
        print_args(args...);
        cerr << " = " << _to_string_(result) << endl;

        return result;
    }
};

#define debugf(f) _dbgf_struct_(__LINE__, #f, f)

template<typename T>
auto _dbgv_(size_t line_n, const string& v_name, T v) {
    vector<string> names = {v_name};
    _show_line_number_(line_n);
    _show_(names.size(), names.begin(), v);
    cerr << endl;
    return v;
}

#define debugv(v) _dbgv_(__LINE__, #v, v)

#define PRINT_FIELD(field) _o_ << ", " << #field << ": " << _to_string_(_s_.field);
#define PRINT_STRUCT(_class_name_, _first_arg_, ...) \
    _o_ << "[" << #_first_arg_ << ": " << _to_string_(_s_._first_arg_); \
    __VA_OPT__(MAP(PRINT_FIELD, __VA_ARGS__);) \
    _o_ << "]"; \

#define OSTREAM(_class_name_, ...) ostream& operator<<(ostream& _o_, const _class_name_& _s_) { \
    __VA_OPT__(PRINT_STRUCT(_class_name_, __VA_ARGS__)) \
    return _o_; \
}

#define PRINT_FIELD0(field) _o_ << ", " << _to_string_(_s_.field);
#define PRINT_STRUCT0(_class_name_, _first_arg_, ...) \
    _o_ << "[" << _to_string_(_s_._first_arg_); \
    __VA_OPT__(MAP(PRINT_FIELD0, __VA_ARGS__);) \
    _o_ << "]"; \

#define OSTREAM0(_class_name_, ...) ostream& operator<<(ostream& _o_, const _class_name_& _s_) { \
    __VA_OPT__(PRINT_STRUCT0(_class_name_, __VA_ARGS__)) \
    return _o_; \
}
