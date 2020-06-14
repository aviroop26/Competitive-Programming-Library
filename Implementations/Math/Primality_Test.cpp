//Miller rabin test using first n primes doesn't work for - 
//2047, 1373653, 25326001, 3215031751, 2152302898747, 3474749660383, 341550071728321, 341550071728321, 3825123056546413051, 3825123056546413051, 3825123056546413051, 318665857834031151167461, 3317044064679887385961981

bool isprime(int n)
{
    if(n == 2 || n == 3) return 1;
    if(!(n & 1)) return 0;
    vector<int> v = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int r = 0;
    int x, d = n - 1;
    while(!(d & 1))
    {
        r++;
        d >>= 1;
    }
    f80 divi = f80(1) / f80(n);
    for(auto &a:v)
    {
        if(a >= n) break;
        x = pwr(a, d, n);
        if(x == 1 || x == (n - 1)) continue;
        bool flag = 1;
        fr(i, 1, r - 1)
        {
            x = mul_mod(x, x, n, divi);
            if(x == 1) return 0;
            if(x == (n - 1))
            {
               flag = 0;
               break;
            }
        }
        if(flag) return 0;
    }
    return 1;
}
