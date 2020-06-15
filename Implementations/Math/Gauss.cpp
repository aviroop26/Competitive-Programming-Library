template<typename T, size_t B>
struct gaussian_elimination {
	T basis[B];
	int cnt;
	gaussian_elimination() : cnt(0) {
		fill(basis, basis + B, 0);
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
