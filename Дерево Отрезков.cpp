#include <iostream>

using namespace std;

const int NMAX = 1e6 + 7;

int a[NMAX];
int t_AND[4*NMAX], t_OR[4*NMAX];

void build_OR(int v, int tl, int tr) {
    if (tl == tr) {
        t_OR[v] = a[tl];
    } else {
        int tm = (tl + tr) / 2;
        build_OR(2*v, tl, tm);
        build_OR(2*v+1, tm + 1, tr);
        t_OR[v] = (t_OR[2*v] | t_OR[2*v+1]);
    }
}

int OR(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t_OR[v];
    int tm = (tl + tr) / 2;
    return (OR(2*v, tl, tm, l, min(r, tm)) | OR(2*v+1, tm + 1, tr, max(l, tm + 1), r));
}

void build_AND(int v, int tl, int tr) {
    if (tl == tr) {
        t_AND[v] = a[tl];
    } else {
        int tm = (tl + tr) / 2;
        build_AND(2*v, tl, tm);
        build_AND(2*v+1, tm + 1, tr);
        t_AND[v] = max(t_AND[2*v], t_AND[2*v+1]);
    }
}

int AND(int v, int tl, int tr, int l, int r) {
    if (l > r)
        return 0;
    if (l == tl && r == tr)
        return t_AND[v];
    int tm = (tl + tr) / 2;
    return max(AND(2*v, tl, tm, l, min(r, tm)), AND(2*v+1, tm + 1, tr, max(l, tm + 1), r));
}

void Update(int v, int tl, int tr, int pos, int x) {
    if (tl == tr)
        t_AND[v] = x;
    else {
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            Update(2*v, tl, tm, pos, x);
        } else {
            Update(2*v+1, tm + 1, tr, pos, x);
        }
        t_AND[v] = max(t_AND[2*v], t_AND[2*v+1]);
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    
    int n, m, l, r;
    cin >> n;
    
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }
    cin >> m;
    string s;
    
    build_OR(1, 1, n);
    build_AND(1, 1, n);
    
    while (m --> 0) {
        cin >> s >> l >> r;
        if (s == "AND")
            cout << AND(1, 1, n, l, r) << "\n";
        else
            cout << OR(1, 1, n, l, r) << "\n";
    }
    return 0;
}
