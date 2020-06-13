// Version 1

int cnt[maxn];
void dfs(int v, int p, bool keep) {
    int mx = -1, bigChild = -1;
    for(auto u : g[v]) {
       if(u != p && sz[u] > mx) {
          mx = sz[u], bigChild = u;
       }
    }
    for(auto u : g[v]) {
        if(u != p && u != bigChild) {
            dfs(u, v, 0);  // run a dfs on small childs and clear them from cnt
        }
    }
    if(bigChild != -1)
        dfs(bigChild, v, 1);  // bigChild marked as big and not cleared from cnt
    for(auto u : g[v]) {
        if(u != p && u != bigChild) {
            for(int p = st[u]; p < ft[u]; p++) {
                cnt[col[ver[p]]]++;
            }
         }
    }
    cnt[col[v]]++;
    // now cnt[c] is the number of vertices in subtree of vertex v that has color c. You can answer the queries easily.
    if(keep == 0) {
        for(int p = st[v]; p < ft[v]; p++) {
            cnt[col[ver[p]]]--;
        }
    }
}

// Version 2

void dfs(int u) {
    int h = -1, S = -1;
    for(int v : g[u]) {
        if(sz[v] > S) {
            S = sz[v], h = v;
        }
    }
    if(h == -1) {
        lol[u] = u;
    }
    else {
        dfs(h);
        lol[u] = lol[h];
    }
    dat[lol[u]].add(u);
    for(int v : g[u]) {
        if(v != h) {
            dfs(v);
            fr(i, st[v], en[v]) {
                dat[lol[u]].add(ver[i]);
            }
        }
    }
    // now you can query dat[lol[u]]
    return;
}
