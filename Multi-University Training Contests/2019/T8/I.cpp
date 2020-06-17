#include "bits/stdc++.h"
using namespace std;
#ifndef LOCAL
#define endl '\n'
#endif

#define fr(i, a, b) for(int i = a; i <= b; i++)
#define rf(i, a, b) for(int i = a; i >= b; i--)
#define pf push_front
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define sz(x) (int)x.size()
#define lb lower_bound
#define ub upper_bound

typedef long long ll;
typedef long double f80;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;

int pct(int x) { return __builtin_popcount(x); }
int pct(ll x) { return __builtin_popcountll(x); }
int bt(int x) { return 31 - __builtin_clz(x); } // floor(log2(x))
int bt(ll x) { return 63 - __builtin_clzll(x); } // floor(log2(x))
int cdiv(int a, int b) { return a / b + !(a < 0 || a % b == 0); }
ll cdiv(ll a, ll b) { return a / b + !(a < 0 || a % b == 0); }
int nxt_C(int x) { int c = x & -x, r = x + c; return (((r ^ x) >> 2) / c) | r; }
ll nxt_C(ll x) { ll c = x & -x, r = x + c; return (((r ^ x) >> 2) / c) | r; }

vector<int> get_bits(int mask) {
	vector<int> bb;
	while(mask) { int b = bt(mask); bb.pb(b); mask ^= (1 << b); }
	reverse(all(bb));
	return bb;
}

int get_mask(vector<int> v) {
	int mask = 0;
	for(int x : v) { mask ^= (1 << x); }
	return mask;
}

template<typename T>
void uniq(vector<T> &v) { sort(all(v)); v.resize(unique(all(v)) - v.begin()); }

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rand(ll l, ll r){
	uniform_int_distribution<ll> uid(l, r);
	return uid(rng);
}

void sc() {}

template <typename Head, typename... Tail>
void sc(Head &H, Tail &... T) { cin >> H; sc(T...); }

#ifdef LOCAL
#define debug(...) cerr << "[L:" << __LINE__ << "][" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(...) 42
#endif

// #ifndef LOCAL
// string to_string(__int128 x) {
// 	string s = "";
// 	bool neg = 0;
// 	if(x < 0) { s += "-"; neg = 1; x = -x; }
// 	if(!x) s += '0';
// 	while(x) {
// 		int rem = x % 10;
// 		s += to_string(rem);
// 		x /= 10;
// 	}
// 	reverse(s.begin() + neg, s.end());
// 	return s;
// }
// #endif

const int mod = 1e9 + 7; // 998244353;

int pwr(int a,ll b) {
	int ans = 1;
	while(b) {
		if(b & 1) ans = (ans * 1LL * a) % mod;
		a = (a * 1LL * a) % mod;
		b >>= 1;
	}
	return ans;
}

/*
	Lookout for overflows!!
	Check array sizes!!
	Clear before test cases!!
	Use the correct modulo!!
	Check for corner cases!!
	Are you forgetting something?!
	Read problem statement carefully!!!
*/

ll p[2][4];
int dx[4] = {1, 1, -1, -1};
int dy[4] = {1, -1, 1, -1};

const int N = 1005;
vector<int> g[N];
bool vis[N];

void dfs(int u) {
    vis[u] = 1;
    for(int v : g[u]) {
        if(!vis[v]) {
            dfs(v);
        }
    }
}

void solve() {
    vector<int> xs, ys;
	fr(i, 0, 1) {
        fr(j, 0, 3) {
            sc(p[i][j]);
        }
        swap(p[i][1], p[i][2]);
        fr(j, 0, 3) {
            if(j <= 1) xs.pb(p[i][j]);
            else ys.pb(p[i][j]);
        }
    }
    vector<pair<f80, f80>> v;
    uniq(xs);
    uniq(ys);
    fr(i, 0, sz(xs) - 1) {
        fr(j, 0, sz(ys) - 1) {
            fr(k, 0, 3) {
                f80 xx = xs[i] + 0.5 * dx[k];
                f80 yy = ys[j] + 0.5 * dy[k];
                bool in = 0;
                fr(r, 0, 1) {
                    if(p[r][0] < xx && xx < p[r][1] && p[r][2] < yy && yy < p[r][3]) {
                        in = 1;
                    }
                }
                if(in)
                    v.pb({xx, yy});
            }
        }
    }
    uniq(v);
    debug(v);
    fr(i, 0, sz(v) - 1) {
        vis[i] = 0;
    }
    fr(i, 0, sz(v) - 1) {
        fr(j, i + 1, sz(v) - 1) {
            if(v[i].se == v[j].se) {
                bool e = 1;
                fr(k, 0, 1) {
                    fr(l, 0, 1) {
                        auto p1 = v[i], p2 = v[j];
                        int y1 = p[k][2], y2 = p[k][3];
                        f80 x = p[k][l];
                        if(y1 > y2) swap(y1, y2);
                        if(min(p1.fi, p2.fi) < x && x < max(p1.fi, p2.fi)) {
                            if(y1 < v[i].se && v[i].se < y2) {
                                e = 0;
                            }
                        }
                    }
                }
                if(e) {
                    debug(i, j);
                    g[i].pb(j);
                    g[j].pb(i);
                }
            }
            else if(v[i].fi == v[j].fi) {
                bool e = 1;
                fr(k, 0, 1) {
                    fr(l, 2, 3) {
                        auto p1 = v[i], p2 = v[j];
                        int x1 = p[k][0], x2 = p[k][1];
                        f80 y = p[k][l];
                        if(x1 > x2) swap(x1, x2);
                        if(min(p1.se, p2.se) < y && y < max(p1.se, p2.se)) {
                            if(x1 < v[i].fi && v[i].fi < x2) {
                                e = 0;
                            }
                        }
                    }
                }
                if(e) {
                    debug(i, j);
                    g[i].pb(j);
                    g[j].pb(i);
                }
            }
        }
    }
    int ans = 1;
    fr(i, 0, sz(v) - 1) {
        if(!vis[i]) {
            dfs(i);
            ans++;
        }
        g[i].clear();
    }
    cout << ans << endl;
}

int main() {
	ios :: sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int t = 1;
	cin >> t;
	for(int tt = 1; tt <= t; tt++) {
		solve();
	}
	return 0;
}
