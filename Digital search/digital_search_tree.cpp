#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class DSNode {
public:
    int key;
    int data;
    DSNode* zero;
    DSNode* one;

    DSNode(int key, int data): key(key), data(data) {
        zero = nullptr;
        one = nullptr;
    }

    int digit(int x, int n) {
        return (x >> n) & 1;
    }

    DSNode *search(int key, int n = 0) {
        if (!this) return nullptr;
        if (this->key == key) return this;
        if (digit(key, n)) return this->one->search(key, n+1);
        else return this->zero->search(key, n+1);
    }

    void insert(int key, int data, int n = 0){
        if (digit(key, n)) {
            if (this->one) {
                return this->one->insert(key, data, n+1);
            } else {
                DSNode* new_node = new DSNode(key, data);
                this->one = new_node;
            }
        } else {
            if (this->zero) {
                return this->zero->insert(key, data, n+1);
            } else {
                DSNode* new_node = new DSNode(key, data);
                this->zero = new_node;
            }
        }
    }

    DSNode *replacement(DSNode *node) {
        DSNode *current = node;
        while(current->zero) current = current->zero;
        return current;
    }

    DSNode *del(DSNode *root, int key, int n = 0) {
        if (root->key == key) {
            // у вершины менее 2 детей
            if (!root->zero || !root->one) {
                DSNode *tmp = root->zero ? root->zero : root->one;
                if (!tmp){
                    tmp = root;
                    root = nullptr;
                } 
                else *root = *tmp;
                delete tmp;
            } else {
                // у вершины есть 2 ребёнка
                DSNode *tmp = replacement(root->one);
                root->key = tmp->key;
                root->data = tmp->data;
                root->one = del(root->one, tmp->key, n+1);
            } 
        } else {
            if (digit(key, n)) {
                if (root->one) root->one = del(root->one, key, n+1);
                else return nullptr;
            } else {
                if (root->zero) root->zero = del(root->zero, key, n+1);
                else return nullptr;
            }
        }
        return root;
    }
};

class DSTree {
private:

public:
    DSNode* root;

    DSTree() {
        root = nullptr;
    };

    void print(DSNode* root, int offset = 0) {
        if (!root) return;
        print(root->one, offset+8);
        for (int i = 0; i < offset; ++i) cout << " ";
        cout << root->key << ":" << root->data << "\n";
        print(root->zero, offset+8);
    }

    DSNode *search(int key) {
        if (root == nullptr) return nullptr;
        else return root->search(key);
    }

    void insert(int key, int data) {
        if (!root) {
            root = new DSNode(key, data);
            return;
        }
        return root->insert(key, data);
    }

    void del(int key){
        if(!root) return;
        if (root->search(key)) root = root->del(root, key);
        else return;
    }

};

/*
int main() {
    auto t = DSTree();
    int n = 10;
    vector <pair<int,int>> a(n);
    for (int i = 1; i < n+1; i++) {
        a[i-1] = {i, i+100};
    }
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(a), std::end(a), rng);

    for (auto x:a) {
        t.insert(x.first, x.second);
    }

    for (auto x:a) {
        if (!t.search(x.first)) cout << "problem" << endl;
    }

    for (auto x:a) {
        if (t.search(x.first+100)) cout << "problem" << endl;
    }
    t.print(t.root);
    for (auto x:a) {
        cout << x.first << " del" << endl;
        t.del(x.first);
        t.print(t.root);
        cout << "----------------------------------" << endl;
    }
    
    //t.print(t.root);
    return 0;
}
*/