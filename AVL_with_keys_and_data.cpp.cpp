#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

class Node {
    public:
    int key;
    int data;
    Node *left;
    Node *right;
    int height;
};

int get_height(Node *node){
    if (node == nullptr) return 0;
    return node->height;
}

int get_balance(Node *node){
    if (node == nullptr) return 0;
    return get_height(node->left) - get_height(node->right);
}

void print_all(Node* root, int offset = 0) {
    if (!root) return;
    print_all(root->right, offset+8);
    for (int i = 0; i < offset; ++i) cout << " ";
    cout << root->key << ":" << root->data << "\n";
    print_all(root->left, offset+8);
}

Node* new_node(int key, int data){
    Node* node = new Node();
    node->key = key;
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}

Node* right_rotate(Node* y) {
    Node* x = y->left;
    Node* z = x->right;

    x->right = y;
    y->left = z;
    
    y->height = max(get_height(y->left), get_height(y->right)) + 1;
    x->height = max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

Node* left_rotate(Node *x) {
    Node* y = x->right;
    Node* z = y->left;

    y->left = x;
    x->right = z;

    x->height = max(get_height(x->left), get_height(x->right)) + 1;
    y->height = max(get_height(y->left), get_height(y->right)) + 1;

    return y;
}

Node* rebalance(Node* node) {
    int balance = get_balance(node);
    if (balance > 1) {
        if (get_balance(node->left) < 0) node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1) {
        if (get_balance(node->right) > 0) node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}

Node *insert(Node* node, int key, int data){
    if (node == nullptr) return(new_node(key, data));
    if (key < node->key) node->left = insert(node->left, key, data);
    else if (key > node->key) node->right = insert(node->right, key, data);
    else return node;

    node->height = 1 + max(get_height(node->left), get_height(node->right));
    return rebalance(node);
}

Node* replacement(Node* node) {
    Node* current = node;
    while(current->left != nullptr) current = current->left;
    return current;
}

Node* del(Node* root, int key){
    if(root == nullptr) return root;
    if (key < root->key) root->left = del(root->left, key);
    else if (key > root->key) root->right = del(root->right, key);
    else {
        // Случай, когда у вершины менее 2 детей
        if (!(root->left) || !(root->right)) {
            Node* tmp = root->left? root->left : root->right;

            // Случай, когда у вершины вообще нет детей
            if (!(tmp)) {
                tmp = root;
                root = nullptr;
            }

            // Случай, когда у вершины есть один ребёнок
            else *root = *tmp;
            delete tmp;
        }
        else {
            // Случай, когда у вершины есть два ребёнка
            Node* tmp = replacement(root->right);
            root->key = tmp->key;
            root->data = tmp->data;
            root->right = del(root->right, tmp->key);
        }
    }

    if (!root) return root; // Возвращаем пустое дерево
    return rebalance(root);
}

int main(){
    Node* root = nullptr;

    int n = 30;
    vector<pair<int, int>> a(n);
    for(int i = 1; i < n + 1; ++i)
        a[i - 1] = {i, i+100};
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(a), std::end(a), rng);

    for(auto x: a)
        root = insert(root, x.first, x.second);
    print_all(root);


    for(auto x: a) {
        cout << "-------------------------" << endl;
        cout << "delete " << x.first << ":" << x.second << "\n\n" << endl;
        root = del(root, x.first);
        print_all(root);
    }
    return 0;
}
