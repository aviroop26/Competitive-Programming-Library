template<size_t N>
struct DSU {
	int dsu[N], val[N], sz[N];
	vector<int> changes;
 
	DSU() {
		iota(dsu, dsu + N, 0);
		fill(sz, sz + N, 1);
		fill(val, val + N, 0);
	}
 
	int root(int u) {
		while(u != dsu[u]) u = dsu[u];
		return u;
	}
 
	int query(int u) {
		int ans = 0;
		while(u != dsu[u]) {
			ans ^= val[u];
			u = dsu[u];
		}
		return ans;
	}
 
	int connect(int u,int v,int d) {
		int q1 = query(u), q2 = query(v);
		u = root(u), v = root(v);
		int cyc = q1 ^ q2 ^ d;
		if(u == v) {
			changes.pb(0); // no change
			return cyc;
		}
		if(sz[u] > sz[v]) swap(u, v);
		changes.pb(u); // make u root again during rollback
		dsu[u] = v;
		val[u] = cyc;
		sz[v] += sz[u];
		return 0;
	}
 
	void rollback() {
		int u = changes.back();
		changes.pop_back();
		if(!u) return;
		sz[dsu[u]] -= sz[u];
		dsu[u] = u;
		val[u] = 0;
	}
};
