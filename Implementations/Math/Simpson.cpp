// Simpson 3/8 Rule
// integration of function ‘fun’ from a to b
const int ITER1 = 100000; // number of intervals to break

f80 fun(f80 x) {
    return x * x;
}

f80 simpson_3by8(f80 a, f80 b) {
    assert(a <= b);
    int n = 3 * ITER1;
    f80 h = (b - a) / n;
    f80 ans = fun(a) + fun(b);
    for(int i = 1, j = 1; i < n; i++, j++) {
        if(j == 3) j = 0;
        ans += (2 + (j > 0)) * fun(a + i * h);
    }
    ans *= (3 * h) / 8;
    return ans;
}
