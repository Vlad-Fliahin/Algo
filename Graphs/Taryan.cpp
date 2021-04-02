#include <iostream>
#include <vector>
#include <unordered_map>

#define ll int64_t

const int MAXN = 150005;

struct vertex {
    int parent = 0;
    bool used = false;
    ll weight = 0;
};

std::vector <std::vector<std::pair<int, int>>> g(MAXN);
std::vector <std::vector<std::pair<int, int>>> queries(MAXN);
std::vector <ll> answers;
std::vector <vertex> vertexes(MAXN);

int n, m;
int ancestor[MAXN], p[MAXN], size[MAXN];

void make_set(int v) {
    p[v] = v;
    ancestor[v] = v;
}

int find_parent(int v) {
    if (p[v] == v)
        return v;
    return p[v] = find_parent(p[v]);
}

void union_sets(int v, int u) {
    int a = find_parent(v);
    int b = find_parent(u);
    if (a != b) {
        if (size[a] < size[b])
            std::swap(a, b);
        size[a] += b;
        p[b] = a;
    }
}

void dfs(int v) {
    vertexes[v].used = true;
    make_set(v);
    for (auto [to, w] : g[v]) {
        if (!vertexes[to].used) {
            vertexes[to].parent = v;
            vertexes[to].weight = vertexes[v].weight + w;
            dfs(to);

            union_sets(v, to);
            ancestor[find_parent(v)] = v;
        }
    }

    for (auto [u, i] : queries[v]) {
        if (vertexes[u].used) {
            int lca = ancestor[find_parent(u)];
            answers[i] = vertexes[u].weight +
                         vertexes[v].weight -
                         2*vertexes[lca].weight;
        }
    }
}

int main()
{
    std::cin >> n;
    for (int i = 1, u, v, w; i < n; ++i) {
        std::cin >> u >> v >> w;
        g[u].push_back(std::make_pair(v, w));
        g[v].push_back(std::make_pair(u, w));
    }

    std::cin >> m;
    for (int i = 0, u, v; i < m; ++i) {
        std::cin >> u >> v;
        queries[u].push_back(std::make_pair(v, i));
        queries[v].push_back(std::make_pair(u, i));
    }

    answers = std::vector<ll>(m);
    dfs(0);

    for (auto len : answers)
        std::cout << len << '\n';

    return 0;
}
