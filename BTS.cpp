#include <iostream>

using namespace std;

const int INF = 1e18;

struct tree_node {
    tree_node *left, *right;
    int key, priority;
    
    tree_node(int new_key)
    {
        key = new_key;
        priority = rand() % INF;
        left = right = nullptr;
    }
};

pair<tree_node*, tree_node*> split(tree_node *root, int key) {
    if (root == nullptr)
        return make_pair(nullptr, nullptr);
    if (root->key < key)
    {
        pair<tree_node*, tree_node*> splitted = split(root->right, key);
        root->right = splitted.first;
        return make_pair(root, splitted.second);
    }
    else
    {
        pair<tree_node*, tree_node*> splitted = split(root->left, key);
        root->left = splitted.second;
        return make_pair(splitted.first, root);
    }
}

tree_node* merge(tree_node *left, tree_node *right) {
    if (left == nullptr || right == nullptr)
        return right == nullptr ? left : right;
    if (left->priority < right->priority) {
        right->left = merge(left, right->left);
        return right;
    }
    else
    {
        left->right = merge(left->right, right);
        return left;
    }
}

void insert(tree_node *&root, int key) {
    pair<tree_node*, tree_node*> splitted = split(root, key);
    root = merge(merge(splitted.first, new tree_node(key)), splitted.second);
}

void erase(tree_node *&root, int key) {
//    assert(root == nullptr);
//    if (root == nullptr) return;
    if (root->key < key)
        erase(root->right, key);
    else if (root->key > key)
        erase(root->left, key);
    else
        root = merge(root->left, root->right);
}

bool find(tree_node *root, int key) {
    if (root == nullptr) return false;
    if (root->key == key) return true;
    if (root->key < key)
        return find(root->right, key);
    else
        return find(root->left, key);
}

int main() {
    tree_node *node = nullptr;
    int key;
    string s;
    
    while (true) {
        s = "";
        cin >> s >> key;
        if(s == "") break;
        
        if (s == "insert") {
            if (!find(node, key)) {
                insert(node, key);
            }
        } else if (s == "delete") {
            if (find(node, key)) {
                erase(node, key);
            }
        } else find(node, key) ? cout << "true\n" : cout << "false\n";
    }
    
    return 0;
}
