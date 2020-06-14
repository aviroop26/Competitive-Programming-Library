// NOTE : add lines in increasing order of slope
#define sz(c)     (int)c.size()
#define EPS   1E-10
struct line
{
  ll m, c;
  line(ll _M = 0, ll _C = 0) : m(_M), c(_C) {}
};
struct cht
{
    vector<line> lines;
    cht() { lines.clear(); }
    f80 get_x(line l1, line l2)
    { return (f80(1) * l1.c - l2.c) / (f80(1) * l2.m - l1.m); }
    bool bad(line l1, line l2, line l3)
    {
        f80 x12 = get_x(l1,l2);
        f80 x13 = get_x(l1,l3);
        return (x12 >= x13);
    }
    void add_line(ll M, ll C)
    {
        line l(M,C);
        while(sz(lines) >= 2 && bad(lines[sz(lines) - 2], lines[sz(lines) - 1], l))
        lines.pop_back();
        lines.pb(l);
    }
    ll eval(line L, ll x) { return (x * L.m + L.c); }
    ll getMin(ll x)
    {
        int lo = 0, hi = sz(lines);
        assert(lo != hi);
        while(hi - lo > 1)
        {
            int mid = (lo + hi) >> 1;
            ll v1 = eval(lines[mid - 1], x);
            ll v2 = eval(lines[mid], x);
            if(v1 < v2) hi = mid;
            else lo = mid;
        }
        return eval(lines[lo], x);
    }
}dp;
