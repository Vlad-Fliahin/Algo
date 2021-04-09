#include <iostream>
#include <vector>
#include <cstring>
#include <queue>

const int MAXN = 405;

int n, k;
int p[MAXN], base[MAXN], match[MAXN];
bool used[MAXN], blossom[MAXN];
std::queue<int> q;
std::vector<int> g[MAXN];

void make_path(int v, int b, int children) {
	while (base[v] != b) {
		blossom[base[v]] = blossom[base[match[v]]] = true;
		p[v] = children;
		children = match[v];
		v = p[match[v]];
	}
}

int lca(int a, int b) {
	bool used[MAXN]{ 0 };
	for (;;) {
		a = base[a];
		used[a] = true;
		if (match[a] == -1)
			break;
		a = p[match[a]];
	}
	for (;;) {
		b = base[b];
		if (used[b])
			return b;
		b = p[match[b]];
	}
}

int find_path(int root) {
	memset(used, 0, sizeof(used));
	memset(p, -1, sizeof(p));
	
	for (int i = 0; i < n; ++i) {
		base[i] = i;
	}

	q = std::queue<int>();
	q.push(root);
	used[root] = true;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (auto to : g[v]) {
			if (base[v] == base[to] || match[v] == to) continue;
			if (to == root || match[to] != -1 && p[match[to]] != -1) {
				int curbase = lca(v, to);
				memset(blossom, 0, sizeof(blossom));
				make_path(v, curbase, to);
				make_path(to, curbase, v);

				for (int i = 0; i < n; ++i) {
					if (blossom[base[i]]) {
						base[i] = curbase;
						if (!used[i]) {
							used[i] = true;
							q.push(i);
						}
					}
				}
			}
			else if (p[to] == -1) {
				p[to] = v;
				if (match[to] == -1)
					return to;
				to = match[to];
				used[to] = true;
				q.push(to);
			}
		}
	}
	return -1;
}

int main() {
	std::cin >> n >> k;
	for (int i = 0, from, to; i < k; ++i) {
		std::cin >> from >> to;
		--from; --to;
		g[from].push_back(to);
		g[to].push_back(from);
	}

	memset(match, -1, sizeof(match));
	for (int i = 0; i < n; ++i) {
		if (match[i] == -1) {
			int v = find_path(i);
			while (v != -1) {
				int pv = p[v], ppv = match[pv];
				match[pv] = v, match[v] = pv;
				v = ppv;
			}
		}
	}

	int ans = 0;
	for (int i = 0; i < n; ++i) {
		if (match[i] != -1) {
			++ans;
		}
	}
	std::cout << ans / 2 << '\n';
}
