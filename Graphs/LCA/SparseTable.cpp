#include <iostream>
#include <vector>

const int MAXN = 50005;
const int MAXLIST = 2 * MAXN;
const int MAXLIST_LOGN = 17;

int n, m;
int a_pos[MAXN], h[MAXN];
std::vector<int> a;
std::vector<int> g[MAXN];
int st[MAXLIST_LOGN][MAXLIST];
int logs[MAXLIST];

void dfs(int v, int curh) {
    h[v] = curh;
    a_pos[v] = a.size();
    a.push_back(v);
    for (auto to : g[v]) {
        dfs(to, curh + 1);
        a.push_back(v);
    }
}

int min_h(int i, int j) {
    return h[a[i]] < h[a[j]] ? i : j;
}

int get_lca(int v1, int v2) {
    int l = a_pos[v1], r = a_pos[v2];
    if (l > r)
        std::swap(l, r);
    int pw2 = logs[r - l + 1];
    int ans1 = st[pw2][l];
    int ans2 = st[pw2][r - (1 << pw2) + 1];
    int ans = min_h(ans1, ans2);
    return a[ans];
}

int main()
{
    std::cin >> n;
    for (int i = 1, parent; i < n; ++i) {
        std::cin >> parent;
        g[parent - 1].push_back(i);
    }

    dfs(0, 0);

    for (int i = 2, p = 0; i < MAXLIST; ++i) {
        if (1 << (p + 1) == i)
            ++p;
        logs[i] = p;
    }

    int sz = a.size();

    for (int i = 0; i < sz; ++i) {
        st[0][i] = i;
    }

    for (int i = 1; i <= logs[sz]; ++i) {
        for (int j = 0; j < sz; ++j) {
            int nj = j + (1 << (i - 1));
            if (nj < sz)
                st[i][j] = min_h(st[i - 1][j], st[i - 1][nj]);
            else
                st[i][j] = st[i - 1][j];
        }
    }

    std::cin >> m;
    for (int i = 0, v1, v2; i < m; ++i) {
        std::cin >> v1 >> v2;
        --v1; --v2;
        int lca = get_lca(v1, v2);
        std::cout << lca + 1 << '\n';
    }
}
