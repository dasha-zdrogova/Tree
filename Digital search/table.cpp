#include <iostream>
#include <string>
#include "digital_search_tree.cpp"

void table(DSNode *node) {
    if (node->zero) table(node->zero);
    if (node->one) table(node->one);
    cout << node->key << ":" << node->data << endl;
}

int main() {
    auto t = DSTree();
    string filename;
    int n;
    cout << "filename:";
    cin >>  filename;
    freopen(filename.c_str(), "r", stdin);

    // Создание дерева
    while (cin >> n)
    {
        DSNode *node = t.search(n);
        if (node) node->data++;
        else t.insert(n, 1);
    }
    table(t.root);
    return 0;
}