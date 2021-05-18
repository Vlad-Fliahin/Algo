#include <iostream>
#include <vector>
#include <cstring>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <random>

typedef int64_t ll;

const ll MAXN = 200, INF = 1e18;

ll n, m, k, s, t;

struct rib {
    ll b, c, w, f, id, back;
};

std::vector < std::vector<rib> > g(MAXN);

void add_rib(std::vector < std::vector<rib> > * g,
             ll a, ll b, ll c, ll w, ll id) {
    rib r1 = { b, c, w, 0, id, (ll)(*g)[b].size() };
    rib r2 = { a, 0, -w, 0, id, (ll)(*g)[a].size() };
    (*g)[a].push_back(r1);
    (*g)[b].push_back(r2);
}

std::vector<ll> dfs(ll v = s, std::vector<ll> path = std::vector<ll>()) {
    if (v == t) return path;
    for (auto &r : g[v]) {
        if (r.f > 0) {
            r.f = 0;
            path.push_back(r.id);
            return dfs(r.b, path);
        }
    }
    return path;
}

int main()
{
    std::cin >> n >> m >> k;
    s = 0, t = n - 1;
    for (ll i = 1, a, b, w; i <= m; ++i) {
        std::cin >> a >> b >> w;
        --a; --b;
        add_rib(&g, a, b, 1, w, i);
        add_rib(&g, b, a, 1, w, i);
    }

    ll flow = 0,  cost = 0;
    while (flow < k) {
        std::vector<ll> id(n, 0), d(n, INF), q(n), p(n), p_rib(n);
        ll q_left = 0, q_last = 0;
        q[q_last++] = s;
        d[s] = 0;

        while (q_left != q_last) {
            ll v = q[q_left++];
            id[v] = 2;
            if (q_left == n)  q_left = 0;
            for (ll i = 0; i < g[v].size(); ++i) {
                auto r = g[v][i];
                if (r.f < r.c && d[v] + r.w < d[r.b]) {
                    d[r.b] = d[v] + r.w;
                    if (id[r.b] == 0) {
                        q[q_last++] = r.b;
                        if (q_last == n)  q_last = 0;
                    } else if (id[r.b] == 2) {
                        if (--q_left == -1)  q_left = n-1;
                        q[q_left] = r.b;
                    }
                    id[r.b] = 1;
                    p[r.b] = v;
                    p_rib[r.b] = i;
                }
            }
        }
        if (d[t] == INF)  break;
        ll addflow = INF;
        for (ll v = t; v != s; v = p[v]) {
            ll pv = p[v];
            ll pr = p_rib[v];
            addflow = std::min(addflow, g[pv][pr].c - g[pv][pr].f);
        }

        for (ll v = t; v != s; v = p[v]) {
            ll pv = p[v];  ll pr = p_rib[v],  r = g[pv][pr].back;
            g[pv][pr].f += addflow;
            g[v][r].f -= addflow;
            cost += g[pv][pr].w * addflow;
        }

        flow += addflow;
    }

    if (flow < k) {
        std::cout << "-1\n";
        return 0;
    }

    long double average_time = 1.0 * cost / k;
    std::cout << std::fixed << std::setprecision(10) << average_time << '\n';
    for (int i = 0; i < k; ++i) {
        std::vector<ll> path = dfs();
        std::cout << path.size() << " ";
        for (auto r : path) {
            std::cout << r << " ";
        }
        std::cout << "\n";
    }
}
