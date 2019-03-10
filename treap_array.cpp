mt19937 rnd = mt19937(chrono::steady_clock::now().time_since_epoch().count());

struct treapArNode {
    treapArNode * left = nullptr;
    treapArNode * right = nullptr;
    int value;
    int size;

    treapArNode(int _value) : value(_value), size(1) {}

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

struct treapAr {
    treapArNode * root = nullptr;

    treapAr() {}
    treapAr(treapArNode * _root) {
        root = _root;
    }

    pair< treapArNode*, treapArNode* > split(treapArNode * node, int k) {
        if (node == nullptr) {
            return {nullptr, nullptr};
        }
        int leftSize = 0;
        if (node->left != nullptr) {
            leftSize = node->left->size;
        }
        if (leftSize >= k) {
            auto ans = split(node->left, k);
            node->left = ans.second;
            node->reset();
            return {ans.first, node};
        } else {
            auto ans = split(node->right, k - 1 - leftSize);
            node->right = ans.first;
            node->reset();
            return {node, ans.second};
        }
    }

    treapArNode * merge(treapArNode * a, treapArNode * b) {
        if (a == nullptr) {
            return b;
        } else if (b == nullptr) {
            return a;
        }
        if (rnd() % (a->size + b->size) < a->size) {
            a->right = merge(a->right, b);
            a->reset();
            return a;
        } else {
            b->left = merge(a, b->left);
            b->reset();
            return b;
        }
    }

    void insert(int i, int k) {
        treapArNode * newNode = new treapArNode(k);
        auto p = split(root, i);
        root = merge(merge(p.first, newNode), p.second);
    }

    void insert(int i, treapArNode * node) {
        auto p = split(root, i);
        root = merge(merge(p.first, node), p.second);
    }

    treapArNode * get(int i) {
        treapArNode * node = root;
        while (node != nullptr) {
            if (node->left == nullptr) {
                if (i == 0) {
                    return node;
                } else {
                    --i;
                    node = node->right;
                }
            } else {
                if (i == node->left->size) {
                    return node;
                } else if (i < node->left->size) {
                    node = node->left;
                } else {
                    i -= node->left->size + 1;
                    node = node->right;
                }
            }
        }
    }

    int operator[] (int i) {
        return get(i)->value;
    }

    vector< int > toVector(treapArNode * node) {
        if (node == nullptr) {
            return vector< int >({});
        }
        vector< int > vLeft = toVector(node->left);
        vector< int > vRight = toVector(node->right);
        vLeft.push_back(node->value);
        vLeft.insert(vLeft.end(), vRight.begin(), vRight.end());
        return vLeft;
    }

    vector< int > toVector() {
        return toVector(root);
    }

    void erase(int k) {
        auto p1 = split(root, k);
        auto p2 = split(p1.second, 1);
        root = merge(p1.first, p2.second);
    }

    void erase(int l, int r) {
        auto p1 = split(root, l);
        auto p2 = split(p1.second, r + 1 - l);
        root = merge(p1.first, p2.second);
    }

    treapArNode * cut(int l, int r) {
        auto p1 = split(root, l);
        auto p2 = split(p1.second, r + 1 - l);
        root = merge(p1.first, p2.second);
        return p2.first;
    }

    void clear() {
        delete root;
    }
};

#ifdef HOME
void print(treapArNode * node, string spaces = "") {
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
