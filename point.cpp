struct point {
    long double x, y;

    point(long double x = 0, long double y = 0): x(x), y(y) {}
    point(point a, point b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    long double dist(point b) const {
        return sqrt((b.x - x) * (b.x - x) + (b.y - y) * (b.y - y));
    }
    long double len() const {
        return sqrt(x * x + y * y);
    }
    long double slen() const {
        return x * x + y * y;
    }
    point ort() const {
        return point(-y, x);
    }
    long double dp(point p) const {
        return x * p.x + y * p.y;
    }
    long double cp(point p) const {
        return x * p.y - y * p.x;
    }

    point operator + (point a) const {
        return point(a.x + x, a.y + y);
    }
    point operator - (point a) const {
        return point(x - a.x, y - a.y);
    }
    point operator * (long double k) const {
        return point(x * k, y * k);
    }
    point operator / (long double k) const {
        return point(x / k, y / k);
    }

    point& norm() {
        ld d = len();
        if (d != 0) {
            x /= d;
            y /= d;
        }
        return *this;
    }
};

#ifdef HOME
string to_string(point p) {
    return "(" + to_string(p.x) + ", " + to_string(p.y) + ")";
}
#endif
