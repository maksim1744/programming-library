string to_string(__int128 x) {
    if (x == 0) return "0";
    __uint128_t k = x;
    if (k == (((__uint128_t)1) << 127)) return "-170141183460469231731687303715884105728";
    string result;
    if (x < 0) {
        result += "-";
        x *= -1;
    }
    string t;
    while (x) {
        t.push_back('0' + x % 10);
        x /= 10;
    }
    reverse(t.begin(), t.end());
    return result + t;
}

ostream &operator << (ostream &o, __int128 x) {
    return o << to_string(x);
}
