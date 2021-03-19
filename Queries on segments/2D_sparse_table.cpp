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

const int MAXN = 1010, LOGN = 10;

int n, L;
int logs[MAXN];
int st[LOGN][LOGN][MAXN][MAXN];

inline int get_min(int i1, int j1, int i2, int j2) {
    int pi = logs[i2 - i1 + 1], pj = logs[j2 - j1 + 1];

    return std::min({st[pi][pj][i1][j1],
                     st[pi][pj][i2 - (1 << pi) + 1][j1],
                     st[pi][pj][i1][j2 - (1 << pj) + 1],
                     st[pi][pj][i2 - (1 << pi) + 1][j2 - (1 << pj) + 1]});
}

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int p = 0;
    for (int i = 2; i < MAXN; ++i) {
        if (i == (1 << (p + 1)))
            ++p;
        logs[i] = p;
    }

    std::cin >> n >> L;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            std::cin >> st[0][0][i][j];

    for (int i = 1; i <= logs[n]; ++i)
        for (int j = 0; j + (1 << i) <= n; ++j)
            for (int k = 0; k < n; ++k)
                st[i][0][j][k] = std::min(st[i - 1][0][j][k],
                                          st[i - 1][0][j + (1 << (i - 1))][k]);

    for (int j = 1; j <= logs[n]; ++j)
        for (int l = 0; l + (1 << j) <= n; ++l)
            for (int i = 0; i <= logs[n]; ++i)
                for (int k = 0; k + (1 << i) <= n; ++k)
                    st[i][j][k][l] = std::min(st[i][j - 1][k][l],
                                          st[i][j - 1][k][l + (1 << (j - 1))]);

    for (int i = 0; i + L - 1 < n; ++i) {
        for (int j = 0; j + L - 1 < n; ++j) {
            std::cout << get_min(i, j, i + L - 1, j + L - 1) << " ";
        }
        std::cout << "\n";
    }
}
