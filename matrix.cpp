template<typename T, int N>
struct Matrix {
    array<array<T, N>, N> m;

    Matrix() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] = 0;
            }
        }
    }
    Matrix(std::initializer_list<std::initializer_list<T>> s) {
        int i = 0;
        for (auto it = s.begin(); it != s.end(); ++it) {
            int j = 0;
            for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
                m[i][j] = *it2;
                ++j;
            }
            ++i;
        }
    }

    static Matrix E() {
        Matrix e;
        for (int i = 0; i < N; ++i) {
            e[i][i] = 1;
        }
        return e;
    }

    array<T, N> &operator[](int i) {
        return m[i];
    }

    const array<T, N> &operator[](int i) const {
        return m[i];
    }

    Matrix operator * (const Matrix &b) const {
        Matrix c;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k) {
                    c[i][k] += m[i][j] * b[j][k];
                }
            }
        }
        return c;
    }

    Matrix &operator *= (const Matrix &other) {
        *this = (*this) * other;
        return *this;
    }

    Matrix &operator *= (const T &x) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] *= x;
            }
        }
        return *this;
    }
    Matrix operator * (const T &x) const {
        Matrix a = *this;
        a *= x;
        return a;
    }

    Matrix &operator /= (const T &x) {
        T inv = T(1) / x;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] *= inv;
            }
        }
        return *this;
    }
    Matrix operator / (const T &x) const {
        Matrix a = *this;
        a /= x;
        return a;
    }

    Matrix &operator += (const Matrix &other) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] += other[i][j];
            }
        }
        return *this;
    }
    Matrix operator + (const Matrix &other) const {
        Matrix a = *this;
        a += other;
        return a;
    }

    Matrix &operator -= (const Matrix &other) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] -= other[i][j];
            }
        }
        return *this;
    }
    Matrix operator - (const Matrix &other) const {
        Matrix a = *this;
        a -= other;
        return a;
    }
};

template<typename T, int N>
Matrix<T, N> pow(Matrix<T, N> m, ll p) {
    Matrix<T, N> res = Matrix<T, N>::E();
    while (p) {
        if (p & 1) res *= m;
        m *= m;
        p >>= 1;
    }
    return res;
}
