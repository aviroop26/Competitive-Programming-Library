// UNTESTED //

#define vi vector<int>
const int mod = 998244353;
const int root = 3;
const int MAX = 18;

ll iv[(1 << MAX) + 1];
struct ntt {
    ll rt[1 << MAX];
    void pre(int n) {
        rt[n / 2] = 1;
        rt[n / 2 + 1] = pwr(root, (mod - 1) / n);
        for(int i = n / 2 + 2; i < n; ++i)
            rt[i] = rt[i - 1] * rt[n / 2 + 1] % mod;
        for(int i = n / 2; --i; )
            rt[i] = rt[i << 1];
        iv[1] = 1;
        for(int i = 2; i <= (1 << MAX); ++i)
            iv[i] = mod - ((mod / i) * iv[mod % i]) % mod;
    }
    void go(vi &a) {
        int n = a.size();
        for(int i = 0, j = 0; i < n; ++i) {
            if(i > j)
                swap(a[i], a[j]);
            for(int k = n / 2; (j ^= k) < k; k >>= 1);
        }
        for(int st = 1; (st << 1) <= n; st <<= 1) {
            for(int i = 0; i < n; i += (st << 1)) {
                for(int j = i; j < i + st; ++j) {
                    int z = (rt[j - i + st] * a[j + st]) % mod;
                    a[j + st] = a[j] - z;
                    if(a[j + st] < 0)
                        a[j + st] += mod;
                    a[j] += z;
                    if(a[j] >= mod)
                        a[j] -= mod;
                }
            }
        }
    }
} nt;
vi mult(vi v, vi w) {
    int s = sz(v) + sz(w) - 1, t = 1;
    while(t < s)
        t <<= 1;
    v.resize(t, 0), w.resize(t, 0);
    nt.go(v). nt.go(w);
    ll I = iv[t];
    for(int i = 0; i < t; ++i)
        v[i] = (((v[i] * 1LL * w[i]) % mod) * I) % mod;
    reverse(v.begin() + 1, v.end());
    nt.go(v);
    while(!v.empty() && !v.back())
        v.pop_back();
    return v;
}

int main(){
    nt.pre(1 << MAX);
    return 0;
}
