int suf_inf = 1e7;
string suf_string;
struct SufNode;
int suf_down = 0;
struct SufEdge {
    SufNode* to = nullptr;
    int l;
    int r;

    SufEdge() {}
    SufEdge(int l, int r, SufNode* node = nullptr) : l(l), r(r), to(node) {}

    int len() {
        return r - l + 1;
    }
};
struct SufNode {
    map<char, SufEdge*> to;
    SufNode* suflink = nullptr;
    ll cnt = 0;

    void count() {
        cnt = 1;
        for (auto [c, edge] : to) {
            if (edge->r + 1 == suf_string.size())
                --edge->r;
            edge->to->count();
            cnt += edge->to->cnt + edge->len() - 1;
        }
    }
};
SufNode* suf_root = new SufNode();
SufNode* suf_cur = suf_root;
SufNode* suf_saved = nullptr;
void split_suf_edge(SufEdge* edge, int k) {
    SufNode* middle = new SufNode();
    SufEdge* next = new SufEdge(edge->l + k, edge->r, edge->to);
    edge->to = middle;
    middle->to[suf_string[edge->l + k]] = next;
    edge->r = edge->l + k - 1;
}
void add_char(char c) {
    suf_string.pb(c);
    ++suf_down;
    suf_saved = nullptr;
    while (suf_down != 0) {
        while (suf_cur->to.find(suf_string[suf_string.size() - suf_down]) != suf_cur->to.end()) {
            SufEdge* edge = suf_cur->to[suf_string[suf_string.size() - suf_down]];
            if (suf_down <= edge->len())
                break;
            suf_down -= edge->len();
            suf_cur = edge->to;
        }
        if (suf_cur->to.find(suf_string[suf_string.size() - suf_down]) == suf_cur->to.end()) {
            SufNode* new_node = new SufNode();
            suf_cur->to[suf_string[suf_string.size() - suf_down]] =
                    new SufEdge(suf_string.size() - suf_down, suf_inf, new_node);
            if (suf_saved != nullptr)
                suf_saved->suflink = suf_cur;
            suf_saved = nullptr;
        } else {
            SufEdge* edge = suf_cur->to[suf_string[suf_string.size() - suf_down]];
            if (suf_string[edge->l + suf_down - 1] == c) {
                if (suf_saved != nullptr)
                    suf_saved->suflink = suf_cur;
                break;
            } else {
                split_suf_edge(edge, suf_down - 1);
                edge->to->to[c] = new SufEdge(suf_string.size() - 1, suf_inf, new SufNode());
                if (suf_saved != nullptr)
                    suf_saved->suflink = edge->to;
                suf_saved = edge->to;
            }
        }
        if (suf_cur == suf_root)
            --suf_down;
        else
            suf_cur = suf_cur->suflink;
    }
}
void suf_build(string s) {
    for (auto c : s) {
        add_char(c);
    }
    suf_root->count();
    --suf_root->cnt;
}
string get_lex(ll n) {
    string res;
    SufNode* node = suf_root;
    while (n > 0) {
        show(n, res);
        for (auto [c, edge] : node->to) {
            if (n <= edge->to->cnt + edge->len() - 1) {
                if (n <= edge->len()) {
                    res += suf_string.substr(edge->l, n);
                    return res;
                } else {
                    res += suf_string.substr(edge->l, edge->len());
                    n -= edge->len();
                    node = edge->to;
                    break;
                }
            }
            n -= (edge->to->cnt + edge->len() - 1);
        }
    }
    return res;
}
