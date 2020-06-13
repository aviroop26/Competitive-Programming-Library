// Ford Fulkerson Algo
// Complexity : O(Maxflow * (N + M))
#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef unsigned long long int ull;
#define pb emplace_back
#define fi first
#define se second
const int mod = 1e9 + 7;
#define all(x) (x).begin(),(x).end()
typedef long double f80;
const int inf = 1e9 + 5;
typedef pair<int,int> pii;

struct edge {int to, cap, rev;};
const int MV = 5001, INF = 1e9 + 5;
vector<edge> G[MV];
bitset<MV> viss;
int prevv[MV], preve[MV];
queue<int> que;

void add_edge(int from,int to,int cap){
    edge e1={to, cap, (int)G[to].size()};
    edge e2={from, 0, (int)G[from].size()};
    G[from].pb(e1);
    G[to].pb(e2);
}
ll max_flow(int s, int t){
    ll f = 0;
    while(1){
        viss.reset();
        viss[s] = 1;
        que.emplace(s);
        while(!que.empty()){
            int v = que.front();
            que.pop();
            for(int i = 0; i < G[v].size(); i++){
                edge &e = G[v][i];
                if(viss[e.to] || !e.cap)
                    continue;
                viss[e.to] = 1;
                prevv[e.to] = v;
                preve[e.to] = i;
                que.emplace(e.to);
            }
        }
        if(!viss[t]) break;
        int d = inf;
        for(int v = t; v != s; v = prevv[v])
            d = min(d, G[prevv[v]][preve[v]].cap);
        f += d;
        for(int v = t; v != s; v = prevv[v]){
            edge &e = G[prevv[v]][preve[v]];
            e.cap -= d;
            G[v][e.rev].cap += d;
        }
    }
    return f;
}
void reset(int n){ // V must be set before_hand
    for(int i = 1; i <= n; i++)
        G[i].clear();
}
signed main()
{
    freopen("inp.txt", "r", stdin);
    //ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    return 0;
}
