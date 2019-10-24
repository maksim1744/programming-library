struct Pt {
    long double x, y, z;

    Pt() : x(0), y(0), z(0) {}
    Pt(long double x, long double y, long double z) : x(x), y(y), z(z) {}

    Pt operator - (Pt b) {
        return Pt(x - b.x, y - b.y, z - b.z);
    }
    Pt operator + (Pt b) {
        return Pt(x + b.x, y + b.y, z + b.z);
    }
    Pt operator * (long double k) {
        return Pt(x * k, y * k, z * k);
    }
    Pt operator / (long double k) {
        return Pt(x / k, y / k, z / k);
    }

    long double len() {
        return sqrt(x * x + y * y + z * z);
    }

    long double dp(Pt b) {
        return x * b.x + y * b.y + z * b.z;
    }

    Pt cp(Pt b) {
        return Pt(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }
};
long double tp(Pt a, Pt b, Pt c) {
    return a.dp(b.cp(c));
}
struct Tr {
    Pt a, b, c;
    Pt norm;

    Tr() {}
    Tr(Pt _a, Pt _b, Pt _c, Pt _norm = Pt(0, 0, 0)) : a(_a), b(_b), c(_c), norm(_norm) {
        // if (a.i > b.i)
        //     swap(a, b);
        // if (a.i > c.i)
        //     swap(a, c);
        // if (b.i > c.i)
        //     swap(b, c);
    }
};
