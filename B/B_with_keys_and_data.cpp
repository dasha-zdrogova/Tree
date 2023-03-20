#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

struct Item {
    int key;
    int data;
    Item(int key = 0, int data = 0):
        key(key), data(data){}
};

class BTreeNode {
    Item* elems;
    int t;

    BTreeNode** C;
    int n;
    bool leaf;

public:
    BTreeNode(int _t, bool _leaf);
    void print(int offset = 0);

    BTreeNode *search(int k);
    int find_key(int k);
    void insert_non_full(int k, int data);
    void split(int i, BTreeNode *y);

    void remove(int k);
    void remove_from_leaf(int index);
    void remove_from_non_leaf(int index);
    Item get_pred(int index);
    Item get_succ(int index);
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

    void insert(int k, int d);
    void remove(int k);
};

BTreeNode::BTreeNode(int _t, bool _leaf){
    t = _t;
    leaf = _leaf;
    elems = new Item[2*t-1];
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
            cout << elems[i].key << ":" << elems[i].data << "\n";
        }
        if (!leaf) {
            C[0]->print(level + 1);
        }
    }
}

int BTreeNode::find_key(int k) {
    int index = 0;
    while (index < n && elems[index].key < k) index++;
    return index;
}

BTreeNode* BTreeNode::search(int k) {
    int i = this->find_key(k);
    if (elems[i].key == k) return this;
    if (leaf) return nullptr;
    return C[i]->search(k);
}

void BTree::insert(int k, int d) {
    if (!root) {
        root = new BTreeNode(t, true);
        root->elems[0] = Item(k, d);
        root->n = 1;
    }
    else {
        if (root->n == 2*t - 1) {
            BTreeNode *new_root = new BTreeNode(t, false);
            new_root->C[0] = root;
            new_root->split(0, root);

            int i = 0;
            if (new_root->elems[0].key < k) i++;
            new_root->C[i]->insert_non_full(k, d);
            root = new_root;
        }
        else root->insert_non_full(k, d);
    }
}

void BTreeNode::insert_non_full(int k, int d) {
    int i = n-1;

    if (leaf) {
        while (i >= 0 && elems[i].key > k) {
            elems[i+1] = elems[i]; i--;
        }
        elems[i+1] = Item(k, d); n++;
    }
    else {
        while (i >= 0 && elems[i].key > k) i--;
        if (C[i+1]->n == 2*t-1) {
            split(i+1, C[i+1]);
            if (elems[i+1].key < k) i++;
        }
        C[i+1]->insert_non_full(k, d);
    }
}

void BTreeNode::split(int i,BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t-1;

    for (int j = 0; j < t-1; j++) z->elems[j] = y->elems[j+t];
    if(!(y->leaf)) for (int j = 0; j < t; j++) z->C[j] = y->C[j+t];

    y->n = t-1;

    for (int j = n; j >= i+1; j--) C[j+1] = C[j];
    C[i+1] = z;

    for (int j = n-1; j >= i; j--) elems[j+1] = elems[j];
    elems[i] = y->elems[t-1];

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
    if (index < n && elems[index].key == k) {
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
    for (int i = index+1; i < n; i++) elems[i-1] = elems[i];
    n--;
    return;
}

void BTreeNode::remove_from_non_leaf(int index) {
    int k = elems[index].key;
    if (C[index]->n >= t) {
        Item pred = get_pred(index);
        elems[index] = pred;
        C[index]->remove(pred.key);
    }
    else if (C[index+1]->n >= t) {
        Item succ = get_succ(index);
        elems[index] = succ;
        C[index+1]->remove(succ.key);
    }
    else {
        merge(index);
        C[index]->remove(k);
    }
    return;
}

Item BTreeNode::get_pred(int index) {
    BTreeNode *current = C[index];
    while (!current->leaf) current = current->C[current->n];
    return current->elems[current->n-1];
}

Item BTreeNode::get_succ(int index) {
    BTreeNode *current = C[index+1];
    while (!current->leaf) current = current->C[0];
    return current->elems[0];
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

    for (int i = child->n-1; i >= 0; i--) child->elems[i+1] = child->elems[i];
    if (!child->leaf){
        for (int i = child->n; i >= 0; i--) child->C[i+1] = child->C[i];
    }
    child->elems[0] = elems[index-1];
    
    if(!child->leaf) child->C[0] = sibling->C[sibling->n];

    elems[index-1] = sibling->elems[sibling->n-1];
    child->n++;
    sibling->n--;
    return;
}

void BTreeNode::borrow_from_next(int index) {
    BTreeNode *child = C[index];
    BTreeNode *sibling = C[index+1];

    child->elems[child->n] = elems[index];

    if(!child->leaf) child->C[child->n+1] = sibling->C[0];
    
    elems[index] = sibling->elems[0];

    for (int i = 1; i < sibling->n; i++) sibling->elems[i-1] = sibling->elems[i];
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

    child->elems[t-1] = elems[index];
    for (int i = 0; i < sibling->n; i++) child->elems[i+t] = sibling->elems[i];
    if(!child->leaf) {
        for (int i = 0; i <= sibling->n; i++) child->C[i+t] = sibling->C[i];
    }

    for (int i = index + 1; i < n; i++) elems[i-1] = elems[i];
    for (int i = index + 2; i <= n; i++) C[i-1] = C[i];

    child->n += sibling->n+1;
    n--;
    delete sibling;
    return;
}

int main() {
    BTree t(3);

    int n = 30;
    vector<pair<int, int>> a(n);
    for(int i = 1; i < n + 1; ++i)
        a[i - 1] = {i, 100+i};
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(a), std::end(a), rng);

    for(auto x: a) {
        t.insert(x.first, x.second);
        t.print();
        cout << "--------------------------------" << endl;
    }

    for (auto x: a) {
        cout << "delete " << x.first << "\n\n";
        t.remove(x.first);
        t.print();
        cout << "--------------------------------" << endl;
    }
    return 0;
}
