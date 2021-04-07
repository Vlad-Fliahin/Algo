# include <iostream>
# include <cmath>
# include <algorithm>
# include <cstdint>
# include <cstring>
# include <string>
# include <cstdlib>
# include <vector>
# include <bitset>
# include <map>
# include <queue>
# include <ctime>
# include <stack>
# include <set>
# include <list>
# include <random>
# include <deque>
# include <functional>
# include <iomanip>
# include <sstream>
# include <fstream>
# include <complex>
# include <numeric>
# include <cassert>
# include <array>
# include <tuple>

const int MAXN = 200200, LOGN = 18;
int n, m, a, b, k;
int st[LOGN][MAXN];
int first[MAXN], logs[MAXN];
std::vector <int> order, depth;
std::vector <std::vector <int> > g(MAXN);

void dfs(int v, int h) {
    first[v] = order.size();
    depth.push_back(h);
    order.push_back(v);

    for (auto to : g[v]) {
        dfs(to, h + 1);
        order.push_back(v);
        depth.push_back(h);
    }
}

int lca(int u, int v) {
    int l = first[u], r = first[v];

    if (l > r)
        std::swap(l, r);

    int p = logs[r - l + 1];

    if (((r - l + 1) & (r - l)) == 0) {
        return order[st[logs[r - l + 1]][l]];
    } else {
        if (depth[st[p][l]] < depth[st[p][r - (1 << p) + 1]]) {
            return order[st[p][l]];
        } else {
            return order[st[p][r - (1 << p) + 1]];
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int p = 1;
    for (int i = 2; i < MAXN; ++i) {
        if (i == (1 << (p + 1)))
            ++p;
        logs[i] = p;
    }

    std::cin >> n >> m;

    for (int i = 1; i < n; ++i) {
        std::cin >> p;
        g[p].push_back(i);
    }

    dfs(0, 0);

    for (int i = 0; i < depth.size(); ++i) {
        st[0][i] = i;
    }

    for (int i = 1; i <= logs[depth.size()]; ++i) {
        for (int j = 0; j + (1 << i) <= depth.size(); ++j) {
            int l = st[i - 1][j], r = st[i - 1][j + (1 << (i - 1))];
            if (depth[l] <= depth[r])
                st[i][j] = l;
            else
                st[i][j] = r;
        }
    }

    int a1, a2, x, y, z;
    std::cin >> a1 >> a2 >> x >> y >> z;

    int64_t summary = 0;
    int ans = 0;

    for (int i = 0; i < m; ++i) {
        ans = lca((a1 + ans) % n, a2);
        summary += ans;

        a1 = (1ll*x*a1 + 1ll*y*a2 + z) % n;
        a2 = (1ll*x*a2 + 1ll*y*a1 + z) % n;
    }

    std::cout << summary << '\n';
}
