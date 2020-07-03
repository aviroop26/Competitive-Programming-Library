const int N = 1e5 + 5;

Mint fact[N], ifact[N];
template<typename T>
vector<T> shift(const vector<T> &p, Mint t) {
    int n = sz(p);
    vector<T> p1(n, 0), p2(n, 0);
    T val = 1;
    fr(i, 0, n - 1) {
        p1[i] = val * ifact[i];
        p2[i] = fact[n - 1 - i] * p[n - 1 - i];
        val *= t;
    }
    vector<T> res = p1 * p2;
    res.resize(n);
    reverse(all(res));
    fr(i, 0, n - 1) res[i] *= ifact[i];
    return res;
}

void pre() {
    fact[0] = 1;
    fr(i, 1, N - 1)
        fact[i] = i * fact[i - 1];
    ifact[N - 1] = 1 / fact[N - 1];
    rf(i, N - 2, 0)
        ifact[i] = ifact[i + 1] * (i + 1);
}
