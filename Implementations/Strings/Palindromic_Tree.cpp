struct node {
    int next[26];
    int len;
    int sufflink; // link to largest suffix which is a palindrome
    int num; // number of distinct suffix palindromes
    ll cnt; // number of palindromes
};

struct Palindromic_Tree {
    vector<node> tree;
    int num;      // node 1 - root with len -1, node 2 - root with len 0
    int suff;     // max suffix palindrome
    string s;

    bool add(int pos) {
        int cur = suff, curlen = 0;
        int ch = s[pos] - 'a';

        while (true) {
            curlen = tree[cur].len;
            if (pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos])
                break;
            cur = tree[cur].sufflink;
        }
        if(tree[cur].next[ch]) {
            suff = tree[cur].next[ch];
            return false;
        }
        // node created
        num++;
        suff = num;
        tree[num].len = tree[cur].len + 2;
        fr(i, 0, 25)
            tree[num].next[i] = 0;
        tree[num].cnt = 0;
        tree[cur].next[ch] = num;

        if(tree[num].len == 1) {
            tree[num].sufflink = 2;
            tree[num].num = 1;
            return true;
        }

        while (true) {
            cur = tree[cur].sufflink;
            curlen = tree[cur].len;
            if(pos - 1 - curlen >= 0 && s[pos - 1 - curlen] == s[pos]) {
                tree[num].sufflink = tree[cur].next[ch];
                break;
            }
        }

        tree[num].num = 1 + tree[tree[num].sufflink].num;

        return true;
    }

    void initTree() {
        num = 2;
        suff = 2;
        tree[1].len = -1;
        tree[1].sufflink = 1;
        tree[1].cnt = 0;
        fr(i, 0, 25)
            tree[1].next[i] = tree[2].next[i] = 0;
        tree[2].len = 0;
        tree[2].sufflink = 1;
        tree[2].cnt = 0;
    }
    void compute(string ss) {
        s = ss;
        tree.resize(sz(s) + 5);
        initTree();
        int n = sz(s);
        fr(i, 0, n - 1){
            add(i);
            tree[suff].cnt++;
        }
        for(int i = num; i >= 0; i--) {
            if(tree[i].sufflink > 2) {
                tree[tree[i].sufflink].cnt += tree[i].cnt;
            }
        }
    }
};
