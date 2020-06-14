const int ITER = 300;
const f80 eps = 1e-7;

vector<f80> diff(vector<f80> v) {
    for(int i = 1; i < sz(v); i++) {
        v[i - 1] = v[i] * i;
    }
    v.pop_back();
    return v;
}

f80 f(vector<f80> v, f80 r) {
    f80 p = 1, ans = 0;
    for(f80 c : v) {
        ans += p * c;
        p *= r;
    }
    return ans;
}

vector<f80> calc_roots(vector<f80> v) {
    if(sz(v) <= 1) return {};
    vector<f80> v1 = diff(v);
    f80 r = 1e9;
    for(int k = 1; k <= ITER; k++) {
        r -= f(v, r) / f(v1, r);
    }
    if(f(v, r) > eps) {
        return {};
    }
    else {
        vector<f80> nv;
        nv.resize(sz(v) - 1);
        nv[sz(v) - 2] = v.back();
        for(int i = sz(v) - 3; i >= 0; i--) {
            nv[i] = r * nv[i + 1] + v[i + 1];
        }
        while(!nv.empty() && nv.back() < eps) nv.pop_back();
        auto ans = calc_roots(nv);
        ans.pb(r);
        return ans;
    }
}
