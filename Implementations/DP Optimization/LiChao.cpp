struct LiChao_max
{
    struct line
    {
        ll a, b;
        line() { a = 0; b = 0; }
        line(ll _a, ll _b) { a = _a; b = _b; }
        ll eval(ll x) { return a * x + b; }
    };
    struct node
    {
        node *l, *r; line f;
        node() { f = line(); l = NULL; r = NULL; }
        node(ll a, ll b) { f = line(a, b); l = NULL; r = NULL; }
        node(line v) { f = v; l = NULL; r = NULL; }
    };
    typedef node* pnode;
    pnode root; ll sz;
    void init(ll _sz) { sz = _sz + 1; root = NULL; }
    void add_line(ll a, ll b) { line v = line(a, b); insert(v, -sz, sz, root); }
    ll query(ll x) { return query(x, -sz, sz, root); }
    void insert(line &v, ll l, ll r, pnode &nd)
    {
        if(!nd) { nd = new node(v); return; }
        ll trl = nd->f.eval(l), trr = nd->f.eval(r);
        ll vl = v.eval(l), vr = v.eval(r);
        if(trl >= vl && trr >= vr) return;
        if(trl < vl && trr < vr) { nd->f = v; return; }
        ll mid = (l + r) >> 1;
        if(trl < vl) swap(nd->f, v);
        if(nd->f.eval(mid) > v.eval(mid)) insert(v, mid + 1, r, nd->r);
        else swap(nd->f, v), insert(v, l, mid, nd->l);
    }
    ll query(ll x, ll l, ll r, pnode &nd)
    {
        if(!nd) return -inf;
        if(l == r) return nd->f.eval(x);
        ll mid = (l + r) >> 1;
        if(mid >= x) return max(nd->f.eval(x), query(x, l, mid, nd->l));
        return max(nd->f.eval(x), query(x, mid + 1, r, nd->r));
    }
};
