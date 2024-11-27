#include <bits/stdc++.h>

using namespace std;

// Type definitions
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vpii;
typedef vector<pll> vpll;
typedef vector<vi> vvi;
typedef vector<vl> vvl;

// Macros
#define nline "\\n"
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

/************/

void solve()
{
 
    int n, m;
    cin >> n >> m;
    char ch[n + 1][m + 1];
 
    int left = m + 1, right = 1, top = n + 1, bottom = 1;
 
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> ch[i][j];
            if (ch[i][j] == '#')
            {
                left = min(left, j);
                right = max(right, j);
                top = min(top, i);
                bottom = max(bottom, i);
            }
        }
    }
 
    int ansJ = (left + right)/2;
    int ansI = (top +bottom)/2;
    cout << ansI << " " << ansJ << "\n";
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

