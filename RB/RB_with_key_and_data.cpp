#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

enum COLOR {RED, BLACK};

struct Item {
    int key;
    int data;

    Item (int key = 0, int data = 0):
        key(key), data (data) {}
};

class Node{
public:
    Item elem;
    COLOR color;
    Node *left;
    Node *right;
    Node *parent;

    Node(Item elem): elem(elem) {
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    void print(int offset) {
        if (!this) return;
        right->print(offset+5);
        for (int i = 0; i < offset; ++i) cout << " ";
        cout << elem.key << ":" << (color == 0 ? "R" : "B") << "\n";
        left->print(offset+5);
    }

    Node* uncle() {
        if (!parent || !parent->parent) return nullptr;
        if (parent->parent->left == parent) return parent->parent->right;
        else return parent->parent->left;
    }

    Node* sibling() {
        if (!parent) return nullptr;
        if(parent->left == this) return parent->right;
        else return parent->left;
    }

    bool has_red_child() {
        return (left && left->color == RED) or (right && right->color == RED);
    }
    
};

class RBTree {
    Node* root;

    void left_rotate(Node *x) {
        Node* p = x->parent;
        Node* y = x->right;
        Node* z = y->left;

        y->left = x;
        x->right = z;
        x->parent = y;
        y->parent = p;
        if (z) z->parent = x;
        if(p) {
            if (p->left == x) p->left = y;
            else p->right = y;
        } else root = y;
    }

    void right_rotate(Node* y) {
        Node* p = y->parent;
        Node* x = y->left;
        Node* z = x->right;

        x->right = y;
        y->left = z;
        y->parent = x;
        x->parent = p;
        if (z) z->parent = y;
        if (p) {
            if (p->left == y) p->left = x;
            else p->right = x;
        } else root = x;
    }

    void swap_color(Node* x1, Node* x2) {
        COLOR tmp = x1->color;
        x1->color = x2->color;
        x2->color = tmp;
    }

    void swap_elem(Node* x1, Node* x2) {
        Item tmp = x1->elem;
        x1->elem = x2->elem;
        x2->elem = tmp;
    }

    Node *search_(int key) {
        Node* tmp = root;
        while(tmp) {
            if (key < tmp->elem.key) {
                if (!tmp->left) break;
                else tmp = tmp->left;
            }
            else if (key == tmp->elem.key) break;
            else { 
                if (!tmp->right) break;
                else tmp = tmp->right;
            }
        }
        return tmp;
    }

    Node* succestor(Node* x) {
        Node* current = x;
        while(current->left) current = current->left;
        return current;
    }

    Node* replacement(Node* x) {
        if (x->left && x->right) return succestor(x->right);
        if (!x->left && !x->right) return nullptr;
        if (x->left) return x->left;
        else return x->right;
    }

    void fix_red(Node* x) {
        if (x == root) {
            x->color = BLACK;
            return;
        }
        
        Node* parent = x->parent;
        Node *grandparent = parent->parent;
        Node* uncle = x->uncle();

        // Отец и сын красные
        if (parent->color == RED) {
            // Дядя красный
            if (uncle && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fix_red(grandparent);

            // Дядя чёрный
            } else {
                if (grandparent->left == parent) {
                    if (parent->left == x) swap_color(parent, grandparent);
                    else {
                        left_rotate(parent);
                        swap_color(x, grandparent);
                    }
                    right_rotate(grandparent);
                } else {
                    if (parent->right == x) swap_color(parent, grandparent);
                    else {
                        right_rotate(parent);
                        swap_color(x, grandparent);
                    }
                    left_rotate(grandparent);
                }
            }
        }
    }
    
    void erase(Node *v) {
        Node *u = replacement(v);
        bool uvBlack = ((!u || u->color == BLACK) && (v->color == BLACK));
        Node *parent = v->parent;

        // удаляемый узел - лист
        if (!u) {
            // удаляемый лист - корень 
            if (v == root) root = nullptr;
            // удаляемый лист - не корень
            else {
                // удаляемый лист - чёрный
                if (uvBlack) fix_black(v);
                // удаляемый лист - красный
                else {
                    if (v->sibling()) v->sibling()->color = RED;
                }
                // удаляем лист
                if (v->parent->left == v) parent->left = nullptr;
                else parent->right = nullptr;
            }
            delete v;
            return;
        }

        // У удаляемого узла есть ровно один ребёнок
        if (!v->left || !v->right) {
            // Удаляемый узел - корень
            if (v == root) {
                v->elem = u->elem;
                v->left = v->right = nullptr;
                delete u;
            }
            // Удаляемый узел - не корень
            else {
                if (v->parent->left == v) parent->left = u;
                else parent->right = u;
                delete v;
                u->parent = parent;
                if (uvBlack) {
                    cout << "выполнено"<< endl;
                    fix_black(u);
                }
                else u->color = BLACK;
            }
            return;
        }
        // У удаляемого узла 2 ребёнка
        swap_elem(u, v);
        erase(u);
    }

    void fix_black (Node* x) {
        if (x == root) return;
        
        Node* sibling = x->sibling();
        Node* parent = x->parent;

        if (!sibling) fix_black(parent);
        else {
            if(sibling->color == RED) {
                parent->color = RED;
                sibling->color = BLACK;
                if (sibling->parent->left == sibling) right_rotate(parent);
                else left_rotate(parent);
                fix_black(x);
            } else {
                if (sibling->has_red_child()) {
                    if (sibling->left && sibling->left->color == RED) {
                        if (sibling->parent->left == sibling) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            right_rotate(parent);
                        } else {
                            sibling->left->color = parent->color;
                            right_rotate(sibling);
                            left_rotate(parent);
                        }
                    } else {
                        if (sibling->parent->left == sibling) {
                            sibling->right->color = parent->color;
                            left_rotate(sibling);
                            right_rotate(parent);
                        } else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            left_rotate(parent);
                        }
                    }
                    parent->color = BLACK;
                } else {
                    // У sibling 2 чёрных ребёнка
                    sibling->color = RED;
                    if (parent->color == BLACK) fix_black(parent);
                    else parent->color = BLACK;
                }
            }
        }
    }

public:
    RBTree() {root = nullptr;}
    Node* get_root() {return root;}

    void print(int offset = 0) {
        if (root) root->print(offset);
    }

    int *search(int key) {
        Node* tmp = root;
        while(tmp) {
            if (key < tmp->elem.key) tmp = tmp->left;
            else if (key == tmp->elem.key) return &tmp->elem.data;
            else tmp = tmp->right;
        }
        return nullptr;
    }

    void insert(Item n) {
        int key = n.key;
        Node* new_node = new Node(n);
        if (!root) {
            new_node->color = BLACK;
            root = new_node;
        } else {
            Node *tmp = search_(key);

            if (tmp->elem.key == key) return;
            new_node->parent = tmp;
            if (key < tmp->elem.key) tmp->left = new_node;
            else tmp->right = new_node;
            fix_red(new_node);
        }
    }

    void del(int k) {
        if (!root) return;
        Node *v = search_(k);
        if (v->elem.key != k) return;
        else erase(v);
    }

};

// int main() {
//     RBTree t;
//     int n = 10;
//     vector <pair<int,int>> a(n);
//     for (int i = 1; i < n+1; i++) {
//         a[i-1] = {i, i+100};
//     }
//     auto rng = std::default_random_engine {};
//     std::shuffle(std::begin(a), std::end(a), rng);

//     for (auto x:a) {
//         t.insert(Item(x.first, x.second));
//     }
//     t.print();
//     for (auto x:a) {
//         t.del(x.first);
//         cout << "------------------------------" << endl;
//         cout << x.first << " del" << endl;
//         t.print();
//     }
//     return 0;
// }