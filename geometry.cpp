const long double e = 1e-19;

// struct:
//     point
//     line
//     circle

struct point {
    long double x, y;
    point() {};
    point(long double x, long double y): x(x), y(y) {};
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
    point ort() {
        return point(-y, x);
    }

    long double dp(point other) {
        return x * other.x + y * other.y;
    }

    point operator+(point a) {
        return point(a.x + x, a.y + y);
    }
    point operator-(point a) {
        return point(x - a.x, y - a.y);
    }
    point operator*(long double k) {
        return point(x * k, y * k);
    }
    point operator/(long double k) {
        return point(x / k, y / k);
    }
    friend ostream& operator<< (ostream &os, const point & p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};

string to_string(point p) {
    stringstream ss;
    ss << p;
    return ss.str();
}

struct line {
    long double a, b, c;
    line() {};
    line(long double a, long double b, long double c): a(a), b(b), c(c) {};
    line(point m, point n) {
        a = n.y - m.y;
        b = m.x - n.x;
        c = -a * m.x - b * m.y;
    }
    point intersect(line l) {
        return point((b * l.c - c * l.b) / (a * l.b - b * l.a), (a * l.c - c * l.a) / (b * l.a - a * l.b));
    }
    long double dist(point p) {
        return abs(a * p.x + b * p.y + c) / sqrt(a * a + b * b);
    }

    friend ostream& operator<< (ostream &os, const line & l) {
        if (l.a != 0) {
            os << l.a << "x ";
            if (l.b != 0) {
                if (l.b > 0) {
                    os << "+";
                } else {
                    os << "-";
                }
                os << " " << abs(l.b) << "y ";
            }
        } else {
            os << l.b << "y ";
        }
        if (l.c != 0) {
            if (l.c > 0) {
                os << "+";
            } else {
                os << "-";
            }
            os << " " << abs(l.c) << " ";
        }
        os << "= 0";
        return os;
    }
};

string to_string(line l) {
    stringstream ss;
    ss << l;
    return ss.str();
}

struct circle {
    long double r;
    point o;
    circle() {};
    circle(long double x, long double y, long double new_r) {
        o = point(x, y);
        r = new_r;
    };
    circle(point p, long double new_r) {
        o = p;
        r = new_r;
    };

    long double dist(line l) {
        return l.dist(o);
    }

    vector<point> intersect(line l) {
        vector<point> ans;

        long double d = dist(l);
        if (d > r + e) {
            return ans;
        }
        if (d > r - e) {
            point ort = point(l.a, l.b);
            ort = ort / ort.len() * r;
            auto ort2 = ort * -1;
            if (l.dist(o + ort2) < l.dist(o + ort))
                swap(ort, ort2);
            ans.push_back(ort + o);
            return ans;
        }

        point ort = point(l.a, l.b);
        point m = line(o, o + ort).intersect(l);
        point nap = point(-l.b, l.a);
        nap = nap * sqrt(r * r - d * d) / nap.len();

        ans.push_back(m + nap);
        ans.push_back(m - nap);

        return ans;
    }
    vector<point> intersect(circle s) {
        vector<point> ans;

        long double d = o.dist(s.o);
        if (d > r + s.r + e) {
            return ans;
        }
        if (d > r + s.r - e) {
            point oo = point(o, s.o);
            ans.push_back(o + oo * r / oo.len());
            return ans;
        }
        if (d < abs(r - s.r) - e) {
            return ans;
        }
        if (d < abs(r - s.r) + e) {
            point oo = point(o, s.o);
            oo = oo * r / oo.len();
            if (r < s.r) {
                oo = oo * (-1);
            }
            ans.push_back(o + oo);
            return ans;
        }
        line l = line((long double)2 * (s.o.x - o.x), (long double)2 * (s.o.y - o.y),
                        o.x * o.x + o.y * o.y - s.o.x * s.o.x - s.o.y * s.o.y - r * r + s.r * s.r);
        return intersect(l);
    }
    
    friend ostream& operator<< (ostream &os, const circle & s) {
        if (s.o.x == 0) {
            os << "x^2 + ";
        } else if (s.o.x > 0) {
            os << "(x - " << abs(s.o.x) << ")^2 + ";
        } else {
            os << "(x + " << abs(s.o.x) << ")^2 + ";
        }
        if (s.o.y == 0) {
            os << "y^2 = ";
        } else if (s.o.y > 0) {
            os << "(y - " << abs(s.o.y) << ")^2 = ";
        } else {
            os << "(y + " << abs(s.o.y) << ")^2 = ";
        }
        os << abs(s.r) << "^2";
        return os;
    }
};

string to_string(circle c) {
    stringstream ss;
    ss << c;
    return ss.str();
}

vector<line> tangentToCircle(point p, circle s) {
    vector<line> ans;

    circle s1 = circle((p + s.o) / 2.0, (s.o - p).len() / 2.0);
    vector<point> pts = s1.intersect(s);
    for (auto pt : pts) {
        ans.push_back(line(pt, pt + (pt - s.o).ort()));;
    }

    return ans;
}

vector<line> outer_tangents_to_circles(circle a, circle b) {
    vector<line> ans;
    if (a.r < b.r) {
        swap(a, b);
    }
    long double d = a.o.dist(b.o);
    if (d > a.r - b.r + e) {
        if (abs(a.r - b.r) < e) {
            line oo = line(a.o, b.o);
            point ort = point(oo.a, oo.b);
            ort = ort * a.r / ort.len();
            ans.push_back(line(a.o + ort, b.o + ort));
            ans.push_back(line(a.o - ort, b.o - ort));
        } else {
            vector<point> pts = circle(a.o, a.r - b.r).intersect(circle((a.o + b.o) / 2, (a.o - b.o).len() / 2));
            vector<point> vcts(pts.size());
            for (int i = 0; i < pts.size(); ++i) {
                vcts[i] = pts[i] - a.o;
                vcts[i] = vcts[i] * b.r / vcts[i].len();
                ans.push_back(line(pts[i] + vcts[i], b.o + vcts[i]));
            }
        }
    } else if (d > a.r - b.r - e) {
        point oo = point(a.o, b.o);
        oo = oo * a.r / oo.len();
        ans.push_back(line(a.o + oo, a.o + oo + oo.ort()));
    }
    return ans;
}

vector<line> inner_tangents_to_circle(circle a, circle b) {
    vector<line> ans;
    if (a.r < b.r) {
        swap(a, b);
    }
    long double d = a.o.dist(b.o);
    if (d > a.r + b.r + e) {
        vector<point> pts = circle(a.o, a.r + b.r)
                .intersect(circle((a.o + b.o) / 2, (a.o - b.o).len() / 2));
        vector<point> vcts(pts.size());
        for (int i = 0; i < pts.size(); ++i) {
            vcts[i] = a.o - pts[i];
            vcts[i] = vcts[i] * b.r / vcts[i].len();
            ans.push_back(line(pts[i] + vcts[i], b.o + vcts[i]));
        }
    } else if (d > a.r + b.r - e) {
        point oo = point(a.o, b.o);
        oo = oo * a.r / oo.len();
        ans.push_back(line(a.o + oo, a.o + oo + oo.ort()));
    }
    return ans;
}

vector<line> tangents_to_circles(circle a, circle b) {
    vector<line> ans;
    vector<line> inner = inner_tangents_to_circle(a, b);
    vector<line> outer = outer_tangents_to_circles(a, b);
    for (auto i : inner) {
        ans.push_back(i);
    }
    for (auto i : outer) {
        ans.push_back(i);
    }
    return ans;
}
