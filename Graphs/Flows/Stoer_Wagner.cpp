#include <iostream>
#include <vector>
#include <cstring>

typedef int64_t ll;

const ll MAXN = 500;
ll n, m, g[MAXN][MAXN];
ll best_cost = 1e18;
std::vector<ll> best_cut;

void Stoer_Wagner() {
    std::vector<ll> v[MAXN];
    for (ll i = 0; i < n; ++i)
        v[i].assign(1, i);
    ll w[MAXN];
    bool exist[MAXN], in_a[MAXN];
    memset (exist, true, sizeof exist);
    for (ll ph = 0; ph < n - 1; ++ph) {
        memset (in_a, false, sizeof in_a);
        memset (w, 0, sizeof w);
        for (ll it = 0, prev; it < n - ph; ++it) {
            ll sel = -1;
            for (ll i = 0; i < n; ++i)
                if (exist[i] && !in_a[i] && (sel == -1 || w[i] > w[sel]))
                    sel = i;
            if (it == n - ph - 1) {
                if (w[sel] < best_cost)
                    best_cost = w[sel],  best_cut = v[sel];
                v[prev].insert(v[prev].end(), v[sel].begin(), v[sel].end());
                // updating adjacency matrix
                for (ll i = 0; i < n; ++i)
                    g[prev][i] = g[i][prev] += g[sel][i];
                exist[sel] = false;
            } else {
                in_a[sel] = true;
                for (ll i = 0; i < n; ++i)
                    w[i] += g[sel][i];
                prev = sel;
            }
        }
    }

    std::cout << best_cost << '\n';
    std::cout << best_cut.size() << '\n';
    for (auto vx : best_cut) {
        std::cout << vx + 1 << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    std::cin >> n >> m;
    for (ll i = 0, a, b, wi; i < m; ++i) {
        std::cin >> a >> b >> wi;
        --a; --b;
        g[a][b] += wi;
        g[b][a] += wi;
    }

    Stoer_Wagner();
}
