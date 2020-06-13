struct KMP{
    string p;
    int n;
    vector<int> lcp;
    void init(string s){ p = s, n = p.length(); }
    void compute_lcp(){
        lcp.resize(n, 0);
        int j = 0;
        fr(i, 1, n - 1){
            while(j > 0 && p[j] != p[i]) j = lcp[j - 1];
            if(p[j] == p[i]) j++;
            lcp[i] = j;
        }
    }
    void kmp(string t){
        int m = t.length(), j = 0;
        fr(i, 0, m - 1){
            while(j > 0 && p[j] != t[i])
                j = lcp[j - 1];
            if(p[j] == t[i]) j++;
            if(j == n){ 
                cout << i << " "; // pattern found at position i
                j = lcp[j - 1]; // j = 0 for non-overlapping patterns 
            }
        }
    }
};
