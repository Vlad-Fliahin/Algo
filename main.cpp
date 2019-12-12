#include <iostream>
#include <vector>

using namespace std;

struct tree_node {
    tree_node *left, *right;
    int key;
    
    tree_node(int new_key) {
        key = new_key;
        left = right = nullptr;
    }
};

bool search(tree_node *v, int key) {
    if (v == nullptr) return false;
    if (v->key == key) return true;
    if (key < v->key)
        return search(v->left, key);
    else
        return search(v->right, key);
}

void insert(tree_node *&v, int key) {
    if (v == nullptr) {
        v = new tree_node(key);
        return;
    }
    if (key < v->key)
        return insert(v->left, key);
    else
        return insert(v->right, key);
}

tree_node getMin(tree_node *&v, tree_node *&p) {
    if (v->left != nullptr)
        return getMin(v->left, v);
    else if (v->right == nullptr)
        return *v;
    else
        p->left = v->right;
        return *v;
}

void deylete(tree_node *&v, int key) {
    if (v == nullptr) return;
    if (v->key == key) {
        tree_node t = getMin(v, v);
        v->key = t.key;
        return;
    }
    if (key < v->key)
        deylete(v->left, key);
    else
        deylete(v->right, key);
}

int main() {
    tree_node *node = nullptr;
    int key;
    string s;
    
    while (true) {
        s = "0";
        cin >> s >> key;
        if(s == "0") break;
        
        if (s == "insert")
            insert(node, key);
        else if (s == "delete")
            deylete(node, key);
        else
            search(node, key) ? cout << "true\n" : cout << "false\n";
    }
    return 0;
}
