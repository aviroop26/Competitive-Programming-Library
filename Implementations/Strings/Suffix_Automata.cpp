struct state {
    int len, link;
    map<char, int> next;
};
const int MAXLEN = 3e5 + 5;

int cnt[2 * MAXLEN];

state st[MAXLEN * 2];
int sz, last;
void sa_init() {
    while(sz > 0) {
        sz--;
        st[sz].next.clear();
        cnt[sz] = 0;
    }
    st[0].len = 0;
    st[0].link = -1;
    sz++;
    last = 0;
}
void sa_extend(char c, int ck) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
    cnt[cur] += ck;
}

void lift() {
    vector<int> deg(sz, 0);
    queue<int> q;
    fr(i, 1, sz - 1) {
        deg[st[i].link]++;
    }
    fr(i, 1, sz - 1) {
        if(!deg[i]) {
            q.push(i);
        }
    }
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        int p = st[u].link;
        deg[p]--;
        cnt[p] += cnt[u];
        if(!deg[p] && p != 0) q.push(p);
    }
}
