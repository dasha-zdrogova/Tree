#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

class BTreeNode {
    int* keys;
    int t;

    BTreeNode** C;
    int n;
    bool leaf;

public:
    BTreeNode(int _t, bool _leaf);
    void print(int offset = 0);

    BTreeNode *search(int k);
    int find_key(int k);
    void insert_non_full(int k);
    void split(int i, BTreeNode *y);

    void remove(int k);
    void remove_from_leaf(int index);
    void remove_from_non_leaf(int index);
    int get_pred(int index);
    int get_succ(int index);
    void fill(int index);
    void borrow_from_prev(int index);
    void borrow_from_next(int index);
    void merge(int index);

    friend class BTree;
};

class BTree {
    BTreeNode* root;
    int t;
public:
    BTree (int _t) {
        root = nullptr;
        t = _t;
    }

    void print(int offset = 0) {
        if (root) root->print(offset);
    }

    BTreeNode* search(int k) {
        if (!root) return nullptr;
        return root->search(k);
    }

    void insert(int k);
    void remove(int k);
};

BTreeNode::BTreeNode(int _t, bool _leaf){
    t = _t;
    leaf = _leaf;
    keys = new int[2*t-1];
    C = new BTreeNode *[2*t];
    n = 0;
}

void BTreeNode::print(int level) {
    if (this) {
        int i;
        for (i = n - 1; i >= 0; i--) {
            if (!leaf) {
                C[i+1]->print(level + 1);
            }
            for (int j = 0; j < level; j++) {
                cout << "        ";
            }
            cout << keys[i] << "\n";
        }
        if (!leaf) {
            C[0]->print(level + 1);
        }
    }
}

int BTreeNode::find_key(int k) {
    int index = 0;
    while (index < n && keys[index] < k) index++;
    return index;
}

BTreeNode* BTreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i]) i++;
    if (keys[i] == k) return this;
    if (leaf) return nullptr;
    return C[i]->search(k);
}

void BTree::insert(int k) {
    if (!root) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else {
        if (root->n == 2*t - 1) {
            BTreeNode *new_root = new BTreeNode(t, false);
            new_root->C[0] = root;
            new_root->split(0, root);

            int i = 0;
            if (new_root->keys[0] < k) i++;
            new_root->C[i]->insert_non_full(k);
            root = new_root;
        }
        else root->insert_non_full(k);
    }
}

void BTreeNode::insert_non_full(int k) {
    int i = n-1;

    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i+1] = keys[i]; i--;
        }
        keys[i+1] = k; n++;
    }
    else {
        while (i >= 0 && keys[i] > k) i--;
        if (C[i+1]->n == 2*t-1) {
            split(i+1, C[i+1]);
            if (keys[i+1] < k) i++;
        }
        C[i+1]->insert_non_full(k);
    }
}

void BTreeNode::split(int i,BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t-1;

    for (int j = 0; j < t-1; j++) z->keys[j] = y->keys[j+t];
    if(!(y->leaf)) for (int j = 0; j < t; j++) z->C[j] = y->C[j+t];

    y->n = t-1;

    for (int j = n; j >= i+1; j--) C[j+1] = C[j];
    C[i+1] = z;

    for (int j = n-1; j >= i; j--) keys[j+1] = keys[j];
    keys[i] = y->keys[t-1];

    n++;
}

void BTree::remove(int k){
    if (!root) return;
    root->remove(k);
    if (root->n == 0) {
        BTreeNode *tmp = root;
        if (root->leaf) root = nullptr;
        else root = root->C[0];
        delete tmp;
    }
    return;
}

void BTreeNode::remove(int k) {
    int index = find_key(k);
    if (index < n && keys[index] == k) {
        if (leaf) remove_from_leaf(index);
        else remove_from_non_leaf(index);
    }
    else {
        if (leaf) return;
        bool flag = (index == n);
        if (C[index]->n < t) fill(index);
        if (flag && index > n) C[index-1]->remove(k);
        else C[index]->remove(k);
    }
    return;
}

void BTreeNode::remove_from_leaf(int index) {
    for (int i = index+1; i < n; i++) keys[i-1] = keys[i];
    n--;
    return;
}

void BTreeNode::remove_from_non_leaf(int index) {
    int k = keys[index];
    if (C[index]->n >= t) {
        int pred = get_pred(index);
        keys[index] = pred;
        C[index]->remove(pred);
    }
    else if (C[index+1]->n >= t) {
        int succ = get_succ(index);
        keys[index] = succ;
        C[index+1]->remove(succ);
    }
    else {
        merge(index);
        C[index]->remove(k);
    }
    return;
}

int BTreeNode::get_pred(int index) {
    BTreeNode *current = C[index];
    while (!current->leaf) current = current->C[current->n];
    return current->keys[current->n-1];
}

int BTreeNode::get_succ(int index) {
    BTreeNode *current = C[index+1];
    while (!current->leaf) current = current->C[0];
    return current->keys[0];
}

void BTreeNode::fill(int index) {
    if (index != 0 && C[index-1]->n >= t) borrow_from_prev(index);
    else if (index != n && C[index+1]->n >= t) borrow_from_next(index);
    else {
        if (index != n) merge(index);
        else merge(index-1);
    }
    return;
}

void BTreeNode::borrow_from_prev(int index) {
    BTreeNode *child = C[index];
    BTreeNode *sibling = C[index-1];

    for (int i = child->n-1; i >= 0; i--) child->keys[i+1] = child->keys[i];
    if (!child->leaf){
        for (int i = child->n; i >= 0; i--) child->C[i+1] = child->C[i];
    }
    child->keys[0] = keys[index-1];
    
    if(!child->leaf) child->C[0] = sibling->C[sibling->n];

    keys[index-1] = sibling->keys[sibling->n-1];
    child->n++;
    sibling->n--;
    return;
}

void BTreeNode::borrow_from_next(int index) {
    BTreeNode *child = C[index];
    BTreeNode *sibling = C[index+1];

    child->keys[child->n] = keys[index];

    if(!child->leaf) child->C[child->n+1] = sibling->C[0];
    
    keys[index] = sibling->keys[0];

    for (int i = 1; i < sibling->n; i++) sibling->keys[i-1] = sibling->keys[i];
    if (!sibling->leaf) {
        for (int i=1; i <= sibling->n; i++) sibling->C[i-1] = sibling->C[i];
    } 

    child->n++;
    sibling->n--;
    return;
}

void BTreeNode::merge(int index) {
    BTreeNode *child = C[index];
    BTreeNode *sibling = C[index+1];

    child->keys[t-1] = keys[index];
    for (int i = 0; i < sibling->n; i++) child->keys[i+t] = sibling->keys[i];
    if(!child->leaf) {
        for (int i = 0; i <= sibling->n; i++) child->C[i+t] = sibling->C[i];
    }

    for (int i = index + 1; i < n; i++) keys[i-1] = keys[i];
    for (int i = index + 2; i <= n; i++) C[i-1] = C[i];

    child->n += sibling->n+1;
    n--;
    delete sibling;
    return;
}

int main() {
    BTree t(3);

    int n = 30;
    vector<int> a(n);
    for(int i = 1; i < n + 1; ++i)
        a[i - 1] = i;
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(a), std::end(a), rng);

    for(auto x: a) {
        t.insert(x);
        t.print();
        cout << "--------------------" << endl;
    }
    for (auto x: a) {
        cout << "delete " << x << "\n\n";
        t.remove(x);
        t.print();
        cout << "--------------------" << endl;
    }
    return 0;
}
