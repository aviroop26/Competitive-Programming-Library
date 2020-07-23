const int N = 2505;
const int M = 5005;
int U[M], V[M];
vector<int> g[N];

/*
Remove one edge from the graph and find shortest between the endpoints in the remaining graph using bfs.
Complexity : O(M * (N + M))
*/

vector<int> shortest_cycle(int n, int m, vector<int> U, vector<int> V) {
    vector<int> dist(n + 1, 1e9), par(n + 1, 0);
    queue<int> q;
    int best_len = 1e9;
    vector<int> nodes;
    fr(j, 0, m - 1) {
        fr(i, 1, n) g[i].clear();
        int st = U[j], en = V[j];
        fr(i, 0, m - 1) {
            if(i == j) continue;
            g[U[i]].pb(V[i]);
            g[V[i]].pb(U[i]);
        }
        fill(dist.begin() + 1, dist.begin() + n + 1, 1e9);
        dist[st] = 0;
        q.push(st);
        par[st] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v : g[u]) {
                if(dist[v] > dist[u] + 1) {
                    dist[v] = dist[u] + 1;
                    par[v] = u;
                    q.push(v);
                }
            }
        }
        int len = 1 + dist[en];
        if(best_len > len) {
            best_len = len;
            nodes.clear();
            while(en != 0) {
                nodes.pb(en);
                en = par[en];
            }
        }
    }
    if(best_len == 1e9) return {}; // no cycle
    return nodes;
}

void solve() {
	int n, m;
    sc(n, m);
    vector<int> U, V;
    fr(i, 0, m - 1) {
        int u, v;
        sc(u, v);
        U.pb(u), V.pb(v);
    }
    auto cyc = shortest_cycle(n, m, U, V);
    int ans = sz(cyc);
    if(!ans) ans = -1;
    cout << ans;
}
