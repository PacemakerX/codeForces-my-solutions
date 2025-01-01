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
    vvi arr(n, vi(n, 0));

    int first = 1;
    int last = n * n;

    bool check = false;

    int i = 0, j = 0;

    while (i < n)
    {
        int j = (i % 2 ? n - 1 : 0);
        while (i % 2 ? j >= 0 : j < n)
        {
            if ((i + j) % 2)
                arr[i][j] = last--;
            else
                arr[i][j] = first++;

            i % 2 ? j-- : j++;
        }

        i++;
    }
    for (auto i : arr)
    {
        for (auto j : i)
        {
            cout << j << " ";
        }

        cout << nline;
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
* The maximum beauty of an matrix will always be in the rant 0 to n^2 -1 , why?

* Lets take n= 3 , the maximum and min value we can have? 1 and 9 okay,now  that we have it
* The difference between the two is 8
* so we can differencces like 1,2,3,4,5,6,7,8 ,

* Now we know we can get the difference by placing 1 9 , then what?
* now we want the diffferenmce 7, for this we can placce 1 9 2 ,

* following this trene

* 1 -> 9 -> 2
* 7 <- 3 <- 8
* 4 -> 6 -> 5

* --->
* <---|
* \--->

* A z pattern is followed
*/
