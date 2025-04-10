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
#define elasped_time 1.0 * clock() / CLOCKS_PER_SEC

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
ll findsqrt(ll n)
{
    for (int i = 1; i * i <= n; i++)
    {
        if (i * i == n)
            return i;
    }

    return -1;
}
void solve()
{
    ll n, x;
    cin >> n;

    ll sum = 0LL, cnt = 0LL;

    forn(i, n)
    {
        cin >> x;
        sum += x;
        ll k = findsqrt(sum);
        if (k != -1 && (k - 1) % 2 == 0)
        {
            cnt++;
        }
    }

    cout << cnt << nline;
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
// Explanation
/*
% From the diagram we can observe the first layer has 1 block, the second 8, the third 16 , so on and so forth
% over a days if a person complete 1 layer each day, arr will look like ( 1+8+16+24 ...)
% it form ( 1 + n(2.8 +(n-1)8)/2) which can be solvd as 4n^2 +4n + 1 , we use running sum
% x , putting into equatin , 4n^2 +4n +1 =x , 4n^2 +4n +(x-1), using determinnet
% n = (-1 +(x)^1/2)2, we just have to find n, if n exists n>=0, then Alyona is happy
 */