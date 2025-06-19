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
/***********/
void solve()
{
    int n;
    cin >> n;
    vi pre(n, 0);
    vi suff(n, 0);

    // pre[0] = 0;
    // suff[0] = 0;
    forn(i, n)
    {
        cin >> pre[i];
        suff[i] = pre[i];
    }

    sort(all(pre));
    sort(all(suff));

    forn(i, n)
    {
        if (i)
        {
            pre[i] += pre[i - 1];
            suff[n - i - 1] += suff[n - i];
        }
    }

    int cnt = n;
    ford(i, n - 1, 0)
    {
        if (i && suff[i] > pre[i - 1])
        {
            cnt = (n - i);
            break;
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

    // int t;
    // cin >> t;
    // while (t--)
    solve();

    return 0;
}
// Explanation
/*
% This code solves a problem where, given an array of integers, we need to find the minimum number of largest elements
% such that their sum is strictly greater than the sum of the remaining elements.
% The input is read into the 'pre' and 'suff' vectors.
% Both vectors are sorted in non-decreasing order.
% 'pre' is used to store prefix sums (smallest to largest), and 'suff' is used to store suffix sums (largest to smallest).
% The prefix sum 'pre[i]' contains the sum of the smallest (i+1) elements.
% The suffix sum 'suff[i]' contains the sum of the largest (n-i) elements.
% The code iterates from the end, checking for the smallest group of largest elements whose sum exceeds the sum of the rest.
% The answer is printed as 'cnt', which is the minimum number of elements required.
*/
