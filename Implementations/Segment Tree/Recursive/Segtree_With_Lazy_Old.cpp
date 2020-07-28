const int N = 2e5 + 5;
int a[N];
struct node {
    int val;
    node() {
        val = 0;
    }
};
node tr[4 * N];
int lz[4 * N];

node merge(node a, node b) {
    node c;
    c.val = a.val + b.val;
    return c;
}

void build(int nd,int s,int e) {
    if(s == e) {
        tr[nd].val = a[s];
        return;
    }
    int m = (s + e) >> 1;
    build(nd << 1, s, m);
    build(nd << 1 | 1, m + 1, e);
    tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
}

void push_down(int nd,int s,int e) {
    if(!lz[nd]) return;
    tr[nd].val += (e - s + 1) * lz[nd];
    if(s != e) {
        lz[nd << 1] += lz[nd];
        lz[nd << 1 | 1] += lz[nd];
    }
    lz[nd] = 0;
}

void upd(int nd,int s,int e,int l,int r,int x) {
    push_down(nd, s, e);
    if(s > r || e < l) {
        return;
    }
    if(l <= s && e <= r) {
        lz[nd] += x;
        push_down(nd, s, e);
        return;
    }
    int m = (s + e) >> 1;
    upd(nd << 1, s, m, l, r, x);
    upd(nd << 1 | 1, m + 1, e, l, r, x);
    tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
}

node query(int nd,int s,int e,int l,int r) {
    if(s > r || e < l) return node();
    push_down(nd, s, e);
    if(l <= s && e <= r) {
        return tr[nd];
    }
    int m = (s + e) >> 1;
    return merge(query(nd << 1, s, m, l, r), query(nd << 1 | 1, m + 1, e, l, r));
}

void solve() {
	int n, q;
    sc(n, q);
    fr(i, 1, n) {
        sc(a[i]);
    }
    build(1, 1, n);
    fr(i, 1, q) {
        int t, l, r;
        sc(t, l, r);
        if(t == 1) {
            int x;
            sc(x);
            upd(1, 1, n, l, r, x);
        }
        else {
            cout << query(1, 1, n, l, r).val << endl;
        }
    }
}
