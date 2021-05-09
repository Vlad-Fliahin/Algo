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

struct Edge{
    ll from, to, id;
};

std::map<ll, std::map<ll, std::map<ll, ll>>> c, f;
ll excess[MAXN], position[MAXN];
bool blocked[MAXN], used[MAXN];

std::vector<Edge> g[MAXN];
std::vector<ll> d, p, c_min, topological_order;

bool bfs() {
    d = std::vector<ll>(n, INF);
    std::queue <ll> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        ll v = q.front();
        q.pop();
        for (auto edge : g[v]) {
            if (f[v][edge.to][edge.id] <
                c[v][edge.to][edge.id] && d[edge.to] == INF) {
                d[edge.to] = d[v] + 1;
                q.push(edge.to);
            }
        }
    }
    return d[t] != INF;
}

void topological_sort(ll v = s) {
    used[v] = true;
    for (auto edge : g[v]) {
        if (c[v][edge.to][edge.id] > 0 && !used[edge.to]) {
            topological_sort(edge.to);
        }
    }
    topological_order.push_back(v);
}

bool up(ll v) {
    bool blocking = true;
    bool smth_changed = false;
    for (auto edge : g[v]) {
        if (excess[v] == 0) {
            blocking = false;
            break;
        }
        if (position[v] < position[edge.to] && !blocked[edge.to] &&
            f[v][edge.to][edge.id] < c[v][edge.to][edge.id]) {
            
            ll extra_flow = std::min(excess[v], c[v][edge.to][edge.id] - f[v][edge.to][edge.id]);
            f[v][edge.to][edge.id] += extra_flow;
            f[edge.to][v][edge.id] -= extra_flow;
            excess[v] -= extra_flow;
            excess[edge.to] += extra_flow;
            smth_changed = true;
        }
    }
    if (blocking && v != t) blocked[v] = true;
    return smth_changed;
}

bool down(ll v) {
    bool smth_changed = false;
    for (auto edge : g[v]) {
        if (excess[v] == 0) break;
        if (position[edge.to] < position[v] && f[v][edge.to][edge.id] < 0) {
            ll declined_flow = std::min(excess[v], c[v][edge.to][edge.id] - f[v][edge.to][edge.id]);
            f[v][edge.to][edge.id] += declined_flow;
            f[edge.to][v][edge.id] -= declined_flow;
            excess[v] -= declined_flow;
            excess[edge.to] += declined_flow;
            smth_changed = true;
        }
    }
    
    return smth_changed;
}

void Karsanov() {
    memset(used, false, sizeof(used));
    memset(excess, 0, sizeof(excess));
    memset(blocked, false, sizeof(blocked));
    
    topological_order = {t};
    used[t] = true;
    topological_sort();
    
    std::reverse(topological_order.begin(), topological_order.end());
    for (ll i = 0; i < n; ++i) {
        position[topological_order[i]] = i;
    }
    
    excess[s] = INF;
    
    ll smth_changed = true;
    while (smth_changed) {
        smth_changed = false;
        for (ll i = 0; i < n; ++i) {
            ll v = topological_order[i];
            if (!blocked[v] && excess[v]) {
                smth_changed |= up(v);
            }
        }
        
        for (ll i = n - 1; i >= 0; --i) {
            ll v = topological_order[i];
            if (blocked[v] && excess[v]) {
                smth_changed |= down(v);
            }
        }
    }
}

void Dinic() {
    while (bfs()) {
        Karsanov();
    }

    ll max_flow = 0;
    for (auto edge : g[s]) {
        max_flow += f[s][edge.to][edge.id];
    }
    
    std::cout << max_flow << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    std::cin >> n >> m;
    for (ll i = 1, a, b, cf; i <= m; ++i) {
        std::cin >> a >> b >> cf;
        --a; --b;
        g[a].push_back(Edge({a, b, i}));
        g[b].push_back(Edge({b, a, i}));
        c[a][b][i] = cf;
        c[b][a][i] = f[a][b][i] = f[b][a][i] = 0;
    }

    s = 0;
    t = n - 1;
    Dinic();
}
