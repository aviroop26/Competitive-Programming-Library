template<class T, size_t MAXN> struct RMQ {
    int n;
    T st[MAXN << 1];
    void build(int n) {
        this->n = n;
        for (int i = n - 1; i > 0; i--) {
            st[i] = max(st[i << 1], st[i << 1 | 1]);
        }
    }
    void upd(int p, int val) {
        st[p += n] = val;
        for(; p > 1; p >>= 1) {
            st[p >> 1] = max(st[p], st[p ^ 1]);
        }
    }
    T query(int l, int r) { // [l, r]
        l += n, r += n + 1;
        T res = -1e9;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = max(res, st[l++]);
            if (r & 1) res = max(res, st[--r]);
        }
        return res;
    }
};

const int N = 1e5 + 5;

void solve() {
    int n = 10000;
    RMQ<int, N> seg;
    fr(i, 0, n - 1) {
        a[i] = rand(0, 1e9);
        seg.st[n + i] = a[i];
    }
    seg.build(n);
}
