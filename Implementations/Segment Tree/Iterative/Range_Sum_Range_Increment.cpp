template<typename T, size_t N>
struct segtree {
    array<T, 2 * N> tr, sz;
    array<T, N> d;
    int n, h;
    void init(int _n) {
        n = _n;
        h = sizeof(int) * 8 - __builtin_clz(n);
        fr(i, 0, n - 1)
            sz[n + i] = 1, tr[n + i] = 0;
        rf(i, n - 1, 1)
            sz[i] = sz[i << 1] + sz[i << 1 | 1], tr[i] = 0, d[i] = 0;
    }
    void apply(int p, T value) {
        tr[p] += sz[p] * value;
        if (p < n) d[p] += value;
    }

    void build(int p) {
        while (p > 1) p >>= 1, tr[p] = tr[p << 1] + tr[p << 1 | 1] + sz[p] * d[p];
    }

    void push(int p) {
        for(int s = h; s > 0; --s) {
            int i = p >> s;
            if (d[i] != 0) {
                apply(i << 1, d[i]);
                apply(i << 1 | 1, d[i]);
                d[i] = 0;
            }
        }
    }

    void inc(int l, int r, T value) {
        l += n, r += n + 1;
        int l0 = l, r0 = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, value);
            if (r & 1) apply(--r, value);
        }
        build(l0);
        build(r0 - 1);
    }

    T query(int l, int r) {
        l += n, r += n + 1;
        push(l);
        push(r - 1);
        T res = 0;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = res + tr[l++];
            if (r & 1) res = tr[--r] + res;
        }
        return res;
    }
};

const int N = 1e5 + 5;
segtree<ll, N> st;
