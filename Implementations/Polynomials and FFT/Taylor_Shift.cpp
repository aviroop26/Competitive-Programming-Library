// Given P(x), compute P(x + t)

vi translate(vi &p, ll t) {
    int n = sz(p);
    p1.resize(n, 0), p2.resize(n, 0);
    ll val = 1;
    fr(i, 0, n - 1) {
        p1[i] = (val * 1LL * ifact[i]) % mod;
        p2[i] = (fact[n - 1 - i] * 1LL * p[n - 1 - i]) % mod;
        val = (val * t) % mod;
    }
    vi res = p1 * p2;
    res.resize(n);
    reverse(all(res));
    fr(i, 0, n - 1)
        res[i] = (res[i] * 1LL * ifact[i]) % mod;
    return res;
}
