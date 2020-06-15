ll S(ll n,ll k)
{
    ll ans = 0;
    int c = 1;
    for(int j = k; j >= 0; j--)
    {
        ll val = pwr(j, n, mod);
        ans = (ans + (c * ncr(k, j) * val) % mod) % mod;
        c *= (-1);
    }
    if(ans < 0) ans += mod;
    ans = (ans * ifact[k]) % mod;
    return ans;
}
