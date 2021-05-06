#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <queue>

typedef int64_t ll;

const ll MAXN = 500, INF = 1e18;
ll n, m, s, t;

bool used[MAXN];

ll c[MAXN][MAXN], f[MAXN][MAXN];
std::vector<ll> g[MAXN];

ll bfs() {
    std::vector<ll> d(n, INF), p(n), c_min(n, INF);
    std::queue <ll> q;
    d[s] = 0;
    used[s] = true;
    q.push(s);
    while (!q.empty()) {
        ll v = q.front();
        q.pop();
        for (auto to : g[v]) {
            if (!used[to] && f[v][to] < c[v][to]) {
                used[to] = true;
                p[to] = v;
                d[to] = d[v] + 1;
                c_min[to] = std::min(c_min[v], c[v][to] - f[v][to]);
                q.push(to);
            }
        }
    }
    
    if (c_min[t] == INF)
        return 0;
    
    for (ll v = t; v != s; v = p[v]) {
        f[p[v]][v] += c_min[t];
        f[v][p[v]] -= c_min[t];
    }
    
    return c_min[t];
}

void Edmonds_Karp() {
    ll max_flow = 0;
    while (true) {
        memset(used, false, sizeof(used));
        ll additional_flow = bfs();
        if (additional_flow == 0) break;
        max_flow += additional_flow;
    }

    std::cout << max_flow << '\n';
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
    Edmonds_Karp();
}

/*
 4 5
 1 3 100
 3 2 1
 2 4 100
 1 2 100
 3 4 100
 */

