mt19937 rnd = mt19937(chrono::steady_clock::now().time_since_epoch().count());

struct treapArNode {
    treapArNode * left = nullptr;
    treapArNode * right = nullptr;
    int value;
    int size;

    treapArNode(int _value) : value(_value), size(1) {}

    void resetSize() {
        size = 1;
        if (left != nullptr) {
            size += left->size;
        }
        if (right != nullptr) {
            size += right->size;
        }
    }
};

pair<treapArNode*, treapArNode*> split(treapArNode * node, int k);
treapArNode * merge(treapArNode * a, treapArNode * b);

struct treapAr {
    treapArNode * root = nullptr;

    treapAr();
    treapAr(treapArNode * _root);

    void insert(int i, int k);

    void insert(int i, treapArNode * node);

    treapArNode * get(int i);

    int operator[] (int i);

    vector<int> toVector(treapArNode * node);

    vector<int> toVector();

    void erase(int k);

    void erase(int l, int r);

    treapArNode * cut(int l, int r);

    void clear();
};

#ifdef HOME
void print(treapArNode * node, string spaces = "") {
    if (node == nullptr) {
        cout << spaces << "-" << endl;
        return;
    }
    cout << spaces << "|--" << node->value << '(' << node->size << ')' << endl;
    if (node->left == nullptr && node->right == nullptr) {
        return;
    }
    print(node->left, spaces + "|  ");
    print(node->right, spaces + "|  ");
}
#endif

struct treaps {
    vector<treapAr> versions;

    treaps() {
        versions.emplace_back();
    }

    int insert(int version, int i, int k) {
        auto p = split(versions[version].root, i);
        versions.emplace_back(merge(merge(p.first, new treapArNode(k)), p.second));
        return versions.size() - 1;
    }

    auto get(int version, int i) {
        return versions[version].get(i);
    }

    int erase(int version, int k) {
        auto p = split(versions[version].root, k);
        versions.emplace_back(merge(p.first, split(p.second, 1).second));
        return versions.size() - 1;
    }

    int erase(int version, int l, int r) {
        auto p = split(versions[version].root, l);
        versions.emplace_back(merge(p.first, split(p.second, r - l + 1).second));
        return versions.size() - 1;
    }

    auto& operator[] (int i) {
        return versions[i];
    }

    int size() {
        return versions.size();
    }

    auto& back() {
        return versions.back();
    }
};

treapArNode * treapAr::get(int i) {
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

int treapAr::operator[] (int i) {
    return get(i)->value;
}

treapAr::treapAr() {}
treapAr::treapAr(treapArNode * _root) {
    root = _root;
}

pair<treapArNode*, treapArNode*> split(treapArNode * node, int k) {
    if (node == nullptr) {
        return {nullptr, nullptr};
    }
    int leftSize = 0;
    if (node->left != nullptr) {
        leftSize = node->left->size;
    }
    if (leftSize >= k) {
        auto ans = split(node->left, k);
        auto newNode = new treapArNode(node->value);
        newNode->left = ans.second;
        newNode->right = node->right;
        newNode->resetSize();
        return {ans.first, newNode};
    } else {
        auto ans = split(node->right, k - 1 - leftSize);
        treapArNode * newNode = new treapArNode(node->value);
        newNode->left = node->left;
        newNode->right = ans.first;
        newNode->resetSize();
        return {newNode, ans.second};
    }
}

treapArNode * merge(treapArNode * a, treapArNode * b) {
    if (a == nullptr) {
        return b;
    } else if (b == nullptr) {
        return a;
    }
    if (rnd() % (a->size + b->size) < a->size) {
        auto newA = new treapArNode(a->value);
        newA->left = a->left;
        newA->right = merge(a->right, b);
        newA->resetSize();
        return newA;
    } else {
        auto newB = new treapArNode(b->value);
        newB->right = b->right;
        newB->left = merge(a, b->left);
        newB->resetSize();
        return newB;
    }
}

vector<int> treapAr::toVector(treapArNode * node) {
    if (node == nullptr) {
        return vector<int>({});
    }
    vector<int> vLeft = toVector(node->left);
    vector<int> vRight = toVector(node->right);
    vLeft.push_back(node->value);
    vLeft.insert(vLeft.end(), vRight.begin(), vRight.end());
    return vLeft;
}

vector<int> treapAr::toVector() {
    return toVector(root);
}

treapArNode * treapAr::cut(int l, int r) {
    auto p1 = split(root, l);
    auto p2 = split(p1.second, r + 1 - l);
    root = merge(p1.first, p2.second);
    return p2.first;
}

void treapAr::clear() {
    root = nullptr;
}

#ifdef HOME
void print(treaps& t) {
    for (int i = 0; i < t.versions.size(); ++i) {
        cout << "version " << i << endl;
        print(t[i].root);
        cout << endl;
    }
}
#endif
