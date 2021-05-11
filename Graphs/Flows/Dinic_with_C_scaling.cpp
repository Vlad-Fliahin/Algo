#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <queue>

typedef int64_t ll;

const ll MAXN = 500, MAXC = 1 << 30, INF = 1e18;
ll n, m, s, t, max_flow, C = MAXC;

ll c[MAXN][MAXN], f[MAXN][MAXN], active[MAXN];

std::vector<ll> g[MAXN];
std::vector<ll> d;

ll dfs(ll v = s, ll min_c = INF) {
    if (v == t || min_c == 0)
        return min_c;
    for (ll i = active[v]; i < g[v].size(); ++i) {
        ll to = g[v][i];
        if (d[to] == d[v] + 1) {
            ll delta = dfs(to, std::min(min_c, c[v][to] - f[v][to]));
            if (delta > 0) {
                f[v][to] += delta;
                f[to][v] -= delta;
                return delta;
            }
        }
        ++active[v];
    }
    return 0;
}

bool bfs() {
    d = std::vector<ll>(n, INF);
    std::queue <ll> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        ll v = q.front();
        q.pop();
        for (auto to : g[v]) {
            if (f[v][to] < c[v][to] && d[to] == INF && (c[v][to] >= C || std::abs(f[v][to]) >= C)) {
                d[to] = d[v] + 1;
                q.push(to);
            }
        }
    }
    return d[t] != INF;
}

void Dinic() {
    while (bfs()) {
        memset(active, 0, sizeof(active));
        ll flow = dfs();
        while (flow != 0) {
            max_flow += flow;
            flow = dfs();
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    std::cin >> n >> m;
    for (ll i = 0, a, b, cf; i < m; ++i) {
        std::cin >> a >> b >> cf;
        --a; --b;
        g[a].push_back(b);
        g[b].push_back(a);
        c[a][b] += cf;
    }

    s = 0;
    t = n - 1;
    
    while (C) {
        Dinic();
        C >>= 1;
    }
    std::cout << max_flow << "\n";
}
