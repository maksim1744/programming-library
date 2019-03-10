struct point {
    long double x, y;
    point() {}
    point(long double x, long double y): x(x), y(y) {}
    point(point a, point b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    long double dist(point b) {
        return sqrt((b.x - x) * (b.x - x) + (b.y - y) * (b.y - y));
    }
    long double len() {
        return sqrt(x * x + y * y);
    }
    long double slen() {
        return x * x + y * y;
    }
    point ort() {
        return point(-y, x);
    }
    long double dp(point p) {
        return x * p.x + y * p.y;
    }
    long double cp(point p) {
        return x * p.y - y * p.x;
    }

    point operator + (point a) {
        return point(a.x + x, a.y + y);
    }
    point operator - (point a) {
        return point(x - a.x, y - a.y);
    }
    point operator * (long double k) {
        return point(x * k, y * k);
    }
    point operator / (long double k) {
        return point(x / k, y / k);
    }
};

string to_string (point& p) {
    return "(" + to_string(p.x) + ", " + to_string(p.y) + ")";
}
