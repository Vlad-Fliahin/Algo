#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>

typedef int64_t ll;

const ll MAXN = 10, INF = 1e9;
ll n, m, s, t;

bool used[MAXN];

ll c[MAXN][MAXN], f[MAXN][MAXN];
std::vector<ll> g[MAXN];

ll dfs(ll v = s, ll c_min = INF) {
    if (v == t)
        return c_min;
    used[v] = true;
    for (auto to : g[v]) {
        if (!used[to] && f[v][to] < c[v][to]) {
            ll delta = dfs(to, std::min(c_min, c[v][to] - f[v][to]));
            if (delta > 0) {
                f[v][to] += delta;
                f[to][v] -= delta;
                return delta;
            }
        }
    }
    return 0;
}

void solve() {
    ll max_flow = 0;
    while (true) {
        memset(used, false, sizeof(used));
        ll additional_flow = dfs();
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
        c[b][a] += cf;
    }

    s = 0;
    t = n - 1;
    solve();
}

/*
 4 5
 1 3 100
 3 2 1
 2 4 100
 1 2 100
 3 4 100
 */
 
