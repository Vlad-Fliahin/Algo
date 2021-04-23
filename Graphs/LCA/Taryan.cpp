#include <iostream>
#include <cstring>
#include <vector>
#include <queue>

const int MAXN = 50005;

int n, m;
int p[MAXN], sz[MAXN], ancestor[MAXN];
bool used[MAXN];
std::vector<int> g[MAXN];
std::vector<std::pair<int, int>> q[MAXN];
int query[MAXN];

void make_set(int v) {
    p[v] = v;
    ancestor[v] = v;
    sz[v] = 1;
}

int find_set(int v) {
    if (p[v] == v)
        return v;
    return p[v] = find_set(p[v]);
}

void union_set(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (sz[a] < sz[b])
            std::swap(a, b);
        sz[a] += sz[b];
        p[b] = a;
    }
}

void dfs(int v) {
    make_set(v);
    for (auto to : g[v]) {
        dfs(to);
        union_set(v, to);
        ancestor[find_set(v)] = v;
    }

    for (auto to_id : q[v]) {
        auto to = to_id.first;
        auto id = to_id.second;
        if (used[to]) {
            query[id] = ancestor[find_set(to)];
        }
    }
    used[v] = true;
}

int main()
{
    std::cin >> n;
    for (int i = 1, parent; i < n; ++i) {
        std::cin >> parent;
        g[parent - 1].push_back(i);
    }

    std::cin >> m;
    for (int i = 0, v1, v2; i < m; ++i) {
        std::cin >> v1 >> v2;
        --v1; --v2;

        if (v1 == v2) {
            query[i] = v1;
        }
        else {
            q[v1].push_back(std::make_pair(v2, i));
            q[v2].push_back(std::make_pair(v1, i));
        }
    }

    dfs(0);
    
    for (int i = 0; i < m; ++i) {
        std::cout << query[i] + 1 << '\n';
    }
}
