/*
	Fill all the ...
	Set the Identity element in the constructor and vector initializer.
	Set the template variable type carefully.
	Set the operations.
	
*/
template<typename T, typename Z>
class Segtree_With_Lazy {
	struct node {
...		T val;
		Z lzy;
	};
	
	// Variables
	int n;
	vector<node> tr;
	T Id;
	Z Id1;

	// Operations
...	inline T op(T a, T b) { return a + b; }
...	inline Z op1(Z a, Z b) { return a + b; }

...	inline node merge(node a, node b) {
		node ans;
		ans.val = op(a.val, b.val);
		ans.lzy = Id1;
		return ans;
	}

	// Private Functions
	void push_down(int nd,int s,int e) {
		if(tr[nd].lzy == Id1) return;
...		tr[nd].val += (e - s + 1) * tr[nd].lzy;
		if(s != e) {
			tr[nd << 1].lzy = op1(tr[nd << 1].lzy, tr[nd].lzy);
			tr[nd << 1 | 1].lzy = op1(tr[nd << 1 | 1].lzy, tr[nd].lzy);
		}
		tr[nd].lzy = Id1;
	}

	void build(int nd, int s, int e, const vector<T> &v) {
		if(s == e) {
...			tr[nd].val = v[s]; tr[nd].lzy = Id1; return;
		}
		int m = (s + e) >> 1;
		build(nd << 1, s, m, v);
		build(nd << 1 | 1, m + 1, e, v);
		tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
	}

	void upd(int l,int r, Z x, int nd, int s, int e) {
		push_down(nd, s, e);
		if(s > r || e < l) return;
		if(l <= s && e <= r) {
			tr[nd].lzy = x;
			push_down(nd, s, e);
			return;
		}
		int m = (s + e) >> 1;
		upd(l, r, x, nd << 1, s, m);
		upd(l, r, x, nd << 1 | 1, m + 1, e);
		tr[nd] = merge(tr[nd << 1], tr[nd << 1 | 1]);
	}

	T query(int l, int r, int nd, int s, int e) {
		if(s > r || e < l) return Id;
		push_down(nd, s, e);
		if(l <= s && e <= r) return tr[nd].val;
		int m = (s + e) >> 1;
		return op(query(l, r, nd << 1, s, m), query(l, r, nd << 1 | 1, m + 1, e));
	}

public:
	// Constructor
...	Segtree_With_Lazy(int nn) : n(nn), Id(0), Id1(0), tr(4 * nn, {0, 0}) {}
	
...	Segtree_With_Lazy(const vector<T> &v) : n((int)v.size() - 1), tr(4 * n), Id(0), Id1(0) {
		build(1, 1, n, v);
	}

	// Functions
	void upd(int l,int r, Z x) {
		if(l > r) return;
		assert(1 <= l && r <= n);
		upd(l, r, x, 1, 1, n);
	}

	T query(int l,int r) {
		if(l > r) return Id;
		assert(1 <= l && r <= n);
		return query(l, r, 1, 1, n);
	}
};
