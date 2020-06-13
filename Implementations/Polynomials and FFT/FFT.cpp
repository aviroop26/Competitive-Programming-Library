struct base{
    f80 x,y;
    base(){x=y=0;}
    base(f80 _x, f80 _y){x = _x,y = _y;}
    base(f80 _x){x = _x, y = 0;}
    void operator = (f80 _x){x = _x,y = 0;}
    f80 real(){return x;}
    f80 imag(){return y;}
    base operator + (const base& b){return base(x+b.x,y+b.y);}
    void operator += (const base& b){x+=b.x,y+=b.y;}
    base operator * (const base& b){return base(x*b.x - y*b.y,x*b.y+y*b.x);}
    void operator *= (const base& b){f80 p = x*b.x - y*b.y, q = x*b.y+y*b.x; x = p, y = q;}
    void operator /= (f80 k){x/=k,y/=k;}
    base operator - (const base& b){return base(x - b.x,y - b.y);}
    void operator -= (const base& b){x -= b.x, y -= b.y;}
    base conj(){ return base(x, -y);}
    base operator / (f80 k) { return base(x / k, y / k);}
    void Print(){ cerr << x <<  " + " << y << "i\n";}
};
double PI = 2.0*acos(0.0);
const int MAXN = 19;
const int maxn = (1<<MAXN);
base W[maxn],invW[maxn], P1[maxn], Q1[maxn];
void precompute_powers(){
    for(int i = 0;i<maxn/2;i++){
        double ang = (2*PI*i)/maxn; 
        f80 _cos = cos(ang), _sin = sin(ang);
        W[i] = base(_cos,_sin);
        invW[i] = base(_cos,-_sin);
    }
}
void fft (vector<base> & a, bool invert) {
    int n = (int) a.size();
    for (int i=1, j=0; i<n; ++i) {
        int bit = n >> 1;
        for (; j>=bit; bit>>=1)
            j -= bit;
        j += bit;
        if (i < j)
            swap (a[i], a[j]);
    }
    for (int len=2; len<=n; len<<=1) {
        for (int i=0; i<n; i+=len) {
            int ind = 0,add = maxn/len;
            for (int j=0; j<len/2; ++j) {
                base u = a[i+j],  v = (a[i+j+len/2] * (invert?invW[ind]:W[ind]));
                a[i+j] = (u + v);
                a[i+j+len/2] = (u - v);
                ind += add;
            }
        }
    }
    if (invert) for (int i=0; i<n; ++i) a[i] /= n;
}

// 4 FFTs in total for a precise convolution
void mul_big_mod(vector<ll> &a, vector<ll> & b, ll mod){
    int n1 = a.size(),n2 = b.size();
    int final_size = a.size() + b.size() - 1;
    int n = 1;
    while(n < final_size) n <<= 1;
    vector<base> P(n), Q(n);
    int SQRTMOD = (int)sqrt(mod) + 10;
    for(int i = 0;i < n1;i++) P[i] = base(a[i] % SQRTMOD, a[i] / SQRTMOD);
    for(int i = 0;i < n2;i++) Q[i] = base(b[i] % SQRTMOD, b[i] / SQRTMOD);
    fft(P, 0);
    fft(Q, 0);
    base A1, A2, B1, B2, X, Y;
    for(int i = 0; i < n; i++){
        X = P[i];
        Y = P[(n - i) & (n-1)].conj();
        A1 = (X + Y) * base(0.5, 0);
        A2 = (X - Y) * base(0, -0.5);
        X = Q[i];
        Y = Q[(n - i) & (n-1)].conj();
        B1 = (X + Y) * base(0.5, 0);
        B2 = (X - Y) * base(0, -0.5);
        P1[i] = A1 * B1 + A2 * B2 * base(0, 1);
        Q1[i] = A1 * B2 + A2 * B1;
    }
    for(int i = 0; i < n; i++) P[i] = P1[i], Q[i] = Q1[i];
    fft(P, 1);
    fft(Q, 1);
    a.resize(final_size);
    for(int i = 0; i < final_size; i++){
        ll x = (ll)(P[i].real() + 0.5);
        ll y = (ll)(P[i].imag() + 0.5) % mod;
        ll z = (ll)(Q[i].real() + 0.5);
        a[i] = (x + ((y * SQRTMOD + z) % mod) * SQRTMOD) % mod;
    }
}
vector<ll> mul(const vector<ll> &a, const vector<ll> &b) {
    int tot = a.size() + b.size() - 1;
    int L = 32 - __builtin_clz(tot), n = 1 << L;
    vector<base> c(n, 0);
    for(int i = 0; i < a.size(); i++) {
        c[i].x = a[i];
    }
    for(int i = 0; i < b.size(); i++) {
        c[i].y = b[i];
    }
    fft(c, 0);
    vector<base> d(n, 0);
    for(int i = 0; i < n; i++) {
        base A = c[i], B = c[(n - i) & (n - 1)].conj();
        base p = (A + B) * base(0.5, 0);
        base q = (A - B) * base(0, -0.5);
        d[i] = p * q;
    }
    fft(d, 1);
    vector<ll> ans(tot);
    for(int i = 0; i < tot; i++) {
        ans[i] = round(d[i].x);
    }
    return ans;
}
vector<int> v1, v2;
void solve(){
    int n, m;
    cin >> n >> m;
    v1.resize(n + 1), v2.resize(m + 1);
    fr(i, 0, n)
        cin >> v1[i];
    fr(i, 0, m)
        cin >> v2[i];
    mul_big_mod(v1, v2, mod);
    fr(i, 0, n + m)
        cout << v1[i] << " ";
    cout << endl;
}
