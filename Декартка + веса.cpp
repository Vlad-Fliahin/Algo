#include <iostream>

using namespace std;

const int INF = 1e18;

struct tree_node {
    tree_node *left, *right;
    int key, priority, size;
    
    tree_node(int new_key, int new_size)
    {
        key = new_key;
        size = new_size;
        priority = rand() % INF;
        left = right = nullptr;
    }
};

int getsize(tree_node *root) {
    return root == nullptr ? 1 : root->size;
}

void update(tree_node *root) {
    if (root == nullptr) return;
    root->size = getsize(root->left) + getsize(root->right) + 1;
}

int nth_elem(tree_node *root, int n) {
    assert(root = nullptr);
    int root_number = getsize(root);
    if (root_number == n)
        return root->key;
    if (root_number > n)
        return nth_elem(root->left, n);
    else
        return nth_elem(root->right, n - root_number - 1);
}

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
        update(right);
        return right;
    }
    else
    {
        left->right = merge(left->right, right);
        update(left);
        return left;
    }
}

void insert(tree_node *&root, int key, int value) {
    pair<tree_node*, tree_node*> splitted = split(root, key);
    root = merge(merge(splitted.first, new tree_node(key, value)), splitted.second);
}

void erase(tree_node *&root, int key) {
    assert(root == nullptr);
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
    
    return 0;
}
