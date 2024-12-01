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

    map<int, string> mapNum;

    mapNum[0] = "zero";
    mapNum[1] = "one";
    mapNum[2] = "two";
    mapNum[3] = "three";
    mapNum[4] = "four";
    mapNum[5] = "five";
    mapNum[6] = "six";
    mapNum[7] = "seven";
    mapNum[8] = "eight";
    mapNum[9] = "nine";
    mapNum[10] = "ten";
    mapNum[11] = "eleven";
    mapNum[12] = "twelve";
    mapNum[13] = "thirteen";
    mapNum[14] = "fourteen";
    mapNum[15] = "fifteen";
    mapNum[16] = "sixteen";
    mapNum[17] = "seventeen";
    mapNum[18] = "eighteen";
    mapNum[19] = "nineteen";

    map<int, string> mapNum2;

    mapNum[20] = "twenty";
    mapNum[30] = "thirty";
    mapNum[40] = "forty";
    mapNum[50] = "fifty";
    mapNum[60] = "sixty";
    mapNum[70] = "seventy";
    mapNum[80] = "eighty";
    mapNum[90] = "ninety";

    if (n <= 19 || n % 10 == 0)
        cout << mapNum[n] << nline;
    else
    {

        cout << mapNum[n - n % 10] << "-" << mapNum[n % 10] << nline;
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

    // int t;
    // cin >> t;
    // while (t--)
    solve();

    return 0;
}
// Explanation

/*
* mapped all the numbers and the words, efficiently,then 
* prints the output 

*/