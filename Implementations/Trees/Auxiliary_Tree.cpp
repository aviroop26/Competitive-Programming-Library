/*
Description:
	Calculates the auxiliary tree of a subset of nodes in the tree in O(M * LOGM) where
	M is the size of the subset. Returns a pair<A, B> where
	A -> nodes that are present in the aux tree.
	B -> their corresponding parent (or 0 if no parent)
Usage:
	AuxTree T;
	T.init(n)
	T.add(u, v) -> adds edge (u, v)
	T.pre(1) -> preprocesses tree
	T.get({1, 3, 4})
Test:
	https://codeforces.com/contest/613/submission/78842534
*/

struct AuxTree {
	int n, tt;
	LCA L;
	vector<int> st, en;
	void init(int nn) {
		n = nn;
		L.init(n);
		st.resize(n + 1, 0);
		en.resize(n + 1, 0);
		tt = 0;
	}
	void add(int u,int v) {
		L.add(u, v);
	}
	void pre(int root) {
		L.pre(root);
		dfs(root, 0);
	}
	void dfs(int u,int p) {
		st[u] = ++tt;
		for(int v : L.g[u]) {
			if(v != p) {
				dfs(v, u);
			}
		}
		en[u] = tt;
	}
	inline bool in(int u,int v) { // whether u is an ancestor of v
		return st[u] <= st[v] && en[v] <= en[u];
	}
	pair<vector<int>, vector<int>> get(vector<int> nodes) {
		auto cmp = [&](const int &a, const int &b) {
			return L.st[a] < L.st[b];
		};
		sort(all(nodes), cmp);
		int nn = sz(nodes);
		fr(i, 0, nn - 2) {
			nodes.pb(L.lca(nodes[i], nodes[i + 1]));
		}
		sort(all(nodes), cmp);
		nodes.resize(unique(all(nodes)) - nodes.begin());
		vector<int> stk;
		vector<int> par;
		for(int u : nodes) {
			while(!stk.empty() && !in(stk.back(), u)) stk.pop_back();
			int p = ((stk.empty()) ? 0 : stk.back());
			par.eb(p);
			stk.eb(u);
		}
		return {nodes, par};
	}
};
