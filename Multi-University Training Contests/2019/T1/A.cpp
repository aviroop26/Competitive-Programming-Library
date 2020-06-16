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
#define rsz resize()
#define lb lower_bound
#define ub upper_bound
#define br cout << endl

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
template<typename T>
void leftShift(vector<T> &v, ll k) { k %= sz(v); if(k < 0) k += sz(v); rotate(v.begin(), v.begin() + k, v.end()); }
template<typename T>
void rightShift(vector<T> &v, ll k) { leftShift(v, sz(v) - k); }

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

const int mod = 998244353;

int pwr(int a,int b) {
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

const int N = 105;
int dp[2][N][N][N];

int inter[N][5], inter1[N][5];

inline bool in(int a,int b,int c) {
	if(!c) return 1;
	return b < c && c <= a;
}

void solve() {
	memset(dp, 0, sizeof dp);
	memset(inter, 0, sizeof inter);
	memset(inter1, 0, sizeof inter1);
	dp[0][0][0][0] = 1;
	int n, m;
	sc(n, m);
	fr(i, 1, m) {
		int l, r, x;
		sc(l, r, x);
		inter[r][x] = max(inter[r][x], l);
		if(!inter1[r][x]) inter1[r][x] = l;
		else inter1[r][x] = min(inter1[r][x], l);
	}
	fr(i, 0, n) {
		int i1 = i & 1, i2 = i1 ^ 1;
		fr(j, 0, i) {
			fr(k, 0, j) {
				fr(l, 0, k) {
					dp[i2][j][k][l] = 0;
				}
			}
		}
		fr(j, 0, i) {
			fr(k, 0, j) {
				fr(l, 0, k) {
					if(!dp[i1][j][k][l]) continue;
					int val = dp[i1][j][k][l];
					if(in(i, j, inter[i][1]) && in(j, k, inter[i][2]) && in(k, l, inter[i][3]) && in(l, 0, inter[i][4]) && in(i, j, inter1[i][1]) && in(j, k, inter1[i][2]) && in(k, l, inter1[i][3]) && in(l, 0, inter1[i][4])) {
						dp[i2][j][k][l] += val;
						if(dp[i2][j][k][l] >= mod) dp[i2][j][k][l] -= mod;
						dp[i2][i][k][l] += val;
						if(dp[i2][i][k][l] >= mod) dp[i2][i][k][l] -= mod;
						dp[i2][i][j][l] += val;
						if(dp[i2][i][j][l] >= mod) dp[i2][i][j][l] -= mod;
						dp[i2][i][j][k] += val;
						if(dp[i2][i][j][k] >= mod) dp[i2][i][j][k] -= mod;
					}
					else {
						dp[i1][j][k][l] = 0;
					}
				}
			}
		}
	}
	int ans = 0;
	fr(j, 0, n) {
		fr(k, 0, j) {
			fr(l, 0, k) {
				ans += dp[n & 1][j][k][l];
				if(ans >= mod) ans -= mod;
			}
		}
	}
	cout << ans << endl;
}

signed main() {
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
