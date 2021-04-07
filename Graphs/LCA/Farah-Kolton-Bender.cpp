#include <iostream>
#include <vector>
#include <cstring>

const int MAXN = 100005;
const int MAXLIST = 2 * MAXN;
const int LOG_MAXLIST = 18;
const int SQRT_MAXLIST = 447;
const int MAXBLOCKS = MAXLIST / ((LOG_MAXLIST + 1) / 2) + 1;

int n, m;
std::vector<int> g[MAXN];  // adjacency list
int h[MAXN];  // vertex height
bool used[MAXN];  // vertex used flag
std::vector<int> a;  // dfs order list
int a_pos[MAXN];  // positions in dfs list
int block;  // block size = 0.5 log A.size()
int bt[LOG_MAXLIST][MAXBLOCKS];  // sparse table on blocks
int bhash[MAXBLOCKS];  // block hashes
// rmq inside each block, indexed by block hash
int brmq[SQRT_MAXLIST][LOG_MAXLIST / 2][LOG_MAXLIST / 2];
int logs[MAXLIST];  // precalced logarithms (floored values)

void dfs(int v, int curh) {
    h[v] = curh;
    a_pos[v] = static_cast<int>(a.size());
    a.push_back(v);
    for (auto to : g[v]) {
        dfs(to, curh + 1);
        a.push_back(v);
    }
}

// precalc logarithms
void calculate_logs(int n) {
    for (int i = 0, j = 0; i <= n; ++i) {
        if (1 << (j + 1) == i)
            ++j;
        logs[i] = j;
    }
}

inline int min_h(int i, int j) {
    return h[a[i]] < h[a[j]] ? i : j;
}

void build_lca() {
    calculate_logs(static_cast<int>(a.size()));

    int sz = static_cast<int>(a.size());
    block = std::max(1, (logs[sz] + 1) / 2);
    int blocks = sz / block + (sz % block ? 1 : 0);

    // precalc in each block and build sparse table
    std::memset(bt, -1, sizeof(bt));
    for (int j = 0, bl = 0, i = 0; j < sz; ++i, ++j) {
        if (i == block) {
            i = 0;
            ++bl;
        }
        if (bt[0][bl] == -1 || min_h(j, bt[0][bl]) == j)
            bt[0][bl] = j;
    }

    for (int i = 1; i <= logs[blocks]; ++i) {
        for (int j = 0; j < blocks; ++j) {
            int nj = j + (1 << (i - 1));
            if (nj < blocks)
                bt[i][j] = min_h(bt[i - 1][j], bt[i - 1][nj]);
            else
                bt[i][j] = bt[i - 1][j];
        }
    }

    // calc block hashes
    std::memset(bhash, 0, sizeof(bhash));
    for (int i = 0, bl = 0, j = 0; i < sz || j < block; ++i, ++j) {
        if (j == block) {
            j = 0;
            ++bl;
        }
        if (j > 0 && (i >= sz || min_h(i - 1, i) == i - 1))
            bhash[bl] += 1 << (j - 1);
    }

    // precalc RMQ inside each unique block
    std::memset(brmq, -1, sizeof(brmq));
    for (int i = 0; i < blocks; ++i) {
        int id = bhash[i];
        if (brmq[id][0][0] != -1) continue;
        for (int l = 0; l < block; ++l) {
            brmq[id][l][l] = l;
            for (int r = l + 1; r < block; ++r) {
                brmq[id][l][r] = brmq[id][l][r - 1];
                if (i * block + r < sz)
                    brmq[id][l][r] =
                        min_h(i * block + brmq[id][l][r],
                              i * block + r) - i * block;
            }
        }
    }
}

// answers in RMQ in block #bl [l, r] in O(1)
inline int lca_in_block(int bl, int l, int r) {
    return brmq[bhash[bl]][l][r] + bl * block;
}

// answers LCA in O(1)
int lca(int v1, int v2) {
    int l = a_pos[v1], r = a_pos[v2];
    if (l > r)  std::swap(l, r);
    int bl = l / block, br = r / block;

    if (bl == br)
        return a[lca_in_block(bl, l % block, r % block)];

    int ans1 = lca_in_block(bl, l % block, block - 1);
    int ans2 = lca_in_block(br, 0, r % block);
    int ans = min_h(ans1, ans2);

    if (bl < br - 1) {
        int pw2 = logs[br - bl - 1];
        int ans3 = bt[pw2][bl + 1];
        int ans4 = bt[pw2][br - (1 << pw2)];
        ans = min_h(ans, min_h(ans3, ans4));
    }
    return a[ans];
}

int main()
{
    std::cin >> n >> m;

    for (int i = 1, p; i < n; ++i) {
        std::cin >> p;
        g[p].push_back(i);
    }

    dfs(0, 0);
    build_lca();

    int a1, a2, x, y, z;
    std::cin >> a1 >> a2 >> x >> y >> z;

    int64_t sum = 0;
    int ans = 0;

    for (int i = 0; i < m; ++i) {
        ans = lca((a1 + ans) % n, a2);
        sum += ans;

        a1 = (1ll * x * a1 + 1ll * y * a2 + z) % n;
        a2 = (1ll * x * a2 + 1ll * y * a1 + z) % n;
    }

    std::cout << sum << '\n';
}
