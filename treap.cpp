struct treapNode {
    treapNode * left = nullptr;
    treapNode * right = nullptr;
    int value;
    int priority;
    int size;

    treapNode(int _value, int _priority) : value(_value), priority(_priority), size(1) {}

    void reset() {
        size = 1;
        if (left != nullptr) {
            size += left->size;
        }
        if (right != nullptr) {
            size += right->size;
        }
    }
};

struct treap {
    treapNode * root = nullptr;
    mt19937 rnd;

    treap() {
        rnd = mt19937(chrono::steady_clock::now().time_since_epoch().count());
    }
    treap(vector< int >& a) {
        rnd = mt19937(chrono::steady_clock::now().time_since_epoch().count());
        for (auto k : a) {
            insert(k);
        }
    }

    pair< treapNode*, treapNode* > split(treapNode * node, int k) {
        if (node == nullptr) {
            return {nullptr, nullptr};
        } if (k <= node->value) {
            auto ans = split(node->left, k);
            node->left = ans.second;
            node->reset();
            return {ans.first, node};
        } else {
            auto ans = split(node->right, k);
            node->right = ans.first;
            node->reset();
            return {node, ans.second};
        }
    }

    treapNode * merge(treapNode * a, treapNode * b) {
        if (a == nullptr) {
            return b;
        } else if (b == nullptr) {
            return a;
        }
        if (a->priority < b->priority) {
            a->right = merge(a->right, b);
            a->reset();
            return a;
        } else {
            b->left = merge(a, b->left);
            b->reset();
            return b;
        }
    }

    void insert(int k) {
        treapNode * newNode = new treapNode(k, rnd());
        auto p = split(root, k);
        root = merge(merge(p.first, newNode), p.second);
    }

    treapNode * find(int k) {
        treapNode * node = root;
        while (node != nullptr && node->value != k) {
            if (node->value < k) {
                node = node->right;
            } else {
                node = node->left;
            }
        }
        return node;
    }

    treapNode * first() {
        if (root == nullptr) {
            return root;
        }
        treapNode * node = root;
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    treapNode * last() {
        if (root == nullptr) {
            return root;
        }
        treapNode * node = root;
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    treapNode * next(int k, treapNode * node) {
        if (node == nullptr) {
            return node;
        }
        if (node->value == k && node->right == nullptr) {
            return node;
        }
        if (node->value <= k) {
            return next(k, node->right);
        } else {
            auto p1 = next(k, node->left);
            if (p1 == nullptr) {
                p1 = node;
            }
            if (p1->value > k) {
                return p1;
            } else {
                return node;
            }
        }
    }

    treapNode * next(int k) {
        return next(k, root);
    }

    treapNode * prev(int k, treapNode * node) {
        if (node == nullptr) {
            return node;
        }
        if (node->value == k && node->left == nullptr) {
            return node;
        }
        if (node->value >= k) {
            return prev(k, node->left);
        } else {
            auto p1 = prev(k, node->right);
            if (p1 == nullptr) {
                p1 = node;
            }
            if (p1->value < k) {
                return p1;
            } else {
                return node;
            }
        }
    }

    treapNode * prev(int k) {
        return prev(k, root);
    }

    void erase(int k) {
        if (root == nullptr) {
            return;
        }
        auto p1 = split(root, k);
        auto p2 = split(p1.second, k + 1);
        root = merge(p1.first, p2.second);
    }

    void clear() {
        delete root;
    }

    ~treap() {
        delete root;
    }
};

#ifdef HOME
void print(treapNode * node, string spaces = "") {
    if (node == nullptr) {
        cout << spaces << "-\n";
        return;
    }
    cout << spaces << "|--" << node->value << '(' << node->size << ')' << '\n';
    if (node->left == nullptr && node->right == nullptr) {
        return;
    }
    print(node->left, spaces + "|  ");
    print(node->right, spaces + "|  ");
}
#endif
