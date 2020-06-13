/*
Description:
	Calculates lca between two nodes in a tree in O(1) with O(N*LOGN) preprocessing.
Usage:
	LCA L;
	L.init(n); [1, n]
	L.add(u, v) -> add edges
	L.pre(1) -> preprocess
	L.lca(u, v) -> calculate lca
Test:
	https://www.spoj.com/problems/LCA/
*/

struct LCA {
	vector<vector<int>> g;
	vector<vector<pii>> dp;
	vector<int> depth, st, arr;
	int logn, n;
	void init(int nn) {
		n = nn;
		g.resize(n + 1);
		depth.resize(n + 1, 0);
		st.resize(n + 1, 0);
		arr.eb(0);
	}
	void add(int u,int v) {
		g[u].eb(v);
		g[v].eb(u);
	}
	void dfs(int u,int p) {
		arr.eb(u);
		st[u] = sz(arr) - 1;
		depth[u] = depth[p] + 1;
		for(int v : g[u]) {
			if(v != p) {
				dfs(v, u);
				arr.eb(u);
			}
		}
	}
	void pre(int root) {
		dfs(1, 0);
		logn = bit(sz(arr)) + 1;
		dp.resize(logn);
		dp[0] = {{0, 0}};
		fr(i, 1, sz(arr) - 1) {
			dp[0].eb(depth[arr[i]], arr[i]);
		}
		fr(k, 1, logn - 1) {
			dp[k].eb(0, 0);
			for(int i = 1; i + (1 << k) <= sz(arr); i++) {
				dp[k].eb(min(dp[k - 1][i], dp[k - 1][i + (1 << (k - 1))]));
			}
		}
	}
	int lca(int u,int v) {
		int l = st[u], r = st[v];
		if(l > r) swap(l, r);
		int k = bit(r - l + 1);
		return min(dp[k][l], dp[k][r - (1 << k) + 1]).se;
	}
};
