// returns minimal x such that a*x+b \in [l; r] % mod
// l and r don't have to be in [0, mod) (see first couple of lines for understanding)
// O(log(mod))
ll first_in_range(ll a, ll b, ll l, ll r, ll mod, ll no_solution_return = 4e18) {
    r -= b; l -= b;
    ll dif = r - l;
    if (dif >= mod) return 0;
    if (dif < 0) return no_solution_return;
    l = (l % mod + mod) % mod;
    r = (r % mod + mod) % mod;

    ll g = gcd(a, mod);
    const ll inf = 4e18;

    auto get_inv = [&](ll a, ll mod) -> ll {
        ll b = mod, x = 0, y = 1;
        while (a != 0) {
            ll k = b / a;
            b -= k * a; x -= k * y;
            swap(a, b); swap(x, y);
        }
        x %= mod;
        if (x < 0) x += mod;
        return x;
    };

    auto solve_internal = [&](ll mod, ll l, ll r, ll a) -> ll {
        if (a == 0) return (l == 0 ? 0 : inf);
        a /= g;
        mod /= g;
        r /= g;
        l = (l + g - 1) / g;
        if (l > r) return inf;
        if (mod == 1) return 0;

        a = get_inv(a, mod);
        ll b = l * a % mod;
        ll k = r - l;

        ll b0 = b;
        ll mod0 = mod;
        ll ainv = get_inv(a, mod);

        auto get_steps = [&](ll t, ll ia, ll b, ll m) -> ll {
            return (t - b + m) % m * ia % m;
        };

        while (true) {
            if (a == 0) return b;
            if (a < mod - a) {
                ll b1 = (b < a ? b : ((b - mod) % a + a) % a);

                if (get_steps(b1, ainv, b0, mod0) > k) return b;

                ll a1 = ((-mod) % a + a) % a;
                ll m1 = a;

                a = a1;
                b = b1;
                mod = m1;
            } else {
                ll a1 = a % (mod - a);
                ll b1 = b % (mod - a);
                ll m1 = mod - a;

                ll steps = get_steps(b1, ainv, b0, mod0);
                if (steps > k) {
                    ll div = steps - get_steps(b1 + m1, ainv, b0, mod0);
                    return b1 + m1 * ((steps - k + div - 1) / div);
                }

                a = a1;
                b = b1;
                mod = m1;
            }
        }
    };

    ll res;
    if (l <= r) {
        res = solve_internal(mod, l, r, a);
    } else {
        res = min(solve_internal(mod, 0, r, a), solve_internal(mod, l, mod - 1, a));
    }
    if (res == inf) res = no_solution_return;
    return res;
}
