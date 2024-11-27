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

/*********** */

vvi dis;
vector<vector<bool>> check;
int n;

int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

bool valid(int x, int y)
{

    return ((x >= 1 && x <= n) && (y >= 1 && y <= n));
}

void queenMove(pii node)
{

    int x = node.F;
    int y = node.S;
    for (int i = 1; i <= n; i++)
    {
        int nx = x - i;
        int ny = y - i;

        if (valid(nx, ny))
        {
            check[nx][ny] = false;
        }

        nx = x + i;
        ny = y - i;
        if (valid(nx, ny))
        {
            check[nx][ny] = false;
        }

        nx = x - i;
        ny = y + i;
        if (valid(nx, ny))
        {
            check[nx][ny] = false;
        }

        nx = x + i;
        ny = y + i;
        if (valid(nx, ny))
        {
            check[nx][ny] = false;
        }

        for (int i = 1; i <= n; i++)
        {
            int nx = x;
            int ny = i;
            check[nx][ny] = false;
        }
        for (int i = 1; i <= n; i++)
        {
            int nx = i;
            int ny = y;
            check[nx][ny] = false;
        }
    }
}

void kingbfs(pii st)
{

    dis[st.F][st.S] = 0;

    queue<pii> q;

    q.push(st);

    while (!q.empty())
    {

        pii node = q.front();
        q.pop();

        int x = node.F;
        int y = node.S;
        for (int i = 0; i < 8; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (valid(nx, ny) && check[nx][ny] && dis[nx][ny] > dis[x][y] + 1)
            {
                dis[nx][ny] = dis[x][y] + 1;

                q.push(mp(nx, ny));
            }
        }
    }
}

void solve()
{
    cin >> n;
    int a1, a2, b1, b2, c1, c2;
    cin >> a1 >> a2;
    cin >> b1 >> b2;
    cin >> c1 >> c2;

    dis.assign(n + 1, vi(n + 1, 1e9));
    check.assign(n + 1, vector<bool>(n + 1, 1));

    queenMove(mp(a1, a2));

    kingbfs(mp(b1, b2));

    if (dis[c1][c2] == 1e9)

        cout << "NO" << nline;

    else
        cout << "YES" << nline;
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

    // int t;
    // cin >> t;
    // while (t--)
    solve();

    return 0;
}
