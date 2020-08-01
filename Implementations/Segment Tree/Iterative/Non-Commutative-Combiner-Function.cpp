pair<Mint,int> merge(pair<Mint,int> p1, pair<Mint,int> p2) {
    return {p1.fi + p2.fi * pw[p1.se], p1.se + p2.se};
}

template<class T, size_t MAXN> struct T1 {
    int n;
    T st[MAXN << 1];
    void build(int n) {
        this->n = n;
        for (int i = n - 1; i > 0; i--) {
            st[i] = merge(st[i << 1], st[i << 1 | 1]);
        }
    }
    void upd(int p, int val) {
        st[p += n] = {val, 1};
        for(p >>= 1; p; p >>= 1) {
            st[p] = merge(st[p << 1], st[p << 1 | 1]);
        }
    }
    T query(int l, int r) { // [l, r]
        l += n, r += n + 1;
        T res = {0, 0}, res1 = {0, 0};
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = merge(res, st[l++]);
            if (r & 1) res1 = merge(st[--r], res1);
        }
        return merge(res, res1);
    }
};
