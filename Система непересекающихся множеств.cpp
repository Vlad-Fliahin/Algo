#include <iostream>

using namespace std;

const int NMAX = 1e6;

int parent [NMAX];
int size [NMAX];

void make_set(int v) {
    parent[v] = v;
    size[v] = 1;
}

int find_set(int v) {
    if (parent[v] == v)
        return v;
    else
        return parent[v] = find_set(parent[v]);
}

void union_sets(int v1, int v2) {
    int a = find_set(v1);
    int b = find_set(v2);
    
    if (a != b) {
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
}

int main() {
    
    return 0;
}
