/*
    Characters should be in range (0, char_bound - 1)
    For strings, use char_bound = 256
*/

template <typename T>
vector<int> suffix_array(int n, const T &s, int char_bound) {
  vector<int> a(n);
  if (n == 0) {
    return a;
  }
  if (char_bound != -1) {
    vector<int> aux(char_bound, 0);
    for (int i = 0; i < n; i++) {
      aux[s[i]]++;
    }
    int sum = 0;
    for (int i = 0; i < char_bound; i++) {
      int add = aux[i];
      aux[i] = sum;
      sum += add;
    }
    for (int i = 0; i < n; i++) {
      a[aux[s[i]]++] = i;
    }
  } else {
    iota(a.begin(), a.end(), 0);
    sort(a.begin(), a.end(), [&s](int i, int j) { return s[i] < s[j]; });
  }
  vector<int> sorted_by_second(n);
  vector<int> ptr_group(n);
  vector<int> new_group(n);
  vector<int> group(n);
  group[a[0]] = 0;
  for (int i = 1; i < n; i++) {
    group[a[i]] = group[a[i - 1]] + (!(s[a[i]] == s[a[i - 1]]));
  }
  int cnt = group[a[n - 1]] + 1;
  int step = 1;
  while (cnt < n) {
    int at = 0;
    for (int i = n - step; i < n; i++) {
      sorted_by_second[at++] = i;
    }
    for (int i = 0; i < n; i++) {
      if (a[i] - step >= 0) {
        sorted_by_second[at++] = a[i] - step;
      }
    }
    for (int i = n - 1; i >= 0; i--) {
      ptr_group[group[a[i]]] = i;
    }
    for (int i = 0; i < n; i++) {
      int x = sorted_by_second[i];
      a[ptr_group[group[x]]++] = x;
    }
    new_group[a[0]] = 0;
    for (int i = 1; i < n; i++) {
      if (group[a[i]] != group[a[i - 1]]) {
        new_group[a[i]] = new_group[a[i - 1]] + 1;
      } else {
        int pre = (a[i - 1] + step >= n ? -1 : group[a[i - 1] + step]);
        int cur = (a[i] + step >= n ? -1 : group[a[i] + step]);
        new_group[a[i]] = new_group[a[i - 1]] + (pre != cur);
      }
    }
    swap(group, new_group);
    cnt = group[a[n - 1]] + 1;
    step <<= 1;
  }
  return a;
}
template <typename T>
vector<int> suffix_array(const T &s, int char_bound) {
  return suffix_array((int) s.size(), s, char_bound);
}
template <typename T>
vector<int> build_lcp(int n, const T &s, const vector<int> &sa) {
  assert((int) sa.size() == n);
  vector<int> pos(n);
  for (int i = 0; i < n; i++) {
    pos[sa[i]] = i;
  }
  vector<int> lcp(max(n - 1, 0));
  int k = 0;
  for (int i = 0; i < n; i++) {
    k = max(k - 1, 0);
    if (pos[i] == n - 1) {
      k = 0;
    } else {
      int j = sa[pos[i] + 1];
      while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
        k++;
      }
      lcp[pos[i]] = k;
    }
  }
  return lcp;
}
template <typename T>
vector<int> build_lcp(const T &s, const vector<int> &sa) {
  return build_lcp((int) s.size(), s, sa);
}

template<class T, size_t MAXN> struct FastSegmentTree {
    int n;
    T st[MAXN << 1];
    void build(int n) {
        this->n = n;
        for (int i = n - 1; i > 0; i--) {
            st[i] = min(st[i << 1], st[i << 1 | 1]);
        }
    }
    T query(int l, int r) {
        l += n, r += n + 1;
        T res = 1e9;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = min(res, st[l++]);
            if (r & 1) res = min(res, st[--r]);
        }
        return res;
    }
};

template<typename T, size_t N>
struct SuffixArray {
    vector<int> sa, inv_sa, lcp;
    FastSegmentTree<int, N> seg;
    vector<vector<int>> dp;
    vector<int> lg2;
    int n;
    void init(int nn, const T &s, int char_bound) {
        assert(N >= sz(s));
        n = nn;
        inv_sa.resize(n);
        sa = suffix_array(s, char_bound);
        lcp = build_lcp(s, sa);
        fr(i, 0, n - 1) {
            inv_sa[sa[i]] = i;
        }
        generate_lcp_segtree();
        generate_lcp_table();
    }
    void generate_lcp_segtree() {
        int n = sz(lcp);    
        fr(i, 0, n - 1)
            seg.st[n + i] = lcp[i];
        seg.build(n);
    }
    void generate_lcp_table() {
        lg2.resize(n, 0);
        fr(k, 2, n - 1) lg2[k] = lg2[k >> 1] + 1;
        int nn = sz(lcp);
        int LOGN = 32 - __builtin_clz(nn);
        dp.resize(LOGN);
        fr(k, 0, LOGN - 1)
            dp[k].resize(nn - (1 << k) + 1);
        fr(i, 0, nn - 1) {
            dp[0][i] = lcp[i];
        }
        fr(k, 1, LOGN - 1) {
            int p = (1 << (k - 1));
            fr(i, 0, sz(dp[k]) - 1) {
                assert(i + p < sz(dp[k - 1]));
                dp[k][i] = min(dp[k - 1][i], dp[k - 1][i + p]);
            }
        }
    }
    int get_lcp(int l,int r) {
        if(l == r) return n - 1 - sa[l];
        return seg.query(l, r - 1);
    }
    int get_lcp_fast(int l,int r) {
        if(l == r) return n - 1 - sa[l];
        r--;
        int k = lg2[r - l + 1];
        return min(dp[k][l], dp[k][r - (1 << k) + 1]);
    }
};

const int N = 1e6 + 5;
SuffixArray<string, N> S;

void solve() {
    string s = "";
    int n = 1e6;
    fr(i, 1, n) {
        s += char('a' + rand(0, 3));
    }
    S.init(sz(s), s, 256);
}
