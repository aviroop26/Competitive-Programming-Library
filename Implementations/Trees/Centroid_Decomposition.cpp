const int N = 250005;
const int LOGN = 19;
vector<int> g[N];
bool centroid[N];
int dp[LOGN][N], depth[N], par[N], sz[N];
int n, q;
void dfs2(int u,int p){
    sz[u] = 1;
    for(int v : g[u])
        if(v != p && !centroid[v]){
            dfs2(v, u);
            sz[u] += sz[v];
        }
}
int get(int u,int p,int S){
    for(int v : g[u])
        if(v != p && !centroid[v] && sz[v] > S / 2)
            return get(v, u, S);
    return u;
}
int solve(int u){
    dfs2(u, 0);
    int c = get(u, 0, sz[u]);
    centroid[c] = 1;
    for(int v : g[c])
        if(!centroid[v])
            par[solve(v)] = c;
    return c;
}
void dfs1(int u,int p){
    dp[0][u] = p;
    depth[u] = depth[p] + 1;
    for(int v : g[u])
        if(v != p)
            dfs1(v, u);
}
void preprocess(){
    fr(k, 1, LOGN - 1)
        fr(i, 1, n)
            dp[k][i] = dp[k - 1][dp[k - 1][i]];
}
int lca(int u,int v){
    if(depth[u] < depth[v]) swap(u, v);
    for(int k = LOGN - 1; k >= 0; k--)
        if(depth[dp[k][u]] >= depth[v])
            u = dp[k][u];
    if(u == v) return u;
    for(int k = LOGN - 1; k >= 0; k--)
        if(dp[k][u] != dp[k][v]) u = dp[k][u], v = dp[k][v];
    return dp[0][u];
}
int dis(int u,int v){ return depth[u] + depth[v] - 2 * depth[lca(u, v)]; }
signed main(){
    dfs1(1, 0);
    preprocess();
    solve(1);
    return 0;
}
