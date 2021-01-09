namespace dynamic_cht {

const ll inf = 1e9;
int ID = 0;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
ll rnd (ll l, ll r) { return (ll)(rng() % (r - l + 1)) + l; }

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Node* leftest = nullptr;
    Node* rightest = nullptr;
 
    ll k, b;
    int ind;
    ll size;
 
    Node(ll k = 0, ll b = 0) : k(k), b(b), ind(ID), size(1) {
        leftest = rightest = this;
        ID += 5;
    }
 
    void update();
 
    void update_leftest() {
        if (left == nullptr)
            leftest = this;
        else {
            left->update_leftest();
            leftest = left->leftest;
        }
        update();
    }
 
    void update_rightest() {
        if (right == nullptr)
            rightest = this;
        else {
            right->update_rightest();
            rightest = right->rightest;
        }
        update();
    }
 
    string info() {
        stringstream ss;
        ss << k << "x+" << b << ", sz=" << size;
        return ss.str();
    }
};
 
int sz(Node* node) {
    if (node == nullptr)
        return 0;
    return node->size;
}
 
void Node::update() {
        size = 1 + sz(left) + sz(right);
        leftest = rightest = this;
        if (left != nullptr) {
            this->left->parent = this;
            leftest = left->leftest;
        }
        if (right != nullptr) {
            this->right->parent = this;
            rightest = right->rightest;
        }
    }
 
Node* merge(Node* a, Node* b) {
    if (b == nullptr)
        return a;
    if (a == nullptr)
        return b;
    if (rnd(1, sz(a) + sz(b)) < sz(a)) {
        a->right = merge(a->right, b);
        a->update();
        return a;
    } else {
        b->left = merge(a, b->left);
        b->update();
        return b;
    }
}
 
pair<Node*, Node*> split(Node* node, ll k, int ind) {
    if (node == nullptr)
        return {nullptr, nullptr};
    if (mp(node->k, node->ind) == mp(k, ind)) {
        auto tmp = node->right;
        node->right = nullptr;
        node->update();
        return {node, tmp};
    } else if (mp(node->k, node->ind) < mp(k, ind)) {
        auto p = split(node->right, k, ind);
        node->right = p.first;
        node->update();
        return {node, p.second};
    } else {
        auto p = split(node->left, k, ind);
        node->left = p.second;
        node->update();
        return {p.first, node};
    }
}
 
 
void to_vec(Node* node, vector<Node*>& res) {
    if (node == nullptr)
        return;
    res.pb(node);
    to_vec(node->left, res);
    to_vec(node->right, res);
}
 
Node* to_left(Node* node) {
    if (node->left != nullptr)
        return node->left->rightest;
    else
        return node->parent;
}
 
Node* to_right(Node* node) {
    if (node->right != nullptr)
        return node->right->leftest;
    else
        return node->parent;
}
 
ll intersect(Node* l1, Node* l2) {
    return (l1->b - l2->b + (l2->k - l1->k - 1) * (l1->b - l2->b > 0)) / (l2->k - l1->k);
}
 
bool redundant(Node* left, Node* mid, Node* right) {
    return intersect(left, mid) >= intersect(mid, right);
}
 
void add_item(Node*& root, Node* node) {
    if (root == nullptr) {
        root = node;
        return;
    }
    auto p = split(root, node->k, node->ind);
    if (p.first != nullptr) {
        if (p.first->rightest->k == node->k) {
            if (p.first->rightest->b >= node->b) {
                root = merge(p.first, p.second);
                return;
            } else {
                if (p.first->rightest == p.first) {
                    auto tmp = p.first->left;
                    if (tmp != nullptr)
                        tmp->parent = nullptr;
                    p.first = tmp;
                } else {
                    auto pr = split(p.first, p.first->rightest->k, p.first->rightest->ind - 1);
                    p.first = pr.first;
                }
            }
        }
    }
    if (p.second != nullptr) {
        if (p.second->leftest->k == node->k) {
            if (p.second->leftest->b >= node->b) {
                root = merge(p.first, p.second);
                return;
            } else {
                if (p.second->leftest == p.second) {
                    auto tmp = p.second->right;
                    if (tmp != nullptr)
                        tmp->parent = nullptr;
                    p.second = tmp;
                } else {
                    auto pr = split(p.second, p.second->leftest->k, p.second->leftest->ind + 1);
                    p.second = pr.second;
                }
            }
        }
    }
    if (p.first != nullptr && p.second != nullptr && redundant(p.first->rightest, node, p.second->leftest)) {
        root = merge(p.first, p.second);
    } else {
        while (sz(p.first) >= 2) {
            auto lleft = to_left(p.first->rightest);
            if (redundant(lleft, p.first->rightest, node)) {
                if (p.first == p.first->rightest) {
                    p.first = merge(p.first->left, p.first->right);
                    p.first->parent = nullptr;
                } else {
                    auto pr = split(p.first, p.first->rightest->k, p.first->rightest->ind - 1);
                    p.first = pr.first;
                }
            } else {
                break;
            }
        }
        while (sz(p.second) >= 2) {
            auto rright = to_right(p.second->leftest);
            if (redundant(node, p.second->leftest, rright)) {
                if (p.second == p.second->leftest) {
                    p.second = merge(p.second->left, p.second->right);
                    p.second->parent = nullptr;
                } else {
                    auto pr = split(p.second, p.second->leftest->k, p.second->leftest->ind + 1);
                    p.second = pr.second;
                }
            } else {
                break;
            }
        }
        node->parent = node->left = node->right = nullptr;
        node->update();
        root = merge(p.first, merge(node, p.second));
    }
}

struct dynamic_cht {
    Node *root = nullptr;
    dynamic_cht() : root(nullptr) {}

    // kx + b
    void add_line(ll k, ll b) {
        add_item(root, new Node(k, b));
    }

    ll get_max(ll x) const {
        auto node = root;
        while (true) {
            ll xl = -inf;
            if (node->left != nullptr) {
                auto lleft = to_left(node);
                xl = intersect(lleft, node);
            }
            ll xr = inf;
            if (node->right != nullptr) {
                auto rright = to_right(node);
                xr = intersect(node, rright);
            }
            if (xl <= x && x < xr) {
                return node->k * x + node->b;
            } else if (x < xl) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
    }
};
}
