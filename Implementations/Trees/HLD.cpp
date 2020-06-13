/************ UNTESTED ***********/

template<typename T>
struct HLD {
	vector<vector<T>> g;
	vector<int> par, depth, sz, st, en, chainhead;
	Segtree_Without_Lazy<int> *s;
	int tt, n;

	void dfs1(int u,int p) {
		sz[u] = 1;
		par[u] = p;
		depth[u] = depth[p] + 1;
		for(T v : g[u]) {
			if(v.fi != p) {
				dfs1(v.fi, u);
				sz[u] += sz[v.fi];
			}
		}
	}

	void dfs2(int u,int p, int pw) {
		if(!chainhead[u]) chainhead[u] = u;
		st[u] = ++tt;
		s -> upd(st[u], pw);
		T hc = {-1, -1};
		int S = -1;
		for(T v : g[u]) {
			if(v.fi != p && sz[v.fi] > S) {
				S = sz[v.fi], hc = v;
			}
		}
		if(hc != T{-1, -1}) {
			dfs2(hc.fi, u, hc.se);
		}
		for(T v : g[u]) {
			if(v.fi != p && v.fi != hc.fi) {
				dfs2(v.fi, u, v.se);
			}
		}
		en[u] = tt;
	}
	
	void init(vector<T> _g[], int _n, int root) {
		n = _n;
		par.resize(n + 1, 0);
		depth.resize(n + 1, 0);
		chainhead.resize(n + 1, 0);
		st.resize(n + 1, 0);
		en.resize(n + 1, 0);
		sz.resize(n + 1, 0);
		g.resize(n + 1);
		fr(i, 1, n) g[i] = _g[i];
		s = new Segtree_Without_Lazy<int>(n);
		tt = 0;
		dfs1(root, 0);
		dfs2(root, 0, 0);
	}

	int lca(int u,int v) {
		while(chainhead[u] != chainhead[v]) {
			if(depth[chainhead[u]] < depth[chainhead[v]]) swap(u, v);
			u = par[chainhead[u]];
		}
		if(depth[u] > depth[v]) swap(u, v);
		return u;
	}

	int query(int u,int v) {
		int ans = 0;
		while(chainhead[u] != chainhead[v]) {
			if(depth[chainhead[u]] < depth[chainhead[v]]) swap(u, v);
			ans = max(ans, s -> query(st[chainhead[u]], st[u]));
			u = par[chainhead[u]];
		}
		if(u != v) {
			if(depth[u] < depth[v]) swap(u, v);
			ans = max(ans, s -> query(st[v] + 1, st[u]));
		}
		return ans;
	}

	void update(int idx,int val) {
		s -> upd(st[idx], val);
	}
};
