/*
	Fill all the ...
	Set the Identity element.
	Set the template variable type carefully.
*/
template<typename T>
class Segtree_Without_Lazy {
	struct node {
...		T val;
	};
	
	// Variables
	int n;
	vector<node> tr;
	T Id;

	// Operations
	inline T op(T a, T b) {
...		return min(a, b);
	}
	inline node merge(node a, node b) {
...		node ans;
		ans.val = op(a.val, b.val);
		return ans;
	}
	
	// Private Functions
	void build(int nd, int s, int e, const vector<T> &v) {
		if(s == e) {
...			tr[nd].val = v[s]; return;
		}
		int m = (s + e) >> 1;
		build(nd << 1, s, m, v);
		build(nd << 1 | 1, m + 1, e, v);
		tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
	}

	void upd(int idx, T x, int nd, int s, int e) {
		if(s == e) {
...			tr[nd].val = x; return;
		}
		int m = (s + e) >> 1;
		if(idx <= m)
			upd(idx, x, nd << 1, s, m);
		else
			upd(idx, x, nd << 1 | 1, m + 1, e);
		tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
	}

	T query(int l, int r, int nd, int s, int e) {
		if(s > r || e < l) return Id;
		if(l <= s && e <= r) return tr[nd].val;
		int m = (s + e) >> 1;
		return op(query(l, r, nd << 1, s, m), query(l, r, nd << 1 | 1, m + 1, e));
	}

	void print(int l,int r,int nd,int s,int e, vector<T> &v) {
		if(s > r || e < l) return;
		if(s == e) {
			v.push_back(tr[nd].val);
			return;
		}
		int m = (s + e) >> 1;
		print(l, r, nd << 1, s, m, v);
		print(l, r, nd << 1 | 1, m + 1, e, v);
	}

public:
	// Constructor
...	Segtree_Without_Lazy(int nn) : n(nn), Id(INT_MAX), tr(4 * nn, {INT_MAX}) {}
	
...	Segtree_Without_Lazy(const vector<T> &v) : n((int)v.size() - 1), tr(4 * n), Id(INT_MAX) {
		build(1, 1, n, v);
	}

	// Functions
	void upd(int idx, T val) {
		assert(1 <= idx && idx <= n);
		upd(idx, val, 1, 1, n);
	}

	T query(int l,int r) {
		if(l > r) return Id;
		assert(1 <= l && r <= n);
		return query(l, r, 1, 1, n);
	}

	void print(int l,int r) {
		vector<T> v;
		print(l, r, 1, 1, n, v);
		debug(v);
	}
};
