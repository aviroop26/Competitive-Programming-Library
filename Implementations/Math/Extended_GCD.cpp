class EGCD{
public:
	int gg, x, y, a, b, z;
	void init(int A,int B,int Z) { a = A, b = B, z = Z; }
	void go(){ // solve a * x + b * y = z
		egcd(a, b);
		x *= z, y *= z;
	}
	void egcd(int A, int B){
		if(B == 0) { gg = A, x = 1, y = 0; }
		else{
			egcd(B, A % B);
			int t = x;
			x = y;
			y = t - (A / B) * y;
		}
	}
	int inv(int a,int m){
		egcd(a, m);
		int ans = x % m;
		if(ans < 0) ans += m; 
		return ans;
	}
	void shift(int k){
		x += k * (b / gg);
		y -= k * (a / gg);
	}
	void make_x_positive(){
		if(x >= 0) return;
		assert(b != 0);
		int val = b / gg;
		int k = (-x + val - 1) / val;
		shift(k);
	}
	void make_y_positive(){
		if(y >= 0) return;
		assert(a != 0);
		int val = a / gg;
		int k = (-y + val - 1) / val;
		shift(-k);
	}
};
