template<typename T, size_t B>
struct Gauss {
	T basis[B];
	int cnt;
	void init() {
		fill(basis, basis + B, 0);
		cnt = 0;
    	}
	void add(T x) {
		for(int i = B - 1; i >= 0; i--) {
			if((x >> i) & 1) {
				if(!basis[i]) {
					basis[i] = x;
					cnt++;
					return;
				}
				else {
					x ^= basis[i];
				}
			}
		}
	}
	bool chk(T x) {
		for(int i = B - 1; i >= 0; i--) {
			if((x >> i) & 1) {
				if(!basis[i]) {
					return 0;
				}
				else {
					x ^= basis[i];
				}
			}
		}
		return 1;
	}
};
