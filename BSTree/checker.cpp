#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
#include "binary_search_tree.cpp"
using namespace std;

template <typename K, typename D>
bool checker (Node <K, D> *root, K *l, K *g) {
    if (root == nullptr || l == nullptr || g == nullptr) {
        cout << "empty tree of order error" << endl;
    }
    
    *l = *g = root->key;

    if (root->left){
        K left_l, left_g;
        if (!checker(root->left, &left_l, &left_g)) return false;
        if (!f(left_l, left_g) && left_l != left_g) cout << "order_error1" << endl;
        if (!f(left_g, root->key)) return false;
        *l = left_l;
    }

    if (root->right){
        K right_l, right_g;
        if (!checker(root->right, &right_l, &right_g)) return false;
        if (!f(right_l, right_g) && right_l != right_g) cout << "order_error2" << endl;
        if (f(right_l, root->key) || right_l == root->key) return false;
        *g = right_g;
    }
    return true;
}

int main() {
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


    string full_l, full_g;
    if (root == nullptr || checker(root, &full_l, &full_g))
        cout << "the tree is correct" << endl;
    else cout << "the tree is wrong" << endl;

    for(auto x: a) {
        root = del(root, x.first);
    }

    if (root == nullptr || checker(root, &full_l, &full_g))
        cout << "the tree is correct" << endl;
    else cout << "the tree is wrong" << endl;
    return 0;
}