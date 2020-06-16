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
int bit(int x) { return 31 - __builtin_clz(x); } // floor(log2(x))
int bit(ll x) { return 63 - __builtin_clzll(x); } // floor(log2(x))
int cdiv(int a, int b) { return a / b + !(a < 0 || a % b == 0); }
ll cdiv(ll a, ll b) { return a / b + !(a < 0 || a % b == 0); }
int nxt_C(int x) { int c = x & -x, r = x + c; return (((r ^ x) >> 2) / c) | r; }
ll nxt_C(ll x) { ll c = x & -x, r = x + c; return (((r ^ x) >> 2) / c) | r; }

vector<int> get_bits(int mask) {
	vector<int> bb;
	while(mask) { int b = bit(mask); bb.pb(b); mask ^= (1 << b); }
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

const int N = 1e7 + 5;
bool pr[N];
int f[N], I[N], z[N];

vector<int> get_div(int n) {
  vector<int> v;
  for(int i = 1; i * i <= n; i++) {
    if(n % i == 0) {
      v.pb(i);
      if(i * i != n)
        v.pb(n / i);
    }
  }
  return v;
}

int fast(int i,__int128 r) {
  vector<int> divi = get_div(i);
	int ans = 0;
	for(int d : divi) {
		ans += (f[d] * 1LL * ((r / d) % mod)) % mod;
    if(ans >= mod) ans -= mod;
	}
	return ans;
}

int fast(int i,__int128 l,__int128 r) {
	return (fast(i, r) - fast(i, l - 1) + mod) % mod;
}

void pre() {
  I[0] = I[1] = 1;
  fr(i, 2, N - 1) {
    I[i] = (I[mod % i] * 1LL * (mod - mod / i)) % mod;
  }
	fr(i, 1, N - 1) {
    z[i] = 1, f[i] = i;
	}
	for(int i = 2; i < N; i++) {
		if(!pr[i]) {
			for(int j = i; j < N; j += i) {
				pr[j] = 1;
        f[j] -= f[j] / i;
        int k = 0, jj = j;
        while(jj % i == 0) {
          jj /= i, k++;
        }
        z[j] = (z[j] * 1LL * ((k * 1LL * (mod + 1 - I[i]) + 1) % mod)) % mod;
			}
		}
	}
  fr(i, 1, N - 1) {
    z[i] = ((z[i] * 1LL * ((3 * i * 1LL * i + 3 * i) % mod)) + i) % mod;
    z[i] += z[i - 1];
    if(z[i] >= mod) z[i] -= mod;
  }
}

template <class T>
void read(T &x) {
  static char ch;static bool neg;
  for(ch=neg=0;ch<'0' || '9'<ch;neg|=ch=='-',ch=getchar());
  for(x=0;'0'<=ch && ch<='9';(x*=10)+=ch-'0',ch=getchar());
  x=neg?-x:x;
}

void solve() {
	pre();
  int t;
  read(t);
  while(t--) {
    __int128 n;
    read(n);
    int l = 1, r = N - 1;
    while(r - l > 1) {
      int m = (l + r) >> 1;
      __int128 val = m;
      val = val * val * val;
      if(val <= n)
        l = m;
      else
        r = m;
    }
    int nn = l;
    int ans = z[nn - 1];
    __int128 n1 = nn;
    n1 = n1 * n1 * n1;
    ans += fast(nn, n1, n);
    if(ans >= mod) ans -= mod;
    cout << ans << endl;
  }
}

signed main() {
	ios :: sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int t = 1;
	// cin >> t;
	for(int tt = 1; tt <= t; tt++) {
		solve();
	}
	return 0;
}
