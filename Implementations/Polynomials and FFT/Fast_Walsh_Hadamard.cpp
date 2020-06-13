https://codeforces.com/contest/914/submission/63263781

const int mod = 1e9 + 7;

void or_transform(int *from, int *to) 
{ 
    if(to - from == 1) 
        return; 
    int *mid = from + (to - from) / 2; 
    or_transform(from, mid); 
    or_transform(mid, to); 
    for(int i = 0; i < mid - from; i++) {
        mid[i] += from[i]; 
        if(mid[i] >= mod) mid[i] -= mod;
    }
} 

void or_inverse(int *from, int *to) 
{ 
    if(to - from == 1) 
        return; 
    int *mid = from + (to - from) / 2; 
    or_inverse(from, mid); 
    or_inverse(mid, to); 
    for(int i = 0; i < mid - from; i++) {
        mid[i] += mod - from[i]; 
        if(mid[i] >= mod) mid[i] -= mod;
    }
}

void xor_transform(int *from, int *to) 
{ 
    if(to - from == 1) 
        return; 
    int *mid = from + (to - from) / 2; 
    xor_transform(from, mid); 
    xor_transform(mid, to); 
    for(int i = 0; i < mid - from; i++) 
    {
        int a = from[i];
        int b = mid[i];
        from[i] = a + b;
        mid[i] = a - b + mod;
        if(from[i] >= mod) from[i] -= mod;
        if(mid[i] >= mod) mid[i] -= mod;
    }
}

void xor_inverse(int *from, int *to) {
    xor_transform(from, to);
    int tot = to - from;
    ll iv = pwr(tot, mod - 2);
    fr(i, 0, tot - 1) {
        from[i] = (from[i] * iv) % mod;
    }
}

void and_transform(int *from,int *to) {
    reverse(from, to);
    or_transform(from, to);
    reverse(from, to);
}

void and_inverse(int *from, int *to) {
    reverse(from, to);
    or_inverse(from, to);
    reverse(from, to);
}
