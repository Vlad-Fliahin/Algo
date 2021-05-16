#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <chrono>
#include <random>

typedef int64_t ll;

const ll MAXN = 500, INF = 1e18;
ll n, m, s, t;

struct Edge{
    ll from, to, id;
    bool deleted;
};

std::map<ll, std::map<ll, std::map<ll, ll>>> c, f;
ll pin[MAXN], pout[MAXN], p[MAXN];
bool blocked[MAXN], used[MAXN], deleted[MAXN];

std::vector<Edge> g[MAXN];
std::vector<ll> d, to_nullify;

bool bfs() {
    d = std::vector<ll>(n, INF);
    std::queue <ll> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty()) {
        ll v = q.front();
        q.pop();
        for (auto edge : g[v]) {
            if (edge.deleted) continue;
            if (f[v][edge.to][edge.id] <
                c[v][edge.to][edge.id] && d[edge.to] == INF) {
                d[edge.to] = d[v] + 1;
                q.push(edge.to);
            }
            if (d[edge.to] == d[v] + 1 && f[v][edge.to][edge.id] < c[v][edge.to][edge.id] && c[v][edge.to][edge.id] > 0) {
                pout[v] += c[v][edge.to][edge.id] - f[v][edge.to][edge.id];
                pin[edge.to] += c[v][edge.to][edge.id] - f[v][edge.to][edge.id];
            }
        }
    }
    return d[t] != INF;
}

void nullify(ll v) {
    deleted[v] = true;
    p[v] = pin[v] = pout[v] = 0;
    for (auto edge: g[v]) {
        if (edge.deleted) continue;
        if (deleted[edge.to]) continue;
        if (d[edge.to] == d[v] + 1 && c[v][edge.to][edge.id] > 0) {
            pin[edge.to] -= c[v][edge.to][edge.id] - f[v][edge.to][edge.id];
            if (pin[edge.to] == 0) {
                to_nullify.push_back(edge.to);
            }
        }
        if (d[edge.to] == d[v] - 1 && c[edge.to][v][edge.id] > 0) {
            pout[edge.to] -= c[edge.to][v][edge.id] - f[edge.to][v][edge.id];
            if (pout[edge.to] == 0) {
                to_nullify.push_back(edge.to);
            }
        }
    }
}

void up(ll v, ll flow) {
    if (v == t) return;
    for (auto &edge : g[v]) {
        if (flow == 0) break;
        if (deleted[edge.to] || edge.deleted) continue;
        if (d[edge.to] == d[v] + 1 && c[v][edge.to][edge.id] > 0) {
            ll possible_flow = std::min(flow, c[v][edge.to][edge.id] - f[v][edge.to][edge.id]);
            f[v][edge.to][edge.id] += possible_flow;
            f[edge.to][v][edge.id] -= possible_flow;
            
            pout[v] -= possible_flow;
            pin[edge.to] -= possible_flow;
            if (pin[edge.to] == 0) {
                to_nullify.push_back(edge.to);
            }
            
            up(edge.to, possible_flow);
            flow -= possible_flow;
            
            if (f[v][edge.to][edge.id] == c[v][edge.to][edge.id])
                edge.deleted = true;
        }
    }
}

void down(ll v, ll flow) {
    if (v == s) return;
    for (auto &edge : g[v]) {
        if (flow == 0) break;
        if (deleted[edge.to] || edge.deleted) continue;
        if (d[edge.to] == d[v] - 1 && c[edge.to][v][edge.id] > 0) {
            ll possible_flow = std::min(flow, c[edge.to][v][edge.id] - f[edge.to][v][edge.id]);
            f[edge.to][v][edge.id] += possible_flow;
            f[v][edge.to][edge.id] -= possible_flow;
            
            pin[v] -= possible_flow;
            pout[edge.to] -= possible_flow;
            
            if (pout[edge.to] == 0) {
                to_nullify.push_back(edge.to);
            }
            
            down(edge.to, possible_flow);
            flow -= possible_flow;
            
            if (f[edge.to][v][edge.id] == c[edge.to][v][edge.id])
                edge.deleted = true;
        }
    }
}

void nullifying() {
    while (!to_nullify.empty()) {
        ll v = to_nullify[to_nullify.size() - 1];
        if (!deleted[v])
            nullify(v);
        to_nullify.pop_back();
    }
}

ll get_min_vertex() {
    if (!to_nullify.empty()) {
        nullifying();
        return get_min_vertex();
    }
    
    ll v = -1, value = INF;
    for (ll i = 0; i < n; ++i) {
        if (deleted[i]) continue;
        p[i] = std::min(pin[i], pout[i]);
        
        if (p[i] <= 0) {
            to_nullify.push_back(i);
        } else if (p[i] < value) {
            value = p[i];
            v = i;
        }
    }
    
    if (!to_nullify.empty()) {
        nullifying();
        return get_min_vertex();
    }
    
    return v;
}

ll Malhotra_Kumara_Maheshwari() {
    ll v = get_min_vertex();
    if (v != -1) {
        ll flow = p[v];
        up(v, flow);
        down(v, flow);
        to_nullify.push_back(v);
        return flow;
    }
    return 0;
}

void Dinic() {
    ll max_flow = 0;
    while (bfs()) {
        pin[s] = pout[t] = INF;
        ll flow = Malhotra_Kumara_Maheshwari();
        while (flow > 0) {
            max_flow += flow;
            flow = Malhotra_Kumara_Maheshwari();
        }
        
        memset(pin, 0, sizeof(pin));
        memset(pout, 0, sizeof(pout));
        memset(deleted, false, sizeof(deleted));
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
        g[a].push_back(Edge({a, b, i, false}));
        g[b].push_back(Edge({b, a, i, false}));
        c[a][b][i] = cf;
        c[b][a][i] = f[a][b][i] = f[b][a][i] = 0;
    }
    
    s = 0;
    t = n - 1;
    Dinic();
}
 
