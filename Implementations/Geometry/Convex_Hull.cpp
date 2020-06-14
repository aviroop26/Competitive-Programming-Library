Q: http://usaco.org/index.php?page=viewproblem2&cpid=382

ll cross(pll a, pll b) {
    return a.fi * b.se - a.se * b.fi;
}

pll sub(pll a, pll b) {
    return {a.fi - b.fi, a.se - b.se};
}

vector<pll> up, down;

void ConvexHull(vector<pll> &v) {
    up.clear();
    down.clear();
    sort(all(v));
    v.resize(unique(all(v)) - v.begin());
    if(sz(v) == 1) return;
    pll a = v[0], b = v.back();
    up.pb(a);
    down.pb(a);
    fr(i, 1, sz(v) - 1) {
        ll c = cross(sub(v[i], a), sub(b, v[i]));
        if((i == sz(v) - 1) || (c < 0)) {
            while(sz(up) >= 2 && cross(sub(up.back(), up[sz(up) - 2]), sub(v[i], up.back())) >= 0) {
                up.pop_back();
            }
            up.pb(v[i]);
        }
        if((i == sz(v) - 1) || (c > 0)) {
            while(sz(down) >= 2 && cross(sub(down.back(), down[sz(down) - 2]), sub(v[i], down.back())) <= 0) {
                down.pop_back();
            }
            down.pb(v[i]);
        }
    }
    v = down;
    for(int i = sz(up) - 2; i > 0; i--) {
        v.pb(up[i]);
    }
}

bool inside(pll x) {
    auto it = upper_bound(all(up), x);
    int idx = it - up.begin();
    if(idx == 0) return 0;
    if(idx == sz(up)) return 0;
    pll a = up[idx - 1], b = up[idx];
    if(cross(sub(x, a), sub(b, x)) < 0) return 0;
    it = upper_bound(all(down), x);
    idx = it - down.begin();
    if(idx == 0) return 0;
    if(idx == sz(down)) return 0;
    a = down[idx - 1], b = down[idx];
    if(cross(sub(x, a), sub(b, x)) > 0) return 0;
    return 1;
}
