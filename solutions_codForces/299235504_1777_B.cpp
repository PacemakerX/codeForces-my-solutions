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
vl fact(100001);
void factorial()
{
    fact[0] = 1;

    for (int i = 1; i < 100001; i++)
    {

        fact[i] = mod_mul(i, fact[i - 1]);
    }
}
void solve()
{
    int n;
    cin >> n;

    cout << mod_mul(mod_mul(n, n - 1), fact[n]) << nline;
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

    factorial();
    int t;
    cin >> t;
    while (t--)
        solve();

    return 0;
}
// Explanation
/*
 * It starts wwith the basic obseration e.g. 1,2,3,

 * if we concatenate 1,2,3 with 3,2,1, it will be 1,2,3,3,2,1
 * the observation we can see is there is exactly i (i numberrs )  which are smaller
 * than the number on arr[i], 3 is on index 2, ( and there are only 2 numbers smaller ) than
 * 3, since we are concatenating the array, the pairs will be dubled
 * similary we can caluculate the total number of  pairs, and then multiply it by 2
 *
 * NOTE : by  re arranging to we will alway get 4 pairs  for 3 , e.g. 2,1,3,3,1,2 there are total 4 pairs
 *
 * 2 * ( 0+1 +2 +3 ) = 2 * n*(n-1)/2 -> n*n-1 ( the number of pairs for 1 permutation)
 * for n! permuation we will just multiply it , \n!*n*n-1
*/
