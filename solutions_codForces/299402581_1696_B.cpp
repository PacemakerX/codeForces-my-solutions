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
void solve()
{
    int n;
    cin >> n;

    vi arr(n);

    int start = n - 1, end = 0;

    bool hasOnlyZero = true;
    forn(i, n)
    {

        cin >> arr[i];

        if (arr[i] != 0)
        {
            hasOnlyZero = false;
            // too handle the case of {0} , in this case, start=0, and end=0 this will force the loop to enter the else block
            start = min(start, i);
            end = max(end, i);
        }
    }

    if (hasOnlyZero)
    {
        cout << 0 << nline;
    }
    else
    {
        forr(i, start, end)
        {
            if (arr[i] == 0)
            {
                cout << 2 << nline;
                return;
            }
        }

        cout << 1 << nline;
    }
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
* First obseration, was the fact that we can count the number of non -zero segments,
* and the count will be the answer, but it was wrong,

* The whole array can be conerted into 0, in atmost 2 snaps, How? {1,2,3,4,0,8,9,}
* In this array, the mex we will choose l,r as 0 and n-1 therefore the mex wil  be 5,
* we will now replace all the elements with 5, {5,5,5,5,5,5,5},
*  again we will choose, 0 and n-1, now the mex is, 0 , therefore
* the whoe array is destroyed/
*/
