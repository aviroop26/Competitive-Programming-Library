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

/*
 *  Highest label push-relabel algorithm in O(V^2 E^0.5)
 *  Uses various heuristics
 *  Code by dacin21
 */

template<typename cap_t, typename excess_t, bool global_relabeling = true, bool min_cut_only = false, bool shuffle_edges = false>
class Push_Relabel{
public:
    struct Edge{
        int to, rev;
        cap_t f;
    };

    Push_Relabel(int n_):n(n_), m(0){}

    void add_edge(int u, int v, cap_t c, cap_t c_rev = 0){
        edge_pool.emplace_back(u, v, c, c_rev);
        ++m;
    }
    excess_t max_flow(int s_, int t_){
        s = s_; t = t_;
        run_pr();
        return excess[t]-1;
    }

private:
    void compile_g(){
        g_pos.resize(n+1);
        if(shuffle_edges) random_shuffle(edge_pool.begin(), edge_pool.end());
        for(auto &e:edge_pool){
            ++g_pos[get<0>(e)];
            ++g_pos[get<1>(e)];
        }
        for(int i=0;i<n;++i){
            g_pos[i+1]+=g_pos[i];
        }
        g.resize(g_pos.back());
        for(auto &e:edge_pool){
            int u, v; cap_t c, c_rev;
            tie(u, v, c, c_rev) = e;
            const int i = --g_pos[u], j = --g_pos[v];
            g[i] = Edge{v, j, c};
            g[j] = Edge{u, i, c_rev};
        }
    }
    void global_relabel(){
        q.reserve(n);
        fill(h.begin(), h.end(), n);
        fill(h_cnt.begin(), h_cnt.end(), 0);
        h_cnt[n] = 1;
        q.push_back(t);
        h[t] = 0;
        for(auto &e:buck) e.clear();
        for(auto &e:buck_all) e.clear();
        for(auto it = q.begin();it<q.end();++it){
            const int u = *it;
            if(u != t && excess[u]){
                hi = h[u];
                buck[h[u]].push_back(u);
            }
            if(u != t) buck_all[h[u]].push_back(u);
            ++h_cnt[h[u]];
            for(int i = g_pos[u],i_end = g_pos[u+1];i < i_end;++i){
                Edge const&e = g[i];
                if(g[e.rev].f && h[e.to] == n){
                    h[e.to] = h[u]+1;
                    q.push_back(e.to);
                }
            }
        }
        hi_all = h[q.back()];
        assert(h[s] == n);
        q.clear();
    }
    void push(int u, Edge &e, excess_t f){
        if(!excess[e.to]){
            buck[h[e.to]].push_back(e.to);
        }
        Edge&back = g[e.rev];
        e.f-=f;
        back.f+=f;
        excess[e.to]+=f;
        excess[u]-=f;
    }
    void init_pr(){
        compile_g();
        cur.assign(n, 0);
        copy(g_pos.begin(), prev(g_pos.end()), cur.begin());
        h.resize(n);
        excess.assign(n, 0);
        buck.resize(2*n);
        buck_all.resize(n+1);
        h_cnt.assign(2*n, 0);
        h[s] = n;
        h_cnt[n] = 1;
        h_cnt[0] = n-1;
        excess[t] = 1;
    }
    void run_pr(){
        init_pr();
        for(int i = g_pos[s],i_end = g_pos[s+1];i < i_end;++i){
            push(s, g[i], g[i].f);
        }
        hi = hi_all = 0;
        if(global_relabeling) global_relabel();
        if(!buck[hi].empty())
        for(;hi>=0;){
            int u = buck[hi].back(); buck[hi].pop_back();
            int u_cur = cur[u];
            //discharge
            if(!min_cut_only || h[u] < n)
            while(excess[u] > 0){
                if(__builtin_expect(u_cur == g_pos[u+1], false)){
                    int new_h = 1e9;
                    for(int i = g_pos[u],i_end = g_pos[u+1];i < i_end;++i){
                        auto const&e = g[i];
                        if(e.f && new_h > h[e.to]+1){
                            new_h = h[e.to]+1;
                            u_cur = i;
                        }
                    }
                    ++h_cnt[new_h];
                    h[u] = new_h;
                    if(__builtin_expect(!--h_cnt[hi] && hi < n, false)){
                        // gap relabel
                        for(int j = hi;j <= hi_all;++j){
                            for(auto &f:buck_all[j]) if(!min_cut_only || h[f] < n){
                                --h_cnt[h[f]];
                                h[f] = n+1;
                            }
                            buck_all[j].clear();
                        }
                    }
                    hi = h[u];
                } else {
                    Edge &e_cur = g[u_cur];
                    if(e_cur.f && h[u] == h[e_cur.to]+1){
                        push(u, e_cur, min<excess_t>(excess[u], e_cur.f));
                    } else ++u_cur;
                }
            }
            if(h[u] < n) {
                hi_all = max(hi_all, h[u]);
                buck_all[h[u]].push_back(u);
            }
            cur[u] = u_cur;
            while(hi>=0 && buck[hi].empty()) --hi;
        }
    }

    int n, m, s, t, hi, hi_all;
    vector<tuple<int, int, cap_t, cap_t> > edge_pool;
    vector<int> g_pos;
    vector<Edge> g;
    vector<int> q, cur, h, h_cnt;
    vector<excess_t> excess;
    vector<vector<int> > buck, buck_all;
};

const int N = 10005;
vector<pii> g[N], g1[N];

ll dist[N], dist1[N];
int n, m;

void go(int u, ll dist[], vector<pii> g[]) {
    fr(i, 1, n) 
        dist[i] = 1e18;
    dist[u] = 0;
    set<pll> s = {{0, u}};
    while(!s.empty()) {
        pll u = *s.begin();
        s.erase(s.begin());
        if(u.fi > dist[u.se]) continue;
        for(pii v : g[u.se]) {
            if(dist[v.fi] > u.fi + v.se) {
                dist[v.fi] = u.fi + v.se;
                s.emplace(dist[v.fi], v.fi);
            }
        }
    }
}


void solve() {
    sc(n, m);
    fr(i, 1, n) {
        g[i].clear();
        g1[i].clear();
    }
    fr(i, 1, m) {
        int x, y, c;
        sc(x, y, c);
        g[x].eb(y, c);
        g1[y].eb(x, c);
    }
    go(1, dist, g);
    go(n, dist1, g1);
    Push_Relabel<ll, ll, true, true> mf(n + 2);
    fr(i, 1, n) {
        for(auto v : g[i]) {
            if(dist1[v.fi] + dist[i] + v.se == dist[n]) {
                mf.add_edge(i, v.fi, v.se);
            }
        }
    }
    cout << mf.max_flow(1, n) << endl;
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
