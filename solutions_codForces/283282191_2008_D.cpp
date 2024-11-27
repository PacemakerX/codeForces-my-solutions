#include <bits/stdc++.h>

using namespace std;

// Type definitions
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vpii;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vl> vvl;

// Macros
#define nline "\n"
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define forn(i, n) for (int i = 0; i < int(n); i++)
#define forr(i, a, b) for (int i = a; i <= b; i++)
#define ford(i, a, b) for (int i = a; i >= b; i--)

// Constants
const int MOD = 1e9 + 7;
const ll INF = 1e18;
const double EPS = 1e-9;
const double PI = acos(-1);

ll mod_add(ll a, ll b, ll m = MOD) { return ((a % m) + (b % m)) % m; }
ll mod_sub(ll a, ll b, ll m = MOD) { return ((a % m) - (b % m) + m) % m; }
ll mod_mul(ll a, ll b, ll m = MOD) { return ((a % m) * (b % m)) % m; }

ll mod_pow(ll base, ll exp, ll m = MOD)
{
    ll res = 1;
    base %= m;
    while (exp > 0)
    {
        if (exp & 1)
            res = mod_mul(res, base, m);
        base = mod_mul(base, base, m);
        exp >>= 1;
    }
    return res;
}

ll mod_inv(ll a, ll m = MOD)
{
    return mod_pow(a, m - 2, m); // Only works if m is prime
}

ll mod_div(ll a, ll b, ll m = MOD)
{
    return mod_mul(a, mod_inv(b, m), m);
}

// Binary exponentiation (for non-modular calculations)
ll binpow(ll base, ll exp)
{
    ll res = 1;
    while (exp > 0)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

int n;
// vvi g;
// vi vis;
// vi dep;
string str;

// set<int> black;

void dfs(int src, int &cnt, vvi &g, vi &vis, vi &dep, set<int> black)
{

    vis[src] = 1;

    if (str[src]=='0')
        cnt++;

    for (auto v : g[src])
    {
        if (!vis[v])
            dfs(v, cnt, g, vis, dep, black);
    }
    dep[src] = cnt;
}

void solve()
{
    set<int> black;
    vvi g;
    vi vis;
    vi dep;
    g.clear();
    vis.clear();
    dep.clear();

    cin >> n;

    g.resize(n + 1);

    vis.assign(n + 1, 0);
    dep.assign(n + 1, 0);

    forr(i, 1, n)
    {
        int x;
        cin >> x;

        g[i].pb(x);
    }
    cin >> str;
    str = "$" + str;

    forr(i, 1, n)
    {

        if (!vis[i])
        {
            int cst = 0;
            dfs(i, cst, g, vis, dep, black);
        }
    }

    forr(i, 1, n)
    {
        cout << dep[i] << " ";
    }

    cout << nline;
}

int main()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

#ifndef ONLINE_JUDGE
    freopen("./outputs/input.txt", "r", stdin);
    freopen("./outputs/output.txt", "w", stdout);
#endif

    int t;
    cin >> t;
    while (t--)
        solve();

    return 0;
}

