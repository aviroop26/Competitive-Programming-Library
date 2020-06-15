// Need to take care about overflows.
// Q: https://vjudge.net/problem/Gym-102135G
#include <bits/stdc++.h>
using namespace std;
#define sz(v) (int)(v).size()
#define pb push_back
#define fr(i, a, b) for(int i = a; i <= b; i++)
#define pii pair<int,int>
#define pll pair<ll,ll>
#define fi first
#define se second
typedef long long ll;
typedef long double f80;
typedef long double ld;
typedef pair<ld,int> pli;
#define all(x) x.begin(), x.end()
#define endl '\n'
const int mod = 9;

ll go(ll a, ll b, ll c, ll L, ll R)
{
    ll ans = 0;
    if(a >= c) {
        ll q = a / c;
        ans += (q % mod) * ((((R - L + 1) * (L + R)) / 2) % mod);
        ans %= mod;
        a -= q * c;
    }
    if(b >= c) {
        ll q = b / c;
        ans += (q % mod) * ((R - L + 1) % mod);
        ans %= mod;
        b -= q * c;
    }
    if(a == 0)
        return ans;
    ll L1 = (a * L + b) / c;
    ll R1 = (a * R + b) / c;
    if(L1 == R1) {
        ans += (L1 % mod) * ((R - L + 1) % mod);
        ans %= mod;
        return ans;
    }
    ans += (L1 % mod) * ((L - 1) % mod);
    ans += (R1 % mod) * ((R) % mod);
    ans -= go(c, c - b - 1, a, L1, R1 - 1);
    ans %= mod;
    return ans;
}

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

    int t;
    cin >> t;
    while(t--)
    {
        ll a, b, m, k;
        cin >> a >> b >> m >> k;
        if(k==1&&((a%m)==0)){cout << 0 << endl;continue;}
        ll ans = go(b,a,m,0,k-1);
        ans = (((a+a+(k-1)*b))%mod)*(k)*5LL-m*ans;
        ans %= mod;
        if(ans<0)ans+=mod;
        if(ans==0)ans=9;
        cout << ans<< endl;
    }
    return 0;
}
