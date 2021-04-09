#include <iostream>
#include <vector>

const int MAXN = 105;

int n, m, a, b;
char wall[MAXN][MAXN];
std::vector<int> g[MAXN * MAXN];
std::vector<int> mt;
std::vector<char> used;

int try_kuhn(int v) {
    if (used[v]) return false;
    used[v] = true;
    for (auto to : g[v]) {
        if (mt[to] == -1 || try_kuhn(mt[to])) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

bool valid_coords(int i, int j) {
    return 0 <= i && i < n && 0 <= j && j < m && wall[i][j] == '*';
}

void create_edges(int i, int j) {
    int id_left = m * i + j;
    if (valid_coords(i - 1, j)) {
        int id_right = m * (i - 1) + j;
        g[id_left].push_back(id_right);
    }
    if (valid_coords(i, j + 1)) {
        int id_right = m * i + j + 1;
        g[id_left].push_back(id_right);
    }
    if (valid_coords(i + 1, j)) {
        int id_right = m * (i + 1) + j;
        g[id_left].push_back(id_right);
    }
    if (valid_coords(i, j - 1)) {
        int id_right = m * i + j - 1;
        g[id_left].push_back(id_right);
    }
}

int main()
{
    std::cin >> n >> m >> a >> b;

    int black = 0, white = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> wall[i][j];

            if (wall[i][j] == '*') {
                if ((i + j) % 2)
                    ++white;
                else
                    ++black;
            }
        }
    }

    int r = white < black;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if ((i + j) % 2 == r && wall[i][j] == '*') {
                create_edges(i, j);
            }
        }
    }

    int sz = n * m, pairs = 0;
    mt.assign(sz, -1);
    for (int v = 0; v < sz; ++v) {
        if (!g[v].empty()) {
            used.assign(sz, false);
            pairs += try_kuhn(v);
        }
    }

    int64_t ans = std::min(1ll*pairs * a + (1ll*black + white - 2 * pairs) * b,
                           (1ll*black + white) * b);
    std::cout << ans << '\n';
}
