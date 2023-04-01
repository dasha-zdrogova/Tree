#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
using namespace std;

template <typename K, typename D>
class Node {
    public:
    K key;
    D data;
    Node *left;
    Node *right;
    int height;
    Node(K key, D data): key(key), data(data) {}
};

template <typename K>
bool f(K key1, K key2) {
    return (key1 < key2);
}

template <typename K, typename D>
void print_all(Node <K, D> *root, int offset = 0) {
    if (!root) return;
    print_all(root->right, offset+5);
    for (int i = 0; i < offset; ++i) cout << " ";
    cout << root->key << ":" << root->data << "\n";
    print_all(root->left, offset+5);
}

template <typename K, typename D>
Node <K, D> *new_node(K key, D data){
    Node <K, D> *node = new Node(key, data);
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

template <typename K, typename D>
Node <K, D> *search(Node <K, D> *root, K key) {
    if (!root) return nullptr;
    if (root->key == key) return root;
    if (!f(root->key, key)) return search(root->left, key);
    if (f(root->key, key)) return search(root->right, key);
    return nullptr;
}

template <typename K, typename D>
Node <K, D> *insert(Node <K, D> *node, K key, D data){
    if (node == nullptr) return(new_node(key, data));
    if (f(key, node->key) && key != node->key) node->left = insert(node->left, key, data);
    else if (!f(key, node->key) && key != node->key) node->right = insert(node->right, key, data);
    return node;
}

template <typename K, typename D>
Node <K, D> *replacement(Node <K, D> *node) {
    Node <K, D> *current = node;
    while(current->left != nullptr) current = current->left;
    return current;
}

template <typename K, typename D>
Node <K, D> *del(Node <K, D> *root, K key){
    if(root == nullptr) return root;
    if (f(key, root->key) && key != root->key) root->left = del(root->left, key);
    else if (!f(key, root->key) && key != root->key) root->right = del(root->right, key);
    else {
        // Случай, когда у вершины менее 2 детей
        if (!(root->left) || !(root->right)) {
            Node <K, D> *tmp = root->left? root->left : root->right;

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
            Node <K, D> *tmp = replacement(root->right);
            root->key = tmp->key;
            root->data = tmp->data;
            root->right = del(root->right, tmp->key);
        }
    }
    return root;
}
/*
int main(){
    Node <string, int> *root = nullptr;
    int n = 27;
    vector <string> alph = {"a", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    vector<pair<string, int>> a(n);
    for(int i = 1; i < n + 1; ++i)
        a[i - 1] = {alph[i-1], i};
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
}*/
