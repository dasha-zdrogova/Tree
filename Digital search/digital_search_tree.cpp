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

};

class DSTree {
private:

public:
    DSNode* root;


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

};

int main() {
    DSTree t;
    int n = 24;
    vector <pair<int,int>> a(n);
    for (int i = 1; i < n+1; i++) {
        a[i-1] = {i, i+100};
    }
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(a), std::end(a), rng);

    for (auto x:a) {
        t.insert(x.first, x.second);
    }
    t.print(t.root);
    return 0;
}