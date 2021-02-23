#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <ctime>
#include <cassert>
#include <complex>
#include <string>
#include <cstring>
#include <random>
#include <bitset>
#include <iomanip>
#include <stack>

#define endl '\n';
#define ll int64_t
#define ld long double
#define pb push_back
#define mp make_pair

using namespace std;

const int MAXN = 1005, LOGN = 10;

int tin[MAXN], tout[MAXN], parent[MAXN], parents[MAXN][LOGN], t = 0;
bool used[MAXN];

vector<vector<int>> g(MAXN, vector<int>());
vector<pair<int, int>> query;

void dfs(int v) {
    tin[v] = t++;
    for (auto to : g[v]) {
        if (used[to]) continue;
        used[to] = true;
        parent[to] = parents[to][0] =v;
        dfs(to);
    }
    tout[v] = t++;
}

bool is_parent(int parent, int child) {
    return tin[parent] <= tin[child] && tout[parent] >= tout[child];
}

int main()
{
    int k, v, u;
    string s;
    
    cin >> k;
    
    for (int i = 0; i < k; ++i) {
        cin >> s >> v >> u;
        
        if (s == "ADD") {
            g[v].push_back(u);
            g[u].push_back(v);
        }
        else {
            query.push_back(make_pair(v, u));
        }
    }
    
    used[1] = true;
    parent[1] = parents[1][0] = 1;
    dfs(1);
    
    for (int j = 1; j < LOGN; ++j)
        for (int i = 1; i < MAXN; ++i)
            parents[i][j] = parents[parents[i][j - 1]][j - 1];
    
    for (int cnt = 0; cnt < query.size(); ++cnt) {
        v = query[cnt].first;
        u = query[cnt].second;
        
        if (is_parent(v, u)) {
            cout << v << endl;
        }
        else if (is_parent(u, v)) {
            cout << u << endl;
        }
        else {
            for (int i = LOGN - 1; i >= 0; --i)
                if (!is_parent(parents[u][i], v))
                    u = parents[u][i];
            
            cout << parent[u] << endl;
        }
    }
    
    return 0;
}
