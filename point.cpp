struct point {
    ld x, y;

    point(ld x = 0, ld y = 0): x(x), y(y) {}
    point(point a, point b) {
        x = b.x - a.x;
        y = b.y - a.y;
    }

    ld dist(point b) const {
        return sqrt((b.x - x) * (b.x - x) + (b.y - y) * (b.y - y));
    }
    ld sdist(point b) const {
        return ((b.x - x) * (b.x - x) + (b.y - y) * (b.y - y));
    }
    ld len() const {
        return sqrt(x * x + y * y);
    }
    ld slen() const {
        return x * x + y * y;
    }
    point ort() const {
        return point(-y, x);
    }
    ld dp(point p) const {
        return x * p.x + y * p.y;
    }
    ld cp(point p) const {
        return x * p.y - y * p.x;
    }

    point operator + (point a) const {
        return point(a.x + x, a.y + y);
    }
    point operator - (point a) const {
        return point(x - a.x, y - a.y);
    }
    point operator * (ld k) const {
        return point(x * k, y * k);
    }
    point operator / (ld k) const {
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
ostream& operator << (ostream& o, const point &p) {
    return o << "(" << p.x << ", " << p.y << ")";
}
