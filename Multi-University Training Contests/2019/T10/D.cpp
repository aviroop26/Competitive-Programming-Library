/*
Can be made faster by changing map in suffix automata ...
*/

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

#ifndef LOCAL
string to_string(__int128 x) {
	string s = "";
	bool neg = 0;
	if(x < 0) { s += "-"; neg = 1; x = -x; }
	if(!x) s += '0';
	while(x) {
		int rem = x % 10;
		s += to_string(rem);
		x /= 10;
	}
	reverse(s.begin() + neg, s.end());
	return s;
}
#endif

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

struct state {
    int len, link;
    map<char, int> next;
};
const int MAXLEN = 1e5 + 5;

int cnt[2 * MAXLEN];

state st[MAXLEN * 2];
int sz, last;
void sa_init() {
    st[0].len = 0;
    st[0].link = -1;
    sz++;
    last = 0;
}
void sa_extend(char c, int ck) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
    cnt[cur] += ck;
}

void lift() {
    vector<int> deg(sz, 0);
    queue<int> q;
    fr(i, 1, sz - 1)
        deg[st[i].link]++;
    fr(i, 1, sz - 1) {
        if(!deg[i])
            q.push(i);
    }
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        int p = st[u].link;
        deg[p]--;
        cnt[p] += cnt[u];
        if(!deg[p] && p != 0) q.push(p);
    }
}

template<typename T, size_t B>
struct gaussian_elimination {
	T basis[B];
	int cnt;
    vector<T> in;
    void init() {
        in.clear();
        cnt = 0;
        fill(basis, basis + B, 0);
    }
	void add(T x) {
        T cp = x;
		for(int i = B - 1; i >= 0; i--) {
			if((x >> i) & 1) {
				if(!basis[i]) {
					basis[i] = x;
                    in.pb(cp);
					cnt++;
					return;
				}
				else {
					x ^= basis[i];
				}
			}
		}
	}
	bool chk(T x) {
		for(int i = B - 1; i >= 0; i--) {
			if((x >> i) & 1) {
				if(!basis[i]) {
					return 0;
				}
				else {
					x ^= basis[i];
				}
			}
		}
		return 1;
	}
};

const int N = 2e5 + 5;
const int LOGN = 18;
ll w[N];
int dp[LOGN][N];
int eq_st[N];
__int128 ans[N];

vector<int> g[N];

bool vis[N];

gaussian_elimination<ll, 60> b[N];

void dfs(int u, ll x) {
    if(!b[u].chk(x)) {
        ans[u] += x;
        b[u].add(x);
    }
    else return;
    for(auto v : g[u]) {
        dfs(v, x);
    }
}

void solve() {
    while(sz > 0) {
        sz--;
        st[sz].next.clear();
    }
    memset(cnt, 0, sizeof cnt);
    memset(vis, 0, sizeof vis);
	int n;
    sc(n);
    string s;
    sc(s);
    reverse(all(s));
    fr(i, 1, n)
        sc(w[i]);
    sa_init();
    fr(i, 0, sz(s) - 1) {
        sa_extend(s[i], 1);
        eq_st[i + 1] = last;
    }
    lift();
    fr(i, 0, sz - 1)
        g[i].clear();
    fr(i, 0, sz - 1) {
        ans[i] = 0;
        b[i].init();
        for(auto x : st[i].next) {
            g[x.se].pb(i);
        }
    }
    vector<pair<ll, int>> v;
    fr(i, 1, sz - 1) {
        v.eb(w[cnt[i]], i);
    }
    sort(all(v));
    reverse(all(v));
    for(auto x : v) {
        dfs(x.se, x.fi);
    }
    fr(i, 1, sz - 1)
        dp[0][i] = st[i].link;
    fr(k, 1, LOGN - 1)
        fr(i, 1, sz - 1)
            dp[k][i] = dp[k - 1][dp[k - 1][i]];
    int m;
    sc(m);
    fr(i, 1, m) {
        int l, r;
        sc(l, r);
        l = n - l + 1;
        r = n - r + 1;
        swap(l, r);
        int u = eq_st[r];
        for(int k = LOGN - 1; k >= 0; k--) {
            if(st[dp[k][u]].len >= r - l + 1)
                u = dp[k][u];
        }
        cout << to_string(ans[u]) << endl;
    }
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
