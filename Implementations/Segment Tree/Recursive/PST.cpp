struct node{
    int val, lc, rc;
};
const int NN = 5e7 + 5;
node tr[NN];
int SZ = 1;

template<typename T, typename Z>
struct Persistent_Segtree_Point_Updates {
	T st, en, tmx;
	vector<int> version;

	inline void newTime() {
		tmx++;
		version.eb(0);
	}

	void upd(int &cur, int prev, T s, T e, T idx, Z val) {
		if(!cur) {
            cur = SZ++;
			tr[cur].val = tr[prev].val;
		}
		if(s == e) { tr[cur].val += val; return; }
		T m = (s + e) >> 1;
		if(idx <= m){
			upd(tr[cur].lc, tr[prev].lc, s, m, idx, val);
			if(!tr[cur].rc) tr[cur].rc = tr[prev].rc;
		}
		else{
			upd(tr[cur].rc, tr[prev].rc, m + 1, e, idx, val);
			if(!tr[cur].lc) tr[cur].lc = tr[prev].lc;
		}
		tr[cur].val = tr[tr[cur].lc].val + tr[tr[cur].rc].val;
	}

	Z query(int cur, T s, T e, T l, T r) {
		if(s > r || e < l) return 0;
		if(!cur) return 0;
		if(l <= s && e <= r) return tr[cur].val;
		T m = (s + e) >> 1;
		return query(tr[cur].lc, s, m, l, r) + query(tr[cur].rc, m + 1, e, l, r);
	}

    void clear() {
        version.clear();
    }

	void init(T _st,T _en) {
		st = _st, en = _en, tmx = -1;
        newTime();
	}

	void upd(T idx, Z val) {
		newTime();
		upd(version[tmx], version[tmx - 1], st, en, idx, val);
	}

	Z query(T l, T r, int tt) {
		return query(version[tt], st, en, l, r);
	}
};
