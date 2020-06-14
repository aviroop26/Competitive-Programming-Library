// Description:
// B -> stores the nodes in a bcc
// bcc[u] -> which bcc u is in.
// nc -> number of bcc's
// to just get bridges call get_bridge

struct BCC {
    vector<vector<int>> tree, g, B, Q;
    vector<int> U, V, temp, bcc, ind;
    vector<bool> vis, bridge;
    int T, nc, m, n;
    void init(int _n) {
        n = _n, m = 0, nc = 0, T = 0;
        tree.resize(n + 1);
        g.resize(n + 1);
        B.resize(n + 1);
        Q.resize(n + 1);
        vis.resize(n + 1);
        temp.resize(n + 1);
        ind.resize(n + 1, 0);
        bcc.resize(n + 1);
    }
    int adj(int u,int e) { return U[e] == u ? V[e] : U[e]; }
    void add(int u,int v){
        U.pb(u);
        V.pb(v);
        bridge.pb(0);
        g[u].pb(m), g[v].pb(m);
        m++;
    }
    void get_bridges(){
        fill(all(vis), 0);
        fr(i, 1, n)
            if(!vis[i])
                dfs1(i, -1);
    }
    int dfs1(int u,int e){
        vis[u] = 1;
        temp[u] = T++;
        int t = temp[u];
        for(int id: g[u]){
            int v = adj(u, id);
            if(!vis[v]) t = min(t, dfs1(v, id));
            else if(id != e) t = min(t, temp[v]);
        }
        if(t == temp[u] && e != -1) bridge[e] = 1;
        return t;
    }
    void dfs2(int u){
        int cur_cmp = nc;
        Q[cur_cmp].pb(u);
        bcc[u] = cur_cmp;
        B[cur_cmp].pb(u);
        vis[u] = 1;
        while(Q[cur_cmp].size() != ind[cur_cmp])
        {
            int u = Q[cur_cmp][ind[cur_cmp]];    
            ind[cur_cmp]++;
            for(int e: g[u])
            {
                int v = adj(u, e);
                if(vis[v]) continue;
                if(bridge[e])
                {
                    nc++;
                    tree[cur_cmp].pb(nc);
                    tree[nc].pb(cur_cmp);
                    dfs2(v);
                }
                else
                {
                    Q[cur_cmp].pb(v);
                    vis[v] = 1;
                    bcc[v] = cur_cmp;
                    B[cur_cmp].pb(v);
                }
            }
        }
    }
    void go() {
        get_bridges();
        fill(all(vis), 0);
        fr(i, 1, n)
            if(!vis[i])
                { nc++; dfs2(i); }
    }
} B;

void solve() {
    int n = 5, m = 6;
    B.init(n);
    B.add(1, 2);
    B.add(2, 3);
    B.add(3, 4);
    B.add(4, 5);
    B.add(2, 5);
    B.add(1, 2);
    B.go();
}
