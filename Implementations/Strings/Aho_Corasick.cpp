#define val(ch) ch - 'a'
const int CHAR = 26;
class AhoCora {
public:
  struct Node {
    int ch[CHAR];
    int link;
    int val;
    int id;
    int go[CHAR];
  };
  vector<Node> nd;
  vector<int> Q;
  int qs, qe;
  int size;
  int createNode() {
    int ret = size++;
    nd.emplace_back();
    Q.emplace_back();
    for (int i = 0; i<CHAR; i++) {
      nd[ret].go[i] = nd[ret].ch[i] = -1;
    }
    nd[ret].val = 0;
    return ret;
  }
  void init() {
    size = 0;
    nd.clear();
    Q.clear();
    createNode();
  }
  template<typename A>
  void add(A str, int &sz, int &id) {
    int cur = 0;
    for (int i = 0; i<sz; i++) {
      int c = val(str[i]);
      if (nd[cur].ch[c] == -1) {
        int nnd = createNode();
        nd[cur].ch[c] = nnd;
      }
      cur = nd[cur].ch[c];
    }
    nd[cur].val++;
    nd[cur].id = id;
  }
  inline int next(int u, int c) {
    if(nd[u].go[c]!=-1)
      return nd[u].go[c];
    int uu = u;
    while (u&&nd[u].ch[c]<0)
      u = nd[u].link;
    return nd[uu].go[c] = (nd[u].ch[c]<0)?0:nd[u].ch[c];
  }
  void computelink() {
    qs = qe = 0;
    nd[0].link = 0;
    for (int x, i = 0; i<CHAR; i++) {
      if ((x = nd[0].ch[i]) >= 0) {
        Q[qe++] = x;
        nd[x].link = 0;
      }
    }
    while (qs<qe) {
      int u = Q[qs++];
      for (int v, i = 0; i<CHAR; i++) {
        if ((v = nd[u].ch[i]) >= 0) {
          Q[qe++] = v;
          int m = nd[u].link;
          int L = nd[v].link = next(m, i);
          nd[v].val += nd[L].val;
        }
      }
    }
  }
} acc;
